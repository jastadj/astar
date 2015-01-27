#include "astar.hpp"

AStar::AStar(std::vector< std::vector<int> > *tmap, int startx, int starty, int endx, int endy)
{
    m_Map = tmap;

    m_StartX = startx;
    m_StartY = starty;
    m_EndX = endx;
    m_EndY = endy;

}

AStar::~AStar()
{

}

void AStar::getHeuristic(A_Node *tnode)
{
    int ydelta = std::abs(m_EndY - tnode->y);
    int xdelta = std::abs(m_EndX - tnode->x);

    tnode->h = (ydelta + xdelta)*A_ORTHOCOST;
}

void AStar::getFscore(A_Node *tnode)
{
    tnode->f = tnode->h + tnode->g;
}

std::vector<sf::Vector2f> AStar::findPath()
{
    std::vector<sf::Vector2f> pathlist;

    //create starting node
    A_Node *node = new A_Node;
    node->x = m_StartX;
    node->y = m_StartY;
    node->g = 0;
    getHeuristic(node);
    getFscore(node);

    //add starting node to open list
    m_OpenList.push_back(node);


    return pathlist;
}
