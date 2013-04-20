// DialogTest.cpp : implementation file
//

#include "stdafx.h"
#include "LQDetect.h"
#include "DialogTest.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogTest dialog
//UINT	ReadValueTest(LPVOID pParam);		//读数据线程

CDialogTest::CDialogTest(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogTest::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogTest)
	m_sValue1 = _T("");
	m_sValue10 = _T("");
	m_sValue11 = _T("");
	m_sValue12 = _T("");
	m_sValue13 = _T("");
	m_sValue14 = _T("");
	m_sValue15 = _T("");
	m_sValue16 = _T("");
	m_sValue17 = _T("");
	m_sValue18 = _T("");
	m_sValue19 = _T("");
	m_sValue2 = _T("");
	m_sValue20 = _T("");
	m_sValue3 = _T("");
	m_sValue4 = _T("");
	m_sValue5 = _T("");
	m_sValue6 = _T("");
	m_sValue7 = _T("");
	m_sValue8 = _T("");
	m_sValue9 = _T("");
	m_sSecond = _T("");
	m_sInfo = _T("");
	m_sContainer = _T("");
	//}}AFX_DATA_INIT
	m_sInfo = _T("请将待测物品放在选定通道处按《开始测试》");
	m_bReadValueTest	= FALSE;
	m_bUpdateData		= FALSE;	//用于Paint;
	m_bPaint			= FALSE;
}

void CDialogTest::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogTest)
	DDX_Text(pDX, IDC_EDIT_VALUE1, m_sValue1);
	DDX_Text(pDX, IDC_EDIT_VALUE10, m_sValue10);
	DDX_Text(pDX, IDC_EDIT_VALUE11, m_sValue11);
	DDX_Text(pDX, IDC_EDIT_VALUE12, m_sValue12);
	DDX_Text(pDX, IDC_EDIT_VALUE13, m_sValue13);
	DDX_Text(pDX, IDC_EDIT_VALUE14, m_sValue14);
	DDX_Text(pDX, IDC_EDIT_VALUE15, m_sValue15);
	DDX_Text(pDX, IDC_EDIT_VALUE16, m_sValue16);
	DDX_Text(pDX, IDC_EDIT_VALUE17, m_sValue17);
	DDX_Text(pDX, IDC_EDIT_VALUE18, m_sValue18);
	DDX_Text(pDX, IDC_EDIT_VALUE19, m_sValue19);
	DDX_Text(pDX, IDC_EDIT_VALUE2, m_sValue2);
	DDX_Text(pDX, IDC_EDIT_VALUE20, m_sValue20);
	DDX_Text(pDX, IDC_EDIT_VALUE3, m_sValue3);
	DDX_Text(pDX, IDC_EDIT_VALUE4, m_sValue4);
	DDX_Text(pDX, IDC_EDIT_VALUE5, m_sValue5);
	DDX_Text(pDX, IDC_EDIT_VALUE6, m_sValue6);
	DDX_Text(pDX, IDC_EDIT_VALUE7, m_sValue7);
	DDX_Text(pDX, IDC_EDIT_VALUE8, m_sValue8);
	DDX_Text(pDX, IDC_EDIT_VALUE9, m_sValue9);
	DDX_CBString(pDX, IDC_COMBO_SECOND, m_sSecond);
	DDX_Text(pDX, IDC_EDIT_INFO, m_sInfo);
	DDX_CBString(pDX, IDC_COMBO_CONTAINER, m_sContainer);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDialogTest, CDialog)
	//{{AFX_MSG_MAP(CDialogTest)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_RADIO_CHANNEL1, OnRadioChannel1)
	ON_BN_CLICKED(IDC_RADIO_CHANNEL2, OnRadioChannel2)
	ON_BN_CLICKED(IDC_BUTTON_START, OnButtonStart)
	ON_CBN_SELCHANGE(IDC_COMBO_SECOND, OnSelchangeComboSecond)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogTest message handlers

