// DialogOKCANCEL.cpp : implementation file
//

#include "stdafx.h"
#include "LQDetect.h"
#include "DialogOKCANCEL.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogOKCANCEL dialog


CDialogOKCANCEL::CDialogOKCANCEL(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogOKCANCEL::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogOKCANCEL)
	m_sText = _T("");
	//}}AFX_DATA_INIT
}


void CDialogOKCANCEL::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogOKCANCEL)
	DDX_Text(pDX, IDC_EDIT_TEXT, m_sText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogOKCANCEL, CDialog)
	//{{AFX_MSG_MAP(CDialogOKCANCEL)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogOKCANCEL message handlers

BOOL CDialogOKCANCEL::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	::SetWindowPos(m_hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE |SWP_NOSIZE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
