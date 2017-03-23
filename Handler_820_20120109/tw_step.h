#ifndef _TIPSWARE_BUTTON_DIB_STATIC_H_
#define _TIPSWARE_BUTTON_DIB_STATIC_H_

// +------------------------------------------------+
// |  Object Name :  TipsWare Normal Static Control |
// +------------------------------------------------+
//
//  Microsoft Windows에서 제공하는 기본적인 Static 컨트롤을 SubClassing 하거나 직접 생성하여
//  내부의 배경색이나, 문자열의 색상 또는 문자열의 폰트를 쉽게 변경할수 있도록 구성된 컨트롤입니다. 
//
//   최초 설계자 : 김성엽.
//   최초 개발자 : 김성엽.              2000년 3월 13일
// ------------------------------------------------------
//        변경자 : 김성엽.              2000년 3월 14일
//          내용 : 함수별로 설명문 추가.
// ------------------------------------------------------

// Normal Static 컨트롤을 Subclassing 없이 직접 생성하여 사용하고자 하는 경우에 사용하는 함수이다.
// 5가지 인자를 가지며 각 인자의 종류와 내용은 다음과 같다.
// 1. const char *parm_text
//    Static 컨트롤에 사용할 문자열을 입력한다. 
// 2. int parm_style
//    Static 컨트롤의 속성(좌/우 정렬, 단/다행 지원...)을 정의한다.
// 3. CRect parm_rect
//    생성할 컨트롤의 위치 좌표.
// 4. CWnd *parm_parent
//    생성할 컨트롤을 포함할 윈도우의 포인터.
// 5. int parm_id
//    생성할 컨트롤의 컨트롤 ID.
//
// 이 함수는 CWnd *를 반환하는데, 이 값은 내부적으로 생성된 Normal Static 컨트롤 객체의 주소이다. 이 객체는 new 연산자를
// 사용하여 동적으로 생성되기 때문에 넘겨 놓은 주소값을 저장시켜놓았다가 반드시 delete해야 한다. delete시에는 특별한
// 인터페이스를 사용할 필요없이 기억하고 있는 해당 포인터를 직접 delete시키면 된다. 
CWnd *CreateStaticObject(const char *, int , CRect , CWnd *, int );

CWnd *CreateStaticObjectEx(const char *parm_text, int parm_style, int parm_style_ex, CRect parm_rect, CWnd *parm_parent, int parm_id);

// 일반 Static 컨트롤을 Subclassing하여 Normal Static 컨트롤을 생성하는 함수이다.
// 2가지 인자를 가지며 각 인자의 종류와 내용은 다음과 같다.
// 1. int parm_ctrl_id
//    Subclassing 할 Static 컨트롤의 Resource ID.
// 2. CWnd *parm_parent
//    Subclassing 할 Static 컨트롤을 포함하는 윈도우의 포인터.
//
// 이 함수는 CWnd *를 반환하는데, 이 값은 내부적으로 생성된 Normal Static 컨트롤 객체의 주소이다. 이 객체는 new 연산자를
// 사용하여 동적으로 생성되기 때문에 넘겨 놓은 주소값을 저장시켜놓았다가 반드시 delete해야 한다. delete시에는 특별한
// 인터페이스를 사용할 필요없이 기억하고 있는 해당 포인터를 직접 delete시키면 된다. 
CWnd *CreateSubClassingStaticObject(int , CWnd *);


// 생성된 Normal Static 컨트롤에 아래에 나열된 속성을 설정하고자 하는 함수이다.
// 총 5가지 인자를 가지며 각 인자의 종류와 내용은 다음과 같다.
// 1. CWnd *parm_object
//    속성을 변경하고자 하는 Normal Static 컨트롤의 주소정보이다.
// 2. int parm_id
//    어떤 속성을 변경할지를 지정. 변경 가능한 속성의 종류는 함수 설명이 끝난 후에 별도로 설명.
// 3. void *parm_property
//    포인터형 값을 가지는 설정 값을 전달한다.  ( parm_id의 값에 따라 설정값의 의미는 달라진다. )
// 4. int parm_data1
//    상수형 값을 가지는 설정 값을 전달한다.  ( parm_id의 값에 따라 설정값의 의미는 달라진다. )
// 5. int parm_data2
//    상수형 값을 가지는 설정 값을 전달한다.  ( parm_id의 값에 따라 설정값의 의미는 달라진다. )
int SetStaticOneProperty(CWnd *, int , void *, int , int );
// SetStaticOneProperty함수는 2번째 인자인 parm_id의 값에 따라서 3~5에 전달되는 정보가 결정된다. 따라서 ID에 따른 
// 필요 정보가 정확하지 않으면 원하지 않는 형태의 상황이 발생할수 있다. 변경 가능한 속성은 총 7가지이고 기능적인
// 역할을 수행하는 함수를 하나 포함하여 총 8가지의 ID로 구성된다. 각각의 의미와 사용법은 다음과 같다.
   #define TS_SET_COLOR                 1
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TS_SET_COLOR는 Static 컨트롤에 포함된 택스트 색상을 변경하는 기능을 한다. 3번째나 4번째 인자에 원하는 값을 넣어주면 된다.
// 여기서 주의 할 것은 parm_property가 NULL이면 parm_data1의 값이 유효하고 parm_property가 NULL이 아니면 parm_property에
// 설정된 값으로 함수가 동작한다.
// COLORREF temp_color = RGB(0, 0, 255);
// SetStaticOneProperty(object_pointer, TS_SET_COLOR, &temp_color, 0, 0);
//                               또는
// SetStaticOneProperty(object_pointer, TS_SET_COLOR, NULL, RGB(0, 0, 255), 0);
   #define TS_SET_WINDOW_TEXT           2
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TS_SET_WINDOW_TEXT는 Static 컨트롤에 포함된 택스트 내용을 변경하는 기능을 한다. 3번째 인자에 원하는 문자열의 주소를 
// 넣어주면 된다.
// CString my_string = "Hello My Control";
// SetStaticOneProperty(object_pointer, TS_SET_WINDOW_TEXT, (const char *)my_string, 0, 0);
//                               또는
// SetStaticOneProperty(object_pointer, TS_SET_WINDOW_TEXT, "Hello My Control", 0, 0);
   #define TS_SET_FONT                  3
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TS_SET_FONT는 Static 컨트롤에 외부에서 생성된 폰트 객체 정보를 강제 설정하는 기능을 한다. 따라서 내부적으로 생성했던
// 폰트 객체 정보는 제거되고 사용자가 전달한 폰트 객체 정보를 사용한다. 단, 외부에서 전달되는 폰트 객체는 영속성을 가지고
// 있어야만 한다. 예를 들어, 지역적으로 선언된 폰트 객체의 주소를 할당하는 경우 Static 컨트롤에 문제가 생긴다. 따라서 
// 외부에서 생성되어 전달되는 폰트 객체는 최소한 Static 컨트롤의 Life Time보다는 길어야 한다.
// CFont m_font;   <--  멤버 변수 또는 전역 변수
// m_font.CreateFont.....
// SetStaticOneProperty(object_pointer, TS_SET_FONT, &m_font, 0, 0);
   #define TS_CHANGE_FONT               4
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TS_CHANGE_FONT는 Static 컨트롤에서 사용하는 폰트 객체 정보를 변경한다. 만약, 이전에 생성된 폰트 객체 정보가 있다면 제거하고
// 외부에서 생성된 폰트 객체 정보가 있다면 해당 정보를 사용하지 않도록 설정해 버린다.
// 이 경우에는 3~5 인자가 모두 사용되며 각각의 의미는 다음과 같다.
// (3, parm_property) : 사용하고자 하는 폰트의 이름
// (4, parm_data1)    : 폰트에 강조의 속성을 사용할 것인가의 여부. (1이면 사용, 0아면 사용 안함.)
// (5, parm_data2)    : 폰트의 크기를 설정한다.  기본 크기는 12가 적당한다.
// 굴림체를 사용하고 크기가 12이면 강조 속성을 지닌 폰트를 사용하고 싶다면 아래와 같이 코팅하면 된다.
// SetStaticOneProperty(object_pointer, TS_CHANGE_FONT, "굴림체", 1, 12);
   #define TS_SET_TRANSPARENT_MODE      5
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TS_SET_TRANSPARENT_MODE는 Static 컨트롤의 배경을 투명하게 사용할 것인지를 결정한다. 3번째 또는 4번째 인자를 이용할 수 있고
// 해당 값이 1이면 배경을 투명하게 사용하고 0이면 지정된 배경색으로 사용한다. 여기서 주의 할 것은 parm_property가 NULL이면
// parm_data1의 값이 유효하고 parm_property가 NULL이 아니면 parm_property에 설정된 값으로 함수가 동작한다.
// char temp_flag = 1;
// SetStaticOneProperty(object_pointer, TS_SET_TRANSPARENT_MODE, &temp_flag, 0, 0);
//                               또는
// SetStaticOneProperty(object_pointer, TS_SET_TRANSPARENT_MODE, NULL, 1, 0);
// Normal Static 컨트롤은 기본적으로 배경을 투명하게 하는 속성을 가지고 있기 때문에 처음 생성될때 배경색이 투명한 형태이다.
// 그리고 3번째 인자를 이용하는 경우 사용하는 변수는 반드시 char형이여야 한다.
   #define TS_SET_MULTI_LINE_MODE       6
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TS_SET_MULTI_LINE_MODE는 Static 컨트롤에 문자열을 다행(Multi-Line)으로 표시할 것인가를 결정한다. 3번째 또는 4번째 인자를 
// 이용할 수 있고 해당 값이 1이면 다행으로 사용하고 0이면 단행으로 사용한다. 여기서 주의 할 것은 parm_property가 NULL이면
// parm_data1의 값이 유효하고 parm_property가 NULL이 아니면 parm_property에 설정된 값으로 함수가 동작한다.
// char temp_flag = 1;
// SetStaticOneProperty(object_pointer, TS_SET_MULTI_LINE_MODE, &temp_flag, 0, 0);
//                               또는
// SetStaticOneProperty(object_pointer, TS_SET_MULTI_LINE_MODE, NULL, 1, 0);
// Normal Static 컨트롤은 기본적으로 단행(Single-Line) 속성을 가지고 있다. 그리고 3번째 인자를 이용하는 경우 사용하는
// 변수는 반드시 char형이여야 한다.
   #define TS_SET_BK_COLOR              7
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TS_SET_BK_COLOR는 Static 컨트롤에 배경색을 지정하는 기능을 한다. 3번째 또는 4번째 인자를 이용할 수 있고 
// parm_property가 NULL이면 parm_data1의 값이 유효하고 parm_property가 NULL이 아니면 parm_property에 설정된 값으로 함수가
// 동작한다. 또한 이 속성은 배경이 투명하지 않다고 설정된 경우에만 속성이 유효하다.
// COLORREF temp_color = RGB(0, 0, 255);
// SetStaticOneProperty(object_pointer, TS_SET_BK_COLOR, &temp_color, 0, 0);
//                               또는
// SetStaticOneProperty(object_pointer, TS_SET_BK_COLOR, NULL, RGB(0, 0, 255), 0);
   #define TS_SET_TEXT_REVISION         8
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TS_SET_TEXT_REVISION는 Static 컨트롤의 텍스트 문자열의 위치를 지정 기능을 한다. 4번째 와 5번째 인자를 이용할 수
// 있고 parm_data1의 값은 문자열이 위치할 X 좌표, parm_data2의 값은 문자열이 위치할 Y 좌표를 지정한다.
// SetStaticOneProperty(object_pointer,TS_SET_TEXT_REVISION,NULL,7,2);
   #define TS_UPDATE_OBJECT             9
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TS_UPDATE_OBJECT는 Static 컨트롤에 특정 속성을 변경하는 기능은 없다. 단지 위에서 열거한 속성으로 변경된 후,
// 변경된 사항을 화면으로 보고자 할때, 객체를 포함한 화면을 갱신시키는 기능을 가지고 있다. 그리고 인자는 사용하지 않는다.
// SetStaticOneProperty(object_pointer, TS_UPDATE_OBJECT, NULL, 0, 0);


