// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include <iostream>
#include <fstream>
#include "error.h"
#include "filelib.h"
#include "grid.h"
#include "maze.h"
#include "mazegraphics.h"
#include "queue.h"
#include "set.h"
#include "stack.h"
#include "vector.h"
#include "testing/SimpleTest.h"
using namespace std;


/* 这个函数的目的是找到当前位置的点在东西南北四个方向的四个相邻的位置
 *
 * 同时这四个相邻位置必须满足以下条件：
 * a. 每次只能从当前位置cur移动一步，并且只能沿东西南北(East, West, Sounth, North)这4个方向进行移动。
 * b.只能在迷宫所在的区域内移动，不能越界。
 * c.只能沿着通道移动，不能穿过墙壁。
 *
 *返回符合条件的点的Set
 */
Set<GridLocation> generateValidMoves(Grid<bool>& maze, GridLocation cur) {
    Set<GridLocation> neighbors;
    //首先找到当前位置cur的四个相邻的的点，分别是在东西南北四个方向上寻找，并且相邻的距离都是1，将这个存储在neighbors
    GridLocation northNeighbor = {cur.row - 1, cur.col};
    GridLocation southNeighbor = {cur.row + 1, cur.col};
    GridLocation westNeighbor = {cur.row, cur.col - 1};
    GridLocation eastNeighbor = {cur.row, cur.col + 1};
    Set<GridLocation> neighborsCopy = {northNeighbor, southNeighbor, westNeighbor, eastNeighbor};

    //方案一：逐个检查neighbors 中的元素是否是符合条件的，如果不符合，就要挑拣出当前的这个GridLocation变量.
    //条件就是，该点要在迷宫所在的区域内，并且只能沿着通道移动， 我使用isBound()来判断这个点是否是在迷宫内，用该点的数值来判断是否是通道
    //因为"Do not modify a collection within a for-each loop" 的原因，也就是说，在一个Set循环中，不能修改Set容器的大小
    //
    //方案二：遍历neighborsCopy, 如果符合条件，就添加进入neighbors Set, 这个可行，但是多使用了一份内存(neighborsCopy)
    for(GridLocation neighbor : neighborsCopy){
        if(maze.inBounds(neighbor) && maze.get(neighbor) == true){
            neighbors.add(neighbor);
        }
    }

    return neighbors;
}

/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
void validatePath(Grid<bool>& maze, Stack<GridLocation> path) {
    GridLocation mazeExit = {maze.numRows()-1,  maze.numCols()-1};
    GridLocation mazeEntrance = {0, 0};

    if (path.peek() != mazeExit) {
        error("Path does not end at maze exit");
    }

    Set<GridLocation> alreadyWalk = {mazeExit};

    bool flag = false;

    while(true){

        GridLocation pathPointFirst = path.pop();
        if(path.size() == 0) {
            if(pathPointFirst == mazeEntrance){
                flag = true;
            }

            break;
        }
        Set<GridLocation> validPoint = generateValidMoves(maze, pathPointFirst);
        GridLocation pathPointSecond = path.peek();

        if(!validPoint.contains(pathPointSecond) || alreadyWalk.contains(pathPointSecond)){
            error("Here is my message about what has gone wrong");
        }
        else if(validPoint.contains(pathPointSecond) && !alreadyWalk.contains(pathPointSecond)){
            alreadyWalk.add(pathPointSecond);
        }
    }
    if(!flag){
        error("Here is my message about what has gone wrong");
    }

    /* TODO: Fill in the remainder of this function. */

    /* If you find a problem with the path, call error() to report it.
     * If the path is a valid solution, then this function should run to completion
     * without throwing any errors.
     */
}

