#include <iostream>


// Write a recursive program to solve the 4x4 Sudoku game.
// The program should be able to solve the following puzzle:
// 1 2 3 4
// 3 4 0 0
// 2 0 4 0
// 4 0 0 1

// 1 2 3 4
// 3 4 1 0
// 2 0 4 0
// 4 0 0 1

// 1 2 3 4
// 3 4 1 2
// 2 0 1 0
// 4 0 0 1

#define SIZE 4
#define SPACE 0
#define SOLVED 1

using namespace std;

class SudokuBoard {
public:
  SudokuBoard() {};
  ~SudokuBoard() {};

  void setCell(int row, int col, int data) {
    if (row >= SIZE || col >= SIZE || data > SIZE) return;
    board[row][col] = data;
  }
  
  int getCell(int row, int col) const {
    return (row >= SIZE || col >= SIZE) ? SPACE : board[row][col];
  }

  void setState(int value) { state = value; }
  int getState() const { return state; };
  
  int isValid(int value) const {
    return (value <= SIZE && value > 0);
  }

  void fillBoard() {
    int i, j, temp;
    cout << "Entering board values..." << endl;
    for (i = 0; i < SIZE; ++i) {
      for (j = 0; j < SIZE; ++j) {
        temp = SIZE + 1;
        while (!isValid(temp) || temp == SPACE ) {
          cout << "BOARD[" << i << "][" << j << "] : ";
          cin >> temp;
          cout << temp << endl;
        }
        board[i][j] = temp;
      }
    }
  };

  int isSolved() const {
    int i, j;
    for (i = 0; i < SIZE; ++i)
      for (j = 0; j < SIZE; ++j)
        if (board[i][j] == SPACE) 
          return !SOLVED;
  };

  friend ostream& operator<<(ostream& out, SudokuBoard& s) {
    int i, j;
    
    for (i = 0; i < SIZE; ++i) {
      for (j = 0; j < SIZE; ++j) {
        out << s.board[i][j];
        if (j < SIZE - 1) cout << " ";
      }
      if (i < SIZE - 1) cout << endl;
    }

    return out;
  };

private:
  int board[SIZE][SIZE];
  int state;
};

int main (int argc, char *argv[]) {
  SudokuBoard b;
  b.fillBoard();
  cout << b << endl;

  return 0;
}

