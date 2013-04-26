// DialogDetect.cpp : implementation file
//

#include "stdafx.h"
#include "LQDetect.h"
#include "DialogDetect.h"
#include "DialogDemarcate.h"
#include "DialogTest.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogDetect dialog

CDialogDetect::CDialogDetect(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogDetect::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogDetect)
	m_sIsSafe = _T("");
	m_sValue = _T("0");
	m_sInfo = _T("请将待测物品放在选定通道处按《测量》");
	m_nTimes = 0;
	m_X_A = _T("");
	m_X_T = _T("");
	m_X_V = _T("");
	m_sSetXA = _T("0.70");
	m_sSetXV = _T("60");
	//}}AFX_DATA_INIT
	m_bWarmUp		= FALSE;	//开始预热
	m_bStartDetect		= FALSE;	//开始测量
	m_bSafeLightOpen	= FALSE;	//安全灯开启
	m_bAlarmLightOpen	= FALSE;	//报警灯开户
}

void CDialogDetect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogDetect)
	DDX_Control(pDX, IDC_SPIN_SET_X_V, m_SpinSetXV);
	DDX_Control(pDX, IDC_SPIN_SET_X_A, m_SpinSetXA);
	DDX_CBString(pDX, IDC_COMBO_CONTAINER, m_sContainer);
	DDX_Text(pDX, IDC_EDIT_ISSAFE, m_sIsSafe);
	DDX_Text(pDX, IDC_EDIT_VALUE, m_sValue);
	DDX_Text(pDX, IDC_EDIT_INFO, m_sInfo);
	DDX_Text(pDX, IDC_EDIT_TIMES, m_nTimes);
	DDX_Text(pDX, IDC_EDIT_X_A, m_X_A);
	DDX_Text(pDX, IDC_EDIT_X_T, m_X_T);
	DDX_Text(pDX, IDC_EDIT_X_V, m_X_V);
	DDX_Text(pDX, IDC_EDIT_SET_X_A, m_sSetXA);
	DDX_Text(pDX, IDC_EDIT_SET_X_V, m_sSetXV);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogDetect, CDialog)
	//{{AFX_MSG_MAP(CDialogDetect)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_RADIO_CHANNEL1, OnRadioChannel1)
	ON_BN_CLICKED(IDC_RADIO_CHANNEL2, OnRadioChannel2)
	ON_BN_CLICKED(IDC_RADIO_SAMPLINGMETHOD1, OnRadioMethodRatio)
	ON_BN_CLICKED(IDC_RADIO_SAMPLINGMETHOD2, OnRadioMethodCount)
	ON_BN_CLICKED(IDC_BUTTON_START, OnButtonStart)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_READ_X, OnButtonReadX)
	ON_BN_CLICKED(IDC_BUTTON_SET_X, OnButtonSetX)
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_WARM_UP, OnButtonWarmUp)
	ON_BN_CLICKED(IDC_BUTTON_DEMARCATE, OnButtonDemarcate)
	ON_BN_CLICKED(IDC_BUTTON_TEST, OnButtonTest)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_TIMES, OnButtonClearTimes)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogDetect message handlers

BOOL CDialogDetect::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	::SetWindowPos(m_hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE |SWP_NOSIZE);

	pApp=(CLQDetectApp *)AfxGetApp();
    m_Serial = pApp->m_SerialP;
    m_SerialX = pApp->m_SerialP;

	if(!pApp->m_serialIsOk)
	{
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_DEMARCATE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_TEST)->EnableWindow(FALSE);
	} 

	if(!pApp->m_serialXIsOk)
	{
		GetDlgItem(IDC_BUTTON_READ_X)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SET_X)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_WARM_UP)->EnableWindow(FALSE);
	}

	OnRadioChannel1();		//默认通道一
	OnRadioMethodRatio();	//默认比值法

	SetControls();

	m_nTimes = pApp->m_para_totaldetecttimes;
	UpdateData(FALSE);

