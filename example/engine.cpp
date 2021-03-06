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
    m_frameCount = 0;

    sf::RectangleShape tStart( sf::Vector2f(TILE_SIZE, TILE_SIZE) );
    sf::RectangleShape tEnd( sf::Vector2f(TILE_SIZE, TILE_SIZE) );

    tStart.setFillColor(sf::Color::Green);
    tEnd.setFillColor(sf::Color::Red);

    sf::Vector2f *posptr = NULL;

    int mousebrush = 0;

    while(!quit)
    {
        sf::Event event;

        screen->clear();

        //if mouse held down
        if(sf::Mouse::isButtonPressed(sf::Mouse::Right))
        {
            sf::Vector2f gridclick = mouseToGrid();

            m_Map[gridclick.y][gridclick.x] = mousebrush;
        }

        if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {

            //if dragging a start/end point
            if(posptr != NULL)
            {
                sf::Vector2f gridclick = mouseToGrid();
                *posptr = gridclick;
            }
        }
        else if(posptr != NULL) posptr = NULL;


        while(screen->pollEvent(event))
        {
            if(event.type == sf::Event::KeyPressed)
            {
                quit = true;
            }
            else if(event.type == sf::Event::MouseButtonPressed)
            {
                if(event.mouseButton.button == sf::Mouse::Left)
                {
                    sf::Vector2f gridclick = mouseToGrid();

                    //check if clicked on start pos or end pos
                    if(m_StartPos == gridclick)
                    {
                        posptr = &m_StartPos;
                    }
                    else if(m_EndPos == gridclick)
                    {
                        posptr = &m_EndPos;
                    }
                    else posptr = NULL;

                }
                else if(event.mouseButton.button == sf::Mouse::Right)
                {
                    sf::Vector2f gridclick = mouseToGrid();

                    if(m_Map[gridclick.y][gridclick.x] == 1) mousebrush = 0;
                    else mousebrush = 1;
                }
            }
        }

        //update
        tStart.setPosition( sf::Vector2f(m_StartPos.x*TILE_SIZE, m_StartPos.y*TILE_SIZE) );
        tEnd.setPosition( sf::Vector2f(m_EndPos.x*TILE_SIZE, m_EndPos.y*TILE_SIZE) );

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

        //update frame count
        m_frameCount++;

        if(m_FPS_Clock.getElapsedTime().asSeconds() >= 1)
        {
            std::cout << "FPS:" << m_frameCount << std::endl;
            m_frameCount = 0;
            m_FPS_Clock.restart();
        }
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

sf::Vector2f Engine::mouseToGrid()
{
    //get mouse position
    sf::Vector2i mousePos = sf::Mouse::getPosition(*screen);

    return sf::Vector2f( mousePos.x/TILE_SIZE, mousePos.y/TILE_SIZE);
}