// 생성된 Normal Static 컨트롤에 아래에 나열된 속성에 어떤 값이 설정되어 있는지 알고자 하는 경우에 사용하는 함수이다.
// 총 5가지 인자를 가지며 각 인자의 종류와 내용은 다음과 같다.
// 1. CWnd *parm_object
//    속성을 알고자 하는 Normal Static 컨트롤의 포인터.
// 2. int parm_id
//    어떤 속성을 알아낼 것인지를 지정. 지정 가능한 속성의 종류는 함수 설명이 끝난 후에 별도로 설명.
// 3. void *parm_property
//    포인터형 값을 가지는 설정 값을 전달한다.  ( parm_id의 값에 따라 설정값의 의미는 달라진다. )
// 4. int parm_data1
//    상수형 값을 가지는 설정 값을 전달한다.  ( parm_id의 값에 따라 설정값의 의미는 달라진다. )
// 5. int parm_data2
//    상수형 값을 가지는 설정 값을 전달한다.  ( parm_id의 값에 따라 설정값의 의미는 달라진다. )
int GetStaticOneProperty(CWnd *, int , void *, int , int );
// GetStaticOneProperty함수는 2번째 인자인 parm_id의 값에 따라서 3~5에 전달되는 정보가 결정되고 자신이 얻고자 하는 정보도
// 결정된다. 따라서 ID를 잘못 지정하거나 ID에 따른 필요 정보가 정확하지 않으면 원하지 않는 형태 정보가 반환될 수도 있다.
// 이 컨트롤 객체가 제공하는 정보는 아래와 같이 모두 5개이며 각가의 사용법은 다음과 같다.
// 주의할 것은 이 함수는 범용적인 용도로 설계되었기 때문에 기능에 따라 적당히 캐스팅(Casting)해서 사용해야 한다.
   #define TS_GET_COLOR                 1
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TS_GET_COLOR는 Static 컨트롤에 포함된 택스트 색상을 알아내는 기능을 한다. 인자들은 의미가 없으며 반환되는 값은 COLORREF형이다.
// COLORREF static_color = (COLORREF)GetStaticOneProperty(object_pointer, TS_GET_COLOR, NULL, 0, 0);
   #define TS_GET_WINDOW_TEXT           2
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TS_GET_WINDOW_TEXT는 Static 컨트롤에 포함된 택스트의 내용을 알아내는 기능을 한다. 인자들은 의미가 없으며 반환되는 값은
// 문자열의 시작주소이다. 즉, const char * 또는 char * 형이다.
// const char *p_text = (const char *)GetStaticOneProperty(object_pointer, TS_GET_WINDOW_TEXT, NULL, 0, 0);
   #define TS_GET_TRANSPARENT_MODE      3
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TS_GET_TRANSPARENT_MODE는 Static 컨트롤의 배경이 투명하게 설정되어있는지를 알아내는 기능을 한다. 인자들은 의미가 없으며
// 반환되는 값은 char 형이다. 반환값이 0이면 투명하지 않은 상태이고 1이면 배경이 투명한 상태이다.
// char static_transparent_flag = (char)GetStaticOneProperty(object_pointer, TS_GET_TRANSPARENT_MODE, NULL, 0, 0);
   #define TS_GET_MULTI_LINE_MODE       4
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TS_GET_MULTI_LINE_MODE는 Static 컨트롤이 다행(Multi-Line)을 지원하도록 설정있는지를 알아내는 기능을 한다. 인자들은 의미가
// 없으며 반환되는 값은 char 형이다. 반환값이 0이면 단행(Single-Line)상태이고 1이면 다행(Multi-Line) 상태이다.
// char static_transparent_flag = (char)GetStaticOneProperty(object_pointer, TS_GET_MULTI_LINE_MODE, NULL, 0, 0);
   #define TS_GET_BK_COLOR              5
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TS_GET_BK_COLOR는 Static 컨트롤에 포함된 택스트의 배경 색상을 알아내는 기능을 한다. 인자들은 의미가 없으며 반환되는 값은
// COLORREF형이다. 그리고 이 값은 배경색이 투명하지 않다고 선언된 경우에만 유효하다.
// COLORREF static_color = (COLORREF)GetStaticOneProperty(object_pointer, TS_GET_BK_COLOR, NULL, 0, 0);

// +------------------------------------------------+
// |  Object Name :  TipsWare Normal DIB Control    |
// +------------------------------------------------+
//
//  Microsoft Windows에서 제공하는 DIB(device-independent bitmap) 형식으로 구성된 파일에서 이미지 정보를 읽어서
//  사용자가 지정한 위치에 출력한다.
//
//   최초 설계자 : 김성엽.
//   최초 개발자 : 김성엽.              2000년 3월 13일
// ------------------------------------------------------
//        변경자 : 김성엽.              2000년 3월 14일
//          내용 : 함수별로 설명문 추가.
// ------------------------------------------------------

// DIB형식의 파일에서 이미지를 읽어 들여서 관리할 객체를 생성하고 지정된 경로에서 이미지 정보를 읽어온다.
// 이 함수는 1개의 인자를 가지며 그 내용은 다음과 같다.
// 1. const char *parm_pathname
//    생성과 동시에 읽어올 DIB 경로를 지정한다. 만약 객체만 단순히 생성하고 싶다면 NULL 또는 ""이라고 적으면 된다.
//
// 이 함수는 char *를 반환하는데, 이 값은 내부적으로 생성된 DIB 컨트롤 객체의 주소이다. 이 객체는 new 연산자를
// 사용하여 동적으로 생성되기 때문에 넘겨 놓은 주소값을 저장시켜놓았다가 반드시 delete해야 한다. 하지만 이 컨트롤은 객체 주소
// 보관에 대해서 다형성을 이용하지 못하기 때문에 delete 연산자를 직접적으로 사용하면 안되고 해당 컨트롤이 지원하는
// DeleteDibObject이라는 함수를 호출하여 제거해야만 한다.
char *CreateDibObject(const char *);


// 사용하던 DIB 컨트롤 객체를 삭제하고자 하는 경우에 이 함수를 호출한다.
// 이 함수는 1개의 인자를 가지며 그 내용은 다음과 같다.
// 1. char *parm_object
//    삭제하고자 하는 DIB 컨트롤의 주소정보이다.
void DeleteDibObject(char *);


// 사용자가 DIB 이미지 객체에 지정한 그림을 사용자가 지정한 위치에 바둑판 형식으로 출력한다.
// 이 함수는 3개의 인자를 가지며 각각의 의미는 다음과 같다.
// 1. char *parm_object
//    출력할 이미지를 포함하고 있는 객체의 주소정보이다.
// 2. CDC *parm_dc
//    이미지를 출력할 DC(Display/Device Context)의 종류를 지정한다.
// 3. CRect parm_rect
//    이미지를 출력한 좌표.. 읽어들인 이미지보다 지정한 좌표가 크면 바둑판 형식으로 채우기를 시도한다.
char DrawBackgroundImage(char *, CDC *, CRect );


// 현재 객체가 읽어들인 이미지 정보를 사용자가 원하는 위치에 출력한다. 출력 크기나, 배경색 제거와 같은 기능은 
// SetDibOneProperty를 통해서 설정하고 여기에서는 단순히 출력 위치만 지정하면 된다. 이 함수는 4개의 인자를 가지며 각각의
// 의미는 다음과 같다.
// 1. char *parm_object
//    출력할 이미지를 포함하고 있는 객체의 주소정보이다.
// 2. CDC *parm_dc
//    이미지를 출력할 DC(Display/Device Context)의 종류를 지정한다.
// 3. int parm_x
//    이미지를 출력할 X축 좌표를 지정한다.
// 3. int parm_y
//    이미지를 출력할 Y축 좌표를 지정한다.
void ShowDibImage(char *, CDC *, int , int );

char ShowDibImageEx(char *parm_object, CDC *parm_dc, int parm_x, int parm_y);
char ShowDibImageExNoPosition(char *parm_object, CDC *parm_dc);

