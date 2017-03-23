// KeyView.cpp : implementation file
//

#include "stdafx.h"
#include "imm.h"
#include "KeyView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKeyView
#define TAB 0x02
#define CAPSLOCK 0x03
#define SHIFT 0x04
#define CTRL 0x05
#define BSPACE 0x06
#define ENTER 0x07
#define ALT 0x08
#define WINDOW 0x9
#define LEFT 0x0A
#define RIGHT 0x0B
#define UP 0x0C
#define DOWN 0x0D
#define ESC 0x0E
#define PUP 0x0F
#define PDN 0x10
#define HOME 0x11
#define END 0x12
#define INS 0x13
#define DEL 0x14
#define WWW 0x15
#define A	0x16
#define a	0x17

IMPLEMENT_DYNCREATE(CKeyView, CScrollView)

CKeyView::CKeyView()
{
	m_bHangul=FALSE;
}

CKeyView::~CKeyView()
{
	unsigned char vk = VK_SHIFT;
	int cnt;

	cnt = cKeys.GetCount();
	
	for(int i=0; i<cnt; i++)
	{
		key = cKeys.RemoveTail();
		delete key;
	}

	if(GetKeyState(vk) & 0xF000)
	{
		keybd_event(vk,0,KEYEVENTF_KEYUP,0);
	}
	vk = VK_CONTROL;
	if(GetKeyState(vk) & 0xF000) //allow toggle
	{
		keybd_event(vk,0,KEYEVENTF_KEYUP,0);
	}

	HookOff();
}


BEGIN_MESSAGE_MAP(CKeyView, CScrollView)
	//{{AFX_MSG_MAP(CKeyView)
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER+10, OnHookMsg)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKeyView drawing

void CKeyView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

//	CSize sizeTotal;
	// TODO: calculate the total size of this view
//	sizeTotal.cx = sizeTotal.cy = 100;
//	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CKeyView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
	DrawKeyboard();
}

/////////////////////////////////////////////////////////////////////////////
// CKeyView diagnostics

#ifdef _DEBUG
void CKeyView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CKeyView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CKeyView message handlers


void CKeyView::InitKey()
{
	SetScrollSizes( MM_TEXT, CSize( 100, 100 ) );
//	AddKey(0x00,0x00,0); //signal row change
//	AddKey(0x00,ESC,10);
/*	for(int fnkey = 0x70; fnkey < 0x7C; fnkey++)
	{
		AddKey(0x00,fnkey & 0xFF,10);
	}
	AddKey(0x00,INS,14);
	AddKey(0x00,DEL,-10);
*/
	AddKey(0x00,0x00,1); //signal row change
	AddKey('`','~',10);
	AddKey('1','!',10);
	AddKey('2','@',10);
	AddKey('3','#',10);
	AddKey('4','$',10);
	AddKey('5','%',10);
	AddKey('6','^',10);
	AddKey('7','&',10);
	AddKey('8','*',10);
	AddKey('9','(',10);
	AddKey('0',')',10);
	AddKey('-','_',10);
	AddKey('=','+',10);
	AddKey(0x00,BSPACE,-12); //backspace
//	AddKey(0x00,PUP,-12);

	//second row
	AddKey(0x00,0x00,2); //signal row change
	AddKey(0x00,TAB,16); //tab
	AddKey('q','Q',10);
	AddKey('w','W',10);
	AddKey('e','E',10);
	AddKey('r','R',10);
	AddKey('t','T',10);
	AddKey('y','Y',10);
	AddKey('u','U',10);
	AddKey('i','I',10);
	AddKey('o','O',10);
	AddKey('p','P',10);
	AddKey('[','{',10);
	AddKey(']','}',10);
	AddKey('\\','|',-10);
//	AddKey(0x00,PDN,-12);

	//third row
	AddKey(0x00,0x00,3); //signal row change
	AddKey(0x00,CAPSLOCK,18); //caps lock
	AddKey('a','A',10);
	AddKey('s','S',10);
	AddKey('d','D',10);
	AddKey('f','F',10);
	AddKey('g','G',10);
	AddKey('h','H',10);
	AddKey('j','J',10);
	AddKey('k','K',10);
	AddKey('l','L',10);
	AddKey(';',':',10);
	AddKey('\'','\"',10);
	AddKey(0x00,ENTER,-16); //enter
//	AddKey(0x00,HOME,-10);

	//fourth row
	AddKey(0x00,0x00,4); //signal row change
	AddKey(0x00,SHIFT,23); //shift
	AddKey('z','Z',10);
	AddKey('x','X',10);
	AddKey('c','C',10);
	AddKey('v','V',10);
	AddKey('b','B',10);
	AddKey('n','N',10);
	AddKey('m','M',10);
	AddKey(',','<',10);
	AddKey('.','>',10);
	AddKey('/','?',10);
	AddKey(0x00,SHIFT,-22); //shift
//	AddKey(0x00,END,-10);

	//fifth row
	AddKey(0x00,0x00,5); //signal row change
	AddKey(0x00,CTRL,13); //ctrl
	AddKey(0x00,ALT,12); //alt
	//AddKey(0x00,WINDOW,12); //window
	AddKey(' ',' ',84);
	//AddKey(0x00,VK_HANGUL,12); //window
	AddKey(0x00,LEFT,10); //left
	AddKey(0x00,UP,10); //up
	AddKey(0x00,DOWN,10); //down
	AddKey(0x00,RIGHT,-10); //right
	AddKey(0x00,WWW,-10); //http://WWW

	CalcWidthHeight();
	
	HookInit(this->m_hWnd);
}

