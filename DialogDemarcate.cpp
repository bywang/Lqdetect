// DialogDemarcate.cpp : implementation file
//

#include "stdafx.h"
#include "LQDetect.h"
#include "DialogDemarcate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogDemarcate dialog
CDialogDemarcate::CDialogDemarcate(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogDemarcate::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogDemarcate)
	m_sNewValue1 = _T("");
	m_sOldValue1 = _T("");
	m_sRatio1 = _T("");
	m_sNewValue2 = _T("");
	m_sOldValue2 = _T("");
	m_sRatio2 = _T("");
	m_sInfo = _T("");
	m_sContainer = _T("");
	m_sLiquid = _T("");
	//}}AFX_DATA_INIT
	m_nOldValue1	= 0;		//原标准值
	m_nOldValue2	= 0;		//原标准值
	m_fRatio1		= 1.0f;		//比例系数
	m_fRatio2		= 1.0f;		//比例系数
}

void CDialogDemarcate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogDemarcate)
	DDX_Text(pDX, IDC_EDIT_NEWVALUE1, m_sNewValue1);
	DDX_Text(pDX, IDC_EDIT_OLDVALUE1, m_sOldValue1);
	DDX_Text(pDX, IDC_EDIT_RATIO1, m_sRatio1);
	DDX_Text(pDX, IDC_EDIT_NEWVALUE2, m_sNewValue2);
	DDX_Text(pDX, IDC_EDIT_OLDVALUE2, m_sOldValue2);
	DDX_Text(pDX, IDC_EDIT_RATIO2, m_sRatio2);
	DDX_Text(pDX, IDC_EDIT_INFO, m_sInfo);
	DDX_CBString(pDX, IDC_COMBO_CONTAINER, m_sContainer);
	DDX_CBString(pDX, IDC_COMBO_LIQUID, m_sLiquid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogDemarcate, CDialog)
	//{{AFX_MSG_MAP(CDialogDemarcate)
	ON_BN_CLICKED(IDC_RADIO_CHANNEL1, OnRadioChannel1)
	ON_BN_CLICKED(IDC_RADIO_CHANNEL2, OnRadioChannel2)
	ON_BN_CLICKED(IDC_BUTTON_START, OnButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_CBN_SELCHANGE(IDC_COMBO_CONTAINER, OnSelchangeComboContainer)
	ON_CBN_SELCHANGE(IDC_COMBO_LIQUID, OnSelchangeComboLiquid)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogDemarcate message handlers

BOOL CDialogDemarcate::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	::SetWindowPos(m_hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE |SWP_NOSIZE);
	pApp=(CLQDetectApp *)AfxGetApp();

	SetControls();

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(FALSE);

	if(GetOldValue())
	{
		m_sInfo = _T("请将待测物品放在选定通道处按《开始标定》");
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
	}

	OnRadioChannel1();		//默认通道一
	
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogDemarcate::SetControls()
{
	//设置下拉列表
	CComboBox*		pCombo;
	pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_CONTAINER);
	pCombo->ResetContent();
	for(int i=0; i<pApp->m_arrayContainer.GetSize(); i++)
	{
		pCombo->AddString(pApp->m_arrayContainer[i]->con_name);
	}
	m_sContainer	= pApp->m_arrayContainer[0]->con_name;
	
	pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_LIQUID);
	pCombo->ResetContent();
	for(i=0; i<pApp->m_arrayLiquid.GetSize(); i++)
	{
		if(pApp->m_arrayLiquid[i]->liq_name==L"标准块" || pApp->m_arrayLiquid[i]->liq_name==L"水")
		{
			pCombo->AddString(pApp->m_arrayLiquid[i]->liq_name);
		}
	}
	m_sLiquid	= L"标准块";
	UpdateData(FALSE);
}

void CDialogDemarcate::EnableControls()
{
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO_CHANNEL1)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO_CHANNEL2)->EnableWindow(TRUE);
	GetDlgItem(IDC_COMBO_CONTAINER)->EnableWindow(TRUE);
	GetDlgItem(IDC_COMBO_LIQUID)->EnableWindow(TRUE);
	GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
}

void CDialogDemarcate::DisableControls()
{
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_CHANNEL1)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_CHANNEL2)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_CONTAINER)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_LIQUID)->EnableWindow(FALSE);
	GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
}

void CDialogDemarcate::OnRadioChannel1() 
{
	m_bChannel1	= TRUE;
	m_bChannel2	= FALSE;
	((CButton *)GetDlgItem(IDC_RADIO_CHANNEL1))->SetCheck(m_bChannel1);//选上
	((CButton *)GetDlgItem(IDC_RADIO_CHANNEL2))->SetCheck(m_bChannel2);
	if(GetOldValue())
	{
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(FALSE);
	}
}

