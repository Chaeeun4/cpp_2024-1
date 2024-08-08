#include "Matrix.h"

int Matrix::nAlloc = 0;
int Matrix::nFree = 0;

int Matrix::get_nAlloc() { return nAlloc; }

int Matrix::get_nFree() { return nFree; }

int Matrix::get_dy() const { return dy; }

int Matrix::get_dx() const { return dx; }

int **Matrix::get_array() const { return array; }

void Matrix::alloc(int cy, int cx) {
  if ((cy <= 0) || (cx <= 0)) {
	dy = 0;
	dx = 0;
	array = NULL;
    nAlloc++;
    return;
  }
  dy = cy;
  dx = cx;
  array = new int*[dy];
  for (int y = 0; y < dy; y++)
    array[y] = new int[dx];
  
  nAlloc++;
}

Matrix::Matrix() { alloc(0, 0); }

void Matrix::dealloc() {   //포인터를 삭제하는 중이네..!
  if (array != NULL) {
    for (int y = 0; y < dy; y++)
      delete[] array[y];
    delete[] array;
    array = NULL;
  }

  nFree++;
}

Matrix::~Matrix() { dealloc(); }

Matrix::Matrix(int cy, int cx) {
  alloc(cy, cx);
  for (int y = 0; y < dy; y++)
    for (int x = 0; x < dx; x++)
      array[y][x] = 0;
}

// 4.4 thu  inline func  ..? test?
Matrix::Matrix(int cy, int cx, int val) {
  alloc(cy, cx);
  for (int y = 0; y < dy; y++)
    for (int x = 0; x < dx; x++)
      array[y][x] = val;
}

Matrix::Matrix(const Matrix *obj) {
  alloc(obj->dy, obj->dx);
  for (int y = 0; y < dy; y++)
    for (int x = 0; x < dx; x++)
      array[y][x] = obj->array[y][x];
}

// Matrix B(A) 라고 해도 A의 주소로 B를 만듦
Matrix::Matrix(const Matrix &obj) { 
  alloc(obj.dy, obj.dx);
  for (int y = 0; y < dy; y++)
    for (int x = 0; x < dx; x++)
      array[y][x] = obj.array[y][x];
}


Matrix::Matrix(int *arr, int row, int col) {
  alloc(row, col);
  for (int y = 0; y < dy; y++)
    for (int x = 0; x < dx; x++)
      array[y][x] = arr[y * dx + x];
}

// 행렬을 떼오는 함수, 대각 부분 끝 좌표만 받음
Matrix *Matrix::clip(int top, int left, int bottom, int right) {
  int cy = bottom - top;
  int cx = right - left;
  Matrix *temp = new Matrix(cy, cx);
  for (int y = 0; y < cy; y++) {
    for (int x = 0; x < cx; x++) {
      if ((top + y >= 0) && (left + x >= 0) &&
	      (top + y < dy) && (left + x < dx))
      	temp->array[y][x] = array[top + y][left + x];
      else {
      	cerr << "invalid matrix range" << endl;
        delete temp;
      	return NULL;
      }
    }
  }
  return temp;
}

Matrix Matrix::clip_(int top, int left, int bottom, int right) {
  int cy = bottom - top;
  int cx = right - left;
  Matrix temp(cy, cx);
  for (int y = 0; y < cy; y++) {
    for (int x = 0; x < cx; x++) {
      if ((top + y >= 0) && (left + x >= 0) &&
	      (top + y < dy) && (left + x < dx))
      	temp.array[y][x] = array[top + y][left + x];
      else {
      	cerr << "invalid matrix range" << endl;
      	return Matrix();
      }
    }
  }
  return temp;
}


// 행렬 obj를 M의 특정 위치에 붙여넣음
// 포인터를 받음..
void Matrix::paste(const Matrix *obj, int top, int left) {
  for (int y = 0; y < obj->dy; y++)
    for (int x = 0; x < obj->dx; x++) {
      if ((top + y >= 0) && (left + x >= 0) &&
	      (top + y < dy) && (left + x < dx))
	      array[y + top][x + left] = obj->array[y][x];
      else {
	      cerr << "invalid matrix range" << endl;
      }
    }
    cout << "call by value" << endl;
}