void CKeyView::DrawKeyboard()
{
	CClientDC dc(this);
	CRect client;
	GetClientRect(client);
	CRgn clipRgn;
	clipRgn.CreateRectRgnIndirect(&client);
	dc.SelectClipRgn(&clipRgn);
	dc.SelectObject(GetStockObject(DEFAULT_GUI_FONT));
	dc.SetBkMode(TRANSPARENT);
	int x = 0;
	int y = -pKeyHeight;
//	KEYDEF * key;
	POSITION pos = cKeys.GetHeadPosition();
	cCurrentKeyState = DescribeKeyState();
	int state = cCurrentKeyState;
	CString strTmp;

	while(pos)
	{
		key = cKeys.GetNext(pos);
		if((key->cNormal == 0x00) && (key->cShifted == 0x00))
		{
			x = 0;
			y += pKeyHeight;
		}
		else
		{
			int width = ((abs(key->cWidth) * pKeyWidth) / 10);
			CRect rc(x,y,x + width, y + pKeyHeight);
			x += width;
			if(key->cWidth < 0)
			{
				rc.right = client.right; 
			}
			BOOL hilight = FALSE;
			if(key->cNormal == 0x00)
			{

				switch(key->cShifted)
				{
				case CAPSLOCK:
					if(state & 0x01)
					{
						hilight = TRUE;
					}
					
					break;
				case SHIFT:
					if(state & 0x02)
					{
						hilight = TRUE;
					}
					break;
				case CTRL:
					if(state & 0x04)
					{
						hilight = TRUE;
					}
					break;

				case TAB:
					if(state & 0x08)
					{
						hilight = TRUE;
					}
					break;

				case WWW:
					if(state & 0x10)
					{
						hilight = TRUE;
					}
					break;
				default:
					break;
				}
			}
			
			DrawKey(&dc, rc, key, hilight);
		}
	}
}