/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
Stack<GridLocation> solveMaze(Grid<bool>& maze) {
    MazeGraphics::drawGrid(maze);
    Stack<GridLocation> path;
    /* TODO: Fill in the remainder of this function. */

    GridLocation mazeExit = {maze.numRows()-1,  maze.numCols()-1};

    Stack<GridLocation> init = {{0, 0}};
    Queue<Stack<GridLocation>> allPath;
    allPath.enqueue(init);

    while(!allPath.isEmpty()) {
        //取出最先处理的任务
        Stack<GridLocation> currentLevel = allPath.dequeue();

        MazeGraphics::highlightPath(currentLevel, "red", 500);
        //查看currentLevel的最后面的元素是否是最终的结果
        GridLocation location = currentLevel.peek();
        //如果是最终的结果就跳出循环
        if(location == mazeExit){
            return currentLevel;
        }

        //求解下一步的location(next)，可以使用generateValidMoves()函数。
        Set<GridLocation> next = generateValidMoves(maze, location);

        auto visited = visitedPoint(currentLevel);

        //遍历next， 生成新一步的Stack<GridLocation>
        //如果visited 包含item, 也就是说，现有的路径中，包含next生成的下一步，那么就不能再把下一个位置添加进入现有路径中了
        for(const GridLocation & item : next){
            if(!visited.contains(item)){
                Stack<GridLocation> copy = currentLevel;
                copy.push(item);
                allPath.enqueue(copy);
            }
        }
    }
    return {};
}

Set<GridLocation> visitedPoint(Stack<GridLocation> currentLevel) {
    Set<GridLocation> visited;
    while (!currentLevel.isEmpty()) {
        visited.add(currentLevel.pop());
    }
    return visited;
}

/*
 * The given readMazeFile function correctly reads a well-formed
 * maze from a file.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readMazeFile(string filename, Grid<bool>& maze) {
    /* The following code reads data from the file into a Vector
     * of strings representing the lines of the file.
     */
    ifstream in;

    if (!openFile(in, filename))
        error("Cannot open file named " + filename);

    Vector<string> lines;
    readEntireFile(in, lines);

    /* Now that the file data has been read into the Vector, populate
     * the maze grid.
     */
    int numRows = lines.size();        // rows is count of lines
    int numCols = lines[0].length();   // cols is length of line
    maze.resize(numRows, numCols);     // resize grid dimensions

    for (int r = 0; r < numRows; r++) {
        if (lines[r].length() != numCols) {
            error("Maze row has inconsistent number of columns");
        }
        for (int c = 0; c < numCols; c++) {
            char ch = lines[r][c];
            if (ch == '@') {        // wall
                maze[r][c] = false;
            } else if (ch == '-') { // corridor
                maze[r][c] = true;
            } else {
                error("Maze location has invalid character: '" + charToString(ch) + "'");
            }
        }
    }
}

/* The given readSolutionFile correctly reads a path from a file.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readSolutionFile(string filename, Stack<GridLocation>& soln) {
    ifstream in;

    if (!openFile(in, filename)) {
        error("Cannot open file named " + filename);
    }

    if (!(in >> soln)) {// if not successfully read
        error("Maze solution did not have the correct format.");
    }
}


/* * * * * * Test Cases * * * * * */

//PROVIDED_TEST("generateValidMoves on location in the center of 3x3 grid with no walls") {
//    Grid<bool> maze = {{true, true, true},
//                       {true, true, true},
//                       {true, true, true}};
//    GridLocation center = {1, 1};
//    Set<GridLocation> expected = {{0, 1}, {1, 0}, {1, 2}, {2, 1}};

//    EXPECT_EQUAL(generateValidMoves(maze, center), expected);
//}

//PROVIDED_TEST("generateValidMoves on location on the side of 3x3 grid with no walls") {
//    Grid<bool> maze = {{true, true, true},
//                       {true, true, true},
//                       {true, true, true}};
//    GridLocation side = {0, 1};
//    Set<GridLocation> expected = {{0,0}, {0,2}, {1, 1}};

//    EXPECT_EQUAL(generateValidMoves(maze, side), expected);
//}

