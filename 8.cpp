#include<iostream>
#include<cstdlib>
#include<ctime>

using namespace std;

#define SIZE 10
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define SPACE 0
#define WALL 1
#define DEAD_END -1

class Grid
{
public:
	Grid()
	{
		Grid(0);
	}
	Grid(int s)
	{
		state = s;
		dir[UP] = NULL;
		dir[DOWN] = NULL;
		dir[LEFT] = NULL;
		dir[RIGHT] = NULL;
	}
	Grid *getDir(int d) { return dir[d]; }
	int getState() { return state; }
	void setDir(int d, Grid *g) { dir[d] = g; }
	void setState(int s) { state = s;}
  Grid* operator=(Grid* p) {
    state = p->getState();
    dir[UP] = p->getDir(UP);
    dir[DOWN] = p->getDir(DOWN);
    dir[LEFT] = p->getDir(LEFT);
    dir[RIGHT] = p->getDir(RIGHT);
    return this;
  }

  int operator==(Grid* p) {
    return state == p->getState() && dir[UP] == p->getDir(UP) && dir[DOWN] == p->getDir(DOWN) 
      && dir[LEFT] == p->getDir(LEFT) && dir[RIGHT] == p->getDir(RIGHT);
  }
private:
	Grid *dir[4];
	int state;
};

struct List
{
public:
	List()
	{
		top = 0;
	}
	/*
	Insert an element from top
	*/
	void addElement(Grid *g)
	{
    if (top < SIZE * SIZE) {
      data[top] = g;
      top++;
    }
	}
	/*
	remove an element from top and return a pointer point to the element.
	If list is empty, return NULL.
	*/
	Grid *removeElement()
	{
    return (top > 0) ? data[--top] : nullptr;
	}
	void printPath()
	{
		int j;
		for(j = 1;j < top;j ++)
		{
			if(data[j] == data[j - 1]->getDir(UP))
			{
				cout<<"UP\n";
			}
			else if(data[j] == data[j - 1]->getDir(DOWN))
			{
				cout<<"DOWN\n";
			}
			else if(data[j] == data[j - 1]->getDir(LEFT))
			{
				cout<<"LEFT\n";
			}
			else if(data[j] == data[j - 1]->getDir(RIGHT))
			{
				cout<<"RIGHT\n";
			}
		}
	}

  Grid* getTop() {
    return data[top - 1];
  }

  int isEmpty() {
    return top == 0;
  }
private:
	Grid *data[SIZE * SIZE];
	int top;
};

class Maze
{
public:
	Maze()
	{
		initMaze(SIZE);
	}
  ~Maze() {
    delete[] maze;
  }
	/*
	function initMaze
	Allocate a 2-D link list with s * s sizes as the map of maze.
	Inside the maze enery gird with state 0 represent empty space and 1 represent wall.
	The gird in left top is start point and right bottom is finish point.
	Randomly generate 20% wall in the maze.
	Make sure start point and finish point are 0

	動態配置一個二維的鏈結串列，大小是 s * s，用這個鏈結串列表示迷宮的地圖
	節點中 0 的值表示可以移動的空間， 1 表示牆壁 
	左上角的位置表示起點，右下角的位置表示終點
	請在迷宮中加入 20% 的牆壁 
	然後確定起點跟終點都是可以移動的位置 
	*/
	