//	CString s;
//	s.Format("初始次数：%d", m_nTimes);
//	AfxMessageBox(s);

	if(pApp->m_serialXIsOk)
	{
		OnButtonReadX();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogDetect::SetControls()
{
	CComboBox*		pCombo;
	pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_CONTAINER);
	pCombo->ResetContent();
	for(int i=0; i<pApp->m_arrayContainer.GetSize(); i++)
	{
		pCombo->AddString(pApp->m_arrayContainer[i]->con_name);
	}

	m_sContainer	= pApp->m_arrayContainer[0]->con_name;

	m_nSetXV = 60;
	m_SpinSetXV.SetRange(60,80);
	m_SpinSetXV.SetBase(1);
	m_SpinSetXV.SetPos(m_nSetXV);

	m_nSetXA = 70;
	m_SpinSetXA.SetRange(70,90);
	m_SpinSetXA.SetBase(1);
	m_SpinSetXA.SetPos(m_nSetXA);

	UpdateData(FALSE);
}

void CDialogDetect::OnRadioChannel1() 
{
	m_bChannel1	= TRUE;
	m_bChannel2	= FALSE;
	((CButton *)GetDlgItem(IDC_RADIO_CHANNEL1))->SetCheck(m_bChannel1);//选上
	((CButton *)GetDlgItem(IDC_RADIO_CHANNEL2))->SetCheck(m_bChannel2);
}

void CDialogDetect::OnRadioChannel2() 
{
	m_bChannel1	= FALSE;
	m_bChannel2	= TRUE;
	((CButton *)GetDlgItem(IDC_RADIO_CHANNEL1))->SetCheck(m_bChannel1);
	((CButton *)GetDlgItem(IDC_RADIO_CHANNEL2))->SetCheck(m_bChannel2);//选上
}

void CDialogDetect::OnRadioMethodRatio() 
{
	m_bMethodRatio	= TRUE;
	m_bMethodCount	= FALSE;
	((CButton *)GetDlgItem(IDC_RADIO_SAMPLINGMETHOD1))->SetCheck(m_bMethodRatio);//选上
	((CButton *)GetDlgItem(IDC_RADIO_SAMPLINGMETHOD2))->SetCheck(m_bMethodCount);
}

void CDialogDetect::OnRadioMethodCount() 
{
	m_bMethodRatio	= FALSE;
	m_bMethodCount	= TRUE;
	((CButton *)GetDlgItem(IDC_RADIO_SAMPLINGMETHOD1))->SetCheck(m_bMethodRatio);
	((CButton *)GetDlgItem(IDC_RADIO_SAMPLINGMETHOD2))->SetCheck(m_bMethodCount);//选上
}

void CDialogDetect::OnDestroy() 
{
	//关灯
	if(m_bSafeLightOpen)
	{
		m_Serial->ControlSafeLight(ControlCommandClose);
	}
	if(m_bAlarmLightOpen)
	{
		m_Serial->ControlAlarmLight(ControlCommandClose);
	}
	m_Serial.Deactivate();
	m_Serial.ClosePort();
	m_SerialX.Deactivate();
	m_SerialX.ClosePort();

	CDialog::OnDestroy();
}

void CDialogDetect::OnOK()
{
	if(m_bStartDetect || m_bWarmUp)
		return;

	//关灯
	if(m_bSafeLightOpen)
	{
		m_Serial->ControlSafeLight(ControlCommandClose);
	}
	if(m_bAlarmLightOpen)
	{
		m_Serial->ControlAlarmLight(ControlCommandClose);
	}
	m_Serial.Deactivate();
	m_Serial.ClosePort();
	m_SerialX.Deactivate();
	m_SerialX.ClosePort();
	CDialog::OnOK();
}

void CDialogDetect::EnableControls()
{
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO_CHANNEL1)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO_CHANNEL2)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO_SAMPLINGMETHOD1)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO_SAMPLINGMETHOD2)->EnableWindow(TRUE);
	GetDlgItem(IDC_COMBO_CONTAINER)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_READ_X)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_SET_X)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_WARM_UP)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_DEMARCATE)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_TEST)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_CLEAR_TIMES)->EnableWindow(TRUE);
}

