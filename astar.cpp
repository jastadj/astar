#include "astar.hpp"

AStar::AStar(std::vector< std::vector<int> > *tmap, int startx, int starty, int endx, int endy)
{
    m_Map = tmap;

    m_StartX = startx;
    m_StartY = starty;
    m_EndX = endx;
    m_EndY = endy;

    //debug
    eng = NULL;

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

bool AStar::validTile(int x, int y)
{
    //in map bounds?
    if(x < 0 || x >= int((*m_Map)[0].size()) || y < 0 || y >= int(m_Map->size()) ) return false;

    //walkable?
    else if( (*m_Map)[y][x] == 1) return false;

    //in openlist?
    else if( inList( &m_OpenList, x, y)) return false;

    //in closed list?
    else if( inList( &m_ClosedList, x, y)) return false;


    return true;
}
bool AStar::inList( std::vector<A_Node*> *tlist, int x, int y)
{
    for(int i = 0; i < int(tlist->size()); i++)
    {
        if( (*tlist)[i]->x == x && (*tlist)[i]->y == y) return true;
    }

    return false;
}

void AStar::moveNodeToClosedList( A_Node *tnode)
{
    //find node in open list
    for(int i = int(m_OpenList.size() -1); i >= 0; i--)
    {
        if(m_OpenList[i] == tnode)
        {
            //add tnode to closed list
            m_ClosedList.push_back(tnode);

            //remove tnode from open list
            m_OpenList.erase(m_OpenList.begin() + i);
            return;
        }
    }
}

A_Node *AStar::getLowestFscoreNode()
{
    A_Node *fnode = NULL;
    //if openlist is empty?
    if(m_OpenList.empty()) return fnode;

    //init fnode as first node in open list
    fnode = m_OpenList[0];

    //walk through open list and find highest f score
    for(int i = 0; i < int(m_OpenList.size()); i++)
    {
        if(fnode->f < m_OpenList[i]->f) fnode = m_OpenList[i];
    }

    return fnode;
}

std::vector<sf::Vector2f> AStar::findPath()
{
    std::vector<sf::Vector2f> pathlist;
    A_Node *currentNode = NULL;

    //create starting node
    A_Node *node = new A_Node;
    node->x = m_StartX;
    node->y = m_StartY;
    node->g = 0;
    getHeuristic(node);
    getFscore(node);

    //add starting node to open list
    m_OpenList.push_back(node);
    currentNode = m_OpenList[0];

    //process starting node
    //check valid adjacent tiles to add to open list
    for(int n = -1 + currentNode->y; n < 2 + currentNode->y; n++)
    {
        for(int p = -1 + currentNode->x; p < 2 + currentNode->x; p++)
        {
            //dont check center node since it is current node position!
            if(n == 0 && p == 0) continue;

            //check if walkable tile
            if( validTile(p, n) )
            {
                //add it to open list
                A_Node *nnode = new A_Node;
                nnode->x = p;
                nnode->y = n;
                nnode->parent = currentNode;
                getHeuristic(nnode);
                //get g score
                if( (p == -1 || p == 1 ) && (n == -1 || p == 1))
                {
                    nnode->g = currentNode->g + A_DIAGCOST;
                }
                else nnode->g = currentNode->g + A_ORTHOCOST;

                //calc f score
                getFscore(nnode);

                //add to open list
                m_OpenList.push_back(nnode);

            }

        }
    } // end adding valid adjacent tiles to open list

    //drop starting node from open list and add to closed list
    moveNodeToClosedList(currentNode);

    //process openlist
    //for now, just use an iterative run
    for(int i = 0; i < 15; i++)
    {
        //set current node as lowest F score node
        currentNode = getLowestFscoreNode();

        if(currentNode == NULL) break;

        //check valid adjacent tiles to add to open list
        for(int n = -1 + currentNode->y; n < 2 + currentNode->y; n++)
        {
            for(int p = -1 + currentNode->x; p < 2 + currentNode->x; p++)
            {
                //dont check center node since it is current node position!
                if(n == 0 && p == 0) continue;

                //check if walkable tile
                if( validTile(p, n) )
                {
                    //add it to open list
                    A_Node *nnode = new A_Node;
                    nnode->x = p;
                    nnode->y = n;
                    nnode->parent = currentNode;
                    getHeuristic(nnode);
                    //get g score
                    if( (p == -1 || p == 1 ) && (n == -1 || p == 1))
                    {
                        nnode->g = currentNode->g + A_DIAGCOST;
                    }
                    else nnode->g = currentNode->g + A_ORTHOCOST;

                    //calc f score
                    getFscore(nnode);

                    //add to open list
                    m_OpenList.push_back(nnode);

                }

            }
        } // end adding valid adjacent tiles to open list

        //move current node to closed list
        moveNodeToClosedList(currentNode);

    }

    //debug
    d_drawNodes(&m_OpenList, sf::Color::Blue);
    d_drawNodes(&m_ClosedList, sf::Color::Magenta);

    return pathlist;
}

/////////////////////////////////////////////////////////////////////
//
void AStar::d_drawNodes(std::vector<A_Node*> *nodelist, sf::Color color)
{
    if(eng == NULL) return;

    //walk through each node
    for(int i = 0; i < int(nodelist->size()); i++)
    {
        //have engine draw tile for each node
        eng->drawTile( (*nodelist)[i]->x, (*nodelist)[i]->y, color);
    }
}
