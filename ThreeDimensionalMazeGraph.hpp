#ifndef THREE_DIMENSIONAL_MAZE_GRAPH_HPP
#define THREE_DIMENSIONAL_MAZE_GRAPH_HPP

#include <string>
#include <list>
#include <fstream>

/**
 * - Class to represent the 3D maze as a graph.
 * - Vertices represent positions in the maze.
 * - Edges represent potential movements between positions.
 */
class ThreeDimensionalMazeGraph
{
private:
    struct PositionVertex;
    struct MovementEdge;

    void initGraph(std::ifstream &input);
    void deleteGraph();
    void breadthFirstSearch();
    size_t pos(const int &level, const int &row, const int &col);

    int levels;
    int rows;
    int cols;
    PositionVertex *vertices;
    PositionVertex *start;
    PositionVertex *goal;

public:
    ThreeDimensionalMazeGraph();
    ~ThreeDimensionalMazeGraph();
    void printGraph();
    void outputSolution();
};

/**
 * - Class representing positions within the maze.
 * - Positions are represented as: (level, row, column)
 */
struct ThreeDimensionalMazeGraph::PositionVertex
{
    PositionVertex() : level(0), row(0), col(0) { }
    PositionVertex(const int &level, const int& row, const int& col) : 
    level(level), row(row), col(col) { }

    std::string toString();

    int level;
    int row;
    int col;
    bool discovered = false;
    bool explored = false;
    std::string pathToVertex = "";
    std::list<MovementEdge> adj;
};

/**
 * - Class to represent potential movements between positions.
 * - Directions specified by the input format.
 */
struct ThreeDimensionalMazeGraph::MovementEdge
{
    enum Direction
    {
        NORTH = 0,
        EAST  = 1,
        SOUTH = 2,
        WEST  = 3,
        UP    = 4,
        DOWN  = 5
    };

    MovementEdge(const char &label, PositionVertex *end) : 
    label(label), end(end) { }

    char label;
    PositionVertex *end;
};

#endif // THREE_DIMENSIONAL_MAZE_GRAPH_HPP