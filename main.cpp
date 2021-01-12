#include <iostream>
#include <cassert>
#include <cstdio>

#include "common.h"
#include "mask.h"
#include "gogen.h"

using namespace std;

int main() {
  /* this section illustrates the use of the pre-supplied helper functions */
  cout << "============== Pre-supplied functions ==================" << endl << endl;
  
  char **board, **words;
  cout << "Loading board from 'board-easy.txt'... ";
  board = load_board("board-easy.txt");
  assert(board);
  cout << "Done!" << endl;
  print_board(board);
  cout << endl;

  cout << "Loading words from 'words-easy.txt'... ";
  words = load_words("words-easy.txt");
  assert(words);
  cout << "Done!" << endl;
  print_words(words);
  cout << endl;

  Mask m(false);
  m.set_element(2, 4, true); // "m[2][4] = true;" also works
  m.set_element(4, 0, true);
  cout << "Here is a mask with " << m.count() << " element(s) set to true: " << endl;
  m.print();
  cout << endl;

  Mask nbr_m = m.neighbourhood();
  cout << "Here is the 1-neighbourhood of the mask: " << endl;
  nbr_m.print();
  cout << endl;

  cout << "See mask.h header for other useful methods e.g." << endl;
  cout << "  intersect_with(...), union_with(...), etc."
       << endl << endl;

  cout << "====================== Question 1 ======================" << endl << endl;

  int row, column;
  if (get_position(board, 'B', row, column))
    cout << "'B' found in row " << row << ", column " << column << endl;
  else
    cout << "'B' could not be found" << endl;

  if (get_position(board, 'O', row, column))
    cout << "'O' found in row " << row << ", column " << column << endl;
  else
    cout << "'O' could not be found" << endl;

  if (get_position(board, 'X', row, column))
    cout << "'X' found in row " << row << ", column " << column << endl;
  else
    cout << "'X' could not be found" << endl;
    
  cout << endl;
  
  cout << "====================== Question 2 ======================" << endl << endl;

  char **solution = load_board("solution-easy.txt");
  cout << "Solution in 'solution-easy.txt' is " << (valid_solution(solution, words) ? "valid" : "invalid") << "!" << endl << endl;
  delete_board(solution);

  solution = load_board("solution-wrong1.txt");
  cout << "Solution in 'solution-wrong1.txt' is " << (valid_solution(solution, words) ? "valid" : "invalid") << "!" << endl << endl;
  delete_board(solution);

  solution = load_board("solution-wrong2.txt");
  cout << "Solution in 'solution-wrong2.txt' is " << (valid_solution(solution, words) ? "valid" : "invalid") << "!" << endl << endl;
  delete_board(solution);

  cout << "====================== Question 3a ======================" << endl << endl;

  Mask x, e, r;
  update(board, 'X', x);
  cout << "Mask for 'X'" << endl;
  x.print();
  cout << endl;
  
  update(board, 'E', e);
  cout << "Mask for 'E'" << endl;
  e.print();
  cout << endl;
  
  update(board, 'R', r);
  cout << "Mask for 'R'" << endl;
  r.print();
  cout << endl;

  cout << "====================== Question 3b ======================" << endl << endl;
  
  neighbourhood_intersect(x, e); // from BOXED
                                 //        ^^
  cout << "After neighbourhood intersection, mask for 'E' is " << endl;
  e.print();
  cout << endl;
  
  neighbourhood_intersect(e, r); // from QUAVER 
                                 //          ^^
  cout << "After further neighbourhood intersection, mask for 'E' is " << endl;
  e.print();
  cout << endl;

  update(board, 'E', e);
  cout << "Board is now " << endl;
  print_board(board);
  cout << endl;
  // if you did this right, your board should now have 'E'
  // in the correct position 

  delete_board(board);
  delete_words(words);
  
  cout << "====================== Question 4 ======================" << endl << endl;

  const char *puzzle[] = {"easy", "medium", "hard", NULL};

  for (int n = 0; puzzle[n] ; n++) {
    cout << "Trying to solve '" << puzzle[n] << "' puzzle:" << endl;
    char board_filename[512], words_filename[512], solution_filename[512];
    sprintf(board_filename, "board-%s.txt", puzzle[n]);
    sprintf(words_filename, "words-%s.txt", puzzle[n]);
    sprintf(solution_filename, "mysolution-%s.txt", puzzle[n]);

    board = load_board(board_filename);
    words = load_words(words_filename);

    if (solve_board(board, words)) {
      cout << "Board solved! Solution: " << endl;
      print_board(board);
      save_board(board, solution_filename);
    } else 
      cout << "Board could not be solved" << endl;
    delete_board(board);
    delete_words(words);
    cout << endl;
  }

  return 0;
}