BOOL CDialogTest::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	::SetWindowPos(m_hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE |SWP_NOSIZE);
	pApp=(CLQDetectApp *)AfxGetApp();

	OnRadioChannel1();

	SetControls();

	MessageUpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogTest::SetControls()
{
	CComboBox*		pCombo;
	pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_SECOND);
	pCombo->ResetContent();
	pCombo->AddString(_T("1"));
	pCombo->AddString(_T("2"));
	pCombo->AddString(_T("3"));
	pCombo->AddString(_T("4"));
	pCombo->AddString(_T("5"));
	pCombo->AddString(_T("6"));
	pCombo->AddString(_T("7"));
	pCombo->AddString(_T("8"));
	pCombo->AddString(_T("9"));
	pCombo->AddString(_T("10"));
	pCombo->AddString(_T("11"));
	pCombo->AddString(_T("12"));
	pCombo->AddString(_T("13"));
	pCombo->AddString(_T("14"));
	pCombo->AddString(_T("15"));
	pCombo->AddString(_T("16"));
	pCombo->AddString(_T("17"));
	pCombo->AddString(_T("18"));
	pCombo->AddString(_T("19"));
	pCombo->AddString(_T("20"));
	m_sSecond	= _T("5");
	m_nSecond	= 5;

	pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_CONTAINER);
	pCombo->ResetContent();
	for(int i=0; i<pApp->m_arrayContainer.GetSize(); i++)
	{
		pCombo->AddString(pApp->m_arrayContainer[i]->con_name);
	}

	m_sContainer	= pApp->m_arrayContainer[0]->con_name;
	
	MessageUpdateData(FALSE);
}

void CDialogTest::EnableControls()
{
//	GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO_CHANNEL1)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO_CHANNEL2)->EnableWindow(TRUE);
	GetDlgItem(IDC_COMBO_CONTAINER)->EnableWindow(TRUE);
	GetDlgItem(IDC_COMBO_SECOND)->EnableWindow(TRUE);
	GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
}

void CDialogTest::DisableControls()
{
//	GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_CHANNEL1)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_CHANNEL2)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_CONTAINER)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_SECOND)->EnableWindow(FALSE);
	GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
}

void CDialogTest::OnRadioChannel1() 
{
	m_bChannel1	= TRUE;
	m_bChannel2	= FALSE;
	((CButton *)GetDlgItem(IDC_RADIO_CHANNEL1))->SetCheck(m_bChannel1);//选上
	((CButton *)GetDlgItem(IDC_RADIO_CHANNEL2))->SetCheck(m_bChannel2);//选上
}

void CDialogTest::OnRadioChannel2() 
{
	m_bChannel1	= FALSE;
	m_bChannel2	= TRUE;
	((CButton *)GetDlgItem(IDC_RADIO_CHANNEL1))->SetCheck(m_bChannel1);//选上
	((CButton *)GetDlgItem(IDC_RADIO_CHANNEL2))->SetCheck(m_bChannel2);//选上
}

void CDialogTest::OnButtonStart() 
{
	if(!m_bReadValueTest)
	{
		//开启自动测量线程
		m_sValue1 = _T("");
		m_sValue10 = _T("");
		m_sValue11 = _T("");
		m_sValue12 = _T("");
		m_sValue13 = _T("");
		m_sValue14 = _T("");
		m_sValue15 = _T("");
		m_sValue16 = _T("");
		m_sValue17 = _T("");
		m_sValue18 = _T("");
		m_sValue19 = _T("");
		m_sValue2 = _T("");
		m_sValue20 = _T("");
		m_sValue3 = _T("");
		m_sValue4 = _T("");
		m_sValue5 = _T("");
		m_sValue6 = _T("");
		m_sValue7 = _T("");
		m_sValue8 = _T("");
		m_sValue9 = _T("");
		SetDlgItemText(IDC_BUTTON_START,_T("停止测试"));
		DisableControls();
		m_bReadValueTest	= TRUE;
		m_hReadThread = CreateThread(NULL,0,ReadTestThread,this,0, NULL);
	}
	else
	{
		m_bReadValueTest	= FALSE;
		SetDlgItemText(IDC_BUTTON_START,_T("开始测试"));
		WaitForSingleObject(m_hReadThread,500);
		CloseHandle(m_hReadThread);
		m_hReadThread=NULL;
		EnableControls();
	}
}

