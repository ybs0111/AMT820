// Dialog_Memo.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Dialog_Memo.h"
#include "Keyboarddll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialog_Memo dialog


CDialog_Memo::CDialog_Memo(CWnd* pParent /*=NULL*/)
	: CDialog(CDialog_Memo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialog_Memo)
	m_strmemo = _T("");
	//}}AFX_DATA_INIT
}


void CDialog_Memo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialog_Memo)
	DDX_Control(pDX, IDOK, m_btn_apply);
	DDX_Control(pDX, IDCANCEL, m_btn_cancel);
	DDX_Control(pDX, IDC_GROUP_MEMO, m_group_memo);
	DDX_Text(pDX, IDC_EDIT_MEMO, m_strmemo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialog_Memo, CDialog)
	//{{AFX_MSG_MAP(CDialog_Memo)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog_Memo message handlers
