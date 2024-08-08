#include <iostream>
#include <string>

using namespace std;

int main1(int argc, char *argv[]) {
  string s1 = "Hello, C++!";
  cout << "s1=" << s1 << endl;
  string s0 = "Hello";  
  string s2 = s0 + ", " + "C++" + '!';
  cout << "s2=" << s2 << endl;

  int len1 = s1.length();
  int len2 = s2.length();
  cout << "len1=" << len1 << endl;
  cout << "len2=" << len2 << endl;

  int b1 = s1.compare(s1); // compare s1 and s2 (in content)
  int b2 = s1.compare(s2); // compare s1 and s2 (in content)
  bool b3 = (s1 == s2); // compare s1 and s2 (in content)

  cout << "s1.compare(s1)=" << b1 << endl;
  cout << "s1.compare(s2)=" << b2 << endl;
  cout << "(s1==s2)=" << b3 << endl;
  return 0;
}

// compare 함수는 identity , content 비교가 구분없이 됨, 조심해야 함
// identity 비교: 공간이 같은가    content 비교: 내용물이 같은가
// == 함수, compare 함수 > 리턴값을 다르게 해석해야 함

// 연산자 오버로딩(적재)  s1 ==(함수) s2 조건문에서 참이면 1



//------------------------------------------------------------------------//
// Patch to Cygwin's bug: stoi, stod, to_string are not supported!
#include <cstdlib> // for strtol, strtod
#include <sstream> // for to_string
namespace patch {
  template < typename T > std::string to_string( const T& n )  {
    std::ostringstream stm ;
    stm << n ;
    return stm.str() ;
  }
}
using namespace patch;
//------------------------------------------------------------------------//

int main2(int argc, char *argv[]) {
  string istr = "1234";
  string dstr = "12.34";

  //int ival = stoi(istr); // string -> integer value
  //double dval = stod(dstr); // string -> double value
  int ival = strtol(istr.c_str(), NULL, 10); // string -> integer value
  // 인자 istr, c_str을 호출해서 시작 주소 알아냄, NULL전까지, 이진수 정수로 바꿔라
    // 정수자료형, 변수의 공간 크기를 알아야함!
    // short int 2B, long int 4B, long long int 8B
    // 실수자료형, float 4B, double 8B 
  double dval = strtod(dstr.c_str(), NULL); // string -> double value
  cout << "before : " << ival << ", " << dval << endl;

  ival = ival + 1111;
  dval = dval + 11.11;
  string istr2 = to_string(ival); // integer value -> string
  // = 이 나오면 무조건 좌우의 Dtype이 같아야 한다고 생각해야함
    // 따라서 to_string(ival)은 string을 리턴한다고 알 수 있음
  string dstr2 = to_string(dval); // double value -> string

  cout << "after  : " << istr2 << ", " << dstr2 << endl;
  return 0;
}

void printArray(int a[], int len) {
  if (a != NULL) {
    for (int i = 0; i < len; i++)
      cout << a[i] << " ";
  }
  cout << endl;
}

