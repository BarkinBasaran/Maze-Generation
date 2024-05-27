/*
 * Yasin Barkın Başaran - CS300 TAKE HOME 1 *
 * */


// Including necessary libraries
#include <iostream>
#include <stdexcept>
#include <vector>
#include <fstream>
#include <random>
#include <algorithm>
#include "Stack.h" // Custom stack implementation

using namespace std;

// Class to hold the dimensions of the grid
class Dimensions {
private:
    int rowSize;
    int columnSize;
    int mazeCountThatWillBeCreated;

public:
    // Constructor
    Dimensions(int rows, int cols, int mazeCount) : rowSize(rows), columnSize(cols), mazeCountThatWillBeCreated(mazeCount) {}

    // Static method to check if the row and column sizes are valid
    static void checkRowAndColumnSize(int r, int c){
        if (r <= 1 || c <= 1)
            throw invalid_argument("Number of rows or columns must be greater than or equal to 1.");
    }

    // Getter for row size
    int getRowSize() const {
        return rowSize;
    }

    // Getter for column size
    int getColumnSize() const{
        return columnSize;
    }

    // Static method to check if the maze count is valid
    static void checkMazeCountThatWillBeCreated(int m){
        if (m < 0)
            throw invalid_argument("Number of mazes must be positive integer.");
    }

    // Getter for maze count
    int getMazeCountThatWillBeCreated() const{
        return mazeCountThatWillBeCreated;
    }
};

// Class to represent the grid of the maze
class Grid{
public:
    // Nested structure to represent a cell in the grid
    struct Cell{
        int x;
        int y;
        bool right, left, up, down; // Booleans to represent if there are walls
        bool visited, pathVisited; // Booleans to track if the cell has been visited
        Cell(int x = 0, int y = 0) : x(x), y(y), right(true), left(true), up(true), down(true), visited(false), pathVisited(false){}
        bool isLeft() const {return left;}
        bool isRight() const {return right;}
        bool isUp() const {return up;}
        bool isDown() const {return down;}
    };
    Dimensions gridDimension; // Dimension of the grid
private:
    vector<vector<Cell>> cells; // 2D vector to store the cells

    // Method to build the grid based on dimensions
    void buildGrid() {
        cells.resize(gridDimension.getRowSize());
        for (int i = 0; i < gridDimension.getRowSize(); i++) {
            cells[i].resize(gridDimension.getColumnSize());
            for (int j = 0; j < gridDimension.getColumnSize(); j++) {
                cells[i][j] = Cell(j,i);
            }
        }
    }

    // Method to check if a row number is within the grid
    bool isRowInBorder(int num) const {
        return num >= 0 && num < gridDimension.getRowSize();
    }

    // Method to check if a column number is within the grid
    bool isColumnInBorder(int num) const {
        return num >= 0 && num < gridDimension.getColumnSize();
    }
public:

    // Constructor
    Grid (const Dimensions& dimensions) : gridDimension(dimensions){
        buildGrid();
    }

    // Method to check if there is a wall between two cells
    bool isWallBetween(int x1, int y1, int x2, int y2) const {
        if (abs(x1 - x2) + abs(y1 - y2) != 1) {
            return true;
        }

        if (x1 == x2) {
            if (y1 < y2) {
                return cells[y1][x1].up || cells[y2][x2].down;
            } else {
                return cells[y1][x1].down || cells[y2][x2].up;
            }
        } else if (y1 == y2) {
            if (x1 < x2) {
                return cells[y1][x1].right || cells[y2][x2].left;
            } else {
                return cells[y1][x1].left || cells[y2][x2].right;
            }
        }
        return true;
    }

    // Method to get a reference to a cell
    Cell& getCell(int x, int y) {
        return cells[y][x];
    }
    // Const method to get a const reference to a cell
    const Cell& getCell(int x, int y) const {
        return cells[y][x];
    }
    // Method to set a cell as visited
    void setVisited(int x, int y){
        cells[gridDimension.getRowSize() - 1 - y][x].visited = true;
    }

    // Method to check if a cell has been visited
    bool isVisited(int x,int y) const {
        const Cell& cell = cells[gridDimension.getRowSize() - 1 - y][x];
        return cell.visited;
    }
    // Method to check if it's possible to move down from a cell
    bool canMoveDown(int x, int y) const {
        return y > 0;
    }
    // Method to check if it's possible to move left from a cell
    bool canMoveLeft(int x, int y) const {
        return x > 0;
    }

