#include <cstdlib>
#include <ctime>
#include <iostream>

#define SIZE 10
#define UP 8
#define DOWN 2
#define LEFT 4
#define RIGHT 6
#define WALL 1
#define SPACE 0
#define DEAD_END -1

using namespace std;

class Node {
public:
  Node() { Node(0, 0); }
  Node(int r, int c) {
    row = r;
    col = c;
  }
  int getRow() { return row; }
  int getCol() { return col; }
  void setRow(int r) {
    if (r >= 0 && r < SIZE)
      row = r;
  }
  void setCol(int c) {
    if (c >= 0 && c < SIZE)
      col = c;
  }

  friend std::ostream &operator<<(std::ostream &out, Node *n) {
    out << "(" << n->getRow() << ", " << n->getCol() << ")";
    return out;
  }

private:
  int col, row;
};

class List {
public:
  List() : top(0) {}

  void addElement(int r, int c) {
    if (top < SIZE * SIZE) {
      data[top] = Node(r, c);
      top++;
    }
  }

  Node *removeElement() { return (top > 0) ? &data[--top] : nullptr; }

  Node *getTop() { return &data[top - 1]; }
  int isEmpty() { return top == 0; }

  void printList() {
    for (int j = 0; j < top; j++) {
      cout << "(" << data[j].getRow() << ", " << data[j].getCol() << ")"
           << endl;
    }
  }

private:
  Node data[SIZE * SIZE];
  int top;
};

class Maze {
public:
  Maze() { initMaze(SIZE); }
  ~Maze() {
    for (int i = 0; i < SIZE; i++) {
      delete[] maze[i];
    }
    delete[] maze;
  }
  /*
  function initMaze
  Alocate a 2-D array with s * s sizes as the map of maze.
  Inside the maze where 0 represent empty space and 1 represent wall.
  [0][0] is start point and [s - 1][s - 1] is finish point.
  Randomly generate 20% wall in the maze.
  Make sure [0][0] and [s - 1][s - 1] are 0

  動態配置一個二維的陣列，大小是 s * s ，用這個陣列表示迷宮的地圖
  陣列中 0 的值表示可以移動的空間， 1 表示牆壁
  [0][0] 的位置表示起點 [s - 1][s - 1] 的位置表示終點
  請在迷宮中加入 20% 的牆壁
  然後確定起點跟終點都是可以移動的位置
  */
  void initMaze(int s) {
    int i, j, wallCount = 0;
    double percentageWall = 0;
    srand(time(NULL));

    maze = new int *[s];

    for (i = 0; i < s; ++i) {
      maze[i] = new int[s];
      for (j = 0; j < s; ++j) {

        percentageWall = static_cast<double>(wallCount + 2) * 100 / (s * s);

        maze[i][j] =
            (percentageWall < 20 && !((!i && !j) || (i == s - 1 && j == i)))
                ? (rand() % 100 < 20)
                : 0;
        wallCount += maze[i][j];
      }
    }

    // Test if 20% wall has been generated
    // std::cout << "Maze Size: " << s << std::endl;
    // std::cout << "Wall Count: " << wallCount << std::endl;
    // std::cout << "Percentate wall: " << percentageWall << "%" << std::endl;
  }
  /*
  function getPath
  This function will find a path between start point and finish point.
  Return a list content the path information inside.
  If there is no path between two point then the list will be empty.

  這個函數會找到起點到終點間的一條路徑
  回傳一個 list 包含著路徑的資訊
  如果找不到路徑的話 list 就會是空的
  */

  int isDeadEnd(Node *p) {
    int r = p->getRow(), c = p->getCol();
    return (c + 1 < SIZE && maze[r][c + 1] == WALL || c + 1 == SIZE) +
               (r + 1 < SIZE && maze[r + 1][c] == WALL || r + 1 == SIZE) +
               (r - 1 >= 0 && maze[r - 1][c] == WALL || r - 1 < 0) +
               (c - 1 >= 0 && maze[r][c - 1] == WALL || c - 1 < 0) ==
           0;
  }

  int findDirection(Node *p) {
    if (isDeadEnd(p))
      return DEAD_END;

    int r = p->getRow(), c = p->getCol();
    return (c + 1 < SIZE && maze[r][c + 1] != WALL)   ? RIGHT
           : (r + 1 < SIZE && maze[r + 1][c] != WALL) ? DOWN
           : (r - 1 >= 0 && maze[r - 1][c] != WALL)   ? UP
                                                      : LEFT;
  }

  Node move(Node p, int direction) {
    maze[p.getRow()][p.getCol()] = WALL;
    switch (direction) {
    case DOWN:
      p.setRow(p.getRow() + 1);
      break;
    case RIGHT:
      p.setCol(p.getCol() + 1);
      break;
    case LEFT:
      p.setCol(p.getCol() - 1);
      break;
    case UP:
      p.setRow(p.getRow() - 1);
      break;
    default:
      break;
    }
    return p;
  }

  int mazeCompleted(Node *p) {
    int r = p->getRow(), c = p->getCol();
    return (r == SIZE - 1 && c == SIZE - 1) ||
           (r == 0 && c == 0 && maze[0][0] == WALL);
  }

  void backtrack(List *path) {
    Node *currentPos = path->getTop();
    int r = currentPos->getRow(), c = currentPos->getCol();
    if (!r && !c && maze[0][0] == WALL) {
      return;
    }

    while (currentPos && isDeadEnd(currentPos)) {
      maze[currentPos->getRow()][currentPos->getCol()] = WALL;
      path->removeElement();
    }
  }

  List *getPath() {
    int direction;
    List *path = new List();
    path->addElement(0, 0);

    Node *currentPos = path->getTop();
    Node temp;

    while (currentPos && !mazeCompleted(currentPos)) {
      direction = findDirection(currentPos);

      if (direction != DEAD_END) {
        temp = move(*currentPos, direction);
        path->addElement(temp.getRow(), temp.getCol());
        // printMazeModified(currentPos);
        // cin.ignore().get();
      } else {
        backtrack(path);
      }

      currentPos = path->getTop();
    }

    return path;
  }

  void printMazeModified(Node *pos) {
    int j, k;
    for (j = 0; j < SIZE; j++) {
      for (k = 0; k < SIZE; k++) {
        if (j == pos->getRow() && k == pos->getCol())
          cout << "x";
        else if (maze[j][k] == 0)
          cout << " ";
        else if (maze[j][k] == 1)
          cout << "*";
      }
      std::cout << std::endl;
    }

    for (j = 0; j < SIZE; j++)
      std::cout << "_";
    std::cout << std::endl;
  }

  void printMaze() {
    int j, k;
    for (j = 0; j < SIZE; j++) {
      for (k = 0; k < SIZE; k++) {
        if (maze[j][k] == 0)
          cout << " ";
        else if (maze[j][k] == 1)
          cout << "*";
      }
      cout << "\n";
    }
  }

private:
  int **maze;
};

int main() {
  Maze *maze = new Maze();
  maze->printMaze();
  // maze->getPath()->printList();
  List *path = maze->getPath();
  path->printList();

  delete maze;
  delete path;

  return 0;
}