void CDialogDemarcate::OnRadioChannel2() 
{
	m_bChannel1	= FALSE;
	m_bChannel2	= TRUE;
	((CButton *)GetDlgItem(IDC_RADIO_CHANNEL1))->SetCheck(m_bChannel1);
	((CButton *)GetDlgItem(IDC_RADIO_CHANNEL2))->SetCheck(m_bChannel2);//选上
	if(GetOldValue())
	{
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(FALSE);
	}
}

BOOL CDialogDemarcate::GetOldValue()		//取样本出厂标定值
{
	UpdateData(TRUE);
	
	m_nOldValue1	= 0;
	m_nOldValue2	= 0;
	m_sOldValue1	= L"";
	m_sOldValue2	= L"";
	UpdateData(FALSE);
	
	BOOL	bFind1(FALSE),bFind2(FALSE);
	UINT nValue;
	for(int i=0; i<pApp->m_arraySwatch.GetSize(); i++)
	{
		if(pApp->m_arraySwatch[i]->con_name==m_sContainer && pApp->m_arraySwatch[i]->liq_name==m_sLiquid)
		{
			if(pApp->m_para_channelinterface1 == UINT(_ttoi(pApp->m_arraySwatch[i]->channel)))
			{
				nValue	= UINT(_ttoi(pApp->m_arraySwatch[i]->countaverage));
				if(nValue>0)
				{
					m_nOldValue1	= nValue;
				}
				bFind1	= TRUE;
			}
			else if(pApp->m_para_channelinterface2 == UINT(_ttoi(pApp->m_arraySwatch[i]->channel)))
			{
				nValue	= UINT(_ttoi(pApp->m_arraySwatch[i]->countaverage));
				if(nValue>0)
				{
					m_nOldValue2	= nValue;
				}
				bFind2	= TRUE;
			}
		}
		if(bFind1 && bFind2)
		{
			break;
		}
	}
	m_sOldValue1.Format(_T("%d"), m_nOldValue1);
	m_sOldValue2.Format(_T("%d"), m_nOldValue2);
	if(m_nOldValue1==0 && m_nOldValue2==0)
	{
		m_sInfo = _T("所选样本第一通道和第二通道出厂标准数据无效，无法标定！");
		UpdateData(FALSE);
		return FALSE;
	}
//	else if(m_nOldValue1==0)
//	{
//		m_sInfo = _T("所选样本第一通道出厂标准数据无效，无法标定！");
//		UpdateData(FALSE);
//		return FALSE;
//	}
//	else if(m_nOldValue2==0)
//	{
//		m_sInfo =_T("所选样本第二通道出厂标准数据无效，无法标定！");
//		UpdateData(FALSE);
//		return FALSE;
//	}
	
	//显示原比值系数
	m_sRatio1.Format(_T("%0.5f"), pApp->m_para_standardratio1);
	m_sRatio2.Format(_T("%0.5f"), pApp->m_para_standardratio2);
	
	UpdateData(FALSE);
	return TRUE;
}

