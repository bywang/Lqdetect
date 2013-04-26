// DialogGeneral.cpp : implementation file
//

#include "stdafx.h"
#include "LQDetect.h"
#include "DialogGeneral.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogGeneral dialog


CDialogGeneral::CDialogGeneral(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogGeneral::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogGeneral)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDialogGeneral::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogGeneral)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogGeneral, CDialog)
	//{{AFX_MSG_MAP(CDialogGeneral)
	ON_BN_CLICKED(IDC_BUTTON_ALUM, OnButtonAlum)
	ON_BN_CLICKED(IDC_BUTTON_CAL, OnButtonCal)
	ON_BN_CLICKED(IDC_BUTTON_MANAGE, OnButtonManage)
	ON_BN_CLICKED(IDC_BUTTON_OTHERS, OnButtonOthers)
	ON_BN_CLICKED(IDC_BUTTON_PLASTIC, OnButtonPlastic)
	ON_BN_CLICKED(IDC_BUTTON_START, OnButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_XRAY, OnButtonXray)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_STATIC_TEMP, OnStaticTemp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogGeneral message handlers

void CDialogGeneral::OnButtonAlum() 
{
	// TODO: Add your control notification handler code here
	
}

void CDialogGeneral::OnButtonCal() 
{
	// TODO: Add your control notification handler code here
	CDialogDemarcate dlg;
	dlg.DoModal();
}

void CDialogGeneral::OnButtonManage() 
{
	// TODO: Add your control notification handler code here
	CDialogPassword dlgPass;
	if(dlgPass.DoModal()==IDOK)
	{
		CDialogDetect dlgDetect;
		dlgDetect.DoModal();
	}

}

void CDialogGeneral::OnButtonOthers() 
{
	// TODO: Add your control notification handler code here
	
}

void CDialogGeneral::OnButtonPlastic() 
{
	// TODO: Add your control notification handler code here
	
}

void CDialogGeneral::OnButtonStart() 
{
	// TODO: Add your control notification handler code here
	
}

void CDialogGeneral::OnButtonXray() 
{
	// TODO: Add your control notification handler code here
	
}

BOOL CDialogGeneral::OnInitDialog() 
{
	CDialog::OnInitDialog();

	pApp=(CLQDetectApp *)AfxGetApp();
    m_pSerial = pApp->m_SerialP;
    m_pSerialX = pApp->m_SerialXP;
	
	if(!pApp->m_serialIsOk)
	{
		GetDlgItem(IDC_BUTTON_CAL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_MANAGE)->EnableWindow(FALSE);
	} 
	if(!pApp->m_serialXIsOk)
	{
		GetDlgItem(IDC_BUTTON_XRAY)->EnableWindow(FALSE);
	}
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogGeneral::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CDialogGeneral::OnStaticTemp() 
{
	// TODO: Add your control notification handler code here
	
}
