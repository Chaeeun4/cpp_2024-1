#include <iostream>
#include <string>
#include "Matrix.h"

using namespace std;

void drawMatrix(Matrix *m) {
  int dy = m->get_dy();
  int dx = m->get_dx();
  int **array = m->get_array();
  for (int y=0; y < dy; y++) {
    for (int x=0; x < dx; x++) {
      if (array[y][x] == 0) cout << "□ ";
      else if (array[y][x] == 1) cout << "■ ";
      else cout << "X ";
    }
    cout << endl;
  }
}

int main(int argc, char *argv[]) {
  // count the number of Matrix objects alive
  {     
    //new를 안 붙이면 스택 할당, {} brace안에서 할당 및 해제를 할 수 있음
    Matrix A(10,10);
  }

  Matrix* m[1000] = { new Matrix(10,10), }; // 힙 할당 1개 m[0] = new Matrix(10, 10) 
  for (int i=1; i<1000; i++)
    m[i] = new Matrix(10, 10); // 힙 할당 999개

  for (int i=0; i<999; i++) // 999개 해제
    delete m[i];

  cout << "nAlloc=" << Matrix::get_nAlloc() << endl;
  cout << "nFree=" << Matrix::get_nFree() << endl;


  // use member functions of Matrix class
  int arrayScreen[6][12] = {
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
  };
  int arrayBlk[3][3] = {
    { 0, 1, 0 },
    { 1, 1, 1 },
    { 0, 0, 0 },
  };
  Matrix *oScreen = new Matrix((int *) arrayScreen, 6, 12);
  cout << "oScreen:" << endl;
  drawMatrix(oScreen); cout << endl;
 
  //생성자를 부를 때는 ? 일차원 배열로 변환해서 줘야 한다 ?
  Matrix *currBlk = new Matrix((int *) arrayBlk, 3, 3);
  cout << "currBlk:" << endl;
  drawMatrix(currBlk); cout << endl;

  //cilp 함수 사용
  int top = 0;
  int left = 4;
  Matrix *tempBlk = oScreen->clip(top, left, top+currBlk->get_dy(), left+currBlk->get_dx());
  cout << "tempBlk (after clip):" << endl;
  drawMatrix(tempBlk); cout << endl;
  
  // add 함수 사용
  Matrix *tempBlk2 = tempBlk->add(currBlk); 
  cout << "tempBlk (after add):" << endl;
  drawMatrix(tempBlk2); cout << endl;

  // paste 함수 사용
  oScreen->paste(tempBlk2, top, left);
  cout << "oScreen (after paste):" << endl;
  drawMatrix(oScreen); cout << endl;

  cout << "currBlk->sum()=" << currBlk->sum() << endl;
  cout << endl;

  tempBlk2->mulc(2);
  cout << "tempBlk2 (after mulc):" << endl;
  tempBlk2->print(); cout << endl;

  cout << "currBlk->anyGreaterThan(1)=" << currBlk->anyGreaterThan(1) << endl;
  cout << "tempBlk->anyGreaterThan(1)=" << tempBlk->anyGreaterThan(1) << endl;
  cout << "tempBlk2->anyGreaterThan(1)=" << tempBlk2->anyGreaterThan(1) << endl;

  cout << "nAlloc=" << Matrix::get_nAlloc() << endl;
  cout << "nFree=" << Matrix::get_nFree() << endl;
  return 0;
}