void CKeyView::DrawKey(CDC *dc, CRect &rc, KEYDEF *key, BOOL cHilight)
{
	HIMC hIMC = NULL;  
	CSize tsize;
	dc->SetBkMode(TRANSPARENT);

	DWORD dwConversion, dwSentence;
	HWND hFocusWnd = ::GetFocus();  
	BOOL bChangedHangulMode;
	//--- IME context를 구한다  
	
	if (hIMC = ImmGetContext(hFocusWnd))  
	{
		
		ImmGetConversionStatus(hIMC, &dwConversion, &dwSentence);
	
		bChangedHangulMode = ImmGetConversionStatus(hIMC, &dwConversion, &dwSentence);
	}
	if(bChangedHangulMode)
	{  
		//--- 한글이면 TRUE, 영문이면 FALSE를 구한다  
		m_bHangul = (dwConversion & IME_CMODE_NATIVE ? TRUE : FALSE);  
	}  

	if(rc.left==667 && m_bHangul)
	{
		rc.DeflateRect(1,1);
		dc->FillSolidRect(rc,RGB(58,110,165)/*GetSysColor(COLOR_3DLIGHT)*/);
		dc->Draw3dRect(rc,GetSysColor(COLOR_3DLIGHT),GetSysColor(COLOR_3DSHADOW));
		rc.DeflateRect(1,1);
		dc->Draw3dRect(rc,GetSysColor(COLOR_3DSHADOW),GetSysColor(COLOR_3DDKSHADOW));
	}
	else
	{
		if(!cHilight)
		{
			rc.DeflateRect(1,1);
			dc->FillSolidRect(rc,RGB(255,255,255)/*GetSysColor(COLOR_3DFACE)*/);
			dc->Draw3dRect(rc,GetSysColor(COLOR_3DSHADOW),GetSysColor(COLOR_3DDKSHADOW));
			rc.DeflateRect(1,1);
			dc->Draw3dRect(rc,GetSysColor(COLOR_3DLIGHT),GetSysColor(COLOR_3DSHADOW));
		}
		else
		{
			rc.DeflateRect(1,1);
			dc->FillSolidRect(rc,RGB(58,110,165));
			dc->Draw3dRect(rc,GetSysColor(COLOR_3DLIGHT),GetSysColor(COLOR_3DSHADOW));
			rc.DeflateRect(1,1);
			dc->Draw3dRect(rc,GetSysColor(COLOR_3DSHADOW),GetSysColor(COLOR_3DDKSHADOW));
			
		}
	}
	rc.DeflateRect(3,1);
	if(key->cNormal == 0x00)
	{
		CString label = " ";
		BOOL special = FALSE;
		if(key->cShifted > 0x6F) //is a function
		{
			int fkeynum = key->cShifted - 0x6F;
			label.Format("F%d",fkeynum);
		}
		else
		{
			switch(key->cShifted)
			{
			case TAB:    // 0x02
				label = "Tab->";
				break;
			case CAPSLOCK:    // 0x03
				label = "Caps";
				break;
			case SHIFT:    // 0x04
				label = "Shift";
				break;
			case CTRL:    // 0x05
				label = "Ctrl";
				break;
			case BSPACE:    // 0x06
				label = "<-Bspc";
				break;
			case ENTER:    // 0x07
				label = "Enter";
				break;
			case ALT:    // 0x08
				label = "Alt";
				break;
			case WINDOW:    // 0x9
				//label = "한자";
				break;
			case ESC:
			//	label = "Esc";
				break;
			case LEFT:    // 0x0A
				special = TRUE;
				{				
					tsize = dc->GetTextExtent("←");
					dc->TextOut(
						((rc.left + rc.right)/2) - (tsize.cx/2), 
						((rc.top + rc.bottom)/2) - (tsize.cy/2), 
						"←");
				}
				break;
			case RIGHT:    // 0x0B
				special = TRUE;
				{
					tsize = dc->GetTextExtent("→");
					dc->TextOut(
						((rc.left + rc.right)/2) - (tsize.cx/2), 
						((rc.top + rc.bottom)/2) - (tsize.cy/2), 
						"→");
				}
				break;
			case UP:    // 0x0C
				special = TRUE;
				{
					tsize = dc->GetTextExtent("↑");
					dc->TextOut(
						((rc.left + rc.right)/2) - (tsize.cx/2), 
						((rc.top + rc.bottom)/2) - (tsize.cy/2), 
						"↑");
				}
				break;
			case DOWN:    // 0x0D
				special = TRUE;
				{
					tsize = dc->GetTextExtent("↓");
					dc->TextOut(
						((rc.left + rc.right)/2) - (tsize.cx/2), 
						((rc.top + rc.bottom)/2) - (tsize.cy/2), 
						"↓");
				}
				break;
			case PUP: //0x0F
			//	label = "PgUp";
				break;
			case PDN: //0x10
			//	label = "PgDn";
				break;
			case HOME: //0x11
			//	label = "Home";
				break;
			case END: //0x12
			//	label = "End";
				break;
			case INS: //0x13
			//	label = "Ins";
				break;
			case DEL: //0x14
			//	label = "Del";
				break;
			case WWW:
			//	label = "한/영";
				break;
			default:
				ASSERT(FALSE);
			//	label = "#ERR#";
				break;
			}
		}
		if(!special)
		{
			CSize tsize;
			tsize = dc->GetTextExtent(label);
			dc->TextOut(
				((rc.left + rc.right)/2) - (tsize.cx/2), 
				((rc.top + rc.bottom)/2) - (tsize.cy/2), 
				label);
		}
	}
	else
	{
//		tsize = dc->GetTextExtent('M');
//		dc->TextOut(rc.left+1, rc.top+5, key->cShifted);

		tsize = dc->GetTextExtent('M');
		dc->TextOut(rc.CenterPoint().x-2, rc.CenterPoint().y-2, key->cShifted);

		int right=rc.top+3;
		if((key->cShifted >= 'A') && (key->cShifted <= 'Z'))
		{
/*			if(key->cShifted=='Q')
			{
				dc->TextOut(
				rc.right - 15, 
				rc.bottom - 16,
				"ㅂ");
				dc->TextOut(rc.right-15, right, "ㅃ");
			}
			if(key->cShifted=='W')
			{
				dc->TextOut(
				rc.right - 15, 
				rc.bottom - 16,
				"ㅈ");
				dc->TextOut(rc.right-15, right, "ㅉ");
			}
			if(key->cShifted=='E')
			{
				dc->TextOut(
				rc.right - 15, 
				rc.bottom - 16,
				"ㄷ");
				dc->TextOut(rc.right-15, right, "ㄸ");
			}
			if(key->cShifted=='R')
			{
				dc->TextOut(
				rc.right - 15, 
				rc.bottom - 16,
				"ㄱ");
				dc->TextOut(rc.right-15, right, "ㄲ");
			}
			if(key->cShifted=='T')
			{
				dc->TextOut(
				rc.right - 15, 
				rc.bottom - 16,
				"ㅅ");
				dc->TextOut(rc.right-15, right, "ㅆ");
			}
			if(key->cShifted=='Y')
			{
				dc->TextOut(
				rc.right - 15, 
				rc.bottom - 16,
				"ㅛ");
			}
			if(key->cShifted=='U')
			{
				dc->TextOut(
				rc.right - 15, 
				rc.bottom - 16,
				"ㅕ");
			}
			if(key->cShifted=='I')
			{
				dc->TextOut(
				rc.right - 15, 
				rc.bottom - 16,
				"ㅑ");
			}
			if(key->cShifted=='O')
			{
				dc->TextOut(
				rc.right - 15, 
				rc.bottom - 16,
				"ㅐ");
				dc->TextOut(rc.right-15, right, "ㅒ");
			}
			if(key->cShifted=='P')
			{
				dc->TextOut(
				rc.right - 15, 
				rc.bottom - 16,
				"ㅔ");
				dc->TextOut(rc.right-15, right, "ㅖ");
			}


			if(key->cShifted=='A')
			{
				dc->TextOut(
				rc.right - 15, 
				rc.bottom - 16,
				"ㅁ");
			}
			if(key->cShifted=='S')
			{
				dc->TextOut(
				rc.right - 15, 
				rc.bottom - 16,
				"ㄴ");
			
			}
			if(key->cShifted=='D')
			{
				dc->TextOut(
				rc.right - 15, 
				rc.bottom - 16,
				"ㅇ");
			
			}
			if(key->cShifted=='F')
			{
				dc->TextOut(
				rc.right - 15, 
				rc.bottom - 16,
				"ㄹ");
			
			}
			if(key->cShifted=='G')
			{
				dc->TextOut(
				rc.right - 15, 
				rc.bottom - 16,
				"ㅎ");
		
			}
			if(key->cShifted=='H')
			{
				dc->TextOut(
				rc.right - 15, 
				rc.bottom - 16,
				"ㅗ");
			
			}
			if(key->cShifted=='J')
			{
				dc->TextOut(
				rc.right - 15, 
				rc.bottom - 16,
				"ㅓ");
		
			}
			if(key->cShifted=='K')
			{
				dc->TextOut(
				rc.right - 15, 
				rc.bottom - 16,
				"ㅏ");
		
			}
			if(key->cShifted=='L')
			{
				dc->TextOut(
				rc.right - 15, 
				rc.bottom - 16,
				"ㅣ");
		
			}


			if(key->cShifted=='Z')
			{
				dc->TextOut(
				rc.right - 15, 
				rc.bottom - 16,
				"ㅋ");
			}if(key->cShifted=='X')
			{
				dc->TextOut(
				rc.right - 15, 
				rc.bottom - 16,
				"ㅌ");
			}
			if(key->cShifted=='C')
			{
				dc->TextOut(
				rc.right - 15, 
				rc.bottom - 16,
				"ㅊ");
			}if(key->cShifted=='V')
			{
				dc->TextOut(
				rc.right - 15, 
				rc.bottom - 16,
				"ㅍ");
			}
			if(key->cShifted=='B')
			{
				dc->TextOut(
				rc.right - 15, 
				rc.bottom - 16,
				"ㅠ");
			}if(key->cShifted=='N')
			{
				dc->TextOut(
				rc.right - 15, 
				rc.bottom - 16,
				"ㅜ");
			}
			if(key->cShifted=='M')
			{
				dc->TextOut(
				rc.right - 15, 
				rc.bottom - 16,
				"ㅡ");
			}*/
		}
		else
		{
//			dc->TextOut(rc.right - 15, 
//						rc.bottom - 16,
//						key->cNormal);

//			dc->TextOut(rc.right - 15, 
//						rc.bottom - 16,
//						key->cNormal);
		}

			
		//}
	}
	Invalidate(FALSE);
}