// 생성된 DIB 컨트롤에 아래에 나열된 속성을 설정하고자 하는 함수이다.
// 총 5가지 인자를 가지며 각 인자의 종류와 내용은 다음과 같다.
// 1. CWnd *parm_object
//    속성을 변경하고자 하는 DIB 컨트롤의 주소정보이다.
// 2. int parm_id
//    어떤 속성을 변경할지를 지정. 변경 가능한 속성의 종류는 함수 설명이 끝난 후에 별도로 설명.
// 3. void *parm_property
//    포인터형 값을 가지는 설정 값을 전달한다.  ( parm_id의 값에 따라 설정값의 의미는 달라진다. )
// 4. int parm_data1
//    상수형 값을 가지는 설정 값을 전달한다.  ( parm_id의 값에 따라 설정값의 의미는 달라진다. )
// 5. int parm_data2
//    상수형 값을 가지는 설정 값을 전달한다.  ( parm_id의 값에 따라 설정값의 의미는 달라진다. )
int SetDibOneProperty(char *, int , void *, int , int );
// SetDibOneProperty함수는 2번째 인자인 parm_id의 값에 따라서 3~5에 전달되는 정보가 결정된다. 따라서 ID에 따른 
// 필요 정보가 정확하지 않으면 원하지 않는 형태의 상황이 발생할수 있다. 변경 가능한 속성은 총 5가지이다. 그리고
// 각각의 의미와 사용법은 다음과 같다.
   #define TD_SET_DIB                   1
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TD_SET_DIB는 지정한 DIB 파일에서 그림 정보를 읽어들이는 기능을 한다. 3번째인자에 읽고자 하는 파일의 경로를 적어주면 된다.
// CString my_path = "my_image.bmp";
// int ret = SetDibOneProperty(object_pointer, TD_SET_DIB, (const char *)my_path, 0, 0);
//                               또는
// int ret = SetDibOneProperty(object_pointer, TD_SET_DIB, "my_image.bmp", 0, 0);
// 이함수는 하나의 int 반환값을 가지는데 1이면 성공적으로 파일을 열어서 데이터를 읽어 들인것이며 -1이면 그림 파일 읽기에
// 실패한 것이다.
   #define TD_SET_DIB_DLG               2
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TD_SET_DIB_DLG는 지정한 DIB 파일에서 그림 정보를 읽어들이는 기능을 한다. 하지만 TD_SET_DIB와는 달리 직접적인 데이터 파일의
// 경로를 사용하지 않고 함수에서 제공하는 파일열기 대화상자를 이용하여 그림 파일을 선택한다. 따라서 인자를 이용하지 않는다.
// int ret = SetDibOneProperty(object_pointer, TD_SET_DIB_DLG, NULL, 0, 0);
// 이함수는 하나의 int 반환값을 가지는데 1이면 성공적으로 파일을 열어서 데이터를 읽어 들인것이고 -1이면 그림 파일 읽기에
// 실패한 것이며 0이면 파일열기 대화상자에서 취소버튼을 선택한 경우이다.
   #define TD_SET_FIX_SIZE              3
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TD_SET_FIX_SIZE는 읽어들인 그림파일을 출력할때 어떤 크기로 출력할것인지를 결정한다. 이 함수는 전달된 매개값을 절대적인
// 출력 크기로 생각하고 그림을 출력한다. 예를들어, 가로/세로가 30/40이면 가로는 30 pixel크기로 출력되고 세로는 40 pixel크기로
// 출력된다. 이 함수는 값을 설정하기 위해서 4~5번째 매개변수를 이용한다.
// (4, parm_data1)    : 출력할 그림의 x축 절대 크기
// (5, parm_data2)    : 출력할 그림의 y축 절대 크기
// SetDibOneProperty(object_pointer, TD_SET_FIX_SIZE, NULL, 30, 40);
// 그림을 읽어들인 후, 그림의 기본 출력 크기는 그림의 실제 크기이다.
   #define TD_SET_RATE_SIZE             4
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TD_SET_RATE_SIZE는 읽어들인 그림파일을 출력할때 어떤 크기로 출력할것인지를 결정한다. 이 함수는 전달된 매개값을 상대적인
// 출력 크기로 생각하고 그림을 출력한다. 예를들어, 가로/세로가 2/1.5이면 가로는 읽어들인 그림의 기본 크기의 2배이고 세로는
// 그림 기본 크기의 1.5배로 각각 출력한다. 이 함수는 값을 설정하기 위해서 3번째 매개변수만을 이용한다. 실제로 배율에 대한
// 정보가 실수 값이기 때문에 4~5번째 매개변수를 이용하기 어렵기 때문에 3번째 매개변수를 double 배열을 선언하여 x, y값을
// 모두 전달한다.
// double pos[2] = { double(2), double(1.5) };
// SetDibOneProperty(object_pointer, TD_SET_RATE_SIZE, pos, 0, 0);
// 그림을 읽어들인 후, 그림의 기본 출력 크기는 그림의 실제 크기이다.
   #define TD_SET_TRANSPARENT_MODE      5
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TD_SET_TRANSPARENT_MODE는 읽어들인 그림파일을 출력할때 그림의 특정 색상 한가지를 제거하고출력할 것인지를 결정한다.
// (4, parm_data1)    : 색상 기능 제거 기능을 사용할것인지를 지정한다. 1이면 사용하고 0이면 사용안함.
// (5, parm_data2)    : parm_data1가 1인 경우에만 유효하며, 제거할 색상을 지정하면 된다.
// 배경색 제거 기능을 사용하여 RGB(0, 0, 255)에 해당하는 색상을 제거하는 경우는 아래와 같이하면 된다.
// SetDibOneProperty(object_pointer, TD_SET_TRANSPARENT_MODE, NULL, 1, RGB(0, 0, 255));
// 그리고 배경색 제거 기능을 사용하지 않는 경우에는 아래와 같이하면 된다.
// SetDibOneProperty(object_pointer, TD_SET_TRANSPARENT_MODE, NULL, 0, 0);
   #define TD_SET_DRAW_POSITION         6
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// 생성된 DIB 컨트롤에 아래에 나열된 속성에 어떤 값이 설정되어 있는지 알고자 하는 경우에 사용하는 함수이다.
// 총 5가지 인자를 가지며 각 인자의 종류와 내용은 다음과 같다.
// 1. CWnd *parm_object
//    속성을 알고자 하는 DIB 컨트롤의 포인터.
// 2. int parm_id
//    어떤 속성을 알아낼 것인지를 지정. 지정 가능한 속성의 종류는 함수 설명이 끝난 후에 별도로 설명.
// 3. void *parm_property
//    포인터형 값을 가지는 설정 값을 전달한다.  ( parm_id의 값에 따라 설정값의 의미는 달라진다. )
// 4. int parm_data1
//    상수형 값을 가지는 설정 값을 전달한다.  ( parm_id의 값에 따라 설정값의 의미는 달라진다. )
// 5. int parm_data2
//    상수형 값을 가지는 설정 값을 전달한다.  ( parm_id의 값에 따라 설정값의 의미는 달라진다. )
int GetDibOneProperty(char *, int , void *, int , int );
// GetStaticOneProperty함수는 2번째 인자인 parm_id의 값에 따라서 3~5에 전달되는 정보가 결정되고 자신이 얻고자 하는 정보도
// 결정된다. 따라서 ID를 잘못 지정하거나 ID에 따른 필요 정보가 정확하지 않으면 원하지 않는 형태 정보가 반환될 수도 있다.
// 이 컨트롤 객체가 제공하는 정보는 아래와 같이 모두 5개이며 각가의 사용법은 다음과 같다.
// 주의할 것은 이 함수는 범용적인 용도로 설계되었기 때문에 기능에 따라 적당히 캐스팅(Casting)해서 사용해야 한다.
   #define TD_GET_DIB_SIZE              1
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TD_GET_DIB_SIZE는 읽어들인 그림의 실제 크기를 알고자 하는 경우에 사용된다. 특별한 매개변수는 사용하지 않으며 반환되는
// 값은 CSize *형이다.
// CSize *p_size = (CSize *)GetDibOneProperty(object_pointer, TD_GET_DIB_SIZE, NULL, 0, 0);
   #define TD_GET_DIB_HANDLE            2
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TD_GET_DIB_HANDLE는 읽어들인 그림정보가 구성된 메모리의 Handle을 얻고자 하는 경우에 사용된다. 반환값은 HANDLE형이다.
// HANDLE h_dib = (HANDLE)GetDibOneProperty(object_pointer, TD_GET_DIB_HANDLE, NULL, 0, 0);
   #define TD_GET_DIB_PALETTE           3
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TD_GET_DIB_PALETTE는 읽어들인 그림정보 중에서 Palette정보를 얻고자 하는 경우에 사용된다. 반환값은 CPalette *형이다.
// CPalette *p_palette = (CPalette *)GetDibOneProperty(object_pointer, TD_GET_DIB_PALETTE, NULL, 0, 0);
   #define TD_GET_DIB_PATH              4
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TD_GET_DIB_PATH는 읽어들인 파일의 경로명을 알려준다. 반환되는 값은 char *형이다.
// char *p_path = (char *)GetDibOneProperty(object_pointer, TD_GET_DIB_PATH, NULL, 0, 0);
   #define TD_IS_LOAD_DIB               5
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TD_IS_LOAD_DIB는 생성된 컨트롤 객체에 읽어들인 이미지 정보가 있는지를 알려준다. 반환되는 값은 char 형이다.
// char load_flag = (char)GetDibOneProperty(object_pointer, TD_IS_LOAD_DIB, NULL, 0, 0);
   #define TD_GET_DRAW_POSITION         6
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


// +------------------------------------------------+
// |  Object Name :  TipsWare Normal Button Control |
// +------------------------------------------------+
//
// Microsoft Windows에서 제공하는 기본적인 Bitmap Button 컨트롤은 Bitmap을 이용하여 버튼 자체를 표현하는데
// 많은 제약을 가지고 있다. 그래서 많은 프로그래머들이 사용자 정의 그리기(Owner Drawing)기법을 사용해 이런 
// 단점을 극복하고 있는데, 이 객체는 그러한 단점을 보완하는 방법을 제공한다.
//
//   최초 설계자 : 김성엽.
//   최초 개발자 : 김성엽.              2000년 4월 3일
// ------------------------------------------------------
//        변경자 : 김성엽.              2000년 4월 7일
//          내용 : 함수별로 설명문 추가.
// ------------------------------------------------------

// Tips Button 컨트롤을 Subclassing 없이 직접 생성하여 사용하고자 하는 경우에 사용하는 함수이다.
// 5가지 인자를 가지며 각 인자의 종류와 내용은 다음과 같다.
// 1. const char *parm_text
//    CButton 컨트롤에 사용할 문자열을 입력한다. 
// 2. int parm_style
//    CButton 컨트롤의 속성(좌/우 정렬, 단/다행 지원...)을 정의한다.
// 3. CRect parm_rect
//    생성할 컨트롤의 위치 좌표.
// 4. CWnd *parm_parent
//    생성할 컨트롤을 포함할 윈도우의 포인터.
// 5. int parm_id
//    생성할 컨트롤의 컨트롤 ID.
//
// 이 함수는 CButton *를 반환하는데, 이 값은 내부적으로 생성된  Tips Button 컨트롤 객체의 주소이다. 이 객체는 new 연산자를
// 사용하여 동적으로 생성되기 때문에 넘겨 놓은 주소값을 저장시켜 놓았다가 반드시 delete해야 한다. delete시에는 특별한
// 인터페이스를 사용할 필요없이 기억하고 있는 해당 포인터를 직접 delete시키면 된다. 
CButton *CreateTipsButtonObject(const char *, int , CRect , CWnd *, int );


// 일반 Bitmap Button 컨트롤을 Subclassing하여 Tips Button 컨트롤을 생성하는 함수이다.
// 2가지 인자를 가지며 각 인자의 종류와 내용은 다음과 같다.
// 1. int parm_ctrl_id
//    Subclassing 할 Bitmap Button 컨트롤의 Resource ID.
// 2. CWnd *parm_parent
//    Subclassing 할 Bitmap Button 컨트롤을 포함하는 윈도우의 포인터.
//
// 이 함수는 CButton *를 반환하는데, 이 값은 내부적으로 생성된 Tips Button 컨트롤 객체의 주소이다. 이 객체는 new 연산자를
// 사용하여 동적으로 생성되기 때문에 넘겨 놓은 주소값을 저장시켜놓았다가 반드시 delete해야 한다. delete시에는 특별한
// 인터페이스를 사용할 필요없이 기억하고 있는 해당 포인터를 직접 delete시키면 된다. 
CButton *CreateSubClassingTipsButtonObject(int , CWnd * );