void CDialogDetect::DisableControls()
{
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_CHANNEL1)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_CHANNEL2)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_SAMPLINGMETHOD1)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_SAMPLINGMETHOD2)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_CONTAINER)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_READ_X)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_SET_X)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_WARM_UP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_DEMARCATE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_TEST)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CLEAR_TIMES)->EnableWindow(FALSE);
}

void CDialogDetect::OnTimer(UINT nIDEvent) 
{
	//关灯
	if(m_bSafeLightOpen)
	{
		m_Serial->ControlSafeLight(ControlCommandClose);
	}
	if(m_bAlarmLightOpen)
	{
		m_Serial->ControlAlarmLight(ControlCommandClose);
	}
	KillTimer(1);

	CDialog::OnTimer(nIDEvent);
}

BOOL CDialogDetect::SampleIsSafe(UINT nSamplingCount, UINT nSelChannelInterface)	//样品是否安全
{
//	BOOL	bSafe(FALSE);
	CString s;
//	s.Format(L"%d",pApp->m_arraySwatch.GetSize());
//	AfxMessageBox(s);
	for(int i=0; i<pApp->m_arraySwatch.GetSize(); i++)
	{
		if(UINT(_ttoi(pApp->m_arraySwatch[i]->channel)) == nSelChannelInterface && pApp->m_arraySwatch[i]->con_name == m_sContainer)
		{
//			s.Format(L"%s-%d-%d",pApp->m_arraySwatch[i]->liq_name, UINT(_ttoi(pApp->m_arraySwatch[i]->countdown)), UINT(_ttoi(pApp->m_arraySwatch[i]->countup)));
//			AfxMessageBox(s);
			if(pApp->m_arraySwatch[i]->liq_name != L"标准块")	//标准块不参与判断
			{
				if(nSamplingCount>=UINT(_ttoi(pApp->m_arraySwatch[i]->countdown)) && nSamplingCount<=UINT(_ttoi(pApp->m_arraySwatch[i]->countup)))
				{
//					AfxMessageBox(pApp->m_arraySwatch[i]->liq_name);
					if(pApp->m_arraySwatch[i]->issafe==L"1")
					{
						return TRUE;
					}
				}
			}
		}
	}
	return FALSE;
}

