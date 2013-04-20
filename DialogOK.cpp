// DialogOK.cpp : implementation file
//

#include "stdafx.h"
#include "LQDetect.h"
#include "DialogOK.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogOK dialog


CDialogOK::CDialogOK(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogOK::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogOK)
	m_sText = _T("«Î»∑»œ£°");
	//}}AFX_DATA_INIT
}

void CDialogOK::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogOK)
	DDX_Text(pDX, IDC_EDIT_TEXT, m_sText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogOK, CDialog)
	//{{AFX_MSG_MAP(CDialogOK)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogOK message handlers

BOOL CDialogOK::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	::SetWindowPos(m_hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE |SWP_NOSIZE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