void CDialogTest::OnSelchangeComboSecond() 
{
	MessageUpdateData(TRUE);
	m_nSecond	= _ttoi(m_sSecond);
	MessageUpdateData(FALSE);
}

//读数据线程
DWORD WINAPI CDialogTest::ReadTestThread(LPVOID lpParameter)
{
	CDialogTest *pDialog;
	pDialog=(CDialogTest*)lpParameter;

	pDialog->m_sInfo	= _T("打开X射线机...");
	pDialog->MessageUpdateData(FALSE);
	if(!pDialog->m_pSerial->ControlXRadial(ControlCommandOpen))	//开X射线机
	{
		pDialog->m_sInfo	= _T("打开X射线机失败，请检查控制板或通讯线路！");
		pDialog->MessageUpdateData(FALSE);
		pDialog->pApp->m_dlgOK.m_sText = _T("打开X射线机失败，无法测试，请检查控制板或通讯线路！");
		pDialog->pApp->m_dlgOK.DoModal();
		pDialog->OnButtonStart();	//中断线程，恢复按钮状态
		return 0;
	}
	
	//取通道对应接口
	UINT nSelChannelInterface;
	if(pDialog->m_bChannel1)
	{
		nSelChannelInterface = pDialog->pApp->m_para_channelinterface1;
	}
	else
	{
		nSelChannelInterface = pDialog->pApp->m_para_channelinterface2;
	}
	//开始20次测试
	for(int i=0; i<20; i++)
	{
		if(!pDialog->m_bReadValueTest)
		{
			pDialog->m_sInfo	= _T("关闭X射线机...");
			pDialog->MessageUpdateData(FALSE);
			pDialog->m_pSerial->ControlXRadial(ControlCommandClose);
			pDialog->m_sInfo = _T("请将待测物品放在选定通道处按《开始测试》");
			pDialog->MessageUpdateData(FALSE);
			return 0;
		}
		
		pDialog->m_sInfo.Format(_T("正在读取第%d个测试数据..."), i+1);
		pDialog->MessageUpdateData(FALSE);
		
		if(!pDialog->m_pSerial->ControlSampling(nSelChannelInterface))
		{
			pDialog->m_sInfo	= _T("关闭X射线机...");
			pDialog->MessageUpdateData(FALSE);
			pDialog->m_pSerial->ControlXRadial(ControlCommandClose);
			pDialog->m_sInfo	= _T("测试失败，请检查控制板或通讯线路！");
			pDialog->pApp->m_dlgOK.m_sText = _T("测试中止，请检查控制板或通讯线路！");
			pDialog->pApp->m_dlgOK.DoModal();
			pDialog->OnButtonStart();	//中断线程，恢复按钮状态
			return 0;
		}

		//按阈值区间取值并显示
		switch(i)
		{
		case 0:
			pDialog->m_sValue1.Format(_T("%d"),pDialog->m_pSerial->m_nThresholdSamplingCount);
			break;
		case 1:
			pDialog->m_sValue2.Format(_T("%d"),pDialog->m_pSerial->m_nThresholdSamplingCount);
			break;
		case 2:
			pDialog->m_sValue3.Format(_T("%d"),pDialog->m_pSerial->m_nThresholdSamplingCount);
			break;
		case 3:
			pDialog->m_sValue4.Format(_T("%d"),pDialog->m_pSerial->m_nThresholdSamplingCount);
			break;
		case 4:
			pDialog->m_sValue5.Format(_T("%d"),pDialog->m_pSerial->m_nThresholdSamplingCount);
			break;
		case 5:
			pDialog->m_sValue6.Format(_T("%d"),pDialog->m_pSerial->m_nThresholdSamplingCount);
			break;
		case 6:
			pDialog->m_sValue7.Format(_T("%d"),pDialog->m_pSerial->m_nThresholdSamplingCount);
			break;
		case 7:
			pDialog->m_sValue8.Format(_T("%d"),pDialog->m_pSerial->m_nThresholdSamplingCount);
			break;
		case 8:
			pDialog->m_sValue9.Format(_T("%d"),pDialog->m_pSerial->m_nThresholdSamplingCount);
			break;
		case 9:
			pDialog->m_sValue10.Format(_T("%d"),pDialog->m_pSerial->m_nThresholdSamplingCount);
			break;
		case 10:
			pDialog->m_sValue11.Format(_T("%d"),pDialog->m_pSerial->m_nThresholdSamplingCount);
			break;
		case 11:
			pDialog->m_sValue12.Format(_T("%d"),pDialog->m_pSerial->m_nThresholdSamplingCount);
			break;
		case 12:
			pDialog->m_sValue13.Format(_T("%d"),pDialog->m_pSerial->m_nThresholdSamplingCount);
			break;
		case 13:
			pDialog->m_sValue14.Format(_T("%d"),pDialog->m_pSerial->m_nThresholdSamplingCount);
			break;
		case 14:
			pDialog->m_sValue15.Format(_T("%d"),pDialog->m_pSerial->m_nThresholdSamplingCount);
			break;
		case 15:
			pDialog->m_sValue16.Format(_T("%d"),pDialog->m_pSerial->m_nThresholdSamplingCount);
			break;
		case 16:
			pDialog->m_sValue17.Format(_T("%d"),pDialog->m_pSerial->m_nThresholdSamplingCount);
			break;
		case 17:
			pDialog->m_sValue18.Format(_T("%d"),pDialog->m_pSerial->m_nThresholdSamplingCount);
			break;
		case 18:
			pDialog->m_sValue19.Format(_T("%d"),pDialog->m_pSerial->m_nThresholdSamplingCount);
			break;
		case 19:
			pDialog->m_sValue20.Format(_T("%d"),pDialog->m_pSerial->m_nThresholdSamplingCount);
			break;
		}
		pDialog->MessageUpdateData(FALSE);
			
		//采样间隔延时
		if(i<19)
		{
			for(UINT n=0; n<pDialog->m_nSecond; n++)
			{
				if(!pDialog->m_bReadValueTest)
				{
					pDialog->m_sInfo	= _T("关闭X射线机...");
					pDialog->MessageUpdateData(FALSE);
					pDialog->m_pSerial->ControlXRadial(ControlCommandClose);
					pDialog->m_sInfo = _T("请将待测物品放在选定通道处按《开始测试》");
					pDialog->MessageUpdateData(FALSE);
					return 0;
				}
				pDialog->m_sInfo.Format(_T("采样间隔延时%d秒，请稍候..."), pDialog->m_nSecond-n);
				pDialog->MessageUpdateData(FALSE);
				Sleep(1000);
			}
		}
	}

	pDialog->m_sInfo	= _T("关闭X射线机...");
	pDialog->MessageUpdateData(FALSE);
	pDialog->m_pSerial->ControlXRadial(ControlCommandClose);
	pDialog->m_sInfo = _T("请将待测物品放在选定通道处按《开始测试》");
	pDialog->MessageUpdateData(FALSE);
	pDialog->OnButtonStart();	//中断线程，恢复按钮状态
	return 0;
}

void CDialogTest::OnOK()
{
	return;

	CDialog::OnOK();
}

void CDialogTest::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	if(m_bPaint)
	{
		UpdateData(m_bUpdateData);
		m_bPaint = FALSE;
	}
}

//发送UpdateData消息，避免线程UpdateData时出错
void CDialogTest::MessageUpdateData(BOOL bUpdateData)
{
	m_bPaint		= TRUE;
	m_bUpdateData	= bUpdateData;
	SendMessage(WM_PAINT,0,0);
}

void CDialogTest::OnCancel() 
{
	if(m_hReadThread !=NULL)   
	{   
		if(::WaitForSingleObject(m_hReadThread, 500)   ==   WAIT_TIMEOUT)   
		{   
			//如果超时：   
			TerminateThread(m_hReadThread, NULL);
		}
	}
	
	CDialog::OnCancel();
}