// 생성된 Tips Button 컨트롤에 아래에 나열된 속성을 설정하고자 하는 함수이다.
// 총 5가지 인자를 가지며 각 인자의 종류와 내용은 다음과 같다.
// 1.CButton *parm_object
//    속성을 변경하고자 하는 Tips Button 컨트롤의 주소정보이다.
// 2. int parm_id
//    어떤 속성을 변경할지를 지정. 변경 가능한 속성의 종류는 함수 설명이 끝난 후에 별도로 설명.
// 3. void *parm_property
//    포인터형 값을 가지는 설정 값을 전달한다.  ( parm_id의 값에 따라 설정값의 의미는 달라진다. )
// 4. int parm_data1
//    상수형 값을 가지는 설정 값을 전달한다.  ( parm_id의 값에 따라 설정값의 의미는 달라진다. )
// 5. int parm_data2
//    상수형 값을 가지는 설정 값을 전달한다.  ( parm_id의 값에 따라 설정값의 의미는 달라진다. )
int SetTipsButtonOneProperty(CButton *, int , void *, int , int );
// SetTipsButtonOneProperty함수는 2번째 인자인 parm_id의 값에 따라서 3~5에 전달되는 정보가 결정된다. 따라서 ID에 따른 
// 필요 정보가 정확하지 않으면 원하지 않는 형태의 상황이 발생할수 있다. 변경 가능한 속성은 총 14가지이다. 그리고
// 각각의 의미와 사용법은 다음과 같다.
   #define TNB_SET_TEXT_REVISION            1
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TNB_SET_TEXT_REVISION는 Tips Button 컨트롤의 텍스트 문자열의 위치를 지정 기능을 한다. 4번째 와 5번째 인자를 이용할 수
// 있고 parm_data1의 값은 문자열이 위치할 X 좌표, parm_data2의 값은 문자열이 위치할 Y 좌표를 지정한다.
// SetTipsButtonOneProperty(object_pointer, TNB_SET_TEXT_REVISION, NULL, 7, 2);
   #define TNB_SET_IMAGE_REVISION           2
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TNB_SET_IMAGE_REVISION는 Tips Button 컨트롤의 이미지 위치를 지정 기능을 한다. 4번째 와 5번째 인자를 이용할 수 있고
// parm_data1의 값은 이미지가 위치할 X 좌표, parm_data2의 값은 문자열이 위치할 Y 좌표를 지정한다.
// SetTipsButtonOneProperty(object_pointer, TNB_SET_IMAGE_REVISION, NULL, 2, 2);
   #define TNB_SET_FONT                     3
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TNB_SET_FONT는 Tips Button 컨트롤에 외부에서 생성된 폰트 객체 정보를 강제 설정하는 기능을 한다. 따라서 내부적으로 생성했던
// 폰트 객체 정보는 제거되고 사용자가 전달한 폰트 객체 정보를 사용한다. 단, 외부에서 전달되는 폰트 객체는 영속성을 가지고
// 있어야만 한다. 예를 들어, 지역적으로 선언된 폰트 객체의 주소를 할당하는 경우 Tips Button 컨트롤에 문제가 생긴다. 따라서 
// 외부에서 생성되어 전달되는 폰트 객체는 최소한 Tips Button 컨트롤의 Life Time보다는 길어야 한다.
// CFont m_font;   <--  멤버 변수 또는 전역 변수
// m_font.CreateFont.....
// SetTipsButtonOneProperty(object_pointer, TNB_SET_FONT, &m_font, 0, 0);
   #define TNB_CHANGE_FONT                  4
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TNB_CHANGE_FONT Tips Button  컨트롤에서 사용하는 폰트 객체 정보를 변경한다. 만약, 이전에 생성된 폰트 객체 정보가 있다면 제거하고
// 외부에서 생성된 폰트 객체 정보가 있다면 해당 정보를 사용하지 않도록 설정해 버린다. 이 경우에는 3~5 인자가 모두 사용되며
// 각각의 의미는 다음과 같다.
// (3, parm_property) : 사용하고자 하는 폰트의 이름
// (4, parm_data1)    : 폰트에 강조의 속성을 사용할 것인가의 여부. (1이면 사용, 0아면 사용 안함.)
// (5, parm_data2)    : 폰트의 크기를 설정한다.  기본 크기는 12가 적당한다.
// 굴림체를 사용하고 크기가 12이면 강조 속성을 지닌 폰트를 사용하고 싶다면 아래와 같이 코팅하면 된다.
// SetTipsButtonOneProperty(object_pointer, TNB_CHANGE_FONT, "굴림체", 1, 12);
   #define TNB_SET_BORDER_COLOR             5
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TNB_SET_BORDER_COLOR는 Tips Button 컨트롤이 포커스를 소유하고 있는때와 포커스를 잃었을때의 버튼 외부의 색상을 지정하는
// 기능을 한다. 4번째 와 5번째 인자를 이용할 수 있고 parm_data1의 값은 포커스 소유 상태 일때의 색상을 지정하고, 
// parm_data2의 값은 포커스를 잃었을 시의 색상을 지정한다.
// SetTipsButtonOneProperty(object_pointer, TNB_SET_BORDER_COLOR, NULL, RGB(0,0,255), RGB(255,0,0));
   #define TNB_SET_TEXT_COLOR               6
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TNB_SET_TEXT_COLOR Tips Button 컨트롤의 텍스트 문자열의 색상을 지정하는 기능을 한다. 4번째 와 5번째 인자를 이용할 수
// 있고 parm_data1의 값은 포커스 소유 상태 일때의 색상을 지정하고, parm_data2의 값은 포커스를 잃었을 시의 색상을 지정한다.
// SetTipsButtonOneProperty(object_pointer, TNB_SET_TEXT_COLOR, NULL, RGB(0,0,255), RGB(255,0,0));
   #define TNB_SET_WINDOW_TEXT              7
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TNB_SET_WINDOW_TEXT Tips Button 컨트롤에 포함된 택스트 내용을 변경하는 기능을 한다. 3번째 인자에 원하는 문자열의 주소를 
// 넣어주면 된다.
// CString my_string = "Hello My Control";
// SetTipsButtonOneProperty(object_pointer, TNB_SET_WINDOW_TEXT, (const char *)my_string, 0, 0);
//                               또는
// SetTipsButtonOneProperty(object_pointer, TNB_SET_WINDOW_TEXT, "Hello My Control", 0, 0);
   #define TNB_SET_BITMAP                   8
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TNB_SET_BITMAP는 Tips Button 컨트롤에 비트맵을 올리는 기능을 한다. 3~5번째 인자를 지정해야한다.
// parm_property는 값은 버튼위에 위치할 bitmap의 경로 및 화일명을 지정하고, parm_data1은 배경색 제거 기능을 사용할 것인지를
// 결정하는 것(1:사용, 0:사용안함)이고 parm_data2는 parm_data1이 1인 경우 제거할 색상을 지정한다.
// SetTipsButtonOneProperty(object_pointer, TNB_SET_BITMAP, ".\image\test.bmp", 0, RGB(192,192,192));
   #define TNB_SET_DEFAULT                  9
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TNB_SET_DEFAULT는 Tips Button 컨트롤을 Deafult 속성을 지정하는 기능을 한다. 3번째 또는 4번째 인자를 이용할 수 있다.
// 여기서 주의 할 것은 parm_property가 NULL이면 parm_data1의 값이 유효하고 parm_property가 NULL이 아니면 parm_property에
// 설정된 값으로 함수가 동작한다. defaul로 설정이 되면 포커스를 잃어 버려도 포커스 소유 상태의 색상을 유지하게 된다.
// 각각의 매개변수는 char 형으로 전달 하여야 한다.
// char m_set_dafault = 1;
// SetTipsButtonOneProperty(object_pointer, TNB_SET_DEFAULT,&m_set_dafault, 0);
//									또는
// SetTipsButtonOneProperty(object_pointer, TNB_SET_DEFAULT,NULL, 1, 0);
   #define TNB_SET_FOCUS_MODE              10
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TNB_SET_FOCUS_MODE Tips Button 컨트롤을 focus 상태를 지정하는 기능을 한다. 3번째 또는 4번째 인자를 이용할 수 있다.
// 여기서 주의 할 것은 parm_property가 NULL이면 parm_data1의 값이 유효하고 parm_property가 NULL이 아니면 parm_property에
// 설정된 값으로 함수가 동작한다. 각각의 매개변수는 char 형으로 전달 하여야 한다.
// char m_set_focus = 1;
// SetTipsButtonOneProperty(object_pointer, TNB_SET_FOCUS_MODE, &m_set_focus, 0, 0);
//									또는
// SetTipsButtonOneProperty(object_pointer, TNB_SET_FOCUS_MODE, NULL, 1, 0);
   #define TNB_SET_TRANSPARENT_MODE        11
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TNB_SET_TRANSPARENT_MODE는 Bitmap Button 컨트롤의 배경을 투명하게 사용할 것인지를 결정한다. 3번째 또는 4번째 인자를
// 이용할수 있고 해당 값이 1이면 배경을 투명하게 사용하고 0이면 지정된 배경색으로 사용한다. 여기서 주의 할 것은 
// parm_property가 NULL이면 parm_data1의 값이 유효하고 parm_property가 NULL이 아니면 parm_property에 설정된 값으로 
// 함수가 동작한다.
// char temp_flag = 1;
// SetTipsButtonOneProperty(object_pointer, TNB_SET_TRANSPARENT_MODE, &temp_flag, 0, 0);
//                               또는
// SetTipsButtonOneProperty(object_pointer, TNB_SET_TRANSPARENT_MODE, NULL, 1, 0);
// Normal Bitmap Button 컨트롤은 기본적으로 배경을 투명하게 하는 속성을 가지고 있기 때문에 처음 생성될때 배경색이 
// 투명한 형태이다. 그리고 3번째 인자를 이용하는 경우 사용하는 변수는 반드시 char형이여야 한다.
   #define TNB_SET_FILL_COLOR              12
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TNB_SET_FILL_COLOR Bitmap Button 컨트롤의 배경색을 지정하는 기능을 한다. 3번째 와 4번째 인자를 이용할 수 있고 
// parm_property가 NULL이면 parm_data1의 값이 유효하고 parm_property가 NULL이 아니면 parm_property에 설정된 값으로 
// 함수가 동작한다. 여기서 주의 해야 할 것은 버튼의 기본 속성에 TRANSPARENT_MODE 가 설정 되어 있으므로 반드시 
// TRANSPARENT_MODE를 해제한 후 사용해야 한다.
// COLORREF m_color = RGB(0,0,255);
// SetTipsButtonOneProperty(object_pointer, TNB_SET_FILL_COLOR, &m_color, 0, 0);
//									또는
// SetTipsButtonOneProperty(object_pointer, TNB_SET_FILL_COLOR, NULL, RGB(0,255,0), 0);
   #define TNB_RESET_BITMAP                13
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TNB_RESET_BITMAP는 Tips Button 컨트롤에 올린 비트맵을 제거 하는 기능을 한다. 2번째 매개변수만을 지정 하면 되며
// 다른 설정 값은 의미가 없다.
// SetTipsButtonOneProperty(object_pointer, TNB_RESET_BITMAP,NULL, 0, 0);
   #define TNB_UPDATE_OBJECT               14
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TNB_UPDATE_OBJECT는 Tips Button 컨트롤에 특정 속성을 변경하는 기능은 없다. 단지 위에서 열거한 속성으로 변경된 후,
// 변경된 사항을 화면으로 보고자 할때, 객체를 포함한 화면을 갱신시키는 기능을 가지고 있다. 그리고 인자는 사용하지 않는다.
// SetTipsButtonOneProperty(object_pointer, TNB_UPDATE_OBJECT, NULL, 0, 0);