#include <vector>
#include <iterator>
int main3(int argc, char *argv[]) {
  int* A0 = NULL;  //int A0[] = NULL;
  int A1[] = {1, 2, 3, 4, 5};
  // int* 와 int []은 같은 의미, A의 Dtype은 int*
  int* A2 = new int[5]; //int[] A2 = new int[5];
  int* A3 = new int[5]{1, 2, 3, 4, 5}; //int A3[] = new int[]{1, 2, 3, 4, 5};

  cout << "A0:";  printArray(A0, 0);
  cout << "A1:";  printArray(A1, 5);
  cout << "A2:";  printArray(A2, 5);
  cout << "A3:";  printArray(A3, 5);
  // c++식으로 하려면 원소들을 객체 취급해야 하는데 주소라서 불가능(?) 


  vector<int> v1(A1, A1 + 5); 
  //vector 가변 길이 배열
    //v1(길이 초기화)
    //v1[3] -> 인덱스 방식이 고정 길이 배열과 같음
    //v1[3]의 문법적 변환: v1.[]3    벡터 객체는 오버로딩을 통해..?? A[3] 포인터[] : 포인터 연산
    //v1은 포인터가 아님, 변수임
  vector<int> v3(A3, A3 + 5);  // A3: 첫번째 원소의 주소   A3+5: 배열의 끝주소 (다섯개의 원소만큼 이동한 주소)
                               // 배열이 A3의 원소로 초기화됨
  cout << "equal(v1, v3)=" << equal(v1.begin(), v1.end(), v3.begin()); 
    // equal 함수: 원소 개수가 overlap되는 부분만 비교 
    // 결과 == 1 (content 비교)   cf) (참) compare == 0, == 1
    // 벡터 객체의 content는 v1 v3 동일
  cout << endl;
  
  //03-20 실습
  v1.push_back(10); 
    // push_back 함수는 원소 벡터에 넣어줌, 뒤쪽에 한칸 더 늘리고 10 추가
    // 오류: v1[5] = 10; , 여섯번째 공간이 아직 창조되지 않음
  v1.push_back(20);
  
  //v1의 전체 원소 확인 
  for (int idx = 0; idx < v1.size(); idx++)
    cout << "v1[" << idx << "] = " << v1[idx] << endl;  
  
  //응용 , 2차원 가변 길이 배열
  vector<vector<int>> v2d;
    //배열 선언: vector 안에 int 타입의 원소인 vector를 넣어라
    // 2차원 (가변길이)배열의 원소는 1차원 정수 (가변길이)배열
  v2d.push_back(v1);  
  v2d.push_back(v3);
    // 좀 리스트 같음
   

  for (int idx1 = 0; idx1 < v2d.size(); idx1++){  //idx1 은 0, 1
    for (int idx2 = 0; idx2 < v2d[idx1].size(); idx2++){  //v2d[0] 일떄 idx2는 0~6, v2d[1] 일때 idx2는 0~4
      cout << "v2d[" << idx1 << "][" << idx2 << "] = " << v2d[idx1][idx2] << endl;
    }
  }
  //03-20 실습 끝

  return 0;
}



class Nested {
public: // private:
  int dy; // dynamic variable
  static int dx; // static variable
public:
  Nested(int cy, int cx) { dy = cy; dx = cx; }
  int get_dy() { return dy; } 
    // get_dy는 static을 붙일 수 없음
  static int get_dx() { return dx; } // can be declared 'dynamic'
  // class InnerD {
  // public:
  //   int get_dy() { return dy; }
  //   int sum() { return dy+dx; }
  //   int get_dx() { return dx; }
  // };
  class InnerS {  // An inner class inherently is a static class
                  // even without 'static' keyword
  public:
    int get_dx() { return dx; }  // dx는 outerclass의 dx
  };
};

int Nested::dx = 0;
  //클래스 속 클래스 (둥지를 틀고 있음)

int main4(int argc, char *argv[]) {
  Nested* m1 = new Nested(1, 2);  // new를 쓰면 힙할당? 힙할당은 이름이 없다, 그래서 주소값을 전달함 (m1, m2 포인터가 주소 보관)
    //이곳에서 dx를 출력한다면, 4 아니고 2가 나왔을 거임
  Nested* m2 = new Nested(3, 4);  // 객체는 변수고 변수는 공간 
  cout << "m1->get_dy()=" << m1->get_dy() << ", m1->get_dx()=" << m1->get_dx() << endl;
    //m1 m2가 독립적인 객체면 각각 값이 나올것임, dx는 하나만 존재한당
    //dy 공간만 존재, dx는 class object (클래스 대표 객체 안에 static 변수가 존재) 컴파일러가 자동 생성
    //개별 객체는 클래스 대표 객체에 포인터로 접근할 수 있음
    // 결론 : static 키워드가 붙은 변수는 공유 변수 및 class object 안에 위치
  cout << "m2->get_dy()=" << m2->get_dy() << ", m2->get_dx()=" << m2->get_dx() << endl;
  //Nested::InnerD *d1 = m1::InnerD();
  //Nested::InnerD *d2 = m2::InnerD();
  Nested::InnerS *s = new Nested::InnerS();
  cout << "s->get_dx()=" << s->get_dx() << endl;
    //get_dx() 라고 괄호 안에 아무것도 안쓴다는 것은 생성자를 호출한다는 것인데 InnerS에는 생성자가 없음 -- 디폴드 생성자
    //근데 생성자가 필요하진 않음 (생성자는 멤버변수 초기화 위한 건데 InnerS는 멤버변수가 없음)
    // cout의 실행 결과 : outerclass의 dx를 참조해서 4라고 출력한다

    // static 변수: class 생태계 안에서 걔네가 공유하는 변수, static 함수, static 클래스도 있음
    // static : class 객체에 소속된다 는 것을 의미 (중요해용! 클라우드 인 아키텍쳐..?) 

  return 0;
}

