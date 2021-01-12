// This header contains a Mask class which can help to answer
// Questions 3 and 4 efficiently. It is not necessary to understand 
// its detailed operation. It is better to understand how to use
// the class effectively, as described in the specification in 
// the section "More about using masks"

class Mask {
 private:
  bool mask[HEIGHT][WIDTH];

 public:
  Mask(bool value = true) {
    set_all(value);
  }
  
  void set_all(bool value) {
    for(int r=0; r<HEIGHT; r++) 
      for (int c=0; c<WIDTH; c++)
        mask[r][c] = value;
  }

  bool get_element(int row, int col) const {
    assert(row >= 0 && row <= HEIGHT);
    assert(col >= 0 && col <= WIDTH);
    return mask[row][col];
  }

  void set_element(int row, int col, bool value) {
    assert(row >= 0 && row <= HEIGHT);
    assert(col >= 0 && col <= WIDTH);
    mask[row][col] = value;
  } 

  void print() const {
    for(int r=0; r<HEIGHT; r++) {
      for (int c=0; c<WIDTH; c++)
	std::cout << bool_to_int(mask[r][c]);
      std::cout << std::endl;
    } 
  }

  Mask neighbourhood(int d = 1) const {
    Mask output(false);
    for(int r=0; r<HEIGHT; r++) 
      for (int c=0; c<WIDTH; c++)
        if (mask[r][c])
  	  output.set_region(r-d, c-d, r+d, c+d, true);      
    return output;
  }

  int count() const {
    int result = 0;
    for(int r=0; r<HEIGHT; r++) 
      for (int c=0; c<WIDTH; c++)
        result += bool_to_int(mask[r][c]);
    return result; 
  }

  bool get_position(bool value, int &row, int &col) const {
    for (int r=0; r<HEIGHT; r++)
      for (int c=0; c<WIDTH; c++)
        if (mask[r][c] == value) {
 	  row = r;
	  col = c;
	  return true;
        }
     row = col = -1;
     return false;
  }

  void intersect_with(const Mask &other) {
    *this = binary_operation(other, op_and);
  }

  void union_with(const Mask &other) {
    *this = binary_operation(other, op_or);
  }

  Mask operator*(const Mask &other) {
    return binary_operation(other, op_and);
  }

  Mask &operator*=(const Mask &other) {
    *this = binary_operation(other, op_and);
    return *this;
  }
  
  Mask operator+(const Mask &other) {
    return binary_operation(other, op_or);
  }

  Mask &operator+=(const Mask &other) {
    *this = binary_operation(other, op_or);
    return *this;
  }

  Mask operator-(const Mask &other) {
    return binary_operation(other, op_subtract);
  }

  Mask &operator-=(const Mask &other) {
    *this = binary_operation(other, op_subtract);
    return *this;
  }
  
  Mask operator!() const {
    return unary_operation(op_not);
  }
  
  bool *operator[](int n) { return mask[n]; }

 private:

  static bool op_and(bool one, bool two) {
    return one && two;
  }

  static bool op_subtract(bool one, bool two) {
    return one && !two;
  }

  static bool op_or(bool one, bool two) {
    return one || two;
  }

  static bool op_not(bool input) {
    return !input;
  }

  static int bool_to_int(bool input) {
    return (input) ? 1 : 0;
  }

  static void clip(int &val, int min, int max) {
    val = (val < min) ? min : val;
    val = (val > max) ? max : val;
  }

  void set_region(int tl_row, int tl_col, int br_row, int br_col, bool value) {
  clip (tl_row, 0, HEIGHT-1);
  clip (tl_col, 0, WIDTH-1);
  clip (br_row, 0, HEIGHT-1);
  clip (br_col, 0, WIDTH-1);
  for (int r=tl_row; r<=br_row; r++)
    for (int c=tl_col; c<=br_col; c++)
      set_element(r, c, value);
  }

  Mask binary_operation(const Mask other, bool (*op)(bool,bool)) const {
    Mask answer;
    for (int r=0; r<HEIGHT; r++)
      for (int c=0; c<WIDTH; c++)
	answer[r][c] = op(get_element(r,c), other.get_element(r,c));
    return answer;
  }
 
  Mask unary_operation( bool (*op) (bool) ) const {
    Mask answer;
    for (int r=0; r<HEIGHT; r++)
      for (int c=0; c<WIDTH; c++)
	answer[r][c] = op(get_element(r,c));
    return answer;
  } 
 
};