// 생성된 Tips Button 컨트롤에 아래에 나열된 속성에 어떤 값이 설정되어 있는지 알고자 하는 경우에 사용하는 함수이다.
// 총 5가지 인자를 가지며 각 인자의 종류와 내용은 다음과 같다.
// 1. CButton *parm_object
//    속성을 알고자 하는 Tips Button 컨트롤의 포인터.
// 2. int parm_id
//    어떤 속성을 알아낼 것인지를 지정. 지정 가능한 속성의 종류는 함수 설명이 끝난 후에 별도로 설명.
// 3. void *parm_property
//    포인터형 값을 가지는 설정 값을 전달한다.  ( parm_id의 값에 따라 설정값의 의미는 달라진다. )
// 4. int parm_data1
//    상수형 값을 가지는 설정 값을 전달한다.  ( parm_id의 값에 따라 설정값의 의미는 달라진다. )
// 5. int parm_data2
//    상수형 값을 가지는 설정 값을 전달한다.  ( parm_id의 값에 따라 설정값의 의미는 달라진다. )
int GetTipsButtonOneProperty(CButton *, int , void *, int , int );
// GetTipsButtonOneProperty함수는 2번째 인자인 parm_id의 값에 따라서 3~5에 전달되는 정보가 결정되고 자신이 얻고자 하는 정보도
// 결정된다. 따라서 ID를 잘못 지정하거나 ID에 따른 필요 정보가 정확하지 않으면 원하지 않는 형태 정보가 반환될 수도 있다.
// 이 컨트롤 객체가 제공하는 정보는 아래와 같이 모두 3개이며 각각의 사용법은 다음과 같다.
// 주의할 것은 이 함수는 범용적인 용도로 설계되었기 때문에 기능에 따라 적당히 캐스팅(Casting)해서 사용해야 한다.
   #define TNB_GET_DEFAULT                  1
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TNB_GET_DEFAULT는 Tips Button 컨트롤이 Default 버튼으로 설정되어있는지를 알아내는 기능을 한다. 인자들은 의미가 없으며
// 반환되는 값은 char 형이다. 반환값이 0이면 Default가 아닌 상태이고 1이면 Default 설정 상태이다.
// char dibbutton_default_flag = (char)GetTipsButtonOneProperty(object_pointer, TNB_GET_DEFAULT, NULL, 0, 0);
   #define TNB_GET_FOCUS_MODE               2
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TNB_GET_FOCUS_MODE는 Tips Button 컨트롤 이 포커스를 가지고 있는지를 알아내는 기능을 한다. 인자들은 의미가 없으며
// 반환되는 값은 char 형이다. 반환값이 0이면 포커스 가지지 않는 상태이고 1이면 포커스를 가지고 있는 상태이다.
// char dibbutton_focus_flag = (char)GetTipsButtonOneProperty(object_pointer, TNB_GET_FOCUS_MODE, NULL, 0, 0);
   #define TNB_GET_TRANSPARENT_MODE         3
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TNB_GET_TRANSPARENT_MODE는 Tips Button 컨트롤의 배경이 투명하게 설정되어있는지를 알아내는 기능을 한다. 인자들은 의미가
// 없으며 반환되는 값은 char 형이다. 반환값이 0이면 투명하지 않은 상태이고 1이면 배경이 투명한 상태이다.
// char bitmapbutton_transparent_flag = (char)GetTipsButtonOneProperty(object_pointer, TNB_GET_TRANSPARENT_MODE, NULL, 0, 0);
   #define TNB_GET_WINDOW_TEXT              4
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TNB_GET_WINDOW_TEXT는 Tips Button 컨트롤에 포함된 택스트의 내용을 알아내는 기능을 한다. 인자들은 의미가 없으며 반환되는
// 값은 문자열의 시작주소이다. 즉, const char * 또는 char * 형이다.
// const char *p_text = (const char *)GetTipsButtonOneProperty(object_pointer, TNB_GET_WINDOW_TEXT, NULL, 0, 0);

// +------------------------------------------------+
// |  Object Name :  Tips CheckButton Control             |
// +------------------------------------------------+
//
//  Microsoft Windows에서 제공하는 기본적인 Check Button 컨트롤을 사용자 정의 그리기(Owner Drawing)기법을 이용해 Check Button의
//  단순한 표현 방식을 보완 하도록 한다.
//
//   최초 설계자 : 김성엽 팀장.
//   최초 개발자 : 김성엽 팀장.    2000년 3월 28일
// ------------------------------------------------
//        변경자 :                     년  월   일
//        내용 : 
// ------------------------------------------------

// Check Button 컨트롤을 Subclassing 없이 직접 생성하여 사용하고자 하는 경우에 사용하는 함수이다.
// 5가지 인자를 가지며 각 인자의 종류와 내용은 다음과 같다.
// 1. const char *parm_text
//    Static 컨트롤에 사용할 문자열을 입력한다. 
// 2. int parm_style
//    Static 컨트롤의 속성(좌/우 정렬, 단/다행 지원...)을 정의한다.
// 3. CRect parm_rect
//    생성할 컨트롤의 위치 좌표.
// 4. CWnd *parm_parent
//    생성할 컨트롤을 포함할 윈도우의 포인터.
// 5. int parm_id
//    생성할 컨트롤의 컨트롤 ID.
//
// 이 함수는 CButton *를 반환하는데, 이 값은 내부적으로 생성된 Check Button 컨트롤 객체의 주소이다. 이 객체는 new 연산자를
// 사용하여 동적으로 생성되기 때문에 넘겨 놓은 주소값을 저장시켜 놓았다가 반드시 delete해야 한다. delete시에는 특별한
// 인터페이스를 사용할 필요 없이 기억하고 있는 해당 포인터를 직접 delete시키면 된다. 
CButton *CreateCheckButtonObject(const char *, int , CRect , CWnd *, int );


// 일반 Check Button 컨트롤을 Subclassing하여 Check Button 컨트롤을 생성하는 함수이다.
// 2가지 인자를 가지며 각 인자의 종류와 내용은 다음과 같다.
// 1. int parm_ctrl_id
//    Subclassing 할 Check Button 컨트롤의 Resource ID.
// 2. CWnd *parm_parent
//    Subclassing 할 Check Button 컨트롤을 포함하는 윈도우의 포인터.
//
// 이 함수는 CButton *를 반환하는데, 이 값은 내부적으로 생성된 Check Button 컨트롤 객체의 주소이다. 이 객체는 new 연산자를
// 사용하여 동적으로 생성되기 때문에 넘겨 놓은 주소값을 저장시켜놓았다가 반드시 delete해야 한다. delete시에는 특별한 
// 인터페이스를 사용할 필요없이 기억하고 있는 해당 포인터를 직접 delete시키면 된다. 
CButton *CreateSubClassingCheckButtonObject(int , CWnd *);


