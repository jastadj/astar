#ifndef CLASS_ENGINE
#define CLASS_ENGINE

#include <SFML\Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>

#define TILE_SIZE 16
#define MAP_WIDTH 20
#define MAP_HEIGHT 20

class Engine
{
private:
    sf::RenderWindow *screen;

    std::vector< std::vector<int> > m_Map;
    sf::Vector2f m_StartPos;
    sf::Vector2f m_EndPos;

    void drawTiles();
    void drawTile(int x, int y, sf::Color = sf::Color::Black);

    void mainLoop();

public:
    Engine();

    void start();

};
#endif // CLASS_ENGINE