int CKeyView::DescribeKeyState()
{
	int state = 0;
	
	short ks;

	ks = GetKeyState(VK_CAPITAL);
	if(ks & 0x000F) state += 0x01;
	ks = GetKeyState(VK_SHIFT);
	if(ks & 0xF000) state += 0x02;
	ks = GetKeyState(VK_CONTROL);
	if(ks & 0xF000)	state += 0x04;
	
//	ks = GetKeyState(VK_A);
//	if(ks & 0x8000)
//	{
//		MessageBox("a");
//	}


//	ks = GetKeyState(VK_TAB);
//	if(ks & 0x8000) state += 0x08;//실제 키보드 눌리면 반응

//	ks = GetKeyState(VK_HANGUL);
//	if(ks & 0x000F)  m_bHangul=TRUE;//실제 키보드 눌리면 반응
//	case ALT:    // 0x08
//		vk = VK_MENU;
//		break;
	
	return state;	
}

void CKeyView::AddKey(char pNormal, char pShifted, int pWidth)
{
	key = new KEYDEF;

	key->cNormal = pNormal;
	key->cShifted = pShifted;
	key->cWidth = pWidth;

	cKeys.AddTail(key);

//	delete key;
}

void CKeyView::SendKey(KEYDEF *keydef)
{
	if(keydef->cNormal == 0x00)
	{
		int vk;
		BOOL uptoo = TRUE;
		if(keydef->cShifted > 0x6F) //is a function key
		{
			vk = keydef->cShifted;
		}
		else
		{
			switch(keydef->cShifted)
			{
			case TAB:    // 0x02
				vk = VK_RIGHT;
				break;
			case CAPSLOCK:    // 0x03
				vk = VK_CAPITAL;
				break;
			case SHIFT:    // 0x04
				vk = VK_SHIFT;
				if(GetKeyState(vk) & 0xF000) 
				{
					keybd_event(vk,0,KEYEVENTF_KEYUP,0);
					return;
				}
				uptoo = FALSE;
				break;
			case CTRL:    // 0x05
				vk = VK_CONTROL;
				if(GetKeyState(vk) & 0xF000) 
				{
					keybd_event(vk,0,KEYEVENTF_KEYUP,0);
					return;
				}
				uptoo = FALSE;
				break;
			case BSPACE:    // 0x06
				vk = VK_BACK;
				break;
			case ENTER:    // 0x07
				vk = VkKeyScan(0x0D) & 0xFF;
				break;
			case ALT:    // 0x08
				vk = VK_MENU;
				break;
			case WINDOW:    // 0x9
				vk = VK_HANJA;
				break;
			case LEFT:    // 0x0A
				vk = VK_LEFT;
				break;
			case RIGHT:    // 0x0B
				vk = VK_RIGHT;
				break;
			case UP:    // 0x0C
				vk = VK_UP;
				break;
			case DOWN:    // 0x0D
				vk = VK_DOWN;
				break;
			case PUP: //0x0F
				vk = VK_PRIOR;
				break;
			case PDN: //0x10
				vk = VK_NEXT;
				break;
			case HOME: //0x11
				vk = VK_HOME;
				break;
			case END: //0x12
				vk = VK_END;
				break;
			case INS: //0x13
				vk = VK_INSERT;
				break;
			case DEL: //0x14
				vk = VK_DELETE;
				break;
			case WWW:
			//	KEYDEF key;
				vk = VK_HANGUL;
				break;
			default:
				ASSERT(FALSE);
				break;
			}
		}
		keybd_event(vk,0,0,0);
		
		if(uptoo)
		{
			keybd_event(vk,0,KEYEVENTF_KEYUP,0);
		}
	}
	else
	{
		char pChar = keydef->cNormal;
		SHORT ks = VkKeyScan(pChar);
		BYTE key = ks & 0xFF;

		keybd_event(key,0,0,0);
		keybd_event(key,0,KEYEVENTF_KEYUP,0);

		//turn off the control and shift if they were down
		unsigned char vk = VK_SHIFT;
		if(GetKeyState(vk) & 0xF000)
		{
			keybd_event(vk,0,KEYEVENTF_KEYUP,0);
		}
		vk = VK_CONTROL;
		if(GetKeyState(vk) & 0xF000) //allow toggle
		{
			keybd_event(vk,0,KEYEVENTF_KEYUP,0);
		}
	}
}

