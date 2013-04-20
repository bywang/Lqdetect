// DialogSplash.cpp : implementation file
//

#include "stdafx.h"
#include "LQDetect.h"
#include "DialogSplash.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogSplash dialog


CDialogSplash::CDialogSplash(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogSplash::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogSplash)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDialogSplash::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogSplash)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogSplash, CDialog)
	//{{AFX_MSG_MAP(CDialogSplash)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogSplash message handlers

BOOL CDialogSplash::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	::SetWindowPos(m_hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE |SWP_NOSIZE);

	SetTimer(1,3000,NULL);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogSplash::OnTimer(UINT nIDEvent) 
{
	OnOK();
	KillTimer(1);
	
	CDialog::OnTimer(nIDEvent);
}
