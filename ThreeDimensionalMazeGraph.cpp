#include "ThreeDimensionalMazeGraph.hpp"

#include <queue>
#include <sstream>
#include <iostream>

/**
 * - Default constructor.
 * - Reads in the graph data as specificed in the input section.
 */
ThreeDimensionalMazeGraph::ThreeDimensionalMazeGraph()
{
    const std::string fileName = "input.txt";
    std::ifstream input(fileName);

    int startLevel = 0, startRow = 0, startCol = 0;
    int goalLevel = 0, goalRow = 0, goalCol = 0;

    if (input) {
        // Read in the size of the maze, and the start and goal positions.
        input >> levels >> rows >> cols;
        input >> startLevel >> startRow >> startCol;
        input >> goalLevel >> goalRow >> goalCol;

        // Initialize the vertices and edges.
        initGraph(input);

        // Save pointers to the starting vertex and goal vertex.
        start = &vertices[pos(startLevel, startRow, startCol)];
        goal = &vertices[pos(goalLevel, goalRow, goalCol)];
    }
    else {
        throw std::runtime_error("Error opening " + fileName + 
            ". Ensure file is in the proper directory and run again.\n");
    }

    input.close();
}

/**
 * - Deconstructor. Deallocate the dynamic array.
 */
ThreeDimensionalMazeGraph::~ThreeDimensionalMazeGraph()
{
    deleteGraph();
}

/**
 * - Build the graph via the specified input format in the report.
 */
void ThreeDimensionalMazeGraph::initGraph(std::ifstream &input)
{
    // Allocate the adjacency list on the heap.
    vertices = new PositionVertex[levels * rows * cols];

    std::string edges = "";
    for (int i = 0; i < levels; i++) {
        for (int j = 0; j < rows; j++) {
            for (int k = 0; k < cols; k++) {
                // Assign the appropriate position to each vertex.
                vertices[pos(i, j, k)] = PositionVertex(i, j, k);

                input >> edges;

                // Add edges between connected vertices.
                for (int e = 0; e < edges.size(); e++) {
                    if (edges[e] == '1') {
                        // The position in the edges string corresponds to the direction position vertices are connected.
                        switch (e) {
                            case MovementEdge::Direction::NORTH:
                                vertices[pos(i, j, k)].adj.push_back(
                                    MovementEdge('N', &vertices[pos(i, j - 1, k)]));
                            break;

                            case MovementEdge::Direction::EAST:
                                vertices[pos(i, j, k)].adj.push_back(
                                    MovementEdge('E', &vertices[pos(i, j, k + 1)]));
                            break;

                            case MovementEdge::Direction::SOUTH:
                                vertices[pos(i, j, k)].adj.push_back(
                                    MovementEdge('S', &vertices[pos(i, j + 1, k)]));
                            break;

                            case MovementEdge::Direction::WEST:
                                vertices[pos(i, j, k)].adj.push_back(
                                    MovementEdge('W', &vertices[pos(i, j, k - 1)]));
                            break;

                            case MovementEdge::Direction::UP:
                                vertices[pos(i, j, k)].adj.push_back(
                                    MovementEdge('U', &vertices[pos(i + 1, j, k)]));
                            break;

                            case MovementEdge::Direction::DOWN:
                                vertices[pos(i, j, k)].adj.push_back(
                                    MovementEdge('D', &vertices[pos(i - 1, j, k)]));
                            break;

                            default:
                                throw std::runtime_error("Error reading in edges.\n");
                            break;
                        }
                    }
                }
            }
        }
    }
}

/**
 * - Deallocate the dynamic array.
 */
void ThreeDimensionalMazeGraph::deleteGraph()
{
    if (vertices != nullptr) {
        delete[] vertices;
    }
}

/**
 * - Complete a BFS traversal of the maze to find the shortest path to every vertex in the maze.
 */
void ThreeDimensionalMazeGraph::breadthFirstSearch()
{
    std::queue<PositionVertex*> queue;

    start->discovered = true;
    queue.push(start);

    PositionVertex *v = nullptr;
    while (!queue.empty()) {
        v = queue.front();
        queue.pop();

        v->explored = true;

        for (auto &e : v->adj) {
            if (!e.end->discovered) {
                e.end->discovered = true;

                if (v != start) {
                    e.end->pathToVertex = v->pathToVertex + " " + e.label;
                }
                else {
                    e.end->pathToVertex = e.label;
                }
                
                queue.push(e.end);
            }
        }
    }
}

/**
 * - Complete a BFS traversal.
 * - Output the path to the goal vertex.
 */
void ThreeDimensionalMazeGraph::outputSolution()
{
    breadthFirstSearch();

    std::string filename = "output.txt";
    std::ofstream output(filename);

    if (output) {
        output << goal->pathToVertex;
    }

    output.close();
}

/**
 * - String representation of vertices.
 * - Also lists the connected edges.
 */
std::string ThreeDimensionalMazeGraph::PositionVertex::toString()
{
    std::stringstream ss;

    ss << "(" << level << ", " << row << ", " << col << ") " << pathToVertex << "\n";
    for (auto e : adj) {
        ss << "\t(" << e.label << " (" << e.end->level << ", " << 
            e.end->row << ", " << e.end->col << "))\n"; 
    }
    ss << "\n";

    return ss.str();
}

/**
 * - Print every vertex of the graph, including the paths to them.
 */
void ThreeDimensionalMazeGraph::printGraph()
{
    for (int i = 0; i < levels; i++) {
        for (int j = 0; j < rows; j++) {
            for (int k = 0; k < cols; k++) {
                std::cout << vertices[pos(i, j, k)].toString();
            }
        }
    }

    std::cout << "Start:\n" << start->toString();
    std::cout << "\nGoal:\n" << goal->toString(); 
}

/**
 * - Use an offset function to index the 1D array as if it were a 3D array.
 */
size_t ThreeDimensionalMazeGraph::pos(const int &level, const int &row, const int &col)
{
    return (level * rows * cols) + (row * cols) + col;
}