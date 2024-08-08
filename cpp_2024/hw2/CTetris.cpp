#include "CTetris.h"

using namespace std;

///**************************************************************
/// static member variables and functions
///**************************************************************

Matrix *** CTetris::setOfColorBlockObjects = NULL;

void CTetris::init(int **setOfColorBlockArrays, int nTypes, int nDegrees) {
  if (setOfColorBlockObjects != NULL) // already allocated?
    deinit();

  Tetris::init(setOfColorBlockArrays, nTypes, nDegrees); // call superclass' function

  // write the rest of this function!!
  // setOfcolorBlockArrays를 전달하면 어떻게 될까?
  numTypes = nTypes;
  numDegrees = nDegrees;

  setOfColorBlockObjects = new Matrix**[numTypes]; // 색상 블록 객체 배열 할당

  for (int t = 0; t < numTypes; t++) {
    setOfColorBlockObjects[t] = new Matrix*[numDegrees]; // 색상 블록 객체 배열 할당

    for (int d = 0; d < numDegrees; d++) {
      int *array = setOfColorBlockArrays[nDegrees * t + d]; // 배열 선택

      int idx, size;
      for (idx = 0; array[idx] != -1; idx++); // 배열의 크기 구하기
        for (size = 0; size * size < idx; size++); // 배열의 크기에 맞는 행렬 크기 구하기
          wallDepth = (size > wallDepth ? size : wallDepth); // 벽의 깊이 설정

          int *blockData = new int[size * size + 1]; // 블록 데이터 배열 동적 할당
          for (int k = 0; k < size * size; k++) {
            blockData[k] = array[k]; // 블록 데이터 복사
          }
          blockData[size * size] = -1; // 배열의 끝을 나타내는 값 설정

          setOfColorBlockObjects[t][d] = new Matrix(blockData, size, size); // 색상 블록 객체 생성
          delete[] blockData; // 블록 데이터 배열 메모리 해제
    }
  }  
  
}

void CTetris::deinit(void) {
  Tetris::deinit(); // call superclass' function

  for (int t = 0; t < numTypes; t++)
    for (int d = 0; d < numDegrees; d++)
      delete setOfColorBlockObjects[t][d]; // deallocate matrix objects

  for (int t = 0; t < numTypes; t++)
    delete [] setOfColorBlockObjects[t]; // deallocate 1d array of Matrix pointers

  delete []  setOfColorBlockObjects; // deallocate 1d array of pointers to Matrix pointer arrays

  setOfColorBlockObjects = NULL;
}

///**************************************************************
/// dynamic member variables and functions
///**************************************************************

/// constructors and destructor

CTetris::CTetris(int cy, int cx) : Tetris(cy, cx) {
  iCScreen = new Matrix(iScreen);
  oCScreen = new Matrix(oScreen);
  currCBlk = NULL;
}

CTetris::~CTetris() {
  delete iCScreen;
  delete oCScreen;
}

/// mutators
TetrisState CTetris::accept(char key) {

  TetrisState _state;

  // complete this function!!
  
  _state = Tetris::accept(key); // call superclass' function


  if (_state == TetrisState::Finished)
    return _state;

  else if (_state == TetrisState::NewBlock) {
    if(key == ' '){
      Matrix *tempCBlk = iCScreen->clip(top, left, top + currCBlk->get_dy(), left + currCBlk->get_dx());
      Matrix *tempCBlk2 = tempCBlk->add(currCBlk);
      delete tempCBlk;

      oCScreen->paste(iCScreen, 0, 0);
      oCScreen->paste(tempCBlk2, top, left);
      delete tempCBlk2;
    }
    oCScreen = deleteFullLines(oCScreen, currCBlk, top, wallDepth);
    iCScreen->paste(oCScreen, 0, 0);
    return _state;
  }

  else if (_state == TetrisState::Running) {
    int idx = key - '0';
    if(idx > 0 || idx <= numTypes){
      currCBlk = setOfColorBlockObjects[type][degree];
      Matrix *tempCBlk = iCScreen->clip(top, left, top + currCBlk->get_dy(), left + currCBlk->get_dx());
      Matrix *tempCBlk2 = tempCBlk->add(currCBlk);
      delete tempCBlk;

      // update oScreen before conflict test
      oCScreen->paste(iCScreen, 0, 0);
      oCScreen->paste(tempCBlk2, top, left);
      delete tempCBlk2;
      return _state = TetrisState::Running;
    }

    _state = TetrisState::Running;
    switch (key) { // perform the requested action
      case 'a': break;
      case 'd': break;
      case 'w':
        currCBlk = setOfColorBlockObjects[type][degree]; 
        break;
      case 's': break;
      case ' ': break;
      default: cout << "CTetris::accept: wrong key input" << endl;
    }
    //cout << "C" << top << left << type << endl;

    Matrix *tempCBlk = iCScreen->clip(top, left, top + currCBlk->get_dy(), left + currCBlk->get_dx());
    Matrix *tempCBlk2 = tempCBlk->add(currCBlk);
    delete tempCBlk;

    oCScreen->paste(iCScreen, 0, 0);
    oCScreen->paste(tempCBlk2, top, left);
    delete tempCBlk2;

    return _state;
  }
  
  return _state;
}