// 생성된 Check Button 컨트롤에 아래에 나열된 속성을 설정하고자 하는 함수이다.
// 총 5가지 인자를 가지며 각 인자의 종류와 내용은 다음과 같다.
// 1.CButton *parm_object
//    속성을 변경하고자 하는 Check Button 컨트롤의 주소정보이다.
// 2. int parm_id
//    어떤 속성을 변경할지를 지정. 변경 가능한 속성의 종류는 함수 설명이 끝난 후에 별도로 설명.
// 3. void *parm_property
//    포인터형 값을 가지는 설정 값을 전달한다.  ( parm_id의 값에 따라 설정값의 의미는 달라진다. )
// 4. int parm_data1
//    상수형 값을 가지는 설정 값을 전달한다.  ( parm_id의 값에 따라 설정값의 의미는 달라진다. )
// 5. int parm_data2
//    상수형 값을 가지는 설정 값을 전달한다.  ( parm_id의 값에 따라 설정값의 의미는 달라진다. )
int SetCheckButtonOneProperty(CButton *, int , void *, int , int );
// SetCheckButtonOneProperty함수는 2번째 인자인 parm_id의 값에 따라서 3~5에 전달되는 정보가 결정된다. 따라서 ID에 따른 
// 필요 정보가 정확하지 않으면 원하지 않는 형태의 상황이 발생할수 있다. 변경 가능한 속성은 총 9가지이다. 그리고
// 각각의 의미와 사용법은 다음과 같다.
   #define TC_SET_COLOR                 1
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TC_SET_COLOR는 CheckButton 컨트롤이 포커스를 가지고 있는가? 아닌가? 에 따라 포함된 텍스트 색상을 변경하는 기능을 한다.
// RGB 색상을 4번째나 5번째 인자에 원하는 값을 넣어주면 된다.
//	SetCheckButtonOneProperty(object_pointer, TC_SET_COLOR, NULL, RGB(0, 0, 0), RGB(255,255,255));
   #define TC_SET_FONT                  2
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TC_SET_FONT는 CheckButton 컨트롤에 외부에서 생성된 폰트 객체 정보를 강제 설정하는 기능을 한다. 따라서 내부적으로 생성했던
// 폰트 객체 정보는 제거되고 사용자가 전달한 폰트 객체 정보를 사용한다. 단, 외부에서 전달되는 폰트 객체는 영속성을 가지고
// 있어야만 한다. 예를 들어, 지역적으로 선언된 폰트 객체의 주소를 할당하는 경우 CheckButton 컨트롤에 문제가 생긴다. 따라서 
// 외부에서 생성되어 전달되는 폰트 객체는 최소한 CheckButton 컨트롤의 Life Time보다는 길어야 한다.
// CFont m_font;   <--  멤버 변수 또는 전역 변수
// m_font.CreateFont.....
// SetCheckButtonOneProperty(object_pointer,  TC_SET_FONT, &m_font, 0, 0);
   #define TC_CHANGE_FONT               3
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TC_CHANGE_FONT는 CheckButton 컨트롤에서 사용하는 폰트 객체 정보를 변경한다. 만약, 이전에 생성된 폰트 객체 정보가 있다면
// 제거하고 외부에서 생성된 폰트 객체 정보가 있다면 해당 정보를 사용하지 않도록 설정해 버린다.
// 이 경우에는 3~5 인자가 모두 사용되며 각각의 의미는 다음과 같다.
// (3, parm_property) : 사용하고자 하는 폰트의 이름
// (4, parm_data1)    : 폰트에 강조의 속성을 사용할 것인가의 여부. (1이면 사용, 0아면 사용 안함.)
// (5, parm_data2)    : 폰트의 크기를 설정한다.  기본 크기는 12가 적당한다.
// 굴림체를 사용하고 크기가 12이면 강조 속성을 지닌 폰트를 사용하고 싶다면 아래와 같이 코팅하면 된다.
// SetCheckButtonOneProperty(object_pointer, TC_CHANGE_FONT, "굴림체", 1, 12);
   #define TC_SET_WINDOW_TEXT           4
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TC_SET_WINDOW_TEXT는 CheckButton 컨트롤에 포함된 택스트 내용을 변경하는 기능을 한다. 3번째 인자에 원하는 문자열의 주소를 
// 넣어주면 된다.
// CString my_string = "Hello My Control";
// SetCheckButtonOneProperty(object_pointer, TC_SET_WINDOW_TEXT, (const char *)my_string, 0, 0);
//                               또는
// SetCheckButtonOneProperty(object_pointer, TC_SET_WINDOW_TEXT, "Hello My Control", 0, 0);
   #define TC_SET_TRANSPARENT_MODE      5
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TC_SET_TRANSPARENT_MODE는 CheckButton 컨트롤의 배경을 투명하게 사용할 것인지를 결정한다. 3번째 또는 4번째 인자를 이용할
// 수 있고 해당 값이 1이면 배경을 투명하게 사용하고 0이면 지정된 배경색으로 사용한다. 여기서 주의 할 것은 parm_property가
// NULL이면 parm_data1의 값이 유효하고 parm_property가 NULL이 아니면 parm_property에 설정된 값으로 함수가 동작한다.
// char temp_flag = 1;
// SetCheckButtonOneProperty(object_pointer, TC_SET_TRANSPARENT_MODE, &temp_flag, 0, 0);
//                               또는
// SetCheckButtonOneProperty(object_pointer, TC_SET_TRANSPARENT_MODE, NULL, 1, 0);
// CheckButton 컨트롤은 기본적으로 배경을 투명하게 하는 속성을 가지고 있기 때문에 처음 생성될때 배경색이 투명한 형태이다.
// 그리고 3번째 인자를 이용하는 경우 사용하는 변수는 반드시 char형이여야 한다.
   #define TC_SET_BK_COLOR              6
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TC_SET_BK_COLOR는 CheckButton 컨트롤에 배경색을 지정하는 기능을 한다. 3번째 또는 4번째 인자를 이용할 수 있고 
// parm_property가 NULL이면 parm_data1의 값이 유효하고 parm_property가 NULL이 아니면 parm_property에 설정된 값으로 함수가
// 동작한다. 또한 이 속성은 배경이 투명하지 않다고 설정된 경우에만 속성이 유효하다.
// COLORREF temp_color = RGB(0, 0, 255);
// SetCheckButtonOneProperty(object_pointer, TC_SET_BK_COLOR, &temp_color, 0, 0);
//                               또는
// SetCheckButtonOneProperty(object_pointer, TC_SET_BK_COLOR, NULL, RGB(0, 0, 255), 0);
   #define TC_SET_FIX_POS               7
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TC_SET_FIX_POS는 Check Button 컨트롤의 기본 위치를 지정 하는 기능을 한다. 3번째 또는 4번째 인자를 이용할 수 있고 
// parm_property가 NULL이면 parm_data1의 값이 유효하고 parm_property가 NULL이 아니면 parm_property에 설정된 값으로 함수가
// 동작한다. 
// int checkbutton_pos = 20;
// SetCheckButtonOneProperty(object_pointer, TC_SET_FIX_POS, &checkbutton_pos, 0, 0);
//                               또는
// SetCheckButtonOneProperty(object_pointer, TC_SET_FIX_POS, NULL, 20, 0);
   #define TC_SET_IMAGE_PATH            8
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TC_SET_IMAGE_PATH는 Check Button 컨트롤을 나타내는 bitmap을 지정하는 기능을 한다. 3번째 또는 4번째 인자를 이용하여 
// bitmap을 설정한다. parm_property는 bitmap의 위치이고, parm_data1은 Check Button 용 bitmap을 담고 있는 배열의 위치이다.
// SetCheckButtonOneProperty(object_pointer, TC_SET_IMAGE_PATH, ".\MMCC_IMAGE", 1, 0);
   #define TC_UPDATE_OBJECT             9
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TC_UPDATE_OBJECT는 CheckButton 컨트롤에 특정 속성을 변경하는 기능은 없다. 단지 위에서 열거한 속성으로 변경된 후,
// 변경된 사항을 화면으로 보고자 할때, 객체를 포함한 화면을 갱신시키는 기능을 가지고 있다. 그리고 인자는 사용하지 않는다.
// SetCheckButtonOneProperty(object_pointer, TC_UPDATE_OBJECT, NULL, 0, 0);


// 생성된 Normal CheckButton 컨트롤에 아래에 나열된 속성에 어떤 값이 설정되어 있는지 알고자 하는 경우에 사용하는 함수이다.
// 총 5가지 인자를 가지며 각 인자의 종류와 내용은 다음과 같다.
// 1. CButton *parm_object
//    속성을 알고자 하는 Normal CheckButton 컨트롤의 포인터.
// 2. int parm_id
//    어떤 속성을 알아낼 것인지를 지정. 지정 가능한 속성의 종류는 함수 설명이 끝난 후에 별도로 설명.
// 3. void *parm_property
//    포인터형 값을 가지는 설정 값을 전달한다.  ( parm_id의 값에 따라 설정값의 의미는 달라진다. )
// 4. int parm_data1
//    상수형 값을 가지는 설정 값을 전달한다.  ( parm_id의 값에 따라 설정값의 의미는 달라진다. )
// 5. int parm_data2
//    상수형 값을 가지는 설정 값을 전달한다.  ( parm_id의 값에 따라 설정값의 의미는 달라진다. )
int GetCheckButtonOneProperty(CButton *, int , void *, int , int );
// GetCheckButtonOneProperty함수는 2번째 인자인 parm_id의 값에 따라서 3~5에 전달되는 정보가 결정되고 자신이 얻고자 하는 정보도
// 결정된다. 따라서 ID를 잘못 지정하거나 ID에 따른 필요 정보가 정확하지 않으면 원하지 않는 형태 정보가 반환될 수도 있다.
// 이 컨트롤 객체가 제공하는 정보는 아래와 같이 모두 4개이며 각가의 사용법은 다음과 같다.
// 주의할 것은 이 함수는 범용적인 용도로 설계되었기 때문에 기능에 따라 적당히 캐스팅(Casting)해서 사용해야 한다.
   #define TC_GET_WINDOW_TEXT           1
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TC_GET_WINDOW_TEXT는 CheckButton 컨트롤에 포함된 택스트의 내용을 알아내는 기능을 한다. 인자들은 의미가 없으며 반환되는 값은
// 문자열의 시작주소이다. 즉, const char * 또는 char * 형이다.
// const char *p_text = (const char *)GetCheckButtonOneProperty(object_pointer, TC_GET_WINDOW_TEXT, NULL, 0, 0);
   #define TC_GET_TRANSPARENT_MODE      2
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TC_GET_TRANSPARENT_MODE는 CheckButton 컨트롤의 배경이 투명하게 설정되어있는지를 알아내는 기능을 한다. 인자들은 의미가 없으며
// 반환되는 값은 char 형이다. 반환값이 0이면 투명하지 않은 상태이고 1이면 배경이 투명한 상태이다.
// char checkbutton_transparent_flag = (char)GetCheckButtonOneProperty(object_pointer, TC_GET_TRANSPARENT_MODE, NULL, 0, 0);
   #define TC_GET_BK_COLOR              3
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TC_GET_BK_COLOR는 CheckButton 컨트롤에 포함된 택스트의 배경 색상을 알아내는 기능을 한다. 인자들은 의미가 없으며 반환되는 값은
// COLORREF형이다. 그리고 이 값은 배경색이 투명하지 않다고 선언된 경우에만 유효하다.
// COLORREF checkbutton_color = (COLORREF)GetCheckButtonOneProperty(object_pointer, TC_GET_BK_COLOR, NULL, 0, 0);
   #define TC_GET_FIX_POS               4
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TC_GET_FIX_POS는 CheckButton 컨트롤의 기본 위치를 알아내는 기능을 한다. 인자들은 의미가 없으며
// 반환되는 값은 int 형이다. 반환값만큼 CheckButton 컨트롤의 기본 위치를 나타낸다.
// int checkbutton_pos = GetCheckButtonOneProperty(object_pointer, TC_GET_FIX_POS, NULL, 0, 0);

// +------------------------------------------------+
// |  Object Name :  Tips ListBox Control        |
// +------------------------------------------------+
//
//  Microsoft Windows에서 제공하는 기본적인 ListBox 컨트롤은 자신이 가지고 있는 데이터를 표현하는데 많은 제약을 가지고 있다.
//  그래서 많은 프로그래머들이 사용자 정의 그리기(Owner Drawing)기법을 사용해 이런 단점을 극복하고 있는데, 이 객체는
//  그러한 단점을 보완하고 좀더 쉬운 방법으로 데이터 목록을 제공한다.
//
//   최초 설계자 : 김성엽 팀장.
//   최초 개발자 : 김성엽 팀장.    2000년 3월 17일
// ------------------------------------------------
//        변경자 :                     년  월   일
//          내용 : 
// ------------------------------------------------