    // Method to check if it's possible to move up from a cell
    bool canMoveUp(int x, int y) const {
        return y < gridDimension.getRowSize() - 1;
    }
    // Method to check if it's possible to move right from a cell
    bool canMoveRight(int x, int y) const {
        return x < gridDimension.getColumnSize() - 1;
    }
    // Method to check if a pair of coordinates is valid within the grid
    bool areCoordinatesValid(int x, int y) const{
        return isRowInBorder(y) && isColumnInBorder(x);
    }
};

// Class to represent the maze
class Maze {
private:
    Grid mazeGrid;
    Dimensions mazeDimension;

    // Method to remove walls between two adjacent cells
    void removeWalls(int x1, int y1, int x2, int y2) {

        Grid::Cell& cell1 = mazeGrid.getCell(x1, y1);
        Grid::Cell& cell2 = mazeGrid.getCell(x2, y2);

        if (x1 == x2) {
            if (y1 < y2) {
                cell1.up = false;
                cell2.down = false;
            } else {
                cell1.down = false;
                cell2.up = false;
            }
        } else if (y1 == y2) {
            if (x1 < x2) {
                cell1.right = false;
                cell2.left = false;
            } else {
                cell1.left = false;
                cell2.right = false;
            }
        }
    }

    // Method to get a list of unvisited neighbors of a cell
    vector<pair<int, int>> getUnvisitedNeighbors(int x, int y) const {
        vector<pair<int, int>> neighbors;
        if (mazeGrid.canMoveUp(x, y) && !mazeGrid.isVisited(x, y + 1)) neighbors.emplace_back(x, y + 1);
        if (mazeGrid.canMoveDown(x, y) && !mazeGrid.isVisited(x, y - 1)) neighbors.emplace_back(x, y - 1);
        if (mazeGrid.canMoveLeft(x, y) && !mazeGrid.isVisited(x - 1, y)) neighbors.emplace_back(x - 1, y);
        if (mazeGrid.canMoveRight(x, y) && !mazeGrid.isVisited(x + 1, y)) neighbors.emplace_back(x + 1, y);
        return neighbors;
    }

    // Method to choose a random unvisited neighbor of a cell
    pair<int, int> chooseRandomUnvisitedNeighbor(int x, int y) const {
        auto unvisitedNeighbors = getUnvisitedNeighbors(x, y);
        if (unvisitedNeighbors.empty()) return {-1, -1};
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distr(0, unvisitedNeighbors.size() - 1);
        return unvisitedNeighbors[distr(gen)];
    }

public:
    // Constructor
    explicit Maze(Grid &grid, Dimensions mazeDimension) : mazeGrid(grid), mazeDimension(mazeDimension) {}

    // Getter for the grid
    Grid& getGrid() {
        return mazeGrid;
    }
    // Method to generate the maze using a randomized depth-first search algorithm
    void generateMaze() {
        Stack<pair<int, int>> stack;
        pair<int, int> startCell = {0, 0};
        stack.push(startCell);
        mazeGrid.setVisited(startCell.first, startCell.second);

        while (!stack.isEmpty()) {
            auto currentCell = stack.stackTop();
            auto unvisitedNeighbors = getUnvisitedNeighbors(currentCell.first, currentCell.second);

            if (!unvisitedNeighbors.empty()) {
                auto nextCell = chooseRandomUnvisitedNeighbor(currentCell.first, currentCell.second);
                if (nextCell.first != -1) {
                    removeWalls(currentCell.first, currentCell.second, nextCell.first, nextCell.second);
                    mazeGrid.setVisited(nextCell.first, nextCell.second);
                    stack.push(nextCell);
                }
            } else {
                stack.pop();
            }
        }
    }

    // Method to write the maze to a file
    void writeMazeToFile(int mazeID)  {
        string filename = "maze_" + to_string(mazeID) + ".txt";
        ofstream file(filename);
        if (!file.is_open()) {
            cerr << "Failed to open file: " << filename << endl;
            return;
        }

        file << mazeGrid.gridDimension.getRowSize() << " " << mazeGrid.gridDimension.getColumnSize() << endl;
        for (int y = 0; y < mazeGrid.gridDimension.getRowSize(); ++y) {
            for (int x = 0; x < mazeGrid.gridDimension.getColumnSize(); ++x) {
                auto& cell = mazeGrid.getCell(x, y);
                file << "x=" << x << " y=" << y
                     << " l=" << cell.isLeft()
                     << " r=" << cell.isRight()
                     << " u=" << cell.isUp()
                     << " d=" << cell.isDown() << endl;
            }
        }
        file.close();
    }
};

