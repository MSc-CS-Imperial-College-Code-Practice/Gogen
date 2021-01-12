#include <iostream>
#include <fstream>
#include <cassert>
#include <cstring>
#include <cctype>
#include <cstdlib>

#include "common.h"
#include "mask.h"
#include "gogen.h"

using namespace std;

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* internal helper function which allocates a dynamic 2D array */
char **allocate_2D_array(int rows, int columns) {
  char **m = new char *[rows];
  assert(m);
  for (int r=0; r<rows; r++) {
    m[r] = new char[columns];
    assert(m[r]);
  }
  return m;
}

/* internal helper function which deallocates a dynamic 2D array */
void deallocate_2D_array(char **m, int rows) {
  for (int r=0; r<rows; r++)
    delete [] m[r];
  delete [] m;
}

/* internal helper function which removes unprintable characters like carriage returns and newlines from strings */
void filter (char *line) {
  while (*line) {
    if (!isprint(*line))
     *line = '\0';
    line++;
  }
}

/* loads a Gogen board from a file */
char **load_board(const char *filename) {
  char **board = allocate_2D_array(5, 6);
  ifstream input(filename);
  assert(input);
  char buffer[512];
  int lines = 0;
  input.getline(buffer, 512);
  while (input && lines < HEIGHT) {
    filter(buffer);
    if (strlen(buffer) != WIDTH)
      cout << "WARNING bad input = [" << buffer << "]" << endl;
    assert(strlen(buffer) == WIDTH);
    strcpy(board[lines], buffer);
    input.getline(buffer, 512);
    lines++;
  }
  input.close();
  return board;
}

/* saves a Gogen board to a file */
bool save_board(char **board, const char *filename) {
  ofstream out(filename); 
  if (!out)
    return false;
  for (int r=0; r<HEIGHT; r++) {
    for (int c=0; c<WIDTH; c++) {
      out << board[r][c];
    }
    out << endl;
  }
  bool result = out.good();
  out.close();
  return result;
}

/* internal helper function for counting number of words in a file */
int count_words(const char *filename) {
  char word[512];
  int count = 0;
  ifstream in(filename);
  while (in >> word)
    count++;
  in.close();
  return count;
}

/* loads a word list from a file into a NULL-terminated array of char *'s */
char **load_words(const char *filename) {
  int count = count_words(filename);
  ifstream in(filename);
  assert(in);
  int n=0;
  char **buffer = new char *[count+1]; // +1 because we NULL terminate 
  char word[512];
  for (; (in >> word) && n<count; n++) {
    buffer[n] = new char[strlen(word) + 1];
    strcpy(buffer[n], word);
  }
  buffer[n] = NULL;
  in.close();
  return buffer;
}

/* prints a Gogen board in appropriate format */
void print_board(char **board) {
  for (int r=0; r<HEIGHT; r++) {
    for (int c=0; c<WIDTH; c++) {
      cout << "[" << board[r][c] << "]";
      if (c < WIDTH-1)
	cout << "--";
    }
    cout <<endl;
    if (r < HEIGHT-1) {
      cout << " | \\/ | \\/ | \\/ | \\/ |" << endl;
      cout << " | /\\ | /\\ | /\\ | /\\ |" << endl;
    }
  }
}

/* prints a NULL-terminated list of words */
void print_words(char **words) {
  for (int n=0; words[n]; n++) 
    cout << words[n] << endl;
}

/* frees up the memory allocated in load_board(...) */
void delete_board(char **board) {
  deallocate_2D_array(board, HEIGHT);
}

/* frees up the memory allocated in load_words(...) */
void delete_words(char **words) {
  int count = 0;
  for (; words[count]; count++);
  deallocate_2D_array(words, count);
}

/* add your own function definitions here */

bool get_position(char **board, char ch, int &row, int &column){

  row = -1;
  column = -1;

  for(int r = 0; r < 5; r++){
    for(int c = 0; c < 5; c++){
      if(board[r][c] == ch){
        row = r;
        column = c;
        return true; 
      }
    }
  }
  return false;
}

