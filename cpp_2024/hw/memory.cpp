#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stdio.h>
#include <termios.h>

#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

#include "colors.h"
#include "Matrix.h"

using namespace std;


/**************************************************************/
/**************** Linux System Functions **********************/
/**************************************************************/

char saved_key = 0;
int tty_raw(int fd);	/* put terminal into a raw mode */
int tty_reset(int fd);	/* restore terminal's mode */
  
/* Read 1 character - echo defines echo mode */
char getch() {
  char ch;
  int n;
  while (1) {
    tty_raw(0);
    n = read(0, &ch, 1);
    tty_reset(0);
    if (n > 0)
      break;
    else if (n < 0) {
      if (errno == EINTR) {
        if (saved_key != 0) {
          ch = saved_key;
          saved_key = 0;
          break;
        }
      }
    }
  }
  return ch;
}

void sigint_handler(int signo) {
  // cout << "SIGINT received!" << endl;
  // do nothing;
}

void sigalrm_handler(int signo) {
  alarm(1);
  saved_key = 's';
}

void registerInterrupt() {
  struct sigaction act, oact;
  act.sa_handler = sigint_handler;
  sigemptyset(&act.sa_mask);
#ifdef SA_INTERRUPT
  act.sa_flags = SA_INTERRUPT;
#else
  act.sa_flags = 0;
#endif
  if (sigaction(SIGINT, &act, &oact) < 0) {
    cerr << "sigaction error" << endl;
    exit(1);
  }
}

void registerAlarm() {
  struct sigaction act, oact;
  act.sa_handler = sigalrm_handler;
  sigemptyset(&act.sa_mask);
#ifdef SA_INTERRUPT
  act.sa_flags = SA_INTERRUPT;
#else
  act.sa_flags = 0;
#endif
  if (sigaction(SIGALRM, &act, &oact) < 0) {
    cerr << "sigaction error" << endl;
    exit(1);
  }
  alarm(1);
}

/**************************************************************/
/**************** Tetris Blocks Definitions *******************/
/**************************************************************/
#define MAX_BLK_TYPES 7
#define MAX_BLK_DEGREES 4

int T0D0[] = { 1, 1, 1, 1, -1 };
int T0D1[] = { 1, 1, 1, 1, -1 };
int T0D2[] = { 1, 1, 1, 1, -1 };
int T0D3[] = { 1, 1, 1, 1, -1 };

int T1D0[] = { 0, 1, 0, 1, 1, 1, 0, 0, 0, -1 };
int T1D1[] = { 0, 1, 0, 0, 1, 1, 0, 1, 0, -1 };
int T1D2[] = { 0, 0, 0, 1, 1, 1, 0, 1, 0, -1 };
int T1D3[] = { 0, 1, 0, 1, 1, 0, 0, 1, 0, -1 };

int T2D0[] = { 1, 0, 0, 1, 1, 1, 0, 0, 0, -1 };
int T2D1[] = { 0, 1, 1, 0, 1, 0, 0, 1, 0, -1 };
int T2D2[] = { 0, 0, 0, 1, 1, 1, 0, 0, 1, -1 };
int T2D3[] = { 0, 1, 0, 0, 1, 0, 1, 1, 0, -1 };

int T3D0[] = { 0, 0, 1, 1, 1, 1, 0, 0, 0, -1 };
int T3D1[] = { 0, 1, 0, 0, 1, 0, 0, 1, 1, -1 };
int T3D2[] = { 0, 0, 0, 1, 1, 1, 1, 0, 0, -1 };
int T3D3[] = { 1, 1, 0, 0, 1, 0, 0, 1, 0, -1 };

int T4D0[] = { 0, 1, 0, 1, 1, 0, 1, 0, 0, -1 };
int T4D1[] = { 1, 1, 0, 0, 1, 1, 0, 0, 0, -1 };
int T4D2[] = { 0, 1, 0, 1, 1, 0, 1, 0, 0, -1 };
int T4D3[] = { 1, 1, 0, 0, 1, 1, 0, 0, 0, -1 };

int T5D0[] = { 0, 1, 0, 0, 1, 1, 0, 0, 1, -1 };
int T5D1[] = { 0, 0, 0, 0, 1, 1, 1, 1, 0, -1 };
int T5D2[] = { 0, 1, 0, 0, 1, 1, 0, 0, 1, -1 };
int T5D3[] = { 0, 0, 0, 0, 1, 1, 1, 1, 0, -1 };

int T6D0[] = { 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, -1 };
int T6D1[] = { 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, -1 };
int T6D2[] = { 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, -1 };
int T6D3[] = { 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, -1 };
  