// 참조를 받음 (주소를 받음)
void Matrix::paste(const Matrix &obj, int top, int left) {
  for (int y = 0; y < obj.dy; y++)
    for (int x = 0; x < obj.dx; x++) {
      if ((top + y >= 0) && (left + x >= 0) &&
	      (top + y < dy) && (left + x < dx))
	      array[y + top][x + left] = obj.array[y][x];
      else {
	      cerr << "invalid matrix range" << endl;
      }
    }
    cout << "call by reference" << endl;
}

// 행렬합 함수 (행렬 M + 행렬 obj를 리턴)
Matrix *Matrix::add(const Matrix *obj) {
  if ((dx != obj->dx) || (dy != obj->dy)) return NULL;
  Matrix *temp = new Matrix(dy, dx); // new - 포인터
  for (int y = 0; y < dy; y++)
    for (int x = 0; x < dx; x++)
      temp->array[y][x] = array[y][x] + obj->array[y][x];
  return temp;
}

// 4.11 thu friend 함수, 연산자 오버로딩
const Matrix operator+(const Matrix& m1, const Matrix& m2) { // friend function version of operator+ overloading
  if ((m1.dx != m2.dx) || (m1.dy != m2.dy)) return Matrix(); // 포인터가 아닌 객체를 리턴
  // 멤버함수가 아닌데 dy dx array 를 직접적으로 접근, 권한을 주기 위해 Matrix.h의 선언부에 friend를 붙여줌 
  // add 함수는 matrix::add -> matrix의 멤버함수
  // Matrix::operater+ 가 아님, Main 함수 같은 느낌임

  Matrix temp(m1.dy, m1.dx);
  for (int y = 0; y < m1.dy; y++)
    for (int x = 0; x < m1.dx; x++)
      temp.array[y][x] = m1.array[y][x] + m2.array[y][x];
  return temp;  
}

// const Matrix Matrix::operator+(const Matrix& m2) const  { // member function version of operator+ overloading
//   if ((dx != m2.dx) || (dy != m2.dy)) return Matrix();
//   Matrix temp(dy, dx);
//   for (int y = 0; y < dy; y++)
//     for (int x = 0; x < dx; x++)
//       temp.array[y][x] = array[y][x] + m2.array[y][x];
//   return temp;  
// }


// 행렬 안에 존재하는 원소의 산술 합, 정수값을 리턴
int Matrix::sum() {
  int total = 0;
  for (int y = 0; y < dy; y++)
    for (int x = 0; x < dx; x++)
      total += array[y][x];
  return total;
}

//multiply coef, 행렬 M의 모든 원소에 coef를 곱함
void Matrix::mulc(int coef) {
  for (int y = 0; y < dy; y++)
    for (int x = 0; x < dx; x++)
      array[y][x] = coef * array[y][x];
}

Matrix *Matrix::int2bool() {
  Matrix *temp = new Matrix(dy, dx);
  int **t_array = temp->get_array();
  for (int y = 0; y < dy; y++)
    for (int x = 0; x < dx; x++)
      t_array[y][x] = (array[y][x] != 0 ? 1 : 0);
  
  return temp;
}

// 행렬 M에 val 값 보다 큰 값을 갖는 원소가 있으면 true 리턴
bool Matrix::anyGreaterThan(int val) {
  for (int y = 0; y < dy; y++) {
    for (int x = 0; x < dx; x++) {
      if (array[y][x] > val)
	return true;
    }
  }
  return false;
}

void Matrix::print() {
  cout << "Matrix(" << dy << "," << dx << ")" << endl;
  for (int y = 0; y < dy; y++) {
    for (int x = 0; x < dx; x++)
      cout << array[y][x] << " ";
    cout << endl;
  }
}


ostream& operator<<(ostream& out, const Matrix& obj){
  out << "Matrix(" << obj.dy << "," << obj.dx << ")" << endl;
  for(int y = 0; y < obj.dy; y++){
    for(int x = 0; x < obj.dx; x++)
      out << obj.array[y][x] << " ";
    out << endl;
  }
  out << endl;
  return out;
}

Matrix& Matrix::operator=(const Matrix& obj)
{
  if (this == &obj) return *this;
  if ((dx != obj.dx) || (dy != obj.dy)) {
    if (array != NULL) dealloc();
    alloc(obj.dy, obj.dx);
  }
  for (int y = 0; y < dy; y++)
    for (int x = 0; x < dx; x++)
      array[y][x] = obj.array[y][x];
  return *this;
}
