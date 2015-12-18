#ifndef CLASS_ENGINE
#define CLASS_ENGINE

#include <SFML\Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>

#define TILE_SIZE 16
#define MAP_WIDTH 40
#define MAP_HEIGHT 40

class Engine
{
private:
    sf::RenderWindow *screen;

    std::vector< std::vector<int> > m_Map;
    sf::Vector2f m_StartPos;
    sf::Vector2f m_EndPos;

    void drawTiles();
    void drawTile(int x, int y, sf::Color = sf::Color::Black);

    sf::Vector2i mouseToGrid();

    void mainLoop();

    sf::Clock m_FPS_Clock;
    int m_frameCount;

public:
    Engine();

    void start();

    friend class AStar;

};
#endif // CLASS_ENGINE