// 이함수는 사용자가 지정한 위치, ID, style로 ListBox 객체를 생성한다. 이 함수는 8개의 인자를 가지며 그 내용은 다음과 같다.
// 1. CWnd *parm_parent
//    ListBox 객체를 포함할 윈도우의 주소 정보. 따라서 새로 생성되는 ListBox 객체는 parm_parent 윈도우의 자식 윈도우가 된다.
// 2. int parm_control_id
//    생성될 ListBox 컨트롤의 ID를 입력한다. ListBox 클래스는 자신의 상태변화나 사용자의 조작에 따른 Notify 메시지를 자신의
//    부모 윈도우에게 전달한다. 이때, 부모 윈도우는 이 ID를 가지고 어떤 객체에서 해당 메시지가 왔는지를 구별한다.
// 3. int parm_x
//    생성될 ListBox 컨트롤의 x축 시작점을 지정한다.
// 4. int parm_y
//    생성될 ListBox 컨트롤의 y축 시작점을 지정한다.
// 5. int parm_cx
//    생성될 ListBox 컨트롤의 x축 크기를 지정한다.
// 6. int parm_cy
//    생성될 ListBox 컨트롤의 y축 크기를 지정한다.
// 7. int parm_type
//    생성될 ListBox 컨트롤의 각종 윈도우 속성정보를 설정한다. 기본적으로 사용하는 속성은 다음과 같다.
//    7.1  Single Selection & Single Column.
//		   WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | LBS_OWNERDRAWFIXED | WS_CLIPSIBLINGS | LBS_NOTIFY |
//		   LBS_SORT | LBS_HASSTRINGS | LBS_NOINTEGRALHEIGHT);
//    7.2  Single Selection & Multi Column.
//	       WS_CHILD | WS_VISIBLE | WS_BORDER | WS_HSCROLL | LBS_OWNERDRAWFIXED | WS_CLIPSIBLINGS | LBS_NOTIFY |
//		   LBS_SORT | LBS_HASSTRINGS | LBS_NOINTEGRALHEIGHT | LBS_MULTICOLUMN);
//    7.3  Multi Selection & Single Column.
//         WS_CHILD | WS_BORDER | WS_HSCROLL | LBS_OWNERDRAWFIXED | WS_CLIPSIBLINGS | LBS_NOTIFY | LBS_EXTENDEDSEL |   
//		   LBS_SORT | LBS_HASSTRINGS | LBS_NOINTEGRALHEIGHT);
//    7.4  Multi Selection & Multi Column.
//         WS_CHILD | WS_BORDER | WS_HSCROLL | LBS_OWNERDRAWFIXED | WS_CLIPSIBLINGS | LBS_NOTIFY | LBS_EXTENDEDSEL |   
//		   LBS_SORT | LBS_MULTICOLUMN | LBS_HASSTRINGS | LBS_NOINTEGRALHEIGHT);
// 8. int parm_ext_type
//     리스트 박스의 Extened Window style을 설정한다..
//
// 이 함수는 CListBox *를 반환하는데, 이 값은 내부적으로 생성된 ListBox 컨트롤 객체의 주소이다. 이 객체는 new 연산자를
// 사용하여 동적으로 생성되기 때문에 넘겨 놓은 주소값을 저장시켜놓았다가 반드시 delete해야 한다. delete시에는 특별한
// 인터페이스를 사용할 필요없이 기억하고 있는 해당 포인터를 직접 delete시키면 된다. 
CListBox *CreateListBox(CWnd *, int , int , int , int , int , int, int );


// 생성된 ListBox 컨트롤에 아래에 나열된 속성을 설정하고자 하는 함수이다.
// 총 5가지 인자를 가지며 각 인자의 종류와 내용은 다음과 같다.
// 1. CListBox *parm_object
//    속성을 변경하고자 하는 ListBox 컨트롤의 주소정보이다.
// 2. int parm_id
//    어떤 속성을 변경할지를 지정. 변경 가능한 속성의 종류는 함수 설명이 끝난 후에 별도로 설명.
// 3. void *parm_property
//    포인터형 값을 가지는 설정 값을 전달한다.  ( parm_id의 값에 따라 설정값의 의미는 달라진다. )
// 4. int parm_data1
//    상수형 값을 가지는 설정 값을 전달한다.  ( parm_id의 값에 따라 설정값의 의미는 달라진다. )
// 5. int parm_data2
//    상수형 값을 가지는 설정 값을 전달한다.  ( parm_id의 값에 따라 설정값의 의미는 달라진다. )
int SetListBoxOneProperty(CListBox *, int , void *, int , int );
// SetListBoxOneProperty함수는 2번째 인자인 parm_id의 값에 따라서 3~5에 전달되는 정보가 결정된다. 따라서 ID에 따른 
// 필요 정보가 정확하지 않으면 원하지 않는 형태의 상황이 발생할수 있다. 변경 가능한 속성은 총 5가지이다. 그리고
// 각각의 의미와 사용법은 다음과 같다.
   #define TL_SET_WIDTH                 1
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TL_SET_WIDTH는 생성된 ListBox의 폭(width)의 크기를 설정하는 기능을 한다. 3번째나 4번째 인자에 원하는 값을 넣어주면 된다.
// 여기서 주의 할 것은 parm_property가 NULL이면 parm_data1의 값이 유효하고 parm_property가 NULL이 아니면 parm_property에
// 설정된 값으로 함수가 동작한다.
// int list_width = 240;
// SetListBoxOneProperty(object_pointer, TL_SET_WIDTH, &list_width, 0, 0);
//                               또는
// SetListBoxOneProperty(object_pointer, TL_SET_WIDTH, NULL, 240, 0);
   #define TL_SET_HEIGHT                2
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TL_SET_HEIGHT는 생성된 ListBox의 높이(height)의 크기를 설정하는 기능을 한다. 3번째나 4번째 인자에 원하는 값을 넣어주면 된다.
// 여기서 주의 할 것은 parm_property가 NULL이면 parm_data1의 값이 유효하고 parm_property가 NULL이 아니면 parm_property에
// 설정된 값으로 함수가 동작한다.
// int list_height = 150;
// SetListBoxOneProperty(object_pointer, TL_SET_HEIGHT, &list_height, 0, 0);
//                               또는
// SetListBoxOneProperty(object_pointer, TL_SET_HEIGHT, NULL, 150, 0);
   #define TL_SET_BACKGROUND_COLOR      3
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TL_SET_BACKGROUND_COLOR는 생성된 ListBox의 배경색 및 펜의 색을 변경하는 기능을 한다. 4번째와 5번째 인자에 원하는 색상을 넣어주면 된다.
// SetListBoxOneProperty(object_pointer, TL_SET_BACKGROUND_COLOR, NULL, RGB(0, 0, 255), RGB(0, 0, 255));
   #define TL_SET_CURSOR                4
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TL_SET_CURSOR는 생성된 ListBox의 Item이 선택되었을때 표시되는 커서의 색상을 설정하는 기능을 한다. 4~5에 색상 값을 넣어주면
// 되고 각각의 의미는 다음과 같다.
// (4, parm_data1)    : 커서의 채움색 설정. ( Brush 색상 선택 )
// (5, parm_data2)    : 커서의 테두리 색상 설정. ( Pen 색상 선택 )
// SetListBoxOneProperty(object_pointer, TL_SET_CURSOR, NULL, RGB(255, 251, 240), RGB(192, 192, 192));
   #define TL_SET_DISPLAY_FUNCTION      5
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TL_SET_DISPLAY_FUNCTION는 생성된 ListBox 객체가 가지고 있는 정보를 어떤식으로 보여줄 것인가를 정의한 함수에 대한 
// 포인터이다. ListBox 객체에서 가장 많이 바뀌는 부분은 이 객체가 관리하고 있는 정보에 대한 표현방식이다. 따라서 이러한
// 표현방법에 대한 정의는 객체 외부에서 코딩되어 인터페이스 함수를 통하여 객체의 내부로 전달되어 사용되어 진다.
// 외부에서 정의되어지는 함수 아래와 같은 형태로 정의되어져야 하면 이것은 반드시 지켜져야지만 한다.
// ---------------------------------------------------------------------------------------------------------------------------
// void display_function(CWnd *parm_base_wnd, CListBox *parm_this, int parm_index, CRect parm_rect, HDC parm_dc, char parm_push_flag);
// 이 함수에 전달되는 각각의 매개변수에 대해서 설명하면 다음과 같다.
// -. CWnd *parm_base_wnd
//    ListBox 컨트롤 객체를 생성한 부모 위도우의 포인터..
// -. CListBox *parm_list
//    이 ListBox 컨트롤 객체를 사용하는 것은 아마 CWnd 클래스 계열의 클래스중에 하나일 것이다. 따라서 이 객체는 해당 클래스의
//    데이터 멤버로 선언될 것이고 멤버함수가 아니면 이 객체의 정보를 알아낼수 없다. 하지만 display_function은 어떤 특정
//    클래스에 소속되어서 선언되지 않기 때문에 자신을 이용하게 될 ListBox 객체에 대해서 아무런 정보도 가지고 있지 않다.
//    따라서 display_function을 사용할 ListBox 컨트롤 객체는 이 함수를 호출할때. 첫번재 인자인 parm_list에 자신의 this 값을
//    전달해 준다. 따라서 사용자는 이 주소정보에서 자신이 원하는 정보를 얻어내어 코드를 완성하면 된다.
// -. int parm_index
//    ListBox의 여러 항목중 지금 출력해야할 항목의 index 값. ListBox는 갱신 방식이 전체적인 일괄 갱신이 아니라, 항목별 갱신을
//    하기 때문에 반드시 parm_index 값을 이용해서 해당되는 정보만을 갱신해야 한다.
// -. CRect parm_rect
//    ListBox의 여러 항목중 지금 출력해야할 항목의 위치 정보이다. 다른 항목의 영역을 침범하지 않도록 이 범위 내에서 어떤
//    정보를 나타내어야 한다.
// -. HDC parm_dc
//    사용하고 있는 ListBox가 할당한 DC정보이다. 새로운 DC를 생성해서 정보를 출력하지 말고 반드시 이 DC를 이용해서 출력을
//    해야지만 정확한 출력 결과를 얻을수 있다.
// -. char parm_push_flag
//    각 항목은 선택된 경우, 선택되지 않은 경우라는 두가지의 상태를 가진다. 이 값은 이 상태를 구분지어준다. 이 값이 0이면
//    현재 출력해야 할 항목은 선택이 되지 않은 경우이고 1이면 선택된 경우이다.
// ---------------------------------------------------------------------------------------------------------------------------
// 외부에서 설정된 함수의 주소값은 SetListBoxOneProperty의 3번째 변수로 전달되어진다.
// SetListBoxOneProperty(object_pointer, TL_SET_DISPLAY_FUNCTION, display_function, 0, 0);


// 생성된 ListBox 컨트롤에 아래에 나열된 속성에 어떤 값이 설정되어 있는지 알고자 하는 경우에 사용하는 함수이다.
// 총 5가지 인자를 가지며 각 인자의 종류와 내용은 다음과 같다.
// 1. CListBox *parm_object
//    속성을 알고자 하는 ListBox 컨트롤의 포인터.
// 2. int parm_id
//    어떤 속성을 알아낼 것인지를 지정. 지정 가능한 속성의 종류는 함수 설명이 끝난 후에 별도로 설명.
// 3. void *parm_property
//    포인터형 값을 가지는 설정 값을 전달한다.  ( parm_id의 값에 따라 설정값의 의미는 달라진다. )
// 4. int parm_data1
//    상수형 값을 가지는 설정 값을 전달한다.  ( parm_id의 값에 따라 설정값의 의미는 달라진다. )
// 5. int parm_data2
//    상수형 값을 가지는 설정 값을 전달한다.  ( parm_id의 값에 따라 설정값의 의미는 달라진다. )
int GetListBoxOneProperty(CListBox *, int , void *, int , int );
// GetListBoxOneProperty 2번째 인자인 parm_id의 값에 따라서 3~5에 전달되는 정보가 결정되고 자신이 얻고자 하는 정보도
// 결정된다. 따라서 ID를 잘못 지정하거나 ID에 따른 필요 정보가 정확하지 않으면 원하지 않는 형태 정보가 반환될 수도 있다.
// 이 컨트롤 객체가 제공하는 정보는 아래와 같이 모두 2개이며 각가의 사용법은 다음과 같다.
// 주의할 것은 이 함수는 범용적인 용도로 설계되었기 때문에 기능에 따라 적당히 캐스팅(Casting)해서 사용해야 한다.
   #define TL_GET_WIDTH                 1
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TL_GET_WIDTH는 생성된 ListBox 객체의 폭(width)의 크기를 알고자 하는 경우에 사용된다. 특별한 매개변수는 사용하지 않으며
// 반환되는 값은 int 형이다.
// int list_width = (int)GetDibOneProperty(object_pointer, TL_GET_WIDTH, NULL, 0, 0);
   #define TL_GET_HEIGHT                2
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TL_GET_HEIGHT는 생성된 ListBox 객체의 높이(height)의 크기를 알고자 하는 경우에 사용된다. 특별한 매개변수는 사용하지 않으며
// 반환되는 값은 int 형이다.
// int list_height = (int)GetDibOneProperty(object_pointer, TL_GET_HEIGHT, NULL, 0, 0);


