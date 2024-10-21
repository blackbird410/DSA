#include <iostream>

// TODO: Research how is Sudoku game played

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

class SudokuBoard {
public:
  SudokuBoard();
  SudokuBoard(SudokuBoard &&) = default;
  SudokuBoard(const SudokuBoard &) = default;
  SudokuBoard &operator=(SudokuBoard &&) = default;
  SudokuBoard &operator=(const SudokuBoard &) = default;
  ~SudokuBoard();

private:
  int board[4][4];
  int state;
};

SudokuBoard::SudokuBoard() : state{0} {}

SudokuBoard::~SudokuBoard() {
}