void CDialogDetect::OnButtonStart() 
{
	UpdateData(TRUE);
	m_bStartDetect	= TRUE;
	DisableControls();
	m_sInfo		= _T("打开X射线机...");
	m_sValue	= _T("");
	m_sIsSafe	= _T("");
	UpdateData(FALSE);

	BeginWaitCursor();
	//关灯
	if(m_bSafeLightOpen)
	{
		m_Serial->ControlSafeLight(ControlCommandClose);
	}
	if(m_bAlarmLightOpen)
	{
		m_Serial->ControlAlarmLight(ControlCommandClose);
	}

	if(!m_Serial->ControlXRadial(ControlCommandOpen))
	{
		m_sInfo	= _T("打开X射线机失败，请检查控制板或通讯线路！");
		UpdateData(FALSE);
		EnableControls();
 		EndWaitCursor();
		m_bStartDetect = FALSE;
		return;
	}

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
	for(UINT i=0; i<pApp->m_para_samplingtimes; i++)
	{
		m_sInfo.Format(_T("测量中（第%d次采样），请稍候..."), i+1);
		UpdateData(FALSE);
		if(!m_Serial->ControlSampling(nSelChannelInterface))
		{
			m_sInfo = _T("测量失败，请检查控制板或通讯线路！");
			UpdateData(FALSE);
			m_Serial->ControlXRadial(ControlCommandClose);
			pApp->m_dlgOK.m_sText = _T("测量中止，请检查控制板或通讯线路！");
			pApp->m_dlgOK.DoModal();
			EnableControls();
 			EndWaitCursor();
			m_bStartDetect = FALSE;
			return;
		}
		nSamplingCount += m_Serial->m_nThresholdSamplingCount;
	}

	m_sInfo = _T("关闭X射线机...");
	UpdateData(FALSE);
	m_Serial->ControlXRadial(ControlCommandClose);

	m_sInfo = _T("测量结果分析中，请稍候...");
	UpdateData(FALSE);
	nSamplingCount = UINT(nSamplingCount/pApp->m_para_samplingtimes);
	if(m_bMethodRatio)//比值法
	{
		if(m_bChannel1)
		{
			nSamplingCount	= UINT(nSamplingCount/pApp->m_para_standardratio1);
		}
		else
		{
			nSamplingCount	= UINT(nSamplingCount/pApp->m_para_standardratio2);
		}
	}
	m_sValue.Format(_T("%ld"),nSamplingCount);

	Sleep(500);
	if(SampleIsSafe(nSamplingCount, nSelChannelInterface))
	{
		m_sIsSafe	= L"安全品";
		m_sInfo		= _T("测量结果——安全品！");
		UpdateData(FALSE);
		m_Serial->ControlSafeLight(ControlCommandOpen);
		m_bSafeLightOpen	= TRUE;
//		Sleep(5000);
//		m_Serial->ControlSafeLight(ControlCommandClose);
//		m_bSafeLightOpen	= FALSE;
	}
	else
	{
		m_sIsSafe	= L"可疑品";
		m_sInfo		= _T("测量结果——可疑品！！！");
		UpdateData(FALSE);
		m_Serial->ControlAlarmLight(ControlCommandOpen);
		m_bAlarmLightOpen	= TRUE;
//		Sleep(5000);
//		m_Serial->ControlAlarmLight(ControlCommandClose);
//		m_bAlarmLightOpen	= FALSE;
	}
	SetTimer(1,3000,NULL);
	
	m_nTimes++;
//	CString s;
//	s.Format("保存次数：%d", m_nTimes);
//	AfxMessageBox(s);
	CppSQLite3DB db;
	try
	{
		db.open(pApp->m_strDbPath);		//打开或新建一个数据库
		CString sSQL;
		//累计次数
		sSQL.Format(_T("update parameter set value='%ld' where parameterid='totaldetecttimes'"),m_nTimes);
		db.execDML(sSQL);
	}
	catch(CppSQLite3Exception ex)
	{
		pApp->m_dlgOK.m_sText = ex.errorMessage();
		pApp->m_dlgOK.DoModal();
		pApp->m_dlgOK.m_sText = _T("累计次数保存失败！");
		pApp->m_dlgOK.DoModal();
		return;
	}

	Sleep(500);

 	EndWaitCursor();
	m_sInfo = _T("请将待测物品放在选定通道处按《测量》");
	EnableControls();
	m_bStartDetect = FALSE;
	UpdateData(FALSE);
}

void CDialogDetect::OnButtonReadX() 
{
	UpdateData(TRUE);
	DisableControls();
	m_sInfo		= _T("打开X射线机...");
	UpdateData(FALSE);
	
	BeginWaitCursor();
	if(!m_Serial->ControlXRadial(ControlCommandOpen))
	{
		m_sInfo	= _T("打开X射线机失败，请检查控制板或通讯线路！");
		UpdateData(FALSE);
 		EndWaitCursor();
		EnableControls();
		return;
	}
	m_sInfo		= _T("正在查询X射线机状态值...");
	UpdateData(FALSE);

	if(!m_SerialX->ReadXRadial())
	{
		m_Serial->ControlXRadial(ControlCommandClose);
		EndWaitCursor();
		m_sInfo = _T("X射线机状态值读取失败，请检查X射线机和通讯线路！");
		UpdateData(FALSE);
		pApp->m_dlgOK.m_sText = _T("X射线机状态值读取失败，请检查X射线机和通讯线路！");
		pApp->m_dlgOK.DoModal();
		EnableControls();
		return;
	}
 	m_Serial->ControlXRadial(ControlCommandClose);
	EndWaitCursor();
	m_X_V = m_SerialX->m_sV;
	m_X_A = m_SerialX->m_sA;
	m_X_T = m_SerialX->m_sT;
	m_X_fT	= m_SerialX->m_fT;
	if(m_X_fT>=30.0)
		m_sInfo = _T("请将待测物品放在选定通道处按《测量》");
	else
		m_sInfo = _T("请尽量保证X射线机温度在30℃以上时进行测量");
	EnableControls();
	UpdateData(FALSE);
}