int *setOfBlockArrays[] = {
  T0D0, T0D1, T0D2, T0D3,
  T1D0, T1D1, T1D2, T1D3,
  T2D0, T2D1, T2D2, T2D3,
  T3D0, T3D1, T3D2, T3D3,
  T4D0, T4D1, T4D2, T4D3,
  T5D0, T5D1, T5D2, T5D3,
  T6D0, T6D1, T6D2, T6D3,
};

void drawScreen(Matrix *screen, int wall_depth)
{
  int dy = screen->get_dy();
  int dx = screen->get_dx();
  int dw = wall_depth;
  int **array = screen->get_array();

  for (int y = 0; y < dy - dw + 1; y++) {
    for (int x = dw - 1; x < dx - dw + 1; x++) {
      if (array[y][x] == 0)
	      cout << "□ ";
      else if (array[y][x] == 1)
	      cout << "■ ";
      else if (array[y][x] == 10)
	      cout << "◈ ";
      else if (array[y][x] == 20)
	      cout << "★ ";
      else if (array[y][x] == 30)
	      cout << "● ";
      else if (array[y][x] == 40)
	      cout << "◆ ";
      else if (array[y][x] == 50)
	      cout << "▲ ";
      else if (array[y][x] == 60)
	      cout << "♣ ";
      else if (array[y][x] == 70)
	      cout << "♥ ";
      else
	      cout << "X ";
    }
    cout << endl;
  }
}
  
/**************************************************************/
/******************** Tetris Main Loop ************************/
/**************************************************************/

#define SCREEN_DY  10
#define SCREEN_DX  10
#define SCREEN_DW  4

#define ARRAY_DY (SCREEN_DY + SCREEN_DW)
#define ARRAY_DX (SCREEN_DX + 2*SCREEN_DW)

int arrayScreen[ARRAY_DY][ARRAY_DX] = {
  { 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1 },
  { 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1 },
  { 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1 },
  { 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1 },
  { 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1 },
  { 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1 },
  { 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1 },
  { 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1 },
  { 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1 },
  { 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1 },
  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },  
};

int arrayBlk[3][3] = {
  { 0, 1, 0 },
  { 1, 1, 1 },
  { 0, 0, 0 },
};

// Full line 삭제 함수
void deleteFullLine(Matrix *screen) {

  int top = SCREEN_DY - 1, left = SCREEN_DW;
  int line_start = -1, line_sum = 0;

  // Full line 찾기
  for (int i = top; i >= 0; i--) {
    Matrix* line = screen->clip(i, left, i + 1, left + SCREEN_DX);
    int sum = line->sum();
    delete line;
    if (sum == SCREEN_DX) { // 만약 라인이 가득 찬 경우
      line_sum++;
      if (line_start == -1) { // 처음 발견한 가득 찬 라인이라면
        line_start = i;
      }
    }
  }

  // 가득 찬 라인을 제외한 나머지 복사
  if (line_sum > 0) {
    Matrix non_full = screen->clip_(0, left, line_start - line_sum + 1, left + SCREEN_DX);
    Matrix oScreen = *screen; // screen을 복사하여 oScreen에 저장
    oScreen.paste(non_full, line_sum, left); // oScreen에 non_full을 paste
    *screen = oScreen; // screen에 oScreen을 할당
    cout << "delete Full Line!!" << endl;
    cout << "line_sum: "<< line_sum << "  line_start: " << line_start << endl;
  }
  
  
}