bool valid_solution(char **board, char **words){

  char current_letter;
  char next_letter;

  int current_row, current_col;
  int next_row, next_col;
  int h_move, v_move;

  for(int i = 0; words[i] != NULL; i++){
    for(int j = 0; words[i][j] != '\0'; j++){

      current_letter = words[i][j];
      next_letter = words[i][j+1];
      
      if(next_letter == '\0')
        continue;
      
      if(!get_position(board, current_letter, current_row, current_col))
        return false;
      if(!get_position(board, next_letter, next_row, next_col))
        return false;

      h_move = next_col - current_col;
      v_move = next_row - current_row;

      if(abs(h_move)>1 || abs(v_move)>1)
        return false;
    }
  }  
  return true;
}

void update(char **board, char ch, Mask &mask){

  bool is_fixed_letter;
  int fixed_row, fixed_col;

  is_fixed_letter = get_position(board,ch,fixed_row,fixed_col);
  
  // If fixed letter -> ch is on board
  if(is_fixed_letter){
    mask.set_all(false); // flipping all bits
    mask[fixed_row][fixed_col] = true;
  }

  // Else if free letter -> ch is not on the board
  else{
      for(int free_row = 0; free_row < 5; free_row++){
        for(int free_col = 0; free_col < 5; free_col++){
          if(isalpha(board[free_row][free_col]))
            mask[free_row][free_col] = false; 
        }
      }
  }

  // Update board with ch if mask has only "one" true value
  if(mask.count()==1){
      for(int r= 0; r < 5; r++){
        for(int c = 0; c < 5; c++){
          if(mask[r][c]==1)
            board[r][c] = ch; 
        }
      }
    }
}

void neighbourhood_intersect(Mask &one, Mask &two){

  Mask one_neighbourhood = one.neighbourhood();
  Mask two_neighbourhood = two.neighbourhood();

  one.intersect_with(two_neighbourhood);
  two.intersect_with(one_neighbourhood);

}

bool solve_board(char **board, char **words){

  // Auxiliar variables
  Mask masks[25];
  char current_letter, next_letter;
  int current_letter_index,next_letter_index;
  int mask_index;
  int words_count;
  int available_slots;

  // Create array of 25 masks 
  for (char letter = 'A' ; letter <= 'Y' ; letter++){
    mask_index = static_cast<int>(letter)-65;
    update(board, letter, masks[letter-'A']);
  }

  // Update Masks and Board
  words_count = number_of_words(words);
  while (words_count >= 0){
    for(int i = 0; words[i] != NULL; i++){
      for(int j = 0; words[i][j] != '\0'; j++){

        current_letter = words[i][j];
        next_letter = words[i][j+1];
  
        if(next_letter == '\0')
          continue;

        current_letter_index = static_cast<int>(current_letter) - 65;
        next_letter_index = static_cast<int>(next_letter) - 65;
        
        neighbourhood_intersect(masks[current_letter_index], masks[next_letter_index]);
        update(board, current_letter,masks[current_letter_index]);
        update(board, next_letter,masks[next_letter_index]);
  
      } 
    }
    words_count--;
  }

  available_slots = empty_squares(board);
  if(valid_solution(board,words))
    return true;
  else if(solve_board_recursive(board,words,available_slots))
    return true;
  else
    return false;
}


bool solve_board_recursive(char **board, char **words, int available_slots){
  
  // Base Case -> Not more available spots
  if(available_slots == 0)
    return false;
  
  // Recursive Case
  int row, col;
  get_position(board,'.',row,col);
  for (char letter = 'A'; letter <= 'Y'; letter++){
    if(board[row][col] == '.' && !letter_already_in_board(board,letter)){
      
      board[row][col] = letter;
      available_slots--;
      
      // Base Case -> Problem solved
      if(valid_solution(board,words))
        return true;
      
      // Entering Recursion
      if(solve_board_recursive(board, words,available_slots))
        return true;  
     
      available_slots++; // Bactracking
      board[row][col] = '.'; // Bactracking
    }
  
  }

  return false;
}

bool letter_already_in_board(char **board, char letter){

  for(int r = 0; r < 5; r++){
    for(int c = 0; c < 5; c++){
      if(board[r][c] == letter){
        return true; 
      }
    }
  }
  return false;
}

int number_of_words(char **words){
  int count = 0;
  for(int i = 0; words[i] != NULL; i++){
    count++;
  }
  return count;
}

int empty_squares(char **board){
  int count = 0;
  for(int r = 0; r < 5; r++){
    for(int c = 0; c < 5; c++){
      if(board[r][c] == '.'){
        count++; 
      }
    }
  }
  return count;
}