void CDialogDetect::OnButtonSetX() 
{
	UpdateData(TRUE);
	DisableControls();
	m_sInfo		= _T("正在设置X射线机状态值...");
	UpdateData(FALSE);
	
	BeginWaitCursor();
	if(!m_SerialX->SetXV(m_nSetXV))
	{
		m_sInfo	= _T("设置X射线机电压失败，请检查控制板或通讯线路！");
		UpdateData(FALSE);
 		EndWaitCursor();
		EnableControls();
		return;
	}
	if(!m_SerialX->SetXA(m_nSetXA))
	{
		m_sInfo	= _T("设置X射线机电流失败，请检查控制板或通讯线路！");
		UpdateData(FALSE);
 		EndWaitCursor();
		EnableControls();
		return;
	}
 	EndWaitCursor();
	EnableControls();
	m_sInfo		= _T("请将待测物品放在选定通道处按《测量》");
	UpdateData(FALSE);
}

void CDialogDetect::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	UpdateData(TRUE);
	switch( pScrollBar->GetDlgCtrlID()) 
	{ 
		case IDC_SPIN_SET_X_V: 
		{ 
			m_nSetXV = nPos;
			m_sSetXV.Format(L"%d",nPos);
			break; 
		}
		case IDC_SPIN_SET_X_A:
		{
			m_nSetXA = nPos;
			m_sSetXA.Format(L"%0.2f",m_nSetXA/100.0f); 
			break;
		}
	}
	UpdateData(FALSE);
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CDialogDetect::OnButtonWarmUp() 
{
	UpdateData(TRUE);
	if(!m_bWarmUp)
	{
		m_bWarmUp = TRUE;
		DisableControls();
		SetDlgItemText(IDC_BUTTON_WARM_UP,_T("停止预热"));
		m_sInfo	= _T("打开X射线机...");
		UpdateData(FALSE);
		if(!m_Serial->ControlXRadial(ControlCommandOpen))
		{
			m_bWarmUp = FALSE;
			m_sInfo	= _T("打开X射线机失败，请检查控制板或通讯线路！");
			UpdateData(FALSE);
			pApp->m_dlgOK.m_sText = _T("打开X射线机失败，请检查控制板或通讯线路！预热停止！");
			pApp->m_dlgOK.DoModal();
			EnableControls();
			SetDlgItemText(IDC_BUTTON_WARM_UP,_T("开始预热"));
			return;
		}
		m_sInfo	= _T("正在查询X射线机状态值...");
		UpdateData(FALSE);
		if(!m_SerialX->ReadXRadial())
		{
			m_bWarmUp = FALSE;
			m_Serial->ControlXRadial(ControlCommandClose);
			m_sInfo = _T("X射线机状态值读取失败，请检查X射线机和通讯线路！");
			UpdateData(FALSE);
			pApp->m_dlgOK.m_sText = _T("X射线机状态值读取失败，请检查X射线机和通讯线路！预热停止！");
			pApp->m_dlgOK.DoModal();
			EnableControls();
			SetDlgItemText(IDC_BUTTON_WARM_UP,_T("开始预热"));
			return;
		}
		m_X_T = m_SerialX->m_sT;
		m_X_fT = m_SerialX->m_fT;
		UpdateData(FALSE);
		if(m_X_fT>=30.0)
		{
			m_bWarmUp = FALSE;
			m_sInfo = _T("请将待测物品放在选定通道处按《测量》");
			pApp->m_dlgOK.m_sText = _T("X射线机温度已达30℃，无需再预热！");
			pApp->m_dlgOK.DoModal();
			EnableControls();
			SetDlgItemText(IDC_BUTTON_WARM_UP,_T("开始预热"));
			return;
		}

		m_sInfo = _T("请尽量保证30℃以上时进行测量，X射线机预热中...");
		UpdateData(FALSE);

		//开启自动预热线程
		m_hWarmUpThread = CreateThread(NULL,0,AutoWarmUpThread,this,0, NULL);
		GetDlgItem(IDC_BUTTON_WARM_UP)->EnableWindow(TRUE);
	}
	else
	{
		m_bWarmUp = FALSE;
		BeginWaitCursor();
		GetDlgItem(IDC_BUTTON_WARM_UP)->EnableWindow(FALSE);
		if(m_hWarmUpThread)
		{
			Sleep(1000);
			if(::WaitForSingleObject(m_hWarmUpThread, 1000)   ==   WAIT_TIMEOUT)   
			{   
				//如果超时：   
				m_sInfo	= _T("关闭X射线机...");
				UpdateData(FALSE);
				m_Serial->ControlXRadial(ControlCommandClose);
				TerminateThread(m_hWarmUpThread, NULL);
			}
			else
			{
				CloseHandle(m_hWarmUpThread);
				m_hWarmUpThread=NULL;
			}
		}
		SetDlgItemText(IDC_BUTTON_WARM_UP,_T("开始预热"));
		EnableControls();
 		if(m_X_fT>=30.0)
			m_sInfo = _T("请将待测物品放在选定通道处按《测量》");
		else
			m_sInfo = _T("请尽量保证X射线机温度在30℃以上时进行测量！");
		UpdateData(FALSE);
		EndWaitCursor();
	}
}