  void initMaze(int s)
  {
    int i, j, wallCount = 0;
    double percentageWall = 0;
    srand(time(NULL));

    maze = new Grid[s * s];  // 1D array representing 2D grid

    for (i = 0; i < s; ++i) {
      for (j = 0; j < s; ++j) {
        int index = i * s + j;
        percentageWall = static_cast<double>(wallCount + 2) * 100 / (s * s);

        if (percentageWall < 20 && !((!i && !j) || (i == s - 1 && j == s - 1)))
          maze[index].setState(rand() % 100 < 20);
        else 
          maze[index].setState(SPACE);
        wallCount += maze[index].getState();

        // Set directional pointers
        if (i > 0) maze[index].setDir(UP, &maze[(i - 1) * s + j]);
        if (i < s - 1) maze[index].setDir(DOWN, &maze[(i + 1) * s + j]);
        if (j > 0) maze[index].setDir(LEFT, &maze[i * s + (j - 1)]);
        if (j < s - 1) maze[index].setDir(RIGHT, &maze[i * s + (j + 1)]);
    }
    }

    // Test if 20% wall has been generated
    // std::cout << "Maze Size: " << s << std::endl;
    // std::cout << "Wall Count: " << wallCount << std::endl;
    // std::cout << "Percentage wall: " << percentageWall << "%" << std::endl;
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

  int isDeadEnd(Grid *p) {
    return (!p->getDir(UP) || p->getDir(UP)->getState() == WALL) 
      && (!p->getDir(DOWN) || p->getDir(DOWN)->getState() == WALL)
      && (!p->getDir(LEFT) || p->getDir(LEFT)->getState() == WALL)
      && (!p->getDir(RIGHT) || p->getDir(RIGHT)->getState() == WALL);
  }

  int findDirection(Grid* p) {
    if (isDeadEnd(p)) 
      return DEAD_END;

    return (p->getDir(RIGHT) && p->getDir(RIGHT)->getState() != WALL) ? RIGHT
      : (p->getDir(DOWN) && p->getDir(DOWN)->getState() != WALL) ? DOWN
      : (p->getDir(LEFT) && p->getDir(LEFT)->getState() != WALL) ? LEFT
      : UP;
  }

  void move(Grid** p, int direction) {
    if (direction < 0 || direction > 3) return;
    (*p)->setState(WALL);
    *p = (*p)->getDir(direction);
  }

  int mazeCompleted(Grid * p) {
    return p->getDir(RIGHT) == NULL && p->getDir(DOWN) == NULL;
  }


  void backtrack(List* path) {
    Grid* currentPos = path->getTop();
    
    if (currentPos == &maze[0]) return;

    while (isDeadEnd(currentPos)) {
      currentPos->setState(WALL);  
      path->removeElement();
      currentPos = path->getTop(); 
    }
  }

  List* getPath() {
    int direction;
    List* path = new List();
    path->addElement(&maze[0]);

    Grid* currentPos = path->getTop();

    while (currentPos && !mazeCompleted(currentPos) && !path->isEmpty()) {
      direction = findDirection(currentPos);

      if (direction != DEAD_END) {
        path->addElement(currentPos);
        move(&currentPos, direction);
        currentPos->setState(WALL);
      } else {
        backtrack(path);
        currentPos = path->getTop();
      }
      // printMaze();
      // std::cout << std::endl;
    }

    return path;
  }

  void prettyPrint(Grid** p) {
    int i, j;
    for( i = 0; i < SIZE; i++) 
      std::cout << "_";
    std::cout << std::endl;

    for (i = 0; i < SIZE; ++i) {
      for (j = 0; j < SIZE; ++j) {
        if (p && *p == &maze[i * SIZE + j]) {
          std::cout << "X";
        }
        else {
          std::cout << maze[i * SIZE + j].getState();
        }
      }
      std::cout << std::endl;
    }

    for( i = 0; i < SIZE; i++) 
      std::cout << "_";
    std::cout << std::endl;
  }

	void printMaze()
	{
		Grid *j = maze, *k;
		while(j != NULL)
		{
			k = j;
			while(k != NULL)
			{
				cout<<k->getState();
				k = k->getDir(RIGHT); 
			}
			cout<<endl;
			j = j->getDir(DOWN);
		}
	}
private:
	Grid *maze;
};

int main()
{
	Maze *maze = new Maze();
	maze->printMaze();
	maze->getPath()->printPath();

  delete maze;
  return 0;
}
