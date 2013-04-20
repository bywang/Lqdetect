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
	//��������
	m_iThresholdBegin	= pApp->m_para_thresholdbegin;
	m_iThresholdEnd		= pApp->m_para_thresholdend;
	//���������б�
	CComboBox*		pCombo;
	pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_CHANNELINTERFACE1);
	pCombo->ResetContent();
	pCombo->AddString(_T("�ӿ�1"));
	pCombo->AddString(_T("�ӿ�3"));
	if(pApp->m_para_channelinterface1 == 1)
	{
		m_sChannelInterface1	= _T("�ӿ�1");
	}
	else
	{
		m_sChannelInterface1	= _T("�ӿ�3");
	}

	pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_CHANNELINTERFACE2);
	pCombo->ResetContent();
	pCombo->AddString(_T("�ӿ�2"));
	pCombo->AddString(_T("�ӿ�4"));
	if(pApp->m_para_channelinterface2 == 2)
	{
		m_sChannelInterface2	= _T("�ӿ�2");
	}
	else
	{
		m_sChannelInterface2	= _T("�ӿ�4");
	}

	pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_SAMPLINGTIMES);
	pCombo->ResetContent();
	pCombo->AddString(_T("1"));
	pCombo->AddString(_T("2"));
	pCombo->AddString(_T("3"));
	pCombo->AddString(_T("4"));
	pCombo->AddString(_T("5"));
	m_sSamplingTimes.Format(_T("%d"),pApp->m_para_samplingtimes);

	m_SpinTBegin.SetBuddy(GetDlgItem(IDC_EDIT_THRESHOLDBEGIN));// editbox��ID
	m_SpinTBegin.SetRange(1,1023);//���÷�Χ
	m_SpinTEnd.SetBuddy(GetDlgItem(IDC_EDIT_THRESHOLDEND));// editbox��ID
	m_SpinTEnd.SetRange(2,1024);//���÷�Χ
	UpdateData(FALSE);
}

void CDialogOtherParameter::OnButtonSave() 
{
	UpdateData(TRUE);
	if(m_iThresholdBegin>=m_iThresholdEnd)
	{
		pApp->m_dlgOK.m_sText = _T("���п�ʼ�㲻�ܴ��ڽ����㣡");
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
		db.open(pApp->m_strDbPath);		//�򿪻��½�һ�����ݿ�
		CString sSQL;
		db.execDML(L"begin transaction;");
		//���в���
		if(m_bThresholdModified)
		{
			sSQL.Format(_T("update parameter set value='%d' where parameterid='thresholdbegin'"),m_iThresholdBegin);
			db.execDML(sSQL);
			sSQL.Format(_T("update parameter set value='%d' where parameterid='thresholdend'"),m_iThresholdEnd);
			db.execDML(sSQL);
		}
		//ͨ����Ӧ�ӿڲ���
		if(m_sChannelInterface1==L"�ӿ�1")
		{
			sSQL = _T("update parameter set value='1' where parameterid='channelinterface1'");
			db.execDML(sSQL);
		}
		else
		{
			sSQL = _T("update parameter set value='3' where parameterid='channelinterface1'");
			db.execDML(sSQL);
		}
		if(m_sChannelInterface2==L"�ӿ�2")
		{
			sSQL = _T("update parameter set value='2' where parameterid='channelinterface2'");
			db.execDML(sSQL);
		}
		else
		{
			sSQL = _T("update parameter set value='4' where parameterid='channelinterface2'");
			db.execDML(sSQL);
		}
		//��������
		sSQL.Format(_T("update parameter set value='%s' where parameterid='samplingtimes'"),m_sSamplingTimes);
		db.execDML(sSQL);
		db.execDML(L"commit transaction;");
	}
	catch(CppSQLite3Exception ex)
	{
		pApp->m_dlgOK.m_sText = ex.errorMessage();
		pApp->m_dlgOK.DoModal();
		pApp->m_dlgOK.m_sText = _T("�������ñ���ʧ�ܣ�");
		pApp->m_dlgOK.DoModal();
		return;
	}

	//���в���
	if(m_bThresholdModified)
	{
		pApp->m_para_thresholdbegin	= m_iThresholdBegin;
		pApp->m_para_thresholdend	= m_iThresholdEnd;
	}
	//ͨ����Ӧ�ӿڲ���
	if(m_sChannelInterface1==L"�ӿ�1")
		pApp->m_para_channelinterface1	= 1;
	else
		pApp->m_para_channelinterface1	= 3;
	if(m_sChannelInterface2==L"�ӿ�2")
		pApp->m_para_channelinterface2	= 2;
	else
		pApp->m_para_channelinterface2	= 4;

	//������
	pApp->m_para_samplingtimes	= _ttoi(m_sSamplingTimes);

	pApp->m_dlgOK.m_sText = _T("���������ѱ��棡");
	pApp->m_dlgOK.DoModal();
	
	CDialog::OnOK();
}