//PROVIDED_TEST("generateValidMoves on corner of 2x2 grid with walls") {
//    Grid<bool> maze = {{true, false},
//                       {true, true}};
//    GridLocation corner = {0, 0};
//    Set<GridLocation> expected = {{1, 0}};

//    EXPECT_EQUAL(generateValidMoves(maze, corner), expected);
//}

//STUDENT_TEST("generateValidMoves on location in the center of 3x3 grid with walls") {
//    Grid<bool> maze = {{true, true, true},
//                       {true, true, false},
//                       {true, false, true}};
//    GridLocation center = {1, 1};
//    Set<GridLocation> expected = {{0, 1}, {1, 0}};

//    EXPECT_EQUAL(generateValidMoves(maze, center), expected);
//}

//STUDENT_TEST("generateValidMoves on the side of 2x2 grid with walls") {
//    Grid<bool> maze = {{true, false},
//                       {true, true}};
//    GridLocation center = {1, 1};
//    Set<GridLocation> expected = {{1, 0}};

//    EXPECT_EQUAL(generateValidMoves(maze, center), expected);
//}

//STUDENT_TEST("generateValidMoves on the side of 1x1 grid") {
//    Grid<bool> maze = {{true}
//                       };
//    GridLocation center = {0, 0};
//    Set<GridLocation> expected = {};

//    EXPECT_EQUAL(generateValidMoves(maze, center), expected);
//}

//PROVIDED_TEST("validatePath on correct solution") {
//    Grid<bool> maze = {{true, false},
//                       {true, true}};
//    Stack<GridLocation> soln = { {0 ,0}, {1, 0}, {1, 1} };
//    EXPECT_NO_ERROR(validatePath(maze, soln));
//}

//PROVIDED_TEST("validatePath on correct solution loaded from file for medium maze") {
//    Grid<bool> maze;
//    Stack<GridLocation> soln;
//    readMazeFile("res/5x7.maze", maze);
//    readSolutionFile("res/5x7.soln", soln);

//    EXPECT_NO_ERROR(validatePath(maze, soln));
//}

//PROVIDED_TEST("validatePath on correct solution loaded from file for large maze") {
//    Grid<bool> maze;
//    Stack<GridLocation> soln;
//    readMazeFile("res/25x33.maze", maze);
//    readSolutionFile("res/25x33.soln", soln);

//    EXPECT_NO_ERROR(validatePath(maze, soln));
//}

//PROVIDED_TEST("validatePath on invalid path should raise error") {
//    Grid<bool> maze = {{true, false},
//                       {true, true}};
//    Stack<GridLocation> not_end_at_exit = { {1, 0}, {0, 0} };
//    Stack<GridLocation> not_begin_at_entry = { {1, 0}, {1, 1} };
//    Stack<GridLocation> go_through_wall = { {0 ,0}, {0, 1}, {1, 1} };
//    Stack<GridLocation> teleport = { {0 ,0}, {1, 1} };
//    Stack<GridLocation> revisit = {{0 ,0}, {1, 0}, {0, 0}, {1, 0}, {1, 1}};

//    EXPECT_ERROR(validatePath(maze, not_end_at_exit));
//    EXPECT_ERROR(validatePath(maze, not_begin_at_entry));
//    EXPECT_ERROR(validatePath(maze, go_through_wall));
//    EXPECT_ERROR(validatePath(maze, teleport));
//    EXPECT_ERROR(validatePath(maze, revisit));
//}

//PROVIDED_TEST("solveMaze on file 5x7") {
//    Grid<bool> maze;
//    readMazeFile("res/5x7.maze", maze);
//    Stack<GridLocation> soln = solveMaze(maze);

//    EXPECT_NO_ERROR(validatePath(maze, soln));
//}

PROVIDED_TEST("solveMaze on file 21x23") {
    Grid<bool> maze;
    readMazeFile("res/21x23.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

// TODO: add your test cases here
