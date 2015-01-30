#include "engine.hpp"
#include "astar.hpp"

Engine::Engine()
{
    screen = NULL;

}

void Engine::start()
{
    screen = new sf::RenderWindow(sf::VideoMode(TILE_SIZE*MAP_WIDTH, TILE_SIZE*MAP_HEIGHT,32), "Astar");

    //init map
    m_Map.resize(MAP_HEIGHT);
    for(int i = 0; i < MAP_HEIGHT; i++)
    {
        for(int n = 0; n < MAP_WIDTH; n++) m_Map[i].push_back(0);
    }

    //set walls
    for(int i = 6; i < 13; i++) m_Map[i][6] = 1;

    //set start and end positions
    m_StartPos = sf::Vector2f(3,9);
    m_EndPos = sf::Vector2f(17,3);

    mainLoop();
}

void Engine::mainLoop()
{
    bool quit = false;

    sf::RectangleShape tStart( sf::Vector2f(TILE_SIZE, TILE_SIZE) );
    sf::RectangleShape tEnd( sf::Vector2f(TILE_SIZE, TILE_SIZE) );

    tStart.setFillColor(sf::Color::Green);
    tEnd.setFillColor(sf::Color::Red);
    tStart.setPosition( sf::Vector2f(m_StartPos.x*TILE_SIZE, m_StartPos.y*TILE_SIZE) );
    tEnd.setPosition( sf::Vector2f(m_EndPos.x*TILE_SIZE, m_EndPos.y*TILE_SIZE) );


    while(!quit)
    {
        sf::Event event;

        screen->clear();

        while(screen->pollEvent(event))
        {
            if(event.type == sf::Event::KeyPressed)
            {
                quit = true;
            }
        }

        //draw
        drawTiles();

        //pathfind
        AStar astar(&m_Map, m_StartPos.x, m_StartPos.y, m_EndPos.x, m_EndPos.y );
        astar.eng = this;
        astar.findPath();

        //draw start/stop tiles
        screen->draw(tStart);
        screen->draw(tEnd);



        //update
        screen->display();

    }
}

void Engine::drawTiles()
{

    for(int i = 0; i < MAP_HEIGHT; i++)
    {
        for(int n = 0; n < MAP_WIDTH; n++)
        {
            if(m_Map[i][n] == 0) drawTile(n,i, sf::Color::Black);
            else if(m_Map[i][n] == 1) drawTile(n,i, sf::Color::White);
        }
    }

}

void Engine::drawTile(int x, int y, sf::Color color)
{
    sf::RectangleShape tile(sf::Vector2f(TILE_SIZE-2, TILE_SIZE-2));
    tile.setFillColor( color);
    tile.setOutlineThickness(1);
    tile.setOutlineColor( sf::Color(128,128,128));

    tile.setPosition( sf::Vector2f( x*TILE_SIZE+1, y*TILE_SIZE+1));

    screen->draw(tile);
}