long CKeyView::OnHookMsg(WPARAM wParam, LPARAM lParam)
{
	LPMOUSEHOOKSTRUCT MouseParam;
	MouseParam = (MOUSEHOOKSTRUCT *)lParam;
	CPoint point = MouseParam->pt;;
	ScreenToClient(&point);


	CRect client;
	GetClientRect(client);
	int x = 0;
	int y = -pKeyHeight;
//	KEYDEF * key;
	POSITION pos = cKeys.GetHeadPosition();
	while(pos)
	{
		key = cKeys.GetNext(pos);
		if((key->cNormal == 0x00) && (key->cShifted == 0x00))
		{
			x = 0;
			y += pKeyHeight;
		}
		else
		{
			int width = ((abs(key->cWidth) * pKeyWidth) / 10);
			CRect rc(x,y,x + width, y + pKeyHeight);
			if(key->cWidth < 0)
			{
				rc.right = client.right;
			}
			x += width;
			if(rc.PtInRect(point))
			{
				CClientDC dc(this);
				rc.DeflateRect(1,1);
				dc.FillSolidRect(rc,RGB(58,110,165)/*GetSysColor(COLOR_3DLIGHT)*/);
				dc.Draw3dRect(rc,GetSysColor(COLOR_3DLIGHT),GetSysColor(COLOR_3DSHADOW));
				rc.DeflateRect(1,1);
				dc.Draw3dRect(rc,GetSysColor(COLOR_3DSHADOW),GetSysColor(COLOR_3DDKSHADOW));
				Invalidate(FALSE);
				Sleep(100);
				SendKey(key);	
			}
		}
	}

	return 0;
}