void CDialogDemarcate::OnButtonStart() 
{
	if(m_bChannel1 && m_nOldValue1==0)
	{
		m_sInfo = _T("所选样本第一通道出厂标准数据无效，无法标定！");
		UpdateData(FALSE);
		return;
	}
	else if(m_bChannel2 && m_nOldValue2==0)
	{
		m_sInfo = _T("所选样本第二通道出厂标准数据无效，无法标定！");
		UpdateData(FALSE);
		return;
	}

	DisableControls();
	BeginWaitCursor();

	Sleep(500);

	//取通道对应接口
	UINT nSelChannelInterface;
	if(m_bChannel1)
	{
		nSelChannelInterface = pApp->m_para_channelinterface1;
	}
	else
	{
		nSelChannelInterface = pApp->m_para_channelinterface2;
	}

	UINT	nSamplingCount(0);	//采样计数值
	UINT	nTimes(2);	//标定取值次数
	for(UINT i=0; i<nTimes; i++)
	{
		m_sInfo		= _T("打开X射线机...");
		UpdateData(FALSE);

		if(!m_pSerial->ControlXRadial(ControlCommandOpen))
		{
			m_sInfo	= _T("打开X射线机失败，请检查控制板或通讯线路！");
			UpdateData(FALSE);
			EnableControls();
 			EndWaitCursor();
			return;
		}
		m_sInfo.Format(_T("正在读取第%d个数据..."), i+1);
		UpdateData(FALSE);
		for(UINT j=0; j<pApp->m_para_samplingtimes+1; j++)
		{
			m_sInfo.Format(_T("第%d个数据取样中（第%d次采样），请稍候..."), i+1, j+1);
			UpdateData(FALSE);
			Sleep(1000);
			if(!m_pSerial->ControlSampling(nSelChannelInterface))
			{
				m_sInfo = _T("测量失败，请检查控制板或通讯线路！");
				UpdateData(FALSE);
				m_pSerial->ControlXRadial(ControlCommandClose);
				pApp->m_dlgOK.m_sText = _T("标定中止，请检查控制板或通讯线路！");
				pApp->m_dlgOK.DoModal();
				EnableControls();
 				EndWaitCursor();
				return;
			}
			if(j>0)
				nSamplingCount += m_pSerial->m_nThresholdSamplingCount;
		}
		m_sInfo	= _T("关闭X射线机...");
		UpdateData(FALSE);
		if(!m_pSerial->ControlXRadial(ControlCommandClose))
		{
			m_sInfo	= _T("关闭X射线机失败，请检查控制板或通讯线路！");
			pApp->m_dlgOK.m_sText = _T("标定中止，请检查控制板或通讯线路！");
			pApp->m_dlgOK.DoModal();
			EnableControls();
 			EndWaitCursor();
			return;
		}

		//采样间隔延时
		if(i<nTimes-1)
		{
			for(UINT n=0; n<5; n++)//5S
			{
				m_sInfo.Format(_T("采样间隔延时%d秒，请稍候..."), 5-n);
				UpdateData(FALSE);
				Sleep(1000);
			}
		}
	}
 	EndWaitCursor();
	
	nSamplingCount = UINT(nSamplingCount/pApp->m_para_samplingtimes/nTimes);
	if(m_bChannel1)
	{
		m_sNewValue1.Format(_T("%d"), nSamplingCount);
		m_fRatio1	= float(nSamplingCount)/m_nOldValue1;	//算新比例系数
		m_sRatio1.Format(_T("%0.5f"), m_fRatio1);
	}
	else
	{
		m_sNewValue2.Format(_T("%d"), nSamplingCount);
		m_fRatio2	= float(nSamplingCount)/m_nOldValue2;	//算新比例系数
		m_sRatio2.Format(_T("%0.5f"), m_fRatio2);
	}
	m_sInfo = _T("请将待测物品放在选定通道处按《开始标定》");
	UpdateData(FALSE);
	if(m_fRatio1>0.0f && m_fRatio2>0.0f)
	{
		GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(TRUE);
	}
	EnableControls();
}

void CDialogDemarcate::OnButtonSave()
{
	UpdateData(TRUE);
	CppSQLite3DB db;
	if(m_fRatio1<=0.0f || m_fRatio2<=0.0f)
	{
		pApp->m_dlgOK.m_sText = L"比值系数无效，请重新标定！";
		pApp->m_dlgOK.DoModal();
		return;
	}
	try
	{
		db.open(pApp->m_strDbPath);		//打开或新建一个数据库
		CString sSQL;
		db.execDML(L"begin transaction;");
		sSQL.Format(_T("update parameter set value='%0.5f' where parameterid='standardratio1'"),m_fRatio1);
		db.execDML(sSQL);
		sSQL.Format(_T("update parameter set value='%0.5f' where parameterid='standardratio2'"),m_fRatio2);
		db.execDML(sSQL);
		db.execDML(L"commit transaction;");
	}
	catch(CppSQLite3Exception ex)
	{
		pApp->m_dlgOK.m_sText = ex.errorMessage();
		pApp->m_dlgOK.DoModal();
		pApp->m_dlgOK.m_sText = _T("比值系数保存失败！");
		pApp->m_dlgOK.DoModal();
		return;
	}

	pApp->m_para_standardratio1	= m_fRatio1;
	pApp->m_para_standardratio2	= m_fRatio2;
	GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(FALSE);

	pApp->m_dlgOK.m_sText = _T("比值系数已保存！");
	pApp->m_dlgOK.DoModal();
}

void CDialogDemarcate::OnOK()
{
	return;

	CDialog::OnOK();
}

void CDialogDemarcate::OnCancel() 
{
	CDialog::OnCancel();
}

void CDialogDemarcate::OnSelchangeComboContainer() 
{
	if(GetOldValue())
	{
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(FALSE);
	}
}

void CDialogDemarcate::OnSelchangeComboLiquid() 
{
	if(GetOldValue())
	{
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(FALSE);
	}
}