//预热线程
DWORD WINAPI CDialogDetect::AutoWarmUpThread(LPVOID lpParameter)
{
	CDialogDetect *pDialog;
	pDialog=(CDialogDetect*)lpParameter;

	BOOL bError(FALSE);
	while (pDialog->m_bWarmUp)
	{
		Sleep(1000);
		if(!pDialog->m_SerialX->ReadXRadial())
		{
			bError	= TRUE;
			pDialog->m_sInfo = _T("X射线机状态值读取失败，请检查X射线机和通讯线路！");
			pDialog->UpdateData(FALSE);
			pDialog->pApp->m_dlgOK.m_sText = _T("X射线机状态值读取失败，请检查X射线机和通讯线路！预热停止！");
			pDialog->pApp->m_dlgOK.DoModal();
			pDialog->m_bWarmUp = FALSE;
		}
		else
		{
			pDialog->m_X_T = pDialog->m_SerialX->m_sT;
			pDialog->m_X_fT = pDialog->m_SerialX->m_fT;
			pDialog->UpdateData(FALSE);
			if(pDialog->m_X_fT>30.5)
			{
				pDialog->m_bWarmUp = FALSE;
			}
		}
	}
	if(!bError)
	{
		if(!pDialog->m_Serial->ControlXRadial(ControlCommandClose))
		{
			pDialog->m_sInfo	= _T("关闭X射线机失败，请检查控制板或通讯线路！");
			pDialog->UpdateData(FALSE);
			pDialog->pApp->m_dlgOK.m_sText = pDialog->m_sInfo;
			pDialog->pApp->m_dlgOK.DoModal();
		}
		else
		{
			if(pDialog->m_X_fT>=30.0)
				pDialog->m_sInfo = _T("请将待测物品放在选定通道处按《测量》");
			else
				pDialog->m_sInfo = _T("请尽量保证X射线机温度在30℃以上时进行测量！");
		}
	}
	pDialog->SetDlgItemText(IDC_BUTTON_WARM_UP,_T("开始预热"));
	pDialog->EnableControls();
	pDialog->UpdateData(FALSE);
		
	return 0;
}

void CDialogDetect::OnButtonDemarcate() 
{
	CDialogDemarcate dlg;
	dlg.DoModal();
}

void CDialogDetect::OnButtonTest() 
{
	CDialogTest dlg;
	dlg.DoModal();
}

void CDialogDetect::OnButtonClearTimes() 
{
	UpdateData(TRUE);
	CppSQLite3DB db;
	try
	{
		db.open(pApp->m_strDbPath);		//打开或新建一个数据库
		CString sSQL;
		//累计次数
		sSQL.Format(_T("update parameter set value='0' where parameterid='totaldetecttimes'"));
		db.execDML(sSQL);
	}
	catch(CppSQLite3Exception ex)
	{
		pApp->m_dlgOK.m_sText = ex.errorMessage();
		pApp->m_dlgOK.DoModal();
		pApp->m_dlgOK.m_sText = _T("累计次数清零保存失败！");
		pApp->m_dlgOK.DoModal();
		return;
	}
	m_nTimes = 0;
	UpdateData(FALSE);
}
