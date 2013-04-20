// DialogOtherParameter.cpp : implementation file
//

#include "stdafx.h"
#include "LQDetect.h"
#include "DialogOtherParameter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogOtherParameter dialog


CDialogOtherParameter::CDialogOtherParameter(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogOtherParameter::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogOtherParameter)
	m_iThresholdBegin = 0;
	m_iThresholdEnd = 0;
	m_sChannelInterface1 = _T("");
	m_sChannelInterface2 = _T("");
	m_sSamplingTimes = _T("");
	//}}AFX_DATA_INIT
	m_bModifyAll			= TRUE;
	m_bThresholdModified	= FALSE;
}


void CDialogOtherParameter::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogOtherParameter)
	DDX_Control(pDX, IDC_SPIN_TEND, m_SpinTEnd);
	DDX_Control(pDX, IDC_SPIN_TBEGIN, m_SpinTBegin);
	DDX_Text(pDX, IDC_EDIT_THRESHOLDBEGIN, m_iThresholdBegin);
	DDV_MinMaxUInt(pDX, m_iThresholdBegin, 0, 1023);
	DDX_Text(pDX, IDC_EDIT_THRESHOLDEND, m_iThresholdEnd);
	DDV_MinMaxUInt(pDX, m_iThresholdEnd, 0, 1023);
	DDX_CBString(pDX, IDC_COMBO_CHANNELINTERFACE1, m_sChannelInterface1);
	DDX_CBString(pDX, IDC_COMBO_CHANNELINTERFACE2, m_sChannelInterface2);
	DDX_CBString(pDX, IDC_COMBO_SAMPLINGTIMES, m_sSamplingTimes);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogOtherParameter, CDialog)
	//{{AFX_MSG_MAP(CDialogOtherParameter)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogOtherParameter message handlers

BOOL CDialogOtherParameter::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	pApp=(CLQDetectApp *)AfxGetApp();

	SetControls();

	if(!m_bModifyAll)
	{
		GetDlgItem(IDC_COMBO_CHANNELINTERFACE1)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_CHANNELINTERFACE2)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPIN_TBEGIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPIN_TEND)->EnableWindow(FALSE);
//		((CEdit*)GetDlgItem(IDC_EDIT_THRESHOLDBEGIN))->SetReadOnly(TRUE);
//		((CEdit*)GetDlgItem(IDC_EDIT_THRESHOLDEND))->SetReadOnly(TRUE);
	}

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogOtherParameter::SetControls()
{
	//采样道阈
	m_iThresholdBegin	= pApp->m_para_thresholdbegin;
	m_iThresholdEnd		= pApp->m_para_thresholdend;
	//设置下拉列表
	CComboBox*		pCombo;
	pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_CHANNELINTERFACE1);
	pCombo->ResetContent();
	pCombo->AddString(_T("接口1"));
	pCombo->AddString(_T("接口3"));
	if(pApp->m_para_channelinterface1 == 1)
	{
		m_sChannelInterface1	= _T("接口1");
	}
	else
	{
		m_sChannelInterface1	= _T("接口3");
	}

	pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_CHANNELINTERFACE2);
	pCombo->ResetContent();
	pCombo->AddString(_T("接口2"));
	pCombo->AddString(_T("接口4"));
	if(pApp->m_para_channelinterface2 == 2)
	{
		m_sChannelInterface2	= _T("接口2");
	}
	else
	{
		m_sChannelInterface2	= _T("接口4");
	}

	pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_SAMPLINGTIMES);
	pCombo->ResetContent();
	pCombo->AddString(_T("1"));
	pCombo->AddString(_T("2"));
	pCombo->AddString(_T("3"));
	pCombo->AddString(_T("4"));
	pCombo->AddString(_T("5"));
	m_sSamplingTimes.Format(_T("%d"),pApp->m_para_samplingtimes);

	m_SpinTBegin.SetBuddy(GetDlgItem(IDC_EDIT_THRESHOLDBEGIN));// editbox的ID
	m_SpinTBegin.SetRange(1,1023);//设置范围
	m_SpinTEnd.SetBuddy(GetDlgItem(IDC_EDIT_THRESHOLDEND));// editbox的ID
	m_SpinTEnd.SetRange(2,1024);//设置范围
	UpdateData(FALSE);
}

void CDialogOtherParameter::OnButtonSave() 
{
	UpdateData(TRUE);
	if(m_iThresholdBegin>=m_iThresholdEnd)
	{
		pApp->m_dlgOK.m_sText = _T("道阈开始点不能大于结束点！");
		pApp->m_dlgOK.DoModal();
		return;
	}
	if(pApp->m_para_thresholdbegin != m_iThresholdBegin || pApp->m_para_thresholdend != m_iThresholdEnd)
	{
		m_bThresholdModified = TRUE;
	}
	CppSQLite3DB db;
	try
	{
		db.open(pApp->m_strDbPath);		//打开或新建一个数据库
		CString sSQL;
		db.execDML(L"begin transaction;");
		//道阈参数
		if(m_bThresholdModified)
		{
			sSQL.Format(_T("update parameter set value='%d' where parameterid='thresholdbegin'"),m_iThresholdBegin);
			db.execDML(sSQL);
			sSQL.Format(_T("update parameter set value='%d' where parameterid='thresholdend'"),m_iThresholdEnd);
			db.execDML(sSQL);
		}
		//通道对应接口参数
		if(m_sChannelInterface1==L"接口1")
		{
			sSQL = _T("update parameter set value='1' where parameterid='channelinterface1'");
			db.execDML(sSQL);
		}
		else
		{
			sSQL = _T("update parameter set value='3' where parameterid='channelinterface1'");
			db.execDML(sSQL);
		}
		if(m_sChannelInterface2==L"接口2")
		{
			sSQL = _T("update parameter set value='2' where parameterid='channelinterface2'");
			db.execDML(sSQL);
		}
		else
		{
			sSQL = _T("update parameter set value='4' where parameterid='channelinterface2'");
			db.execDML(sSQL);
		}
		//采样次数
		sSQL.Format(_T("update parameter set value='%s' where parameterid='samplingtimes'"),m_sSamplingTimes);
		db.execDML(sSQL);
		db.execDML(L"commit transaction;");
	}
	catch(CppSQLite3Exception ex)
	{
		pApp->m_dlgOK.m_sText = ex.errorMessage();
		pApp->m_dlgOK.DoModal();
		pApp->m_dlgOK.m_sText = _T("参数设置保存失败！");
		pApp->m_dlgOK.DoModal();
		return;
	}

	//道阈参数
	if(m_bThresholdModified)
	{
		pApp->m_para_thresholdbegin	= m_iThresholdBegin;
		pApp->m_para_thresholdend	= m_iThresholdEnd;
	}
	//通道对应接口参数
	if(m_sChannelInterface1==L"接口1")
		pApp->m_para_channelinterface1	= 1;
	else
		pApp->m_para_channelinterface1	= 3;
	if(m_sChannelInterface2==L"接口2")
		pApp->m_para_channelinterface2	= 2;
	else
		pApp->m_para_channelinterface2	= 4;

	//检测次数
	pApp->m_para_samplingtimes	= _ttoi(m_sSamplingTimes);

	pApp->m_dlgOK.m_sText = _T("参数设置已保存！");
	pApp->m_dlgOK.DoModal();
	
	CDialog::OnOK();
}