#include "Matrix.h"
class MyMatrix : public Matrix {
  // 상속의 뜻인가 (MyMatrix <- Matrix)
public:
  MyMatrix() : Matrix() {}
   //Mymatrix생성자 : (<-) Matrix 생성자 (?)
  MyMatrix(int cy, int cx) : Matrix(cy, cx) { }
  MyMatrix(int *arr, int col, int row) : Matrix(arr, col, row) { }
    //위 두 함수는 생성자 함수
  void print() {
    // override : 부모 함수를 제끼고 자식 함수를 쓰겠다
    int dy = get_dy();
    int dx = get_dx();
    int **array = get_array();
     // 위 세 함수는 부모가 나한테 준거니까 그냥 쓸 수 있으나, 간접 접근하는 중
     // 원칙(부모 함수를 검용? 하지 말라) 
    for (int y=0; y < dy; y++) {
      for (int x=0; x < dx; x++) {
        if (array[y][x] == 0) cout << "□ ";
        else if (array[y][x] == 1) cout << "■ ";
	else cout << "X ";
      }
      cout << endl;
    }
  }
};

int main5(int argc, char *argv[]) {
  Matrix* m1 = new Matrix(3,3);
  m1->print(); cout << endl;
  int A[] = { 0, 1, 0, 1, 1, 1, 0, 0, 0 }; // int A[][]
  Matrix* m2 = new Matrix(A, 3, 3); // new Matrix(A)
  m2->print(); cout << endl;
  MyMatrix* m3 = new MyMatrix(3,3);  
    // 상속 : 부모 클래스의 코드를 상당 부분 재사용하고 싶을 때
  m3->print(); cout << endl;
  MyMatrix* m4 = new MyMatrix(A, 3, 3); // new Matrix(A)
  m4->print(); cout << endl;
  m2 = m4; // polymorphism: Matrix covers MyMatrix!!
    // 반대는 허용 X
  m2->print(); cout << endl; // static binding!! (정적 바인딩: 문법이 표현하는 것을 그래도 바인딩)
    // m2가 new MyMatrix(A, 3, 3)을 가리킴 근데 Matrix(A, 3, 3)을 가리키는 것도 맞음
    // 같은 메소드 - 부모의 함수가 쓰임 (cpp만), 자바나 파이썬은 자식 함수가 쓰임

  return 0;
}

#include <string.h>
int main(int argc, char *argv[]) {
  if (argc != 2) {
    cout << "usage: " << argv[0] << " [function number]" << endl;
    return 0;
  }
    
  if (strcmp(argv[1], "1") == 0) main1(argc, argv);
  else if (strcmp(argv[1], "2") == 0) main2(argc, argv);
  else if (strcmp(argv[1], "3") == 0) main3(argc, argv);
  else if (strcmp(argv[1], "4") == 0) main4(argc, argv);
  else if (strcmp(argv[1], "5") == 0) main5(argc, argv);
  else
    cout << "usage: " << argv[0] << " [function number]" << endl;
  return 0;
}


  
