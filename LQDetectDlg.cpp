// LQDetectDlg.cpp : implementation file
//

#include "stdafx.h"

#include "LQDetect.h"
#include "LQDetectDlg.h"
#include "DialogDetect.h"
#include "DialogTest.h"
//#include "DialogPassword.h"
//#include "DialogParameter.h"
#include "DialogDemarcate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLQDetectDlg dialog

CLQDetectDlg::CLQDetectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLQDetectDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLQDetectDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLQDetectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLQDetectDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLQDetectDlg, CDialog)
	//{{AFX_MSG_MAP(CLQDetectDlg)
	ON_BN_CLICKED(IDC_BUTTON_PARAMETER, OnButtonParameter)
	ON_BN_CLICKED(IDC_BUTTON_DETECT, OnButtonDetect)
	ON_BN_CLICKED(IDC_BUTTON_TEST, OnButtonTest)
	ON_BN_CLICKED(IDC_BUTTON_DEMARCATE, OnButtonDemarcate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLQDetectDlg message handlers

BOOL CLQDetectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

//	MessageBox(L"",NULL,MB_OK);

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	//进入默认打开检测窗口
//	OnButtonDetect();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CLQDetectDlg::OnOK()
{
	CDialog::OnOK();
}

void CLQDetectDlg::OnButtonParameter() 
{
/*
	CDialogPassword dlgPass;
	if(dlgPass.DoModal()==IDCANCEL)
	{
		return;
	}

	CDialogParameter dlgPara;
	dlgPara.DoModal();
*/
}

void CLQDetectDlg::OnButtonDetect() 
{
	CDialogDetect dlg;
	dlg.DoModal();
}

void CLQDetectDlg::OnButtonTest() 
{
	CDialogTest dlg;
	dlg.DoModal();
}

void CLQDetectDlg::OnButtonDemarcate() 
{
	CDialogDemarcate dlg;
	dlg.DoModal();
}