void CKeyView::CalcWidthHeight()
{
	pKeyWidth = 0;
	pKeyHeight = 0;

	POSITION pos = cKeys.GetHeadPosition();
	long totalWidth = 0;
	long longest = 0;
	long rows = 0;
	while(pos)
	{
		key = cKeys.GetNext(pos);
		if((key->cNormal == 0x00) && (key->cShifted == 0x00))
		{
			rows++;
			if(totalWidth > longest)
			{
				longest = totalWidth;
			}
			totalWidth = 0;
		}
		else
		{
			totalWidth += abs(key->cWidth);
		}
	}
	CRect rect;
	GetClientRect(rect);
	pKeyHeight = (rect.Height()+5) / rows;
	pKeyWidth = (int)(((double)rect.Width()) / ((((double)(longest)) / 10)));
}

BOOL CKeyView::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	HWND wnd = ::GetForegroundWindow(); 
	

	if(IsWindow(wnd)) 
	{ 
		if(wnd != this->m_hWnd) 
		{ 
			if(gFocus != wnd) 
			{ 
				if(IsWindow(gFocus)) 
				{ 
					AttachThreadInput(
					GetWindowThreadProcessId(m_hWnd,NULL), 
					GetWindowThreadProcessId(gFocus,NULL), 
					FALSE); 
				} 
				gFocus = wnd; 
				AttachThreadInput(
				GetWindowThreadProcessId(m_hWnd,NULL), 
				GetWindowThreadProcessId(gFocus,NULL), 
				TRUE); 
				HookOn();
			} 
		} 
	} 
	
	
	return CScrollView::PreTranslateMessage(pMsg);
}