int main(int argc, char *argv[]) {
  char key;
  int top = 0, left = SCREEN_DW+4;
  int idxBlockType = 0, idxBlockDegree = 0;

  bool newBlockNeeded = false;
  bool collision = false;
  bool gameOver = false;

  srand((unsigned int)time(NULL)); // 1회 호출
  int blkType = rand() % MAX_BLK_TYPES;  // 난수 필요시 호출

  idxBlockType = blkType;

  Matrix *setOfBlockObjects[7][4]; // Matrix 객체의 배열을 저장할 2차원 배열

  // 28개의 Matrix 객체를 할당하고 배열에 저장
  for (int i = 0; i < 7; i++) {
      for (int j = 0; j < 4; j++) {
          int* arr = setOfBlockArrays[i*4 + j];
          int rows, cols;
          switch (i){
            case 0: rows = 2, cols = 2; break;
            case 6: rows = 4, cols = 4; break;
            default: rows = 3, cols = 3; break;
          }
          setOfBlockObjects[i][j] = new Matrix(arr, rows, cols); // Matrix 객체 할당
      }
  }

  Matrix *iScreen = new Matrix((int *) arrayScreen, ARRAY_DY, ARRAY_DX);
  Matrix *currBlk= setOfBlockObjects[idxBlockType][idxBlockDegree];
  Matrix *tempBlk = iScreen->clip(top, left, top + currBlk->get_dy(), left + currBlk->get_dx());
  Matrix *tempBlk2 = tempBlk->add(currBlk);
  delete tempBlk;

  Matrix *oScreen = new Matrix(iScreen);
  oScreen->paste(tempBlk2, top, left);
  delete tempBlk2;
  drawScreen(oScreen, SCREEN_DW);
  delete oScreen;

  while ((key = getch()) != 'q') {
    switch (key) {
      case 'a': left--; break;
      case 'd': left++; break;
      case 's': top++; break;
      case 'w':
        // 블록을 시계 방향으로 회전
        idxBlockDegree = (idxBlockDegree + 1) % 4;
        currBlk= setOfBlockObjects[idxBlockType][idxBlockDegree]; // 새로운 블록을 가져옴
        
        // 충돌이 일어나는지 검사
        tempBlk = iScreen->clip(top, left, top + currBlk->get_dy(), left + currBlk->get_dx());
        tempBlk2 = tempBlk->add(currBlk);
        delete tempBlk;

        if (tempBlk2->anyGreaterThan(1)) {
            //충돌이 일어나면 원래대로 복귀
            idxBlockDegree = (idxBlockDegree + 3) % 4;
            currBlk = setOfBlockObjects[idxBlockType][idxBlockDegree];
          }
        delete tempBlk2;
        break;
      case ' ': 
        while(1){
          top++;
          tempBlk = iScreen->clip(top, left, top + currBlk->get_dy(), left + currBlk->get_dx());
          tempBlk2 = tempBlk->add(currBlk);
          delete tempBlk;
          if (tempBlk2->anyGreaterThan(1)) {
            top--;
            delete tempBlk2;
            break;
          }
        delete tempBlk2;
        } 
        break;
      default: cout << "wrong key input" << endl;
    }

    tempBlk = iScreen->clip(top, left, top + currBlk->get_dy(), left + currBlk->get_dx());
    tempBlk2 = tempBlk->add(currBlk);
    delete tempBlk;

    if (tempBlk2->anyGreaterThan(1)) {
      
      switch (key) {
        case 'a': left++; break;
        case 'd': left--; break;
        case 's': 
          top--;
          newBlockNeeded = true; 
          collision = true;
          break;
        case 'w': break;
        case ' ': break;
      }
      
      delete tempBlk2;
      tempBlk = iScreen->clip(top, left, top + currBlk->get_dy(), left + currBlk->get_dx());
      tempBlk2 = tempBlk->add(currBlk);    
      delete tempBlk;

    }

    oScreen = new Matrix(iScreen);
    oScreen->paste(tempBlk2, top, left);
    delete tempBlk2;
    drawScreen(oScreen, SCREEN_DW);

    if (collision){
      deleteFullLine(oScreen);
      collision = false;
    }

    if (newBlockNeeded){
      delete iScreen;
      iScreen = new Matrix(oScreen);
      delete oScreen;
      newBlockNeeded = false;

      top = 0;
      left = SCREEN_DW+4;
      
      blkType = rand() % MAX_BLK_TYPES; // 난수 필요시 호출
      idxBlockType = blkType;
      idxBlockDegree = 0;

      cout << "blkType: " << blkType << endl;

      currBlk = setOfBlockObjects[idxBlockType][idxBlockDegree];

      tempBlk = iScreen->clip(top, left, top + currBlk->get_dy(), left + currBlk->get_dx());
      tempBlk2 = tempBlk->add(currBlk);
      delete tempBlk; 

      if(tempBlk2->anyGreaterThan(1)){
        gameOver = true;
      }

      oScreen = new Matrix(iScreen);
      oScreen->paste(tempBlk2, top, left);

      delete tempBlk2;
      drawScreen(oScreen, SCREEN_DW);
            
    }
    delete oScreen;

    if(gameOver){
        cout << "GAME OVER" << endl;
        break;
      }
   }

  delete iScreen;
  //delete currBlk;

  // Matrix 객체 할당 해제
  for (int i = 0; i < 7; i++) {
      for (int j = 0; j < 4; j++) {
          if (setOfBlockObjects[i][j] != nullptr) {
              delete setOfBlockObjects[i][j];
              setOfBlockObjects[i][j] = nullptr; // 중복 해제 방지를 위해 nullptr로 설정
          }
      }
  }

  cout << "(nAlloc, nFree) = (" << Matrix::get_nAlloc() << ',' << Matrix::get_nFree() << ")" << endl;  
  cout << "Program terminated!" << endl;

  return 0;
}

