// DialogPassword.cpp : implementation file
//

#include "stdafx.h"
#include "LQDetect.h"
#include "DialogPassword.h"
#include "sipapi.h"				//软键盘控制

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogPassword dialog


CDialogPassword::CDialogPassword(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogPassword::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogPassword)
	m_password = _T("");
	//}}AFX_DATA_INIT
	m_bKeyboardShowed	= FALSE;
}


void CDialogPassword::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogPassword)
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_password);
	DDV_MaxChars(pDX, m_password, 20);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogPassword, CDialog)
	//{{AFX_MSG_MAP(CDialogPassword)
	ON_EN_SETFOCUS(IDC_EDIT_PASSWORD, OnSetfocusEditPassword)
	ON_EN_KILLFOCUS(IDC_EDIT_PASSWORD, OnKillfocusEditPassword)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogPassword message handlers

void CDialogPassword::OnOK() 
{
	UpdateData(TRUE);
	m_password.TrimLeft();
	m_password.TrimRight();
	if(m_password != "cbs2013")
	{
		pApp->m_dlgOK.m_sText = _T("参数管理口令错误，无法操作！");
		pApp->m_dlgOK.DoModal();
		return;
	}
	CDialog::OnOK();
}

void CDialogPassword::OnSetfocusEditPassword() 
{
	if(!m_bKeyboardShowed)
	{
		SipShowIM(SIPF_ON);  //面板显示
		m_bKeyboardShowed	= TRUE;
	}
}

void CDialogPassword::OnKillfocusEditPassword() 
{
	if(m_bKeyboardShowed)
	{
		SipShowIM(SIPF_OFF);//面板隐藏
		m_bKeyboardShowed	= FALSE;
	}
}

BOOL CDialogPassword::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	::SetWindowPos(m_hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE |SWP_NOSIZE);
	pApp=(CLQDetectApp *)AfxGetApp();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