void CKeyView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
//		LPMOUSEHOOKSTRUCT MouseParam;
//	MouseParam = (MOUSEHOOKSTRUCT *)lParam;
//	CPoint point = point;;
//	ScreenToClient(&point);

/*	Invalidate(FALSE);
	CRect client;
	GetClientRect(client);
	int x = 0;
	int y = -pKeyHeight;
	KEYDEF * key;
	POSITION pos = cKeys.GetHeadPosition();
	while(pos)
	{
	//	AfxMessageBox("TEST");
		key = cKeys.GetNext(pos);
		if((key->cNormal == 0x00) && (key->cShifted == 0x00))
		{
			x = 0;
			y += pKeyHeight;
		}
		else
		{
		//	AfxMessageBox("TEST");
			int width = ((abs(key->cWidth) * pKeyWidth) / 10);
			CRect rc(x,y,x + width, y + pKeyHeight);
			if(key->cWidth < 0)
			{
				rc.right = client.right;
			}
			x += width;
			if(rc.PtInRect(point))
			{
			
			//	AfxMessageBox("TEST");
				CClientDC dc(this);
				rc.DeflateRect(1,1);
				dc.FillSolidRect(rc,RGB(58,110,165));
				dc.Draw3dRect(rc,GetSysColor(COLOR_3DLIGHT),GetSysColor(COLOR_3DSHADOW));
				rc.DeflateRect(1,1);
				dc.Draw3dRect(rc,GetSysColor(COLOR_3DSHADOW),GetSysColor(COLOR_3DDKSHADOW));
			//	Invalidate(FALSE);
				Sleep(20);
			//	SendKey(key);
				
				
			}
		}
	}
//	CScrollView::OnMouseMove(nFlags, point);*/
}

void CKeyView::Close()
{
	int cnt;
	cnt = cKeys.GetCount();

	for(int i=0; i<cnt; i++)
	{
		key = cKeys.RemoveTail();
		delete key;
	}
}