CButton *CreateTipsMenuButtonObject(const char *parm_text, int parm_style, CRect parm_rect, CWnd *parm_parent, int parm_id);
CButton *CreateSubClassingTipsMenuButtonObject(int parm_ctrl_id, CWnd *parm_parent);


#define TMB_SET_TEXT_REVISION            1
#define TMB_SET_IMAGE_REVISION           2
#define TMB_SET_FONT                     3
#define TMB_CHANGE_FONT                  4
#define TMB_SET_BORDER_COLOR             5
#define TMB_SET_TEXT_COLOR               6
#define TMB_SET_WINDOW_TEXT              7
#define TMB_SET_BITMAP                   8
#define TMB_SET_DEFAULT                  9
#define TMB_SET_FOCUS_MODE              10
#define TMB_SET_TRANSPARENT_MODE        11
#define TMB_SET_FILL_COLOR              12
#define TMB_RESET_BITMAP                13
#define TMB_UPDATE_OBJECT               14

int SetTipsMenuButtonOneProperty(CButton *parm_object, int parm_id, void *parm_property, int parm_data1, int parm_data2);

#define TMB_GET_DEFAULT                  1
#define TMB_GET_FOCUS_MODE               2
#define TMB_GET_TRANSPARENT_MODE         3
#define TMB_GET_WINDOW_TEXT              4

int GetTipsMenuButtonOneProperty(CButton *parm_object, int parm_id, void *parm_property, int parm_data1, int parm_data2);


CComboBox *CreateComboBox(CWnd *parm_parent, int parm_control_id, int parm_x, int parm_y, int parm_cx, int parm_cy, int parm_type, int parm_ext_type);

#define TCB_SET_HEIGHT                1
#define TCB_SET_BACKGROUND_COLOR      2
#define TCB_SET_CURSOR                3
#define TCB_SET_DISPLAY_FUNCTION      4
int SetComboBoxOneProperty(CComboBox *parm_object, int parm_id, void *parm_property, int parm_data1, int parm_data2);

#define TCB_GET_HEIGHT                1
int GetComboBoxOneProperty(CComboBox *parm_object, int parm_id, void *parm_property, int parm_data1, int parm_data2);

CListBox *CreateDriveListBox(CWnd *parm_parent, int parm_control_id, int parm_x, int parm_y, int parm_cx, int parm_cy, int parm_type, int parm_ext_type);

#define TDL_SET_BACKGROUND_COLOR      1
#define TDL_SET_SELECTED_COLOR        2
#define TDL_SET_CURSOR                3
int SetDriveListBoxOneProperty(CListBox *parm_object, int parm_id, void *parm_property, int parm_data1, int parm_data2);

void ShowMentBox(HWND parm_parent, int parm_wait_time, char parm_on_top, const char *parm_title, const char *parm_content, int parm_wdith);


extern void TAPI_LoadSystemButtonImage();
extern void TAPI_DeleteSystemButtonImage();
extern void TAPI_ShowDibButtonImage(CDC *pDC, int parm_x, int parm_y, char parm_index);

class TipsDIB;

class StandardWnd : public CWnd
{
private:
	TipsDIB *mp_bk_image[9];

	CFont m_font;
	char m_is_clicked, m_is_active_state;
	CPoint m_original_pos, m_clicked_pos, m_original_size;

protected:
	CString m_caption_main, m_caption_sub;
	char m_is_maximized, m_resize_flag;
	char m_max_flag, m_min_flag, m_close_flag;
	int m_limit_x_size, m_limit_y_size;

// Construction
public:
	StandardWnd();
	virtual ~StandardWnd();

	void SetWindowTextExt(const char *parm_main, const char *parm_sub);
	void DisplayActiveState(CDC *parm_dc, CRect parm_rect);
	void SetWindowSystemMenu(char parm_min, char parm_max, char parm_close);
	virtual void OnViewDraw(CDC *pDC);
	
protected:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);

	DECLARE_MESSAGE_MAP()
};

class StandardDialog : public CDialog
{
private:
	TipsDIB *mp_bk_image[9];

	CFont m_font;
	char m_is_clicked, m_is_active_state;
	CPoint m_original_pos, m_move_pos, m_clicked_pos, m_original_size;

protected:
	CString m_caption_main, m_caption_sub;
	char m_is_maximized, m_resize_flag;
	char m_max_flag, m_min_flag, m_close_flag;
	int m_limit_x_size, m_limit_y_size;

// Construction
public:
	StandardDialog(unsigned int parm_id, CWnd* pParent = NULL);

	void SetWindowTextExt(const char *parm_main, const char *parm_sub);
	void DisplayActiveState(CDC *parm_dc, CRect parm_rect);
	void SetWindowSystemMenu(char parm_min, char parm_max, char parm_close);
	virtual void OnViewDraw(CDC *pDC);

	enum { IDD = 0 };

protected:
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);

	DECLARE_MESSAGE_MAP()
};

struct node {				//  리스트를 구성하게 될 노드의 구조..
	void *p_object;			//  사용자 정의 객체를 저장하기 위해 사용하는 포인터..
	node *p_next;			//  다음 노드를 가리키는 포인터..
};

class TipsWareList
{				  
private : 
	node *mp_head,*mp_tail;	 // 리스트의 처음과 끝을 가리키는 포인터..
	int m_node_count;		 // 노드의 갯수를 가지고 있는 변수..

public :
	TipsWareList();           // 객체 생성자..
	virtual ~TipsWareList();  // 객체 파괴자..

	void InitListObject();
	char AddObject(void *parm_object,int parm_size);    // 새로운 노드를 추가하는 경우, (정적)
	char AddObject(void *parm_object);			        // 새로운 노드를 추가하는 경우, (동적)
	char AddObjectByIndex(int parm_index, void *parm_object);
	char DeleteObject(node *parm_node);	                // 존재하는 노드를 삭제하는 경우..
	char DeleteNodeExceptObject(node *parm_object);
	char DeleteHeadObject();							// 가장 처음 노드를 제거하는 경우..
	void DeleteAllObject();								// 존재하는 객체 모두를 제거하는 경우..
	void DeleteAllObjectExceptObject();			        // 존재하는 노드를 모두 삭제한다..
	char DeleteObjectByIndex(int parm_index);			// 사용자가 지정한 위치에 존재하는 객체를 파괴한다.
	char DeleteNodeExceptObjectByIndex(int parm_index);

	node *GetNodeByIndex(int parm_index);
	void *GetObjectByIndex(int parm_index);
	int FindNodeIndexByObject(void *parm_object);

	node *GetHeadObject(); 								// 가장 첫 노드의 주소를 얻는다..
	node *GetTailObject(); 								// 가장 마지막 노드의 주소를 얻는다..
	int GetNodeCount();									// 노드의 갯수를 얻는다..
};

#define MAX_TOKEN_LEN    120
#define MAX_COMMAND      5

class StandardWndExt;

typedef struct InternalWindowObject {
	int type;
	char *p_name;
	CWnd *p_wnd;
} IWO;

typedef struct CommandType {
	char c_name[MAX_TOKEN_LEN];      
	void (StandardWndExt::*p)();
} CT;

class StandardWndExt : public StandardWnd
{
private:
	char m_token[MAX_TOKEN_LEN];      // m_token string
	char m_token_type;                // m_token type
	char m_tok_id;                       
	char *mp_buff, *mp_script_data;
	CT m_command[MAX_COMMAND];
	CString m_file_path;

protected:
	TipsWareList m_object_list;

// Construction
public:
	StandardWndExt();
	virtual ~StandardWndExt();

	void OnViewDraw(CDC *pDC);

	int GetTokenData();
	int LookUpCommand(char *s);
	int IsDelimiter(char c);
	int IsWhiteSpace(char c);
	void SyntaxErrorInScript();
	void SetScript(char *parm_script);
	const char *GetCurrentFilePath();
	void InitObject();
	void DeleteObject();
	void RunScript();

	IWO *GetControlByName(const char *parm_name);
	void GetRectData(int parm_rect[], char parm_remove_prev_comma);
	const char *GetStringData(char parm_remove_prev_comma);
	int GetIntData(char parm_remove_prev_comma);

	void CreateTipsButton();
	void CreateTipsStatic();
	void CreateTipsCheck();
	void CreateTipsDIB();
	void CreateTipsListBox();

protected:
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	DECLARE_MESSAGE_MAP()
};

class StandardDialogExt : public StandardDialog
{
private:
	char m_token[MAX_TOKEN_LEN];      // m_token string
	char m_token_type;                // m_token type
	char m_tok_id;                       
	char *mp_buff, *mp_script_data;
	CT m_command[MAX_COMMAND];
	CString m_file_path;

protected:
	TipsWareList m_object_list;

// Construction
public:
	StandardDialogExt(unsigned int parm_id, CWnd* pParent = NULL);
	
	virtual void OnViewDraw(CDC *pDC);

	int GetTokenData();
	int LookUpCommand(char *s);
	int IsDelimiter(char c);
	int IsWhiteSpace(char c);
	void SyntaxErrorInScript();
	void SetScript(char *parm_script);
	const char *GetCurrentFilePath();
	void InitObject();
	void DeleteObject();
	void RunScript();

	IWO *GetControlByName(const char *parm_name);
	void GetRectData(int parm_rect[], char parm_remove_prev_comma);
	const char *GetStringData(char parm_remove_prev_comma);
	int GetIntData(char parm_remove_prev_comma);

	void CreateTipsButton();
	void CreateTipsStatic();
	void CreateTipsCheck();
	void CreateTipsDIB();
	void CreateTipsListBox();

	enum { IDD = 0 };

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();

	DECLARE_MESSAGE_MAP()
};

class TipsProcThread : public CWinThread
{
private:
	CWnd *mp_control_wnd;
	HANDLE mh_kill_event;
	int m_exit_notify_id;

// Attributes
public:
	TipsProcThread(CWnd *parm_control_wnd, UINT (*parm_fp)(LPVOID), void *parm_data, int parm_exit_notify_id);
	virtual ~TipsProcThread();

	void StartThread(int parm_level);
	void KillProcThread();
	HANDLE GetKillEventHandle();

	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};

#endif