// Class to find a path through the maze
class MazePathFinder {
private:
    Grid& mazeGrid;
    pair<int, int> entryPoint;
    pair<int, int> exitPoint;
    vector<std::vector<bool>> visited; // 2D vector to track visited cells
    vector<std::pair<int, int>> path; // Vector to store the path

public:
    // Constructor
    MazePathFinder(Grid& grid, pair<int, int> entry, pair<int, int> exit)
            : mazeGrid(grid), entryPoint(entry), exitPoint(exit) {
        visited.resize(grid.gridDimension.getRowSize(), vector<bool>(grid.gridDimension.getColumnSize(), false));
    }

    // Method to find a path from the entry point to the exit point
    bool findPath() {
        path.clear();
        for (auto& row : visited) {
            fill(row.begin(), row.end(), false);
        }

        Stack<pair<int, int>> stack;
        stack.push(entryPoint);

        while (!stack.isEmpty()) {
            auto [x, y] = stack.stackTop();

            if (x == exitPoint.first && y == exitPoint.second) {
                path.push_back(exitPoint);
                return true;
            }

            if (!visited[y][x]) {
                visited[y][x] = true;
                path.push_back({x, y});

                // Explore neighbors: Right, Down, Left, Up
                vector<pair<int, int>> directions = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
                bool moved = false;
                for (auto& dir : directions) {
                    int newX = x + dir.first, newY = y + dir.second;

                    if (mazeGrid.areCoordinatesValid(newX, newY) && !visited[newY][newX] && !mazeGrid.isWallBetween(x, y, newX, newY)) {
                        stack.push({newX, newY});
                        moved = true;
                        break;
                    }
                }
                if (!moved) {
                    stack.pop();
                    path.pop_back();
                }
            } else {
                stack.pop();
                path.pop_back();
            }
        }

        return !path.empty();
    }

    // Getter for the path
    const vector<pair<int, int>>& getPath() const {
        return path;
    }

    // Method to write the path to a file
    void writePathToFile(int mazeID, const pair<int, int>& entry, const pair<int, int>& exit, const vector<pair<int, int>>& path) const {
        string filename = "maze_" + to_string(mazeID) + "_path_" +
                          to_string(entry.first) + "_" + to_string(entry.second) + "_" +
                          to_string(exit.first) + "_" + to_string(exit.second) + ".txt";

        ofstream file(filename);
        if (!file.is_open()) {
            cerr << "Failed to open file: " << filename << endl;
            return;
        }
        if (path.empty()) {
            cerr << "Path is empty." << endl;
            file.close();
            return;
        }

        for (const auto& cell : path) {
            file << cell.first << " " << cell.second << endl;
        }

        file.close();
    }
};

// Main function
int main() {
    try {
        // Input for number of mazes to generate
        int mazeCount;
        cout << "Enter the number of mazes to generate: ";
        cin >> mazeCount;
        Dimensions::checkMazeCountThatWillBeCreated(mazeCount);

        // Input for the dimensions of the mazes
        int rows, cols;
        cout << "Enter the number of rows and columns for the mazes: ";
        cin >> rows >> cols;
        Dimensions::checkRowAndColumnSize(rows,cols);

        // Generate the mazes
        vector<Maze> mazes;
        for (int i = 0; i < mazeCount; ++i) {
            Dimensions dimensions(rows, cols, 1);
            Grid grid(dimensions);
            Maze maze(grid, dimensions);
            maze.generateMaze();
            mazes.push_back(maze);
        }

        // Write the mazes to files
        for (int i = 0; i < mazeCount; ++i) {
            mazes[i].writeMazeToFile(i + 1);
        }

        // Input for selecting a maze and finding a path
        int selectedMazeIndex;
        cout << "Enter a maze ID between 1 to " << mazeCount << " inclusive to find a path: ";
        cin >> selectedMazeIndex;
        selectedMazeIndex -= 1;

        if (selectedMazeIndex < 0 || selectedMazeIndex >= mazeCount) {
            cerr << "Invalid maze selection." << endl;
            return 1;
        }

        pair<int, int> entryPoint, exitPoint;
        cout << "Enter the coordinates for the entry point (column,row): ";
        cin >> entryPoint.first >> entryPoint.second;

        cout << "Enter the coordinates for the exit point (column,row): ";
        cin >> exitPoint.first >> exitPoint.second;

        // Find and write the path to a file
        MazePathFinder pathFinder(mazes[selectedMazeIndex].getGrid(), entryPoint, exitPoint);
        if (pathFinder.findPath()) {
            pathFinder.writePathToFile(selectedMazeIndex + 1, entryPoint, exitPoint, pathFinder.getPath());
        } else {
            cout << "No path found in Maze " << (selectedMazeIndex + 1) << "." << endl;
        }

        return 0;
    }
    catch(const invalid_argument& e){
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
}

