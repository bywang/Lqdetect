// DialogParameter.cpp : implementation file
//

#include "stdafx.h"
#include "LQDetect.h"
#include "DialogParameter.h"
#include "DialogSetDisplay.h"
#include "DialogOtherParameter.h"
#include "DialogContainer.h"
#include "DialogLiquid.h"
#include "..\_Common\color.h"	//颜色定义

//#include "CeFileFind.h"
#include "sipapi.h"				//软键盘控制

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDialogParameter *pDialog;
/////////////////////////////////////////////////////////////////////////////
// CDialogParameter dialog
CDialogParameter::CDialogParameter(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogParameter::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogParameter)
	m_sLiquid = _T("");
	m_sSecond = _T("");
	m_sContainer = _T("");
	m_iCountDown = 0;
	m_iCountUp = 0;
	m_iCountAverage = 0;
	m_sInfo = _T("");
	//}}AFX_DATA_INIT
	m_bReadValueTest	= FALSE;
	
	//用于Paint
	m_bUpdateData		= FALSE;
	m_bPaint			= FALSE;
	//用于结果计算
	m_nCurrentMaxCount			= 0;	//当前样本最大计数值
	m_nCurrentMinCount			= 0;	//当前样本最小计数值
	m_nCurrentAverageCount		= 0;	//当前样本平均计数值
	m_nCurrentSamplingNumber	= 0;	//当前样本采样次数
	
	for(int i=0; i<1024; i++)
	{
		m_fDrawData[i]=0.0f;
	}
	m_nDrawIndexSS	= 0;			//单样本单次画线序号，默认为0画最后一条

	m_bKeyboardEnable	= FALSE;	//不允许显示键盘
	m_bKeyboardShowed	= FALSE;

	m_bWaitThread		= FALSE;
	
}

CDialogParameter::~CDialogParameter()
{
	ClearArrayData();
}

void CDialogParameter::ClearArrayData()
{
	//数组清空
	for(int i=0; i<m_arraySamplingResult.GetSize(); i++)
	{
		delete m_arraySamplingResult[i];
	}
	m_arraySamplingResult.RemoveAll();
	
	for(i=0; i<m_arraySamplingData.GetSize(); i++)
	{
		delete m_arraySamplingData[i];
	}
	m_arraySamplingData.RemoveAll();
}

void CDialogParameter::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogParameter)
	DDX_Control(pDX, IDC_LIST_SWATCH, m_listSwatch);
	DDX_Control(pDX, IDC_LIST_DETECTVALUE, m_listDetectValue);
	DDX_CBString(pDX, IDC_COMBO_LIQUID, m_sLiquid);
	DDX_CBString(pDX, IDC_COMBO_SECOND, m_sSecond);
	DDX_CBString(pDX, IDC_COMBO_CONTAINER, m_sContainer);
	DDX_Text(pDX, IDC_EDIT_COUNTDOWN, m_iCountDown);
	DDX_Text(pDX, IDC_EDIT_COUNTUP, m_iCountUp);
	DDX_Text(pDX, IDC_EDIT_COUNTAVERAGE, m_iCountAverage);
	DDX_Text(pDX, IDC_EDIT_INFO, m_sInfo);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDialogParameter, CDialog)
	//{{AFX_MSG_MAP(CDialogParameter)
	ON_COMMAND(IDR_KEY_ENTER, OnEnter)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_RADIO_CHANNEL1, OnRadioChannel1)
	ON_BN_CLICKED(IDC_RADIO_CHANNEL2, OnRadioChannel2)
	ON_BN_CLICKED(IDC_RADIO_DRAWSS, OnRadioDrawSS)
	ON_BN_CLICKED(IDC_RADIO_DRAWSM, OnRadioDrawSM)
	ON_BN_CLICKED(IDC_RADIO_DRAWMS, OnRadioDrawMS)
	ON_CBN_SELCHANGE(IDC_COMBO_SECOND, OnSelchangeComboSecond)
	ON_BN_CLICKED(IDC_BUTTON_AUTO_DETECT, OnButtonAutoDetect)
	ON_BN_CLICKED(IDC_BUTTON_SET_DISPLAY, OnButtonSetDisplay)
	ON_NOTIFY(NM_CLICK, IDC_LIST_DETECTVALUE, OnClickListDetectValue)
	ON_NOTIFY(NM_CLICK, IDC_LIST_SWATCH, OnClickListSwatch)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_SWATCH, OnButtonDeleteSwatch)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_SWATCH_MODIFY, OnButtonSaveSwatchModify)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_DETECT_VALUE, OnButtonDeleteDetectValue)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE_SWATCH, OnButtonUpdateSwatch)
	ON_BN_CLICKED(IDC_BUTTON_OTHER_PARAMETER, OnButtonOtherParameter)
	ON_CBN_SELCHANGE(IDC_COMBO_CONTAINER, OnSelchangeComboContainer)
	ON_BN_CLICKED(IDC_BUTTON_MANUAL_DETECT, OnButtonManualDetect)
	ON_EN_SETFOCUS(IDC_EDIT_COUNTUP, OnSetfocusEditCountup)
	ON_EN_SETFOCUS(IDC_EDIT_COUNTDOWN, OnSetfocusEditCountdown)
	ON_EN_SETFOCUS(IDC_EDIT_COUNTAVERAGE, OnSetfocusEditCountaverage)
	ON_EN_KILLFOCUS(IDC_EDIT_COUNTUP, OnKillfocusEditCountup)
	ON_EN_KILLFOCUS(IDC_EDIT_COUNTDOWN, OnKillfocusEditCountdown)
	ON_EN_KILLFOCUS(IDC_EDIT_COUNTAVERAGE, OnKillfocusEditCountaverage)
	ON_CBN_SELCHANGE(IDC_COMBO_LIQUID, OnSelchangeComboLiquid)
	ON_BN_CLICKED(IDC_BUTTON_LIQUID, OnButtonLiquid)
	ON_BN_CLICKED(IDC_BUTTON_CONTAINER, OnButtonContainer)
	ON_BN_CLICKED(IDC_BUTTON_DELETEALL_DETECT_VALUE, OnButtonDeleteallDetectValue)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_SAMPLINGDATA, OnButtonSaveSamplingData)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_SAMPLINGDATA, OnButtonLoadSamplingData)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogParameter message handlers
BOOL CDialogParameter::PreTranslateMessage(MSG* pMsg) 
{
    if (WM_KEYFIRST<=pMsg->message && pMsg->message<=WM_KEYLAST && m_hAccel) 
    { 
        HACCEL hAccel = m_hAccel; 
        if (::TranslateAccelerator(m_hWnd, hAccel, pMsg)) 
            return TRUE; 
    } 
	return CDialog::PreTranslateMessage(pMsg);
}

void CDialogParameter::OnEnter() 
{
	DWORD dwStyle = GetFocus()->GetStyle();
	if(dwStyle!=0x50010001L && dwStyle!=0x50010000L)
	{
		CWnd* pWndNext = GetNextDlgTabItem(GetFocus());
		if(pWndNext)
		{
			pWndNext->SetFocus();
		}
	}
	else
	{
		GetFocus()->PostMessage(BM_CLICK);
	}
}

BOOL CDialogParameter::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_hAccel = ::LoadAccelerators(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_KEY_ENTER)); 
	ASSERT(m_hAccel);
	
	::SetWindowPos(m_hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE |SWP_NOSIZE);
	pApp=(CLQDetectApp *)AfxGetApp();

	//打开串口
	m_bOpen	= m_Serial.OpenPort(_T("COM1:"),115200,8,ONESTOPBIT,NOPARITY);
	if(!m_bOpen || !m_Serial.Activate())
	{
		pApp->m_dlgOK.m_sText = _T("控制板通讯口初始化失败，无法进行测试！");
		pApp->m_dlgOK.DoModal();
		CDialog::OnCancel();
		return FALSE;
	}


	GetDlgItem(IDC_BUTTON_SAVE_SWATCH_MODIFY)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_DELETE_SWATCH)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_DELETE_DETECT_VALUE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_UPDATE_SWATCH)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_SAVE_SAMPLINGDATA)->EnableWindow(FALSE);

	((CEdit*)GetDlgItem(IDC_EDIT_COUNTUP))->SetReadOnly(TRUE);
	((CEdit*)GetDlgItem(IDC_EDIT_COUNTDOWN))->SetReadOnly(TRUE);
	((CEdit*)GetDlgItem(IDC_EDIT_COUNTAVERAGE))->SetReadOnly(TRUE);

	SetControls();
	
	OnRadioChannel1();	//选择通道1	
	OnRadioDrawSS();	//单样本单次

	m_sInfo = _T("请将待标定样本放在选定通道处进行采样！");
	MessageUpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogParameter::SetControls()
{
	//设置下拉列表
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
	
	pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_LIQUID);
	pCombo->ResetContent();
	for(i=0; i<pApp->m_arrayLiquid.GetSize(); i++)
	{
		pCombo->AddString(pApp->m_arrayLiquid[i]->liq_name);
	}
	m_sLiquid	= pApp->m_arrayLiquid[0]->liq_name;

	//设置list
	m_listDetectValue.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	i=0;
	m_listDetectValue.InsertColumn(i++, L"次数", LVCFMT_CENTER, 40);
	m_listDetectValue.InsertColumn(i++, L"阈内计数值", LVCFMT_CENTER, 80);
	
	m_listSwatch.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	i=0;
	m_listSwatch.InsertColumn(i++, L"接口", LVCFMT_CENTER, 40);
	m_listSwatch.InsertColumn(i++, L"容器名称", LVCFMT_CENTER, 135);
	m_listSwatch.InsertColumn(i++, L"液体名称", LVCFMT_CENTER, 80);
	m_listSwatch.InsertColumn(i++, L"最大计数", LVCFMT_CENTER, 60);
	m_listSwatch.InsertColumn(i++, L"最小计数", LVCFMT_CENTER, 60);
	m_listSwatch.InsertColumn(i++, L"平均计数", LVCFMT_CENTER, 60);
	m_listSwatch.InsertColumn(i++, L"阈值范围", LVCFMT_CENTER, 60);
	m_listSwatch.InsertColumn(i++, L"样本特性", LVCFMT_CENTER, 60);
	
	MessageUpdateData(FALSE);
}

void CDialogParameter::EnableControls()
{
	//图像显示区
	GetDlgItem(IDC_RADIO_DRAWSS)->EnableWindow(TRUE);					//单样单次
	GetDlgItem(IDC_RADIO_DRAWSM)->EnableWindow(TRUE);					//单样多次
	GetDlgItem(IDC_RADIO_DRAWMS)->EnableWindow(TRUE);					//多样单次
	GetDlgItem(IDC_BUTTON_SET_DISPLAY)->EnableWindow(TRUE);				//显示设置
	
	//采样控制区
	GetDlgItem(IDC_COMBO_SECOND)->EnableWindow(TRUE);					//采样间隔
	GetDlgItem(IDC_RADIO_CHANNEL1)->EnableWindow(TRUE);					//第一通道
	GetDlgItem(IDC_RADIO_CHANNEL2)->EnableWindow(TRUE);					//第二通道
	GetDlgItem(IDC_BUTTON_CONTAINER)->EnableWindow(TRUE);				//容器按钮
	GetDlgItem(IDC_COMBO_CONTAINER)->EnableWindow(TRUE);				//容器选择
	GetDlgItem(IDC_BUTTON_LIQUID)->EnableWindow(TRUE);					//液体按钮
	GetDlgItem(IDC_COMBO_LIQUID)->EnableWindow(TRUE);					//液体选择
	GetDlgItem(IDC_BUTTON_MANUAL_DETECT)->EnableWindow(TRUE);			//手动采样
	GetDlgItem(IDC_BUTTON_AUTO_DETECT)->EnableWindow(TRUE);				//自动采样

	GetDlgItem(IDC_BUTTON_SAVE_SAMPLINGDATA)->EnableWindow(TRUE);		//保存样本
	GetDlgItem(IDC_BUTTON_LOAD_SAMPLINGDATA)->EnableWindow(TRUE);		//载入样本
	GetDlgItem(IDC_BUTTON_OTHER_PARAMETER)->EnableWindow(TRUE);			//其它参数
	GetDlgItem(IDOK)->EnableWindow(TRUE);								//退出
	
/*
	//采样结果
	GetDlgItem(IDC_BUTTON_DELETE_DETECT_VALUE)->EnableWindow(TRUE);		//删除采样值
	GetDlgItem(IDC_BUTTON_DELETEALL_DETECT_VALUE)->EnableWindow(TRUE);	//删除全部采样值
	GetDlgItem(IDC_BUTTON_UPDATE_SWATCH)->EnableWindow(TRUE);			//更新样本

	//样本管理
	GetDlgItem(IDC_BUTTON_SAVE_SWATCH_MODIFY)->EnableWindow(TRUE);		//样本修改保存
	GetDlgItem(IDC_BUTTON_DELETE_SWATCH)->EnableWindow(TRUE);			//删除样本
	((CEdit*)GetDlgItem(IDC_EDIT_COUNTUP))->SetReadOnly(FALSE);			//计数上限
	((CEdit*)GetDlgItem(IDC_EDIT_COUNTDOWN))->SetReadOnly(FALSE);		//计数下限
	((CEdit*)GetDlgItem(IDC_EDIT_COUNTAVERAGE))->SetReadOnly(FALSE);	//计数平均
*/
}

void CDialogParameter::DisableControls()
{
	//图像显示区
	GetDlgItem(IDC_RADIO_DRAWSS)->EnableWindow(FALSE);					//单样单次
	GetDlgItem(IDC_RADIO_DRAWSM)->EnableWindow(FALSE);					//单样多次
	GetDlgItem(IDC_RADIO_DRAWMS)->EnableWindow(FALSE);					//多样单次
	GetDlgItem(IDC_BUTTON_SET_DISPLAY)->EnableWindow(FALSE);			//显示设置

	//采样控制区
	GetDlgItem(IDC_COMBO_SECOND)->EnableWindow(FALSE);					//采样间隔
	GetDlgItem(IDC_RADIO_CHANNEL1)->EnableWindow(FALSE);				//第一通道
	GetDlgItem(IDC_RADIO_CHANNEL2)->EnableWindow(FALSE);				//第二通道
	GetDlgItem(IDC_BUTTON_CONTAINER)->EnableWindow(FALSE);				//容器按钮
	GetDlgItem(IDC_COMBO_CONTAINER)->EnableWindow(FALSE);				//容器选择
	GetDlgItem(IDC_BUTTON_LIQUID)->EnableWindow(FALSE);					//液体按钮
	GetDlgItem(IDC_COMBO_LIQUID)->EnableWindow(FALSE);					//液体选择
	GetDlgItem(IDC_BUTTON_MANUAL_DETECT)->EnableWindow(FALSE);			//手动采样
	GetDlgItem(IDC_BUTTON_AUTO_DETECT)->EnableWindow(FALSE);			//自动采样

	GetDlgItem(IDC_BUTTON_OTHER_PARAMETER)->EnableWindow(FALSE);		//其它参数
	GetDlgItem(IDOK)->EnableWindow(FALSE);								//退出
	
	//采样结果
	GetDlgItem(IDC_BUTTON_DELETE_DETECT_VALUE)->EnableWindow(FALSE);	//删除采样值
	GetDlgItem(IDC_BUTTON_DELETEALL_DETECT_VALUE)->EnableWindow(FALSE);	//删除全部采样值
	GetDlgItem(IDC_BUTTON_UPDATE_SWATCH)->EnableWindow(FALSE);			//更新样本
	GetDlgItem(IDC_BUTTON_SAVE_SAMPLINGDATA)->EnableWindow(FALSE);		//保存样本
	GetDlgItem(IDC_BUTTON_LOAD_SAMPLINGDATA)->EnableWindow(FALSE);		//载入样本

	//样本管理
	GetDlgItem(IDC_BUTTON_SAVE_SWATCH_MODIFY)->EnableWindow(FALSE);		//样本修改保存
	GetDlgItem(IDC_BUTTON_DELETE_SWATCH)->EnableWindow(FALSE);			//删除样本
	((CEdit*)GetDlgItem(IDC_EDIT_COUNTUP))->SetReadOnly(TRUE);			//计数上限
	((CEdit*)GetDlgItem(IDC_EDIT_COUNTDOWN))->SetReadOnly(TRUE);		//计数下限
	((CEdit*)GetDlgItem(IDC_EDIT_COUNTAVERAGE))->SetReadOnly(TRUE);		//计数平均
}

void CDialogParameter::LoadSwatchFromArray() 
{
	UpdateData(TRUE);
	CString	sChannel;
	sChannel.Format(L"%d", m_nSelChannelInterface);
//	AfxMessageBox(sChannel);

	int nRow, j;
	CString strData;
	m_listSwatch.DeleteAllItems();
	
	for(int i=0; i<pApp->m_arraySwatch.GetSize(); i++)
	{
		//过滤通道和容器
		if(pApp->m_arraySwatch[i]->channel==sChannel && pApp->m_arraySwatch[i]->con_name==m_sContainer)
		{
			nRow=m_listSwatch.InsertItem(m_listSwatch.GetItemCount(), pApp->m_arraySwatch[i]->channel);	//通道
			j=1;
			m_listSwatch.SetItemText(nRow, j++, pApp->m_arraySwatch[i]->con_name);						//容器名称
			m_listSwatch.SetItemText(nRow, j++, pApp->m_arraySwatch[i]->liq_name);						//液体名称
			m_listSwatch.SetItemText(nRow, j++, pApp->m_arraySwatch[i]->countup);						//最大计数
			m_listSwatch.SetItemText(nRow, j++, pApp->m_arraySwatch[i]->countdown);						//最小计数
			m_listSwatch.SetItemText(nRow, j++, pApp->m_arraySwatch[i]->countaverage);					//平均计数
			m_listSwatch.SetItemText(nRow, j++, pApp->m_arraySwatch[i]->countrange);					//阈值范围
			if(pApp->m_arraySwatch[i]->issafe==L"1")
				strData = L"安全品";
			else
				strData	= L"可疑品";
			m_listSwatch.SetItemText(nRow, j++, strData);												//样品特性
		}
	}
	UpdateData(FALSE);
}

void CDialogParameter::OnRadioChannel1() 
{
	m_bChannel1	= TRUE;
	m_bChannel2	= FALSE;
	((CButton *)GetDlgItem(IDC_RADIO_CHANNEL1))->SetCheck(m_bChannel1);//选上
	((CButton *)GetDlgItem(IDC_RADIO_CHANNEL2))->SetCheck(m_bChannel2);
	//取通道对应接口
	m_nSelChannelInterface = pApp->m_para_channelinterface1;
//	AfxMessageBox(L"C1");
	LoadSwatchFromArray();
	LoadDetectValueFromSamplingDataArray();
	UpdateData(FALSE);

	CRect rcVal;
	GetDlgItem(IDC_STATIC_DRAW)->GetWindowRect(&rcVal);
	InvalidateRect(&rcVal);
}

void CDialogParameter::OnRadioChannel2() 
{
	m_bChannel1	= FALSE;
	m_bChannel2	= TRUE;
	((CButton *)GetDlgItem(IDC_RADIO_CHANNEL1))->SetCheck(m_bChannel1);
	((CButton *)GetDlgItem(IDC_RADIO_CHANNEL2))->SetCheck(m_bChannel2);//选上
	//取通道对应接口
	m_nSelChannelInterface = pApp->m_para_channelinterface2;

	LoadSwatchFromArray();
	LoadDetectValueFromSamplingDataArray();
	UpdateData(FALSE);

	CRect rcVal;
	GetDlgItem(IDC_STATIC_DRAW)->GetWindowRect(&rcVal);
	InvalidateRect(&rcVal);
}

void CDialogParameter::OnRadioDrawSS() 
{
	m_bDrawSS	= TRUE;
	m_bDrawSM	= FALSE;
	m_bDrawMS	= FALSE;
	((CButton *)GetDlgItem(IDC_RADIO_DRAWSS))->SetCheck(m_bDrawSS);
	((CButton *)GetDlgItem(IDC_RADIO_DRAWSM))->SetCheck(m_bDrawSM);
	((CButton *)GetDlgItem(IDC_RADIO_DRAWMS))->SetCheck(m_bDrawMS);
	m_nDrawIndexSS	= 0;
	UpdateData(FALSE);
	CRect rcVal;
	GetDlgItem(IDC_STATIC_DRAW)->GetWindowRect(&rcVal);
	InvalidateRect(&rcVal);
}

void CDialogParameter::OnRadioDrawSM() 
{
	m_bDrawSS	= FALSE;
	m_bDrawSM	= TRUE;
	m_bDrawMS	= FALSE;
	((CButton *)GetDlgItem(IDC_RADIO_DRAWSS))->SetCheck(m_bDrawSS);
	((CButton *)GetDlgItem(IDC_RADIO_DRAWSM))->SetCheck(m_bDrawSM);
	((CButton *)GetDlgItem(IDC_RADIO_DRAWMS))->SetCheck(m_bDrawMS);
	UpdateData(FALSE);
	CRect rcVal;
	GetDlgItem(IDC_STATIC_DRAW)->GetWindowRect(&rcVal);
	InvalidateRect(&rcVal);
}

void CDialogParameter::OnRadioDrawMS() 
{
	m_bDrawSS	= FALSE;
	m_bDrawSM	= FALSE;
	m_bDrawMS	= TRUE;
	((CButton *)GetDlgItem(IDC_RADIO_DRAWSS))->SetCheck(m_bDrawSS);
	((CButton *)GetDlgItem(IDC_RADIO_DRAWSM))->SetCheck(m_bDrawSM);
	((CButton *)GetDlgItem(IDC_RADIO_DRAWMS))->SetCheck(m_bDrawMS);
	UpdateData(FALSE);
	CRect rcVal;
	GetDlgItem(IDC_STATIC_DRAW)->GetWindowRect(&rcVal);
	InvalidateRect(&rcVal);
}

void CDialogParameter::OnDestroy() 
{
	m_Serial.Deactivate();
	m_Serial.ClosePort();

	CDialog::OnDestroy();
}

void CDialogParameter::OnSelchangeComboSecond() 
{
	UpdateData(TRUE);
	m_nSecond	= _ttoi(m_sSecond);
	UpdateData(FALSE);
}

void CDialogParameter::OnSetfocusEditCountup() 
{
	if(m_bKeyboardEnable && !m_bKeyboardShowed)
	{
		SipShowIM(SIPF_ON);  //面板显示
		m_bKeyboardShowed	= TRUE;
	}
}

void CDialogParameter::OnSetfocusEditCountdown() 
{
	if(m_bKeyboardEnable && !m_bKeyboardShowed)
	{
		SipShowIM(SIPF_ON);  //面板显示
		m_bKeyboardShowed	= TRUE;
	}
}

void CDialogParameter::OnSetfocusEditCountaverage() 
{
	if(m_bKeyboardEnable && !m_bKeyboardShowed)
	{
		SipShowIM(SIPF_ON);  //面板显示
		m_bKeyboardShowed	= TRUE;
	}
}

void CDialogParameter::OnKillfocusEditCountup() 
{
	if(m_bKeyboardShowed)
	{
		SipShowIM(SIPF_OFF);//面板隐藏
		m_bKeyboardShowed	= FALSE;
	}
}

void CDialogParameter::OnKillfocusEditCountdown() 
{
	if(m_bKeyboardShowed)
	{
		SipShowIM(SIPF_OFF);//面板隐藏
		m_bKeyboardShowed	= FALSE;
	}
}

void CDialogParameter::OnKillfocusEditCountaverage() 
{
	if(m_bKeyboardShowed)
	{
		SipShowIM(SIPF_OFF);//面板隐藏
		m_bKeyboardShowed	= FALSE;
	}
}

void CDialogParameter::OnButtonContainer() 
{
	CDialogContainer dlg;
	dlg.DoModal();
	if(dlg.m_bAddNew)
	{
		CComboBox*		pCombo;
		pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_CONTAINER);
		pCombo->ResetContent();
		for(int i=0; i<pApp->m_arrayContainer.GetSize(); i++)
		{
			pCombo->AddString(pApp->m_arrayContainer[i]->con_name);
		}
		m_sContainer	= pApp->m_arrayContainer[0]->con_name;

		OnRadioChannel1();	//选择通道1	
		OnRadioDrawSS();	//单样本单次
	}
}

void CDialogParameter::OnButtonLiquid() 
{
	CDialogLiquid dlg;
	dlg.DoModal();
	if(dlg.m_bAddNew)
	{
		//更新下拉列表
		CComboBox*		pCombo;
		pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_LIQUID);
		pCombo->ResetContent();
		for(int i=0; i<pApp->m_arrayLiquid.GetSize(); i++)
		{
			pCombo->AddString(pApp->m_arrayLiquid[i]->liq_name);
		}
		m_sLiquid	= pApp->m_arrayLiquid[0]->liq_name;

		OnRadioChannel1();	//选择通道1	
		OnRadioDrawSS();	//单样本单次
	}
}

void CDialogParameter::OnButtonSaveSamplingData() 
{
/*
	//造数据
	structSamplingData* pSamplingData		= new structSamplingData;
	pSamplingData->channel					= 1;					//通道接口号
	pSamplingData->con_name					= "玻璃";				//容器名称
	pSamplingData->liq_name					= "水";					//液体名称
	for(UINT n=0; n<1024; n++)
	{
		//拷贝全部采样数据
		pSamplingData->SamplingData[n]	= n;
	}
	m_arraySamplingData.Add(pSamplingData);

	pSamplingData		= new structSamplingData;
	pSamplingData->channel					= 1;					//通道接口号
	pSamplingData->con_name					= "玻璃";				//容器名称
	pSamplingData->liq_name					= "水";					//液体名称
	for(n=0; n<1024; n++)
	{
		//拷贝全部采样数据
		pSamplingData->SamplingData[n]	= n;
	}
	m_arraySamplingData.Add(pSamplingData);	
	//结束造数据*/

	
	if(m_arraySamplingData.GetSize()<=0)
	{
		pApp->m_dlgOK.m_sText = _T("无采样结果数据，无法保存！");
		pApp->m_dlgOK.DoModal();
		return;
	}
	CString StrFilePath;
	CString StrFilter;

	// 文件扩展名
	StrFilter = L"dat(*.dat)|*.dat||";
	// 打开保存对话框
	CFileDialog dlg(FALSE,NULL,NULL,OFN_HIDEREADONLY | 
		            OFN_OVERWRITEPROMPT | OFN_EXPLORER,StrFilter);
	if (dlg.DoModal() != IDOK) 
		return;

	StrFilePath = dlg.GetPathName();

	if(StrFilePath.Right(4)!=L".dat")
		StrFilePath += L".dat";

	//创建保存采样结果数据库
	CString sSQL, sData, s;
	CppSQLite3DB db;

	int i,j;
//	unsigned char data[2048];
//	AfxMessageBox(L"creat db");
	BeginWaitCursor();
	try
	{
		//打开或新建一个数据库
		db.open(StrFilePath);

		db.execDML(L"begin transaction;");
		if(db.tableExists(L"Sampling"))
		{
			db.execDML(L"DROP TABLE Sampling");
		}
		//新建表
		db.execDML(L"create table Sampling(SamplingNo integer primary key, channel integer, con_name nvarchar(50), liq_name nvarchar(50), data varchar(4096))");

		for(i=0; i<m_arraySamplingData.GetSize(); i++)
		{
			sData = "";
			for(j=0; j<1024; j++)
			{
				s.Format(L"%02X%02X",unsigned char(m_arraySamplingData[i]->SamplingData[j]/256), unsigned char(m_arraySamplingData[i]->SamplingData[j]%256));
				sData += s;
			}
//			s.Format(L"%d", sData.GetLength());
//			AfxMessageBox(s);
//			AfxMessageBox(sData);
			sSQL.Format(_T("insert into Sampling(SamplingNo, channel, con_name, liq_name, data) values(%d, %d, '%s', '%s','%s')"),
					i,
					m_arraySamplingData[i]->channel,
					m_arraySamplingData[i]->con_name,
					m_arraySamplingData[i]->liq_name,
					sData);
//			AfxMessageBox(sSQL);
			db.execDML(sSQL);
		}
		db.execDML(L"commit transaction;");

		db.close();
//		AfxMessageBox(L"OK");
	}
	catch(CppSQLite3Exception ex)
	{
	 	EndWaitCursor();
		pApp->m_dlgOK.m_sText = ex.errorMessage();
		pApp->m_dlgOK.DoModal();
		pApp->m_dlgOK.m_sText = _T("采样结果数据保存失败！");
		pApp->m_dlgOK.DoModal();
		return;
	}
	
 	EndWaitCursor();
	pApp->m_dlgOK.m_sText = _T("采样结果数据保存成功！");
	pApp->m_dlgOK.DoModal();
}

void CDialogParameter::OnButtonLoadSamplingData() 
{
	pApp->m_dlgOKCANCEL.m_sText = _T("载入采样结果数据将清空当前测量数据，是否继续？");
	if(pApp->m_dlgOKCANCEL.DoModal()==IDCANCEL)
		return;
	
	CString StrFilePath;
	CString StrFilter;

	// 文件扩展名
	StrFilter = L"dat(*.dat)|*.dat||";
	// 打开保存图像对话框
	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY | 
		            OFN_OVERWRITEPROMPT | OFN_EXPLORER, StrFilter);
	if (dlg.DoModal() != IDOK) 
		return;

	StrFilePath=dlg.GetPathName();
	if(StrFilePath.Right(4)!=L".dat")
		StrFilePath += L".dat";
//	AfxMessageBox(StrFilePath);
	sqlite3_stmt * stat = NULL; //预编译使用到的一个很重要的数据结构

	CString sSQL, sData;
	int count;
	CppSQLite3DB db;
	unsigned char data[2048]; 
	BeginWaitCursor();
	try
	{
		//打开或新建一个数据库
		db.open(StrFilePath);
		if(!db.tableExists(L"Sampling"))
		{
			pApp->m_dlgOK.m_sText = _T("采样结果数据不存在，请重新选择！");
			pApp->m_dlgOK.DoModal();
			return;
		}
		//判断是否存在数据
		count = db.execScalar(L"select count(*) from Sampling");
		if(count<=0)
		{
			pApp->m_dlgOK.m_sText = _T("无采样结果数据，无法载入！");
			pApp->m_dlgOK.DoModal();
			return;
		}

		ClearArrayData();
		
		CppSQLite3Query q = db.execQuery(L"select * from Sampling");
		int n(0);
		while (!q.eof())
        {
			n++;
			sData = q.fieldValue(4);
			if(sData.GetLength()!=4096)
			{
				sSQL.Format(_T("第%d条采样结果曲线数据不正确，无法载入！"), n);
				pApp->m_dlgOK.m_sText = sSQL;
				pApp->m_dlgOK.DoModal();
				q.nextRow();
				continue;
			}
//			sSQL.Format(_T("%d"), sData.GetLength());
//			AfxMessageBox(sSQL);
//			AfxMessageBox(sData);
			for(UINT i=0; i<2048; i++)
			{
				if(sData.GetAt(i*2+1)>='0' && sData.GetAt(i*2+1)<='9')
				{
					data[i]=sData.GetAt(i*2+1)-48;
				}
				else
				{
					data[i]=sData.GetAt(i*2+1)-55;
				}
				if(sData.GetAt(i*2)>='0'&&sData.GetAt(i*2)<='9')
				{
					data[i]+=(sData.GetAt(i*2)-48)*16;
				}
				else
				{
					data[i]+=(sData.GetAt(i*2)-55)*16;
				}
			}
			structSamplingData* pSamplingData	= new structSamplingData;
			pSamplingData->channel	= _ttoi(q.fieldValue(1));		//通道接口号
			pSamplingData->con_name	= q.fieldValue(2);				//容器名称
			pSamplingData->liq_name	= q.fieldValue(3);				//液体名称
			pSamplingData->ThresholdSamplingCount	= 0;			//采样阈内计数值
			pSamplingData->MaxValue					= 0;			//最大数值

			for(i=0; i<1024; i++)
			{
				//拷贝全部采样数据
				pSamplingData->SamplingData[i]	= data[i*2]*256+data[i*2+1];
				//计算阈内计数值
				if(i>=(pApp->m_para_thresholdbegin-1) && i<pApp->m_para_thresholdend)
				{
					pSamplingData->ThresholdSamplingCount += pSamplingData->SamplingData[i];
				}
				//最大数值
				if(pSamplingData->MaxValue<pSamplingData->SamplingData[i])
				{
					pSamplingData->MaxValue=pSamplingData->SamplingData[i];
				}
			}
			m_arraySamplingData.Add(pSamplingData);
			q.nextRow();
		}
		q.finalize();

		db.close();
//		AfxMessageBox(_T("OK"));
	}
	catch(CppSQLite3Exception ex)
	{
	 	EndWaitCursor();
		pApp->m_dlgOK.m_sText = ex.errorMessage();
		pApp->m_dlgOK.DoModal();
		pApp->m_dlgOK.m_sText = _T("采样结果数据导入失败！");
		pApp->m_dlgOK.DoModal();
		return;
	}
//	sSQL.Format(L"%d", m_arraySamplingData.GetSize());
//	AfxMessageBox(sSQL);

	RecountSamplingDataArray();
	LoadDetectValueFromSamplingDataArray();
 	EndWaitCursor();
	pApp->m_dlgOK.m_sText = _T("采样结果数据导入成功！");
	pApp->m_dlgOK.DoModal();
}

void CDialogParameter::RecountSamplingDataArray()	//按新的阈限重新计算采样数据
{
	int i, j, n;
	BOOL bFound(FALSE);
	//重新计算采样详细数据
	for(i=0; i<m_arraySamplingData.GetSize(); i++)
	{
		m_arraySamplingData[i]->ThresholdSamplingCount	= 0;	//采样阈内计数值
		m_arraySamplingData[i]->MaxValue	= 0;				//采样阈内计数值
		for(j=0; j<1024; j++)
		{
			//计算阈内计数值
			if(j>=(pApp->m_para_thresholdbegin-1) && j<pApp->m_para_thresholdend)
			{
				m_arraySamplingData[i]->ThresholdSamplingCount += m_arraySamplingData[i]->SamplingData[j];
			}
			//最大数值
			if(m_arraySamplingData[i]->MaxValue<m_arraySamplingData[i]->SamplingData[j])
			{
				m_arraySamplingData[i]->MaxValue=m_arraySamplingData[i]->SamplingData[j];
			}
		}
		//查找结果数据是否存在
		bFound = FALSE;
		for(j=0; j<m_arraySamplingResult.GetSize(); j++)
		{
			if(m_arraySamplingData[i]->channel == m_arraySamplingResult[j]->channel
				&& m_arraySamplingData[i]->con_name == m_arraySamplingResult[j]->con_name
				&& m_arraySamplingData[i]->liq_name == m_arraySamplingResult[j]->liq_name)
			{
				bFound = TRUE;
				break;
			}
		}
		//不存在结果数据，插入一条
		if(!bFound)
		{
			structSamplingResult* pSamplingResult	= new structSamplingResult;
			pSamplingResult->channel				= m_arraySamplingData[i]->channel;		//通道接口号
			pSamplingResult->con_name				= m_arraySamplingData[i]->con_name;		//容器名称
			pSamplingResult->liq_name				= m_arraySamplingData[i]->liq_name;		//液体名称
			pSamplingResult->CollectionTimes		= 0;									//采集次数（每次多个采样平均）
			pSamplingResult->MaxCount				= 0;									//最大计数
			pSamplingResult->MinCount				= 0x0fffffff;							//最小计数
			pSamplingResult->AverageCount			= 0;									//平均计数
			pSamplingResult->RangeCount				= 0;									//计数范围
			pSamplingResult->MaxValue				= 0;									//最大数值
			for(n=0; n<1024; n++)
			{
				pSamplingResult->AverageSamplingData[n]	= 0.0f;								//平均数据
			}
			m_arraySamplingResult.Add(pSamplingResult);
		}
	}

	//计算结果数据
	for(i=0; i<m_arraySamplingResult.GetSize(); i++)
	{
		m_nCurrentMaxCount			= 0;			//当前样本最大计数值
		m_nCurrentMinCount			= 0x0fffffff;	//当前样本最小计数值
		m_nCurrentAverageCount		= 0;			//当前样本平均计数值
		m_nCurrentSamplingNumber	= 0;			//当前样本采样次数
		m_nCurrentMaxValue			= 0;			//最大数值
		for(n=0; n<1024; n++)
		{
			m_fAverageSamplingData[n]	= 0.0f;
		}
		for(j=0; j<m_arraySamplingData.GetSize(); j++)
		{
			if(m_arraySamplingData[j]->channel == m_arraySamplingResult[i]->channel
				&& m_arraySamplingData[j]->con_name == m_arraySamplingResult[i]->con_name
				&& m_arraySamplingData[j]->liq_name == m_arraySamplingResult[i]->liq_name)
			{
				m_nCurrentSamplingNumber++;
				//计算各计数值
				if(m_nCurrentMaxCount<m_arraySamplingData[j]->ThresholdSamplingCount)
				{
					m_nCurrentMaxCount = m_arraySamplingData[j]->ThresholdSamplingCount;
				}
				if(m_nCurrentMinCount>m_arraySamplingData[j]->ThresholdSamplingCount)
				{
					m_nCurrentMinCount = m_arraySamplingData[j]->ThresholdSamplingCount;
				}
				m_nCurrentAverageCount += m_arraySamplingData[j]->ThresholdSamplingCount;
				if(m_nCurrentMaxValue<m_arraySamplingData[j]->MaxValue)
				{
					m_nCurrentMaxValue = m_arraySamplingData[j]->MaxValue;
				}
				for(int n=0; n<1024; n++)
				{
					m_fAverageSamplingData[n] += m_arraySamplingData[j]->SamplingData[n];
				}
			}
		}
		if(m_nCurrentSamplingNumber>1)
		{
			m_nCurrentAverageCount = UINT(float(m_nCurrentAverageCount)/m_nCurrentSamplingNumber+0.5);
		}
		//多样单次绘图平均值
		for(n=0; n<1024; n++)
		{
			m_fAverageSamplingData[n] = m_fAverageSamplingData[n]/m_nCurrentSamplingNumber;
		}

		m_arraySamplingResult[i]->CollectionTimes	= m_nCurrentSamplingNumber;					//采集次数（每次多个采样平均）
		m_arraySamplingResult[i]->MaxCount			= m_nCurrentMaxCount;						//最大计数
		m_arraySamplingResult[i]->MinCount			= m_nCurrentMinCount;						//最小计数
		m_arraySamplingResult[i]->AverageCount		= m_nCurrentAverageCount;					//平均计数
		m_arraySamplingResult[i]->RangeCount		= m_nCurrentMaxCount-m_nCurrentMinCount;	//计数范围
		m_arraySamplingResult[i]->MaxValue			= m_nCurrentMaxValue;						//最大数值
		for(int n=0; n<1024; n++)
		{
			m_arraySamplingResult[i]->AverageSamplingData[n] = m_fAverageSamplingData[n];		//平均数据
		}
	}
}

void CDialogParameter::InsertSamplingDataToArray()	//采样数据存入数组
{
//	UpdateData(TRUE);

	//插入采样详细数据
	structSamplingData* pSamplingData		= new structSamplingData;
	pSamplingData->channel					= m_nSelChannelInterface;		//通道接口号
	pSamplingData->con_name					= m_sContainer;					//容器名称
	pSamplingData->liq_name					= m_sLiquid;					//液体名称
//	pSamplingData->ThresholdBegin			= pApp->m_para_thresholdbegin;	//阈值起点
//	pSamplingData->ThresholdEnd				= pApp->m_para_thresholdend;	//阈值终点
	pSamplingData->ThresholdSamplingCount	= 0;							//采样阈内计数值
	pSamplingData->MaxValue					= 0;							//最大数值
	for(UINT i=0; i<1024; i++)
	{
		//拷贝全部采样数据
		pSamplingData->SamplingData[i]	= m_nTempSamplingData[i];
		//计算阈内计数值
		if(i>=(pApp->m_para_thresholdbegin-1) && i<pApp->m_para_thresholdend)
		{
			pSamplingData->ThresholdSamplingCount += m_nTempSamplingData[i];
		}
		//最大数值
		if(pSamplingData->MaxValue<pSamplingData->SamplingData[i])
		{
			pSamplingData->MaxValue=pSamplingData->SamplingData[i];
		}
	}
	m_arraySamplingData.Add(pSamplingData);
}

void CDialogParameter::LoadDetectValueFromSamplingDataArray()//循环查询采样数据，计算并显示采样结果
{
	m_nCurrentMaxCount			= 0;			//当前样本最大计数值
	m_nCurrentMinCount			= 0x0fffffff;	//当前样本最小计数值
	m_nCurrentAverageCount		= 0;			//当前样本平均计数值
	m_nCurrentSamplingNumber	= 0;			//当前样本采样次数
	m_nCurrentMaxValue			= 0;			//最大数值
	//用于显示结果列表
	CString strData;
	m_listDetectValue.DeleteAllItems();
	int nRow, i, j;
	for(j=0; j<1024; j++)
	{
		m_fAverageSamplingData[j]	= 0.0f;
	}
	for(i=0; i<m_arraySamplingData.GetSize(); i++)
	{
		if(m_arraySamplingData[i]->channel == m_nSelChannelInterface
			&& m_arraySamplingData[i]->con_name == m_sContainer
			&& m_arraySamplingData[i]->liq_name == m_sLiquid)
		{
			m_nCurrentSamplingNumber++;
			//插入结果列表
			strData.Format(_T("%d"), m_nCurrentSamplingNumber);
			nRow=m_listDetectValue.InsertItem(m_listDetectValue.GetItemCount(), strData);	//次数
			strData.Format(_T("%d"), m_arraySamplingData[i]->ThresholdSamplingCount);
			m_listDetectValue.SetItemText(nRow, 1, strData);							//阈内计数值
			//计算各计数值
			if(m_nCurrentMaxCount<m_arraySamplingData[i]->ThresholdSamplingCount)
			{
				m_nCurrentMaxCount = m_arraySamplingData[i]->ThresholdSamplingCount;
			}
			if(m_nCurrentMinCount>m_arraySamplingData[i]->ThresholdSamplingCount)
			{
				m_nCurrentMinCount = m_arraySamplingData[i]->ThresholdSamplingCount;
			}
			m_nCurrentAverageCount += m_arraySamplingData[i]->ThresholdSamplingCount;
			if(m_nCurrentMaxValue<m_arraySamplingData[i]->MaxValue)
			{
				m_nCurrentMaxValue = m_arraySamplingData[i]->MaxValue;
			}
			for(j=0; j<1024; j++)
			{
				m_fAverageSamplingData[j] += m_arraySamplingData[i]->SamplingData[j];
			}
		}
	}
	if(m_nCurrentSamplingNumber>1)
	{
		m_nCurrentAverageCount = UINT(float(m_nCurrentAverageCount)/m_nCurrentSamplingNumber+0.5);
	}
	//多样单次绘图平均值
	for(j=0; j<1024; j++)
	{
		m_fAverageSamplingData[j] = m_fAverageSamplingData[j]/m_nCurrentSamplingNumber;
	}
}
	
void CDialogParameter::UpdateSamplingResultToArray()//采样结果更新或新增入数组
{
	BOOL bFound(FALSE);
	for(int i=0; i<m_arraySamplingResult.GetSize(); i++)
	{
		if(m_arraySamplingResult[i]->channel == m_nSelChannelInterface
			&& m_arraySamplingResult[i]->con_name == m_sContainer
			&& m_arraySamplingResult[i]->liq_name == m_sLiquid)
		{
			bFound = TRUE;
			m_arraySamplingResult[i]->CollectionTimes	= m_nCurrentSamplingNumber;					//采集次数（每次多个采样平均）
			m_arraySamplingResult[i]->MaxCount			= m_nCurrentMaxCount;						//最大计数
			m_arraySamplingResult[i]->MinCount			= m_nCurrentMinCount;						//最小计数
			m_arraySamplingResult[i]->AverageCount		= m_nCurrentAverageCount;					//平均计数
			m_arraySamplingResult[i]->RangeCount		= m_nCurrentMaxCount-m_nCurrentMinCount;	//计数范围
			m_arraySamplingResult[i]->MaxValue			= m_nCurrentMaxValue;						//最大数值
			for(int j=0; j<1024; j++)
			{
				m_arraySamplingResult[i]->AverageSamplingData[j] = m_fAverageSamplingData[j];	//平均数据
			}
			break;
		}
	}
	if(!bFound)
	{
//		CString s;
//		s.Format(L"%d-%s-%s", m_nSelChannelInterface, m_sContainer, m_sLiquid);
//		AfxMessageBox(s);
		structSamplingResult* pSamplingResult	= new structSamplingResult;
		pSamplingResult->channel				= m_nSelChannelInterface;					//通道接口号
		pSamplingResult->con_name				= m_sContainer;								//容器名称
		pSamplingResult->liq_name				= m_sLiquid;								//液体名称
		pSamplingResult->CollectionTimes		= m_nCurrentSamplingNumber;					//采集次数（每次多个采样平均）
		pSamplingResult->MaxCount				= m_nCurrentMaxCount;						//最大计数
		pSamplingResult->MinCount				= m_nCurrentMinCount;						//最小计数
		pSamplingResult->AverageCount			= m_nCurrentAverageCount;					//平均计数
		pSamplingResult->RangeCount				= m_nCurrentMaxCount-m_nCurrentMinCount;	//计数范围
		pSamplingResult->MaxValue				= m_nCurrentMaxValue;						//最大数值
		for(int j=0; j<1024; j++)
		{
			pSamplingResult->AverageSamplingData[j]	= m_fAverageSamplingData[j];			//平均数据
		}
		m_arraySamplingResult.Add(pSamplingResult);
	}
}

void CDialogParameter::OnButtonManualDetect() 
{
	UpdateData(TRUE);
	DisableControls();
	m_sInfo		= _T("打开X射线机...");
	UpdateData(FALSE);

	BeginWaitCursor();
	if(!m_Serial.ControlXRadial(ControlCommandOpen))
	{
		m_sInfo	= _T("打开X射线机失败，请检查控制板或通讯线路！");
		UpdateData(FALSE);
 		EndWaitCursor();
		EnableControls();
		return;
	}

	Sleep(500);

	UINT i;
	for(i=0; i<1024; i++)
	{
		m_nTempSamplingData[i]=0;
	}
	CString sTemp,sMsg;
	for(i=0; i<pApp->m_para_samplingtimes+1; i++)
	{
		m_sInfo.Format(_T("“%s-%s”测量中（第%d次采样），请稍候..."), m_sContainer, m_sLiquid, i+1);
		UpdateData(FALSE);
		if(!m_Serial.ControlSampling(m_nSelChannelInterface))
		{
			m_sInfo = _T("测量失败，请检查控制板或通讯线路！");
			UpdateData(FALSE);
			m_Serial.ControlXRadial(ControlCommandClose);
 			pApp->m_dlgOK.m_sText = _T("测量中止，请检查控制板或通讯线路！");
			pApp->m_dlgOK.DoModal();
			EndWaitCursor();
			EnableControls();
			return;
		}
#ifdef SHOW_DEBUG_MESSAGE
		sTemp	= L"";
		sMsg.Format(L"收到的数%d：", i+1);
		for(int n=0; n<1024; n++)
		{
//			if(n%64==0)	sMsg += L"\r\n";
			sTemp.Format(L"%d,",m_Serial.m_ReceiveDataBuf[n]);
			sMsg += sTemp;
		}
		AfxMessageBox(sMsg);
#endif
		if(i>0)//第1次不取
		{
			for(int j=0; j<1024; j++)
			{
				m_nTempSamplingData[j] += m_Serial.m_ReceiveDataBuf[j];
			}
#ifdef SHOW_DEBUG_MESSAGE
			sTemp	= L"";
			sMsg.Format(L"绘制的数和%d：", i);
			for(int n=0; n<1024; n++)
			{
//				if(n%64==0)	sMsg += L"\r\n";
				sTemp.Format(L"%d,",m_nTempSamplingData[n]);
				sMsg += sTemp;
			}
			AfxMessageBox(sMsg);
#endif
		}
	}

	m_sInfo = _T("关闭X射线机...");
	UpdateData(FALSE);
	m_Serial.ControlXRadial(ControlCommandClose);

	m_sInfo.Format(L"“%s-%s”采样结果计算中，请稍候...", m_sContainer, m_sLiquid);
	UpdateData(FALSE);
	for(int j=0; j<1024; j++)
	{
		m_nTempSamplingData[j] = UINT(float(m_nTempSamplingData[j])/pApp->m_para_samplingtimes+0.5);
	}
#ifdef SHOW_DEBUG_MESSAGE
	sTemp	= L"";
	sMsg.Format(L"平均的数：");
	for(int n=0; n<1024; n++)
	{
//		if(n%64==0)	sMsg += L"\r\n";
		sTemp.Format(L"%d,",m_nTempSamplingData[n]);
		sMsg += sTemp;
	}
	AfxMessageBox(sMsg);
#endif

	/****以下三个函数注意使用顺序****/
	InsertSamplingDataToArray();				//插入采样数据
	LoadDetectValueFromSamplingDataArray();		//循环查询采样数据，计算并显示采样结果
	UpdateSamplingResultToArray();				//采样结果更新或新增入数组

	m_sInfo = _T("请将待标定样本放在选定通道处进行采样！");
	UpdateData(FALSE);
 	EndWaitCursor();
	EnableControls();	
	CRect rcVal;
	GetDlgItem(IDC_STATIC_DRAW)->GetWindowRect(&rcVal);
	InvalidateRect(&rcVal);
}

void CDialogParameter::OnButtonAutoDetect() 
{
	UpdateData(TRUE);
	if(!m_bReadValueTest)
	{
//		AfxMessageBox(_T("开始采样"));
		//开启自动测量线程
		SetDlgItemText(IDC_BUTTON_AUTO_DETECT,_T("停止采样"));
		DisableControls();
		GetDlgItem(IDC_BUTTON_AUTO_DETECT)->EnableWindow(TRUE);			//自动采样
		m_bReadValueTest	= TRUE;
		m_hReadThread = CreateThread(NULL,0,AutoReadPortThread,this,0, NULL);
	}
	else
	{
//		AfxMessageBox(_T("停止采样"));
		BeginWaitCursor();
		GetDlgItem(IDC_BUTTON_AUTO_DETECT)->EnableWindow(FALSE);			//自动采样
		m_bReadValueTest	= FALSE;
		m_bWaitThread		= TRUE;
		if(m_hReadThread)
		{
			Sleep(2000);
			if(::WaitForSingleObject(m_hReadThread, 1000)   ==   WAIT_TIMEOUT)   
			{   
//				AfxMessageBox(_T("关闭线程"));
				//如果超时：   
				m_sInfo	= _T("关闭X射线机...");
				MessageUpdateData(FALSE);
				m_Serial.ControlXRadial(ControlCommandClose);
				TerminateThread(m_hReadThread, NULL);
			}
			else
			{
				CloseHandle(m_hReadThread);
				m_hReadThread=NULL;
			}
		}
		m_bWaitThread	= FALSE;
		SetDlgItemText(IDC_BUTTON_AUTO_DETECT,_T("自动采样"));
		GetDlgItem(IDC_BUTTON_AUTO_DETECT)->EnableWindow(TRUE);			//自动采样
		m_sInfo = _T("请将待标定样本放在选定通道处进行采样！");
		EnableControls();
 		UpdateData(FALSE);
		EndWaitCursor();
	}
	CRect rcVal;
	GetDlgItem(IDC_STATIC_DRAW)->GetWindowRect(&rcVal);
	InvalidateRect(&rcVal);
}

//读数据线程
DWORD WINAPI CDialogParameter::AutoReadPortThread(LPVOID lpParameter)
{
	CDialogParameter *pDialog;
	pDialog=(CDialogParameter*)lpParameter;

	//开始采样最多20次
	UINT i,j;
	UINT nMaxReadTimes=20;
	for(i=pDialog->m_listDetectValue.GetItemCount(); i<nMaxReadTimes; i++)
	{
		pDialog->m_sInfo	= _T("打开X射线机...");
		pDialog->MessageUpdateData(FALSE);
		if(!pDialog->m_Serial.ControlXRadial(ControlCommandOpen))	//开X射线机
		{
			pDialog->m_sInfo	= _T("打开X射线机失败，请检查控制板或通讯线路！");
			pDialog->MessageUpdateData(FALSE);
			pDialog->pApp->m_dlgOK.m_sText = _T("打开X射线机失败，无法测试，请检查控制板或通讯线路！");
			pDialog->pApp->m_dlgOK.DoModal();
			pDialog->OnButtonAutoDetect();	//中断线程，恢复按钮状态
			return 0;
		}
		if(!pDialog->m_bReadValueTest)
		{
			pDialog->m_sInfo	= _T("关闭X射线机...");
			pDialog->MessageUpdateData(FALSE);
			pDialog->m_Serial.ControlXRadial(ControlCommandClose);
			pDialog->m_sInfo = _T("请将待标定样本放在选定通道处进行采样！");
			pDialog->MessageUpdateData(FALSE);
			return 0;
		}
		pDialog->m_sInfo.Format(_T("正在读取“%s-%s”第%d个数据..."), pDialog->m_sContainer, pDialog->m_sLiquid, i+1);
		pDialog->MessageUpdateData(FALSE);
		
		for(j=0; j<1024; j++)
		{
			pDialog->m_nTempSamplingData[j]=0;
		}
		for(j=0; j<pDialog->pApp->m_para_samplingtimes+1; j++)
		{
			if(!pDialog->m_bReadValueTest)
			{
				pDialog->m_sInfo	= _T("关闭X射线机...");
				pDialog->MessageUpdateData(FALSE);
				pDialog->m_Serial.ControlXRadial(ControlCommandClose);
				pDialog->m_sInfo = _T("请将待标定样本放在选定通道处进行采样！");
				pDialog->MessageUpdateData(FALSE);
				return 0;
			}
			Sleep(1000);
			pDialog->m_sInfo.Format(_T("“%s-%s”第%d个数据取样中（第%d次采样），请稍候..."), pDialog->m_sContainer, pDialog->m_sLiquid, i+1,j+1);
			pDialog->MessageUpdateData(FALSE);
			if(!pDialog->m_Serial.ControlSampling(pDialog->m_nSelChannelInterface))
			{
				pDialog->m_sInfo	= _T("关闭X射线机...");
				pDialog->MessageUpdateData(FALSE);
				Sleep(1000);
				pDialog->m_Serial.ControlXRadial(ControlCommandClose);
				pDialog->m_sInfo	= _T("测试失败，请检查控制板或通讯线路！");
				pDialog->pApp->m_dlgOK.m_sText = _T("测试中止，请检查控制板或通讯线路！");
				pDialog->pApp->m_dlgOK.DoModal();
				pDialog->OnButtonAutoDetect();	//中断线程，恢复按钮状态
				return 0;
			}
			if(j>0)//第1次不取
			{
				for(int k=0; k<1024; k++)
				{
					pDialog->m_nTempSamplingData[k] += pDialog->m_Serial.m_ReceiveDataBuf[k];
				}
			}
		}

		//按阈值区间取值并显示
		for(j=0; j<1024; j++)
		{
			pDialog->m_nTempSamplingData[j] = UINT(float(pDialog->m_nTempSamplingData[j])/pDialog->pApp->m_para_samplingtimes+0.5);
		}
		/****以下三个函数注意使用顺序****/
		pDialog->InsertSamplingDataToArray();				//插入采样数据
		pDialog->LoadDetectValueFromSamplingDataArray();	//循环查询采样数据，计算并显示采样结果
		pDialog->UpdateSamplingResultToArray();				//采样结果更新或新增入数组
		pDialog->MessageUpdateData(FALSE);
			
		pDialog->m_sInfo	= _T("关闭X射线机...");
		pDialog->MessageUpdateData(FALSE);
		if(!pDialog->m_Serial.ControlXRadial(ControlCommandClose))
		{
			pDialog->m_sInfo	= _T("关闭X射线机失败，请检查控制板或通讯线路！");
			pDialog->pApp->m_dlgOK.m_sText = _T("测试中止，请检查控制板或通讯线路！");
			pDialog->pApp->m_dlgOK.DoModal();
			pDialog->OnButtonAutoDetect();	//中断线程，恢复按钮状态
			return 0;
		}

		//采样间隔延时
		if(i<nMaxReadTimes-1)
		{
			for(UINT n=0; n<pDialog->m_nSecond; n++)
			{
				if(!pDialog->m_bReadValueTest)
				{
					pDialog->m_sInfo	= _T("关闭X射线机...");
					pDialog->MessageUpdateData(FALSE);
					pDialog->m_Serial.ControlXRadial(ControlCommandClose);
					pDialog->m_sInfo = _T("请将待标定样本放在选定通道处进行采样！");
					pDialog->MessageUpdateData(FALSE);
					return 0;
				}
				pDialog->m_sInfo.Format(_T("“%s-%s”采样间隔延时%d秒，请稍候..."), pDialog->m_sContainer, pDialog->m_sLiquid, pDialog->m_nSecond-n);
				pDialog->MessageUpdateData(FALSE);
				Sleep(1000);
			}
		}
	}

	pDialog->SetDlgItemText(IDC_BUTTON_AUTO_DETECT,_T("自动采样"));
	pDialog->GetDlgItem(IDC_BUTTON_AUTO_DETECT)->EnableWindow(TRUE);			//自动采样
	pDialog->m_sInfo = _T("请将待标定样本放在选定通道处进行采样！");
	pDialog->m_bReadValueTest	= FALSE;
	pDialog->EnableControls();
	pDialog->UpdateData(FALSE);
	pDialog->OnPaint();
	return 0;
}

void CDialogParameter::OnSelchangeComboContainer() 
{
	UpdateData(TRUE);
	LoadSwatchFromArray();
	LoadDetectValueFromSamplingDataArray();
	UpdateData(FALSE);
}

void CDialogParameter::OnSelchangeComboLiquid() 
{
	UpdateData(TRUE);
	LoadDetectValueFromSamplingDataArray();
	UpdateData(FALSE);
}

void CDialogParameter::OnOK()
{
	if(m_bReadValueTest) return;

	m_Serial.Deactivate();
	m_Serial.ClosePort();
	CDialog::OnOK();
}

//发送UpdateData消息，避免线程UpdateData时出错
void CDialogParameter::MessageUpdateData(BOOL bUpdateData)
{
	m_bPaint		= TRUE;
	m_bUpdateData	= bUpdateData;
	SendMessage(WM_PAINT,0,0);
}

void CDialogParameter::OnCancel() 
{
	m_Serial.Deactivate();
	m_Serial.ClosePort();

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

void CDialogParameter::OnClickListDetectValue(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UpdateData(TRUE);

	POSITION pos = m_listDetectValue.GetFirstSelectedItemPosition();
	int nIndex = m_listDetectValue.GetNextSelectedItem(pos);  // 得到项目索引
	
	if(nIndex<0)
	{
		GetDlgItem(IDC_BUTTON_DELETE_DETECT_VALUE)->EnableWindow(FALSE);
		m_nDrawIndexSS	= 0;
		return;
	}
	GetDlgItem(IDC_BUTTON_DELETE_DETECT_VALUE)->EnableWindow(TRUE);

	m_nDrawIndexSS	= _ttoi(m_listDetectValue.GetItemText(nIndex, 0));
	
	UpdateData(FALSE);
	
	if(m_bDrawSS)
	{
		CRect rcVal;
		GetDlgItem(IDC_STATIC_DRAW)->GetWindowRect(&rcVal);
		InvalidateRect(&rcVal);
	}
	*pResult = 0;
}

void CDialogParameter::OnButtonDeleteDetectValue() 
{
	POSITION pos = m_listDetectValue.GetFirstSelectedItemPosition();
	int nIndex = m_listDetectValue.GetNextSelectedItem(pos);  // 得到项目索引
	
	if(nIndex<0)
	{
		GetDlgItem(IDC_BUTTON_DELETE_DETECT_VALUE)->EnableWindow(FALSE);
		pApp->m_dlgOK.m_sText = _T("请选择删除项目！");
		pApp->m_dlgOK.DoModal();
		return;
	}

	CString	s;
	s.Format(_T("确定删除第%s次采样结果值？"), m_listDetectValue.GetItemText(nIndex, 0));
	pApp->m_dlgOKCANCEL.m_sText = s;
	if(pApp->m_dlgOKCANCEL.DoModal()==IDCANCEL) return;

	int n(0);
	for(int i=0; i<m_arraySamplingData.GetSize(); i++)
	{
		if(m_arraySamplingData[i]->channel == m_nSelChannelInterface
			&& m_arraySamplingData[i]->con_name == m_sContainer
			&& m_arraySamplingData[i]->liq_name == m_sLiquid)
		{
			if(nIndex==n)
			{
				m_arraySamplingData.RemoveAt(i);
				m_nDrawIndexSS	= 0;
				break;
			}
			else
				n++;

		}
	}
	LoadDetectValueFromSamplingDataArray();		//循环查询采样数据，计算并显示采样结果
	UpdateSamplingResultToArray();				//采样结果更新或新增入数组
	UpdateData(FALSE);
}

void CDialogParameter::OnButtonDeleteallDetectValue() 
{
	pApp->m_dlgOKCANCEL.m_sText = _T("确定删除所有采样结果值？");
	if(pApp->m_dlgOKCANCEL.DoModal()==IDCANCEL) return;

	for(int i=m_arraySamplingData.GetSize()-1; i>=0; i--)
	{
		if(m_arraySamplingData[i]->channel == m_nSelChannelInterface
			&& m_arraySamplingData[i]->con_name == m_sContainer
			&& m_arraySamplingData[i]->liq_name == m_sLiquid)
		{
			m_arraySamplingData.RemoveAt(i);
		}
	}
	m_nDrawIndexSS	= 0;
	LoadDetectValueFromSamplingDataArray();		//循环查询采样数据，计算并显示采样结果
	UpdateSamplingResultToArray();				//采样结果更新或新增入数组
	UpdateData(FALSE);
}

void CDialogParameter::OnButtonUpdateSwatch() 
{
	pApp->m_dlgOKCANCEL.m_sText = _T("此操作将更新所有样本标定数据，是否继续？");
	if(pApp->m_dlgOKCANCEL.DoModal()==IDCANCEL) return;

	UpdateData(TRUE);
	BOOL bFound(FALSE);
//	CString s;
//	s.Format(L"%d", m_arraySamplingResult.GetSize());
//	AfxMessageBox(s);
	CppSQLite3DB db;
	try
	{
		db.open(pApp->m_strDbPath);		//打开
	}
	catch(CppSQLite3Exception ex)
	{
		pApp->m_dlgOK.m_sText = ex.errorMessage();
		pApp->m_dlgOK.DoModal();
		pApp->m_dlgOK.m_sText = _T("数据文件打开失败，样本数据更新失败！");
		pApp->m_dlgOK.DoModal();
		return;
	}
	CString sSQL;
	int count;
	for(int i=0; i<m_arraySamplingResult.GetSize(); i++)
	{
		bFound = FALSE;
		try
		{
//			db.open(pApp->m_strDbPath);		//打开
			sSQL.Format(_T("select count(*) from swatch where channel=%d and con_name='%s' and liq_name='%s'"),
						m_arraySamplingResult[i]->channel,
						m_arraySamplingResult[i]->con_name,
						m_arraySamplingResult[i]->liq_name);
//			AfxMessageBox(sSQL);
			count = db.execScalar(sSQL);
			if(count>=1)
			{
				bFound = TRUE;
			}
			if(bFound)
			{
				//更新数据库
				sSQL.Format(_T("update swatch set countup=%d, countdown=%d, countaverage=%d, countrange=%d where channel=%d and con_name='%s' and liq_name='%s'"),
					m_arraySamplingResult[i]->MaxCount,
					m_arraySamplingResult[i]->MinCount,
					m_arraySamplingResult[i]->AverageCount,
					m_arraySamplingResult[i]->RangeCount,
					m_arraySamplingResult[i]->channel,
					m_arraySamplingResult[i]->con_name,
					m_arraySamplingResult[i]->liq_name);
//				AfxMessageBox(sSQL);
				db.execDML(sSQL);
			}
			else
			{
				CString sIsSafe=L"0";
				//插入数据库
				for(int j=0; j<pApp->m_arrayLiquid.GetSize(); j++)
				{
					if(pApp->m_arrayLiquid[j]->liq_name==m_sLiquid)
					{
						sIsSafe = pApp->m_arrayLiquid[j]->issafe;
					}
				}
				sSQL.Format(_T("insert into swatch (channel, liq_name, con_name, countup, countdown, countaverage, countrange, issafe) values(%d, '%s', '%s', %d, %d, %d, %d, %s)"),
					m_arraySamplingResult[i]->channel,
					m_arraySamplingResult[i]->liq_name,
					m_arraySamplingResult[i]->con_name,
					m_arraySamplingResult[i]->MaxCount,
					m_arraySamplingResult[i]->MinCount,
					m_arraySamplingResult[i]->AverageCount,
					m_arraySamplingResult[i]->RangeCount,
					sIsSafe);
//				AfxMessageBox(sSQL);
				db.execDML(sSQL);
			}
		}
		catch(CppSQLite3Exception ex)
		{
			pApp->m_dlgOK.m_sText = ex.errorMessage();
			pApp->m_dlgOK.DoModal();
			pApp->m_dlgOK.m_sText = _T("样本数据更新失败！");
			pApp->m_dlgOK.DoModal();
			return;
		}
	}
	db.close();
	pApp->LoadDataFromSwatch();
	//重新载入Swatch
	LoadSwatchFromArray();
}

void CDialogParameter::OnClickListSwatch(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UpdateData(TRUE);

	POSITION pos = m_listSwatch.GetFirstSelectedItemPosition();
	int nIndex = m_listSwatch.GetNextSelectedItem(pos);  // 得到项目索引
	
	if(nIndex<0)
	{
		GetDlgItem(IDC_BUTTON_SAVE_SWATCH_MODIFY)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_DELETE_SWATCH)->EnableWindow(FALSE);

		((CEdit*)GetDlgItem(IDC_EDIT_COUNTUP))->SetReadOnly(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_COUNTDOWN))->SetReadOnly(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_COUNTAVERAGE))->SetReadOnly(TRUE);
		return;
	}
	m_iCountUp		= _ttoi(m_listSwatch.GetItemText(nIndex, 3));
	m_iCountDown	= _ttoi(m_listSwatch.GetItemText(nIndex, 4));
	m_iCountAverage	= _ttoi(m_listSwatch.GetItemText(nIndex, 5));
	GetDlgItem(IDC_BUTTON_SAVE_SWATCH_MODIFY)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_DELETE_SWATCH)->EnableWindow(TRUE);

	((CEdit*)GetDlgItem(IDC_EDIT_COUNTUP))->SetReadOnly(FALSE);
	((CEdit*)GetDlgItem(IDC_EDIT_COUNTDOWN))->SetReadOnly(FALSE);
	((CEdit*)GetDlgItem(IDC_EDIT_COUNTAVERAGE))->SetReadOnly(FALSE);
	m_bKeyboardEnable	= TRUE;	//允许显示键盘

	UpdateData(FALSE);	
	*pResult = 0;
}

void CDialogParameter::OnButtonDeleteSwatch()
{
	UpdateData(TRUE);

	POSITION pos = m_listSwatch.GetFirstSelectedItemPosition();
	int nIndex = m_listSwatch.GetNextSelectedItem(pos);  // 得到项目索引
	
	if(nIndex<0)
	{
		GetDlgItem(IDC_BUTTON_SAVE_SWATCH_MODIFY)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_DELETE_SWATCH)->EnableWindow(FALSE);

		((CEdit*)GetDlgItem(IDC_EDIT_COUNTUP))->SetReadOnly(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_COUNTDOWN))->SetReadOnly(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_COUNTAVERAGE))->SetReadOnly(TRUE);
		pApp->m_dlgOK.m_sText = _T("请选择需要删除的样本厂家标定值数据！");
		pApp->m_dlgOK.DoModal();
		return;
	}
	
	pApp->m_dlgOKCANCEL.m_sText = _T("确定删除该条样本厂家标定值数据？");
	if(pApp->m_dlgOKCANCEL.DoModal()==IDCANCEL) return;

	CString sChannel, sCon_name, sLiq_name;
	sChannel	= m_listSwatch.GetItemText(nIndex, 0);
	sCon_name	= m_listSwatch.GetItemText(nIndex, 1);
	sLiq_name	= m_listSwatch.GetItemText(nIndex, 2);
	CppSQLite3DB db;
	try
	{
		db.open(pApp->m_strDbPath);		//打开或新建一个数据库
		CString sSQL;
		db.execDML(L"begin transaction;");
		sSQL.Format(_T("delete from swatch where channel=%s and con_name='%s' and liq_name='%s'"),sChannel,sCon_name,sLiq_name);
		db.execDML(sSQL);
		db.execDML(L"commit transaction;");
	}
	catch(CppSQLite3Exception ex)
	{
		pApp->m_dlgOK.m_sText = ex.errorMessage();
		pApp->m_dlgOK.DoModal();
		pApp->m_dlgOK.m_sText = _T("样本厂家标定值数据删除失败！");
		pApp->m_dlgOK.DoModal();
		return;
	}
	db.close();

	for(int i=0; i<pApp->m_arraySwatch.GetSize(); i++)
	{
		if(pApp->m_arraySwatch[i]->channel == sChannel
			&& pApp->m_arraySwatch[i]->con_name == sCon_name
			&& pApp->m_arraySwatch[i]->liq_name == sLiq_name)
		{
			pApp->m_arraySwatch.RemoveAt(i);
			break;
		}
	}
	//重新载入Swatch
	LoadSwatchFromArray();

//	pApp->m_dlgOK.m_sText = _T("样本厂家标定值数据删除成功！");
//	pApp->m_dlgOK.DoModal();
}

void CDialogParameter::OnButtonSaveSwatchModify() 
{
	UpdateData(TRUE);
	if(m_iCountUp<m_iCountDown)
	{
		pApp->m_dlgOK.m_sText = _T("最大计数不能小于最小计数！");
		pApp->m_dlgOK.DoModal();
		return;
	}
	if(m_iCountUp<=0 || m_iCountDown<=0 || m_iCountAverage<=0)
	{
		pApp->m_dlgOK.m_sText = _T("计数值应大于零！");
		pApp->m_dlgOK.DoModal();
		return;
	}

	UINT iCountRange=m_iCountUp-m_iCountDown;

	POSITION pos = m_listSwatch.GetFirstSelectedItemPosition();
	int nIndex = m_listSwatch.GetNextSelectedItem(pos);  // 得到项目索引
	
	if(nIndex<0)
	{
		GetDlgItem(IDC_BUTTON_SAVE_SWATCH_MODIFY)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_DELETE_SWATCH)->EnableWindow(FALSE);

		((CEdit*)GetDlgItem(IDC_EDIT_COUNTUP))->SetReadOnly(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_COUNTDOWN))->SetReadOnly(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_COUNTAVERAGE))->SetReadOnly(TRUE);
		return;
	}
	CString sChannel, sCon_name, sLiq_name;
	sChannel	= m_listSwatch.GetItemText(nIndex, 0);
	sCon_name	= m_listSwatch.GetItemText(nIndex, 1);
	sLiq_name	= m_listSwatch.GetItemText(nIndex, 2);
	CppSQLite3DB db;
	try
	{
		db.open(pApp->m_strDbPath);		//打开
		CString sSQL;
		db.execDML(L"begin transaction;");
		sSQL.Format(_T("update swatch set countup=%d, countdown=%d, countaverage=%d, countrange=%d where channel=%s and con_name='%s' and liq_name='%s'"), m_iCountUp, m_iCountDown, m_iCountAverage, iCountRange, sChannel, sCon_name, sLiq_name);
		db.execDML(sSQL);
		db.execDML(L"commit transaction;");
	}
	catch(CppSQLite3Exception ex)
	{
		pApp->m_dlgOK.m_sText = ex.errorMessage();
		pApp->m_dlgOK.DoModal();
		pApp->m_dlgOK.m_sText = _T("样本厂家标定值数据更新失败！");
		pApp->m_dlgOK.DoModal();
		return;
	}
	db.close();

	for(int i=0; i<pApp->m_arraySwatch.GetSize(); i++)
	{
		if(pApp->m_arraySwatch[i]->channel == sChannel
			&& pApp->m_arraySwatch[i]->con_name == sCon_name
			&& pApp->m_arraySwatch[i]->liq_name == sLiq_name)
		{
			CString strData;
			strData.Format(L"%d", m_iCountUp);
			pApp->m_arraySwatch[i]->countup			= strData;
			strData.Format(L"%d", m_iCountDown);
			pApp->m_arraySwatch[i]->countdown		= strData;
			strData.Format(L"%d", m_iCountAverage);
			pApp->m_arraySwatch[i]->countaverage	= strData;
			strData.Format(L"%d", iCountRange);
			pApp->m_arraySwatch[i]->countrange		= strData;
			break;
		}
	}
	//重新载入Swatch
	LoadSwatchFromArray();

//	pApp->m_dlgOK.m_sText = _T("样本厂家标定值数据更新成功！");
//	pApp->m_dlgOK.DoModal();
}

void CDialogParameter::OnButtonSetDisplay() 
{
	CDialogSetDisplay dlg;
	dlg.DoModal();
}

void CDialogParameter::OnButtonOtherParameter() 
{
	CDialogOtherParameter dlg;
	dlg.DoModal();

	if(m_bChannel1)
	{
		m_nSelChannelInterface = pApp->m_para_channelinterface1;
	}
	else
	{
		m_nSelChannelInterface = pApp->m_para_channelinterface2;
	}
	LoadSwatchFromArray();
	if(dlg.m_bThresholdModified)
	{
		RecountSamplingDataArray();
		LoadDetectValueFromSamplingDataArray();
	}
}

void CDialogParameter::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	if(m_bPaint)
	{
		UpdateData(m_bUpdateData);
		m_bPaint = FALSE;
	}

	if(m_listDetectValue.GetItemCount()>0 && !m_bReadValueTest && !m_bWaitThread)
	{
		GetDlgItem(IDC_BUTTON_UPDATE_SWATCH)->EnableWindow(TRUE);			//更新样本
		GetDlgItem(IDC_BUTTON_DELETEALL_DETECT_VALUE)->EnableWindow(TRUE);	//删除全部采样值
	}
	else
	{
		GetDlgItem(IDC_BUTTON_UPDATE_SWATCH)->EnableWindow(FALSE);			//更新样本
		GetDlgItem(IDC_BUTTON_DELETEALL_DETECT_VALUE)->EnableWindow(FALSE);	//删除全部采样值
	}

	if(m_arraySamplingData.GetSize()>0 && !m_bReadValueTest && !m_bWaitThread)
	{
		LoadDetectValueFromSamplingDataArray();
		GetDlgItem(IDC_BUTTON_SAVE_SAMPLINGDATA)->EnableWindow(TRUE);		//保存样本
	}
	else
	{
		GetDlgItem(IDC_BUTTON_SAVE_SAMPLINGDATA)->EnableWindow(FALSE);		//保存样本
	}
	
	if(m_bDrawSS)	//单样本单次显示
	{
		DrawImageSS();
	}
	else if(m_bDrawSM)	//单样本多次显示
	{
		DrawImageSM();
	}
	else
	{
		DrawImageMS();
	}
}

void CDialogParameter::DrawImageSS()
{
/****绘制基本表格****/
	CDC *pDC;
	CRect DrawRect;
	GetDlgItem(IDC_STATIC_DRAW)->GetWindowRect(&DrawRect);
	ScreenToClient(&DrawRect);
	pDC=GetDC();
	CFont font, *pOldFont;
	CPen penScale,penThreshold,penGridding,penCurve,*pOldPen;

	// 对显示区域的大小进行归整
	int m_GridV=DrawRect.Height()/20;
	DrawRect.bottom=DrawRect.top+DrawRect.Height()/20*20;
	int m_GridH=DrawRect.Width()/(22*5);
	DrawRect.right=DrawRect.left+DrawRect.Width()/(22*5)*(22*5);

	// 比例
	int m_Ratio;
	m_Ratio	= DrawRect.Height();
	// 每个数据所需要的像素
	double m_PixelValue;
	m_PixelValue	= DrawRect.Width()/(1.0*22*5*10);

	// 创建点阵字体
	font.CreatePointFont(90,_T("宋体"),pDC);
	// 创建画笔
	penScale.CreatePen (PS_SOLID,1,COLOR_RED);				//刻度
	penThreshold.CreatePen (PS_SOLID,3,COLOR_YELLOW);		//道阈
	penGridding.CreatePen (PS_DASH,1,RGB(0,128,64));		//网格
	//将字体选入
	pOldFont=pDC->SelectObject(&font);
	//将画笔选入
	pOldPen=pDC->SelectObject (&penScale);
	//设置位图背景模式为不透明  
	pDC->SetBkMode(TRANSPARENT);
	//设置位图背景颜色为白色  
	pDC->SetBkColor(COLOR_WHITE);
	//填充位图 
	pDC->FillSolidRect(DrawRect.left,DrawRect.top,DrawRect.Width(),DrawRect.Height(),COLOR_WHITE); 
	//设置文字显示颜色  
	pDC->SetTextColor(COLOR_BLACK);

	// 绘制网格
	pOldPen=pDC->SelectObject (&penGridding);	//将画笔选入
	CString Str;
	int	i;
	for(i=0; i< DrawRect.Height(); i+=m_GridV)  // 纵向
	{
		pDC->MoveTo(DrawRect.left+0  , DrawRect.top+i);
		pDC->LineTo(DrawRect.right   , DrawRect.top+i);
	}
	for(i=0; i< DrawRect.Width(); i+=m_GridH*5)  // 横向
	{
		pDC->MoveTo(DrawRect.left + i  , DrawRect.top+0);
		pDC->LineTo(DrawRect.left + i  , DrawRect.top+DrawRect.Height());
	}

	// 绘制多道阈线
	pOldPen=pDC->SelectObject (&penThreshold);	//将画笔选入
	pDC->MoveTo(int(DrawRect.left+pApp->m_para_thresholdbegin*m_PixelValue), DrawRect.top);
	pDC->LineTo(int(DrawRect.left+pApp->m_para_thresholdbegin*m_PixelValue), DrawRect.bottom);
	pDC->MoveTo(int(DrawRect.left+pApp->m_para_thresholdend*m_PixelValue), DrawRect.top);
	pDC->LineTo(int(DrawRect.left+pApp->m_para_thresholdend*m_PixelValue), DrawRect.bottom);
	
	// 绘制刻度及字符
	pOldPen=pDC->SelectObject (&penScale);	//将画笔选入
	// 横轴
	pDC->MoveTo(DrawRect.left , DrawRect.bottom);
	pDC->LineTo(DrawRect.right, DrawRect.bottom);
	// 纵轴
	pDC->MoveTo(DrawRect.left, DrawRect.top);
	pDC->LineTo(DrawRect.left, DrawRect.bottom);
	//填充标刻位置 
	pDC->FillSolidRect(DrawRect.left,DrawRect.bottom+2,DrawRect.Width(),15,RGB(236,233,216)); 
	for(i=0; i< DrawRect.Width(); i+=m_GridH)  // 横轴及刻度
	{
		pDC->SetTextColor(COLOR_RED);

		// 10刻度标记
		pDC->MoveTo(DrawRect.left+i,DrawRect.top+DrawRect.Height());
		pDC->LineTo(DrawRect.left+i,DrawRect.top+(DrawRect.Height()-5));
		

		if(i==0 || i%(10*m_GridH))
			continue;

		// 100刻度标记
		if(!(i%(10*m_GridH)))
		{
			pDC->MoveTo(DrawRect.left+i,DrawRect.top+DrawRect.Height());
			pDC->LineTo(DrawRect.left+i,DrawRect.top+(DrawRect.Height()-10));
		}

		Str.Format(_T("%d"),i/m_GridH*10);
		pDC->ExtTextOut(DrawRect.left+i, DrawRect.bottom, NULL, DrawRect, Str, NULL);
	}
	pDC->SetTextColor(COLOR_BLACK);
	// 填充标刻位置 
	for(i=DrawRect.Height(); i>=0 ; i-=m_GridV)  // 纵轴刻度
	{
		if(i%2)	continue;

		// 0.1 刻度
		pDC->MoveTo(DrawRect.left+0 ,DrawRect.top+i);
		pDC->LineTo(DrawRect.left+10,DrawRect.top+i);

		// 刻度
		Str.Format(_T("%.1lf"),(DrawRect.Height()-i)/m_GridV*0.05);
		pDC->SetTextColor(RGB(255,0,0));
		pDC->ExtTextOut(DrawRect.left-20,i+5,NULL, DrawRect, Str, NULL);
	}
	pDC->SetTextColor(COLOR_BLACK);
	
/****绘制数据曲线****/
	BOOL	bFoundData(FALSE), bDrawData(FALSE);
	for(i=0; i<m_arraySamplingResult.GetSize(); i++)
	{
		if(m_arraySamplingResult[i]->channel == m_nSelChannelInterface
			&& m_arraySamplingResult[i]->con_name == m_sContainer
			&& m_arraySamplingResult[i]->liq_name == m_sLiquid)
		{
			if(m_arraySamplingResult[i]->CollectionTimes>0)
			{
				bFoundData			= TRUE;
				if(m_nDrawIndexSS>m_arraySamplingResult[i]->CollectionTimes)
				{
					m_nDrawIndexSS	= 0;	//为零则取最后一条数据
				}
				break;
			}
		}
	}
	//取数并归一化
	if(bFoundData)
	{
		if(m_nDrawIndexSS>0)
		{	
			UINT n(1);
			//取序号所指数据结果（从小到大查找）
			for(int nDrawIndex=0; nDrawIndex<m_arraySamplingData.GetSize(); nDrawIndex++)
			{
				if(m_arraySamplingData[nDrawIndex]->channel == m_nSelChannelInterface
					&& m_arraySamplingData[nDrawIndex]->con_name == m_sContainer
					&& m_arraySamplingData[nDrawIndex]->liq_name == m_sLiquid)
				{
					if(m_nDrawIndexSS==n)	//范围：1-20条
					{
						for(i=0; i<1024; i++)
						{
							if(m_arraySamplingData[nDrawIndex]->MaxValue>0)
							{
								m_fDrawData[i] = float(m_arraySamplingData[nDrawIndex]->SamplingData[i])/m_arraySamplingData[nDrawIndex]->MaxValue;
							}
							else
							{
								m_fDrawData[i] = 0.0f;
							}
						}
						bDrawData	= TRUE;
						break;
					}
					else
					{
						n++;
					}
				}
			}
		}
		else//取最后一次的结果数据（从大到小查找）
		{
			for(int nDrawIndex=m_arraySamplingData.GetSize()-1; nDrawIndex>=0; nDrawIndex--)
			{
				if(m_arraySamplingData[nDrawIndex]->channel == m_nSelChannelInterface
					&& m_arraySamplingData[nDrawIndex]->con_name == m_sContainer
					&& m_arraySamplingData[nDrawIndex]->liq_name == m_sLiquid)
				{
					for(i=0; i<1024; i++)
					{
						if(m_arraySamplingData[nDrawIndex]->MaxValue>0)
						{
							m_fDrawData[i] = float(m_arraySamplingData[nDrawIndex]->SamplingData[i])/m_arraySamplingData[nDrawIndex]->MaxValue;
						}
						else
						{
							m_fDrawData[i] = 0.0f;
						}
					}
					bDrawData	= TRUE;
					break;
				}
			}
		}
	}

	//取color
	COLORREF m_Color;
	m_Color	= COLOR_RED;
	for(i=0; i<pApp->m_arrayLiquid.GetSize(); i++)
	{
		if(pApp->m_arrayLiquid[i]->liq_name==m_sLiquid)
		{
			m_Color	= _ttoi(pApp->m_arrayLiquid[i]->showcolor);
			break;
		}
	}
	penCurve.CreatePen (PS_SOLID,1,m_Color);	//曲线画笔
	pOldPen=pDC->SelectObject (&penCurve);		//将画笔选入
	//绘制数据曲线（单条曲线）
	if(bDrawData)
	{
		pDC->MoveTo(DrawRect.left+0, DrawRect.top+(int)(DrawRect.Height()-m_fDrawData[0]*m_Ratio));
		// 逐个像素进行绘制
		for(i=1; i<1024; i++)
		{
			pDC->LineTo(int(DrawRect.left+i*m_PixelValue) , DrawRect.top+(int)(DrawRect.Height()-m_fDrawData[i]*m_Ratio));
		}
	}
	
/****恢复环境****/
	pDC->SelectObject (pOldPen);	// 选择旧画笔

	font.DeleteObject();
	penScale.DeleteObject();
	penThreshold.DeleteObject();
	penGridding.DeleteObject();
	penCurve.DeleteObject();

	//返回操作结果
	return;
}

//单样本多次
void CDialogParameter::DrawImageSM()
{
//	AfxMessageBox(L"SM");
/****绘制基本表格****/
	CDC *pDC;
	CRect DrawRect;
	GetDlgItem(IDC_STATIC_DRAW)->GetWindowRect(&DrawRect);
	ScreenToClient(&DrawRect);
	pDC=GetDC();
	CFont font, *pOldFont;
	CPen penScale,penThreshold,penGridding,penCurve,*pOldPen;

	// 对显示区域的大小进行归整
	int m_GridV=DrawRect.Height()/20;
	DrawRect.bottom=DrawRect.top+DrawRect.Height()/20*20;
	int m_GridH=DrawRect.Width()/(22*5);
	DrawRect.right=DrawRect.left+DrawRect.Width()/(22*5)*(22*5);

	// 比例
	int m_Ratio;
	m_Ratio	= DrawRect.Height();
	// 每个数据所需要的像素
	double m_PixelValue;
	m_PixelValue	= DrawRect.Width()/(1.0*22*5*10);

	// 创建点阵字体
	font.CreatePointFont(90,_T("宋体"),pDC);
	// 创建画笔
	penScale.CreatePen (PS_SOLID,1,COLOR_RED);				//刻度
	penThreshold.CreatePen (PS_SOLID,3,COLOR_YELLOW);		//道阈
	penGridding.CreatePen (PS_DASH,1,RGB(0,128,64));		//网格
	//将字体选入
	pOldFont=pDC->SelectObject(&font);
	//将画笔选入
	pOldPen=pDC->SelectObject (&penScale);
	//设置位图背景模式为不透明  
	pDC->SetBkMode(TRANSPARENT);
	//设置位图背景颜色为白色  
	pDC->SetBkColor(COLOR_WHITE);
	//填充位图 
	pDC->FillSolidRect(DrawRect.left,DrawRect.top,DrawRect.Width(),DrawRect.Height(),COLOR_WHITE); 
	//设置文字显示颜色  
	pDC->SetTextColor(COLOR_BLACK);

	// 绘制网格
	pOldPen=pDC->SelectObject (&penGridding);	//将画笔选入
	CString Str;
	int	i;
	for(i=0; i< DrawRect.Height(); i+=m_GridV)  // 纵向
	{
		pDC->MoveTo(DrawRect.left+0  , DrawRect.top+i);
		pDC->LineTo(DrawRect.right   , DrawRect.top+i);
	}
	for(i=0; i< DrawRect.Width(); i+=m_GridH*5)  // 横向
	{
		pDC->MoveTo(DrawRect.left + i  , DrawRect.top+0);
		pDC->LineTo(DrawRect.left + i  , DrawRect.top+DrawRect.Height());
	}

	// 绘制多道阈线
	pOldPen=pDC->SelectObject (&penThreshold);	//将画笔选入
	pDC->MoveTo(int(DrawRect.left+pApp->m_para_thresholdbegin*m_PixelValue), DrawRect.top);
	pDC->LineTo(int(DrawRect.left+pApp->m_para_thresholdbegin*m_PixelValue), DrawRect.bottom);
	pDC->MoveTo(int(DrawRect.left+pApp->m_para_thresholdend*m_PixelValue), DrawRect.top);
	pDC->LineTo(int(DrawRect.left+pApp->m_para_thresholdend*m_PixelValue), DrawRect.bottom);
	
	// 绘制刻度及字符
	pOldPen=pDC->SelectObject (&penScale);	//将画笔选入
	// 横轴
	pDC->MoveTo(DrawRect.left , DrawRect.bottom);
	pDC->LineTo(DrawRect.right, DrawRect.bottom);
	// 纵轴
	pDC->MoveTo(DrawRect.left, DrawRect.top);
	pDC->LineTo(DrawRect.left, DrawRect.bottom);
	//填充标刻位置 
	pDC->FillSolidRect(DrawRect.left,DrawRect.bottom+2,DrawRect.Width(),15,RGB(236,233,216)); 
	for(i=0; i< DrawRect.Width(); i+=m_GridH)  // 横轴及刻度
	{
		pDC->SetTextColor(COLOR_RED);

		// 10刻度标记
		pDC->MoveTo(DrawRect.left+i,DrawRect.top+DrawRect.Height());
		pDC->LineTo(DrawRect.left+i,DrawRect.top+(DrawRect.Height()-5));
		

		if(i==0 || i%(10*m_GridH))
			continue;

		// 100刻度标记
		if(!(i%(10*m_GridH)))
		{
			pDC->MoveTo(DrawRect.left+i,DrawRect.top+DrawRect.Height());
			pDC->LineTo(DrawRect.left+i,DrawRect.top+(DrawRect.Height()-10));
		}

		Str.Format(_T("%d"),i/m_GridH*10);
		pDC->ExtTextOut(DrawRect.left+i, DrawRect.bottom, NULL, DrawRect, Str, NULL);
	}
	pDC->SetTextColor(COLOR_BLACK);
	// 填充标刻位置 
	for(i=DrawRect.Height(); i>=0 ; i-=m_GridV)  // 纵轴刻度
	{
		if(i%2)	continue;

		// 0.1 刻度
		pDC->MoveTo(DrawRect.left+0 ,DrawRect.top+i);
		pDC->LineTo(DrawRect.left+10,DrawRect.top+i);

		// 刻度
		Str.Format(_T("%.1lf"),(DrawRect.Height()-i)/m_GridV*0.05);
		pDC->SetTextColor(RGB(255,0,0));
		pDC->ExtTextOut(DrawRect.left-20,i+5,NULL, DrawRect, Str, NULL);
	}
	pDC->SetTextColor(COLOR_BLACK);
	
/****绘制数据曲线****/
	BOOL	bFoundData(FALSE), bDrawData(FALSE);
	UINT	nMaxValue;	//最大数值，用于归一化
	for(i=0; i<m_arraySamplingResult.GetSize(); i++)
	{
		if(m_arraySamplingResult[i]->channel == m_nSelChannelInterface
			&& m_arraySamplingResult[i]->con_name == m_sContainer
			&& m_arraySamplingResult[i]->liq_name == m_sLiquid)
		{
			if(m_arraySamplingResult[i]->CollectionTimes>0)
			{
				bFoundData	= TRUE;
				nMaxValue	= m_arraySamplingResult[i]->MaxValue;
				if(m_nDrawIndexSS>m_arraySamplingResult[i]->CollectionTimes)
				{
					m_nDrawIndexSS	= 0;	//为零则取最后一条数据
				}
				break;
			}
		}
	}
	//取color
	COLORREF m_Color;
	m_Color	= COLOR_RED;
	for(i=0; i<pApp->m_arrayLiquid.GetSize(); i++)
	{
		if(pApp->m_arrayLiquid[i]->liq_name==m_sLiquid)
		{
			m_Color	= _ttoi(pApp->m_arrayLiquid[i]->showcolor);
			break;
		}
	}
	penCurve.CreatePen (PS_SOLID,1,m_Color);	//曲线画笔
	pOldPen=pDC->SelectObject (&penCurve);		//将画笔选入
	//取数并归一化
	if(bFoundData)
	{
		//绘制数据曲线
		//从小到大显示
		for(int nDrawIndex=0; nDrawIndex<m_arraySamplingData.GetSize(); nDrawIndex++)
		{
			if(m_arraySamplingData[nDrawIndex]->channel == m_nSelChannelInterface
				&& m_arraySamplingData[nDrawIndex]->con_name == m_sContainer
				&& m_arraySamplingData[nDrawIndex]->liq_name == m_sLiquid)
			{
				for(i=0; i<1024; i++)
				{
					if(nMaxValue>0)
					{
						m_fDrawData[i] = float(m_arraySamplingData[nDrawIndex]->SamplingData[i])/nMaxValue;
					}
					else
					{
						m_fDrawData[i] = 0.0f;
					}
				}
				pDC->MoveTo(DrawRect.left+0, DrawRect.top+(int)(DrawRect.Height()-m_fDrawData[0]*m_Ratio));
				// 逐个像素进行绘制
				for(i=1; i<1024; i++)
				{
					pDC->LineTo(int(DrawRect.left+i*m_PixelValue) , DrawRect.top+(int)(DrawRect.Height()-m_fDrawData[i]*m_Ratio));
				}
			}
		}
	}
	
/****恢复环境****/
	pDC->SelectObject (pOldPen);	// 选择旧画笔

	font.DeleteObject();
	penScale.DeleteObject();
	penThreshold.DeleteObject();
	penGridding.DeleteObject();
	penCurve.DeleteObject();

	//返回操作结果
	return;
}

//多样本单次
void CDialogParameter::DrawImageMS()
{
/****绘制基本表格****/
	CDC *pDC;
	CRect DrawRect;
	GetDlgItem(IDC_STATIC_DRAW)->GetWindowRect(&DrawRect);
	ScreenToClient(&DrawRect);
	pDC=GetDC();
	CFont font, *pOldFont;
	CPen penScale,penThreshold,penGridding,penCurve,*pOldPen;

	// 对显示区域的大小进行归整
	int m_GridV=DrawRect.Height()/20;
	DrawRect.bottom=DrawRect.top+DrawRect.Height()/20*20;
	int m_GridH=DrawRect.Width()/(22*5);
	DrawRect.right=DrawRect.left+DrawRect.Width()/(22*5)*(22*5);

	// 比例
	int m_Ratio;
	m_Ratio	= DrawRect.Height();
	// 每个数据所需要的像素
	double m_PixelValue;
	m_PixelValue	= DrawRect.Width()/(1.0*22*5*10);

	// 创建点阵字体
	font.CreatePointFont(90,_T("宋体"),pDC);
	// 创建画笔
	penScale.CreatePen (PS_SOLID,1,COLOR_RED);				//刻度
	penThreshold.CreatePen (PS_SOLID,3,COLOR_YELLOW);		//道阈
	penGridding.CreatePen (PS_DASH,1,RGB(0,128,64));		//网格
	//将字体选入
	pOldFont=pDC->SelectObject(&font);
	//将画笔选入
	pOldPen=pDC->SelectObject (&penScale);
	//设置位图背景模式为不透明  
	pDC->SetBkMode(TRANSPARENT);
	//设置位图背景颜色为白色  
	pDC->SetBkColor(COLOR_WHITE);
	//填充位图 
	pDC->FillSolidRect(DrawRect.left,DrawRect.top,DrawRect.Width(),DrawRect.Height(),COLOR_WHITE); 
	//设置文字显示颜色  
	pDC->SetTextColor(COLOR_BLACK);

	// 绘制网格
	pOldPen=pDC->SelectObject (&penGridding);	//将画笔选入
	CString Str;
	int	i;
	for(i=0; i< DrawRect.Height(); i+=m_GridV)  // 纵向
	{
		pDC->MoveTo(DrawRect.left+0  , DrawRect.top+i);
		pDC->LineTo(DrawRect.right   , DrawRect.top+i);
	}
	for(i=0; i< DrawRect.Width(); i+=m_GridH*5)  // 横向
	{
		pDC->MoveTo(DrawRect.left + i  , DrawRect.top+0);
		pDC->LineTo(DrawRect.left + i  , DrawRect.top+DrawRect.Height());
	}

	// 绘制多道阈线
	pOldPen=pDC->SelectObject (&penThreshold);	//将画笔选入
	pDC->MoveTo(int(DrawRect.left+pApp->m_para_thresholdbegin*m_PixelValue), DrawRect.top);
	pDC->LineTo(int(DrawRect.left+pApp->m_para_thresholdbegin*m_PixelValue), DrawRect.bottom);
	pDC->MoveTo(int(DrawRect.left+pApp->m_para_thresholdend*m_PixelValue), DrawRect.top);
	pDC->LineTo(int(DrawRect.left+pApp->m_para_thresholdend*m_PixelValue), DrawRect.bottom);
	
	// 绘制刻度及字符
	pOldPen=pDC->SelectObject (&penScale);	//将画笔选入
	// 横轴
	pDC->MoveTo(DrawRect.left , DrawRect.bottom);
	pDC->LineTo(DrawRect.right, DrawRect.bottom);
	// 纵轴
	pDC->MoveTo(DrawRect.left, DrawRect.top);
	pDC->LineTo(DrawRect.left, DrawRect.bottom);
	//填充标刻位置 
	pDC->FillSolidRect(DrawRect.left,DrawRect.bottom+2,DrawRect.Width(),15,RGB(236,233,216)); 
	for(i=0; i< DrawRect.Width(); i+=m_GridH)  // 横轴及刻度
	{
		pDC->SetTextColor(COLOR_RED);

		// 10刻度标记
		pDC->MoveTo(DrawRect.left+i,DrawRect.top+DrawRect.Height());
		pDC->LineTo(DrawRect.left+i,DrawRect.top+(DrawRect.Height()-5));
		

		if(i==0 || i%(10*m_GridH))
			continue;

		// 100刻度标记
		if(!(i%(10*m_GridH)))
		{
			pDC->MoveTo(DrawRect.left+i,DrawRect.top+DrawRect.Height());
			pDC->LineTo(DrawRect.left+i,DrawRect.top+(DrawRect.Height()-10));
		}

		Str.Format(_T("%d"),i/m_GridH*10);
		pDC->ExtTextOut(DrawRect.left+i, DrawRect.bottom, NULL, DrawRect, Str, NULL);
	}
	pDC->SetTextColor(COLOR_BLACK);
	// 填充标刻位置 
	for(i=DrawRect.Height(); i>=0 ; i-=m_GridV)  // 纵轴刻度
	{
		if(i%2)	continue;

		// 0.1 刻度
		pDC->MoveTo(DrawRect.left+0 ,DrawRect.top+i);
		pDC->LineTo(DrawRect.left+10,DrawRect.top+i);

		// 刻度
		Str.Format(_T("%.1lf"),(DrawRect.Height()-i)/m_GridV*0.05);
		pDC->SetTextColor(RGB(255,0,0));
		pDC->ExtTextOut(DrawRect.left-20,i+5,NULL, DrawRect, Str, NULL);
	}
	pDC->SetTextColor(COLOR_BLACK);
	
/****绘制数据曲线****/
	UINT	nMaxValue(0);	//最大数值，用于归一化
	//取允许的显示液体的最大值
	for(i=0; i<m_arraySamplingResult.GetSize(); i++)
	{
		if(m_arraySamplingResult[i]->channel == m_nSelChannelInterface
			&& m_arraySamplingResult[i]->con_name == m_sContainer)
		{
			for(int j=0; j<pApp->m_arrayLiquid.GetSize(); j++)
			{
				if(m_arraySamplingResult[i]->liq_name == pApp->m_arrayLiquid[j]->liq_name)
				{
					if(pApp->m_arrayLiquid[j]->showenable==L"1")
					{
						if(nMaxValue<m_arraySamplingResult[i]->MaxValue)
						{
							nMaxValue = m_arraySamplingResult[i]->MaxValue;
						}
					}
					break;
				}
			}
		}
	}

	//取color
	COLORREF m_Color;
	m_Color	= COLOR_RED;
	//画多样本平均曲线
	BOOL bShowEnable(TRUE);
	for(int nDrawIndex=0; nDrawIndex<m_arraySamplingResult.GetSize(); nDrawIndex++)
	{
		if(m_arraySamplingResult[nDrawIndex]->channel == m_nSelChannelInterface
			&& m_arraySamplingResult[nDrawIndex]->con_name == m_sContainer)
		{
			//取颜色
			for(i=0; i<pApp->m_arrayLiquid.GetSize(); i++)
			{
				if(m_arraySamplingResult[nDrawIndex]->liq_name == pApp->m_arrayLiquid[i]->liq_name)
				{
					if(pApp->m_arrayLiquid[i]->showenable==L"1")
					{
						m_Color	= _ttoi(pApp->m_arrayLiquid[i]->showcolor);
						bShowEnable	= TRUE;
					}
					else
					{
						bShowEnable	= FALSE;
					}
					break;
				}
			}
			if(bShowEnable)
			{
				penCurve.CreatePen (PS_SOLID,1,m_Color);	//曲线画笔
				pOldPen=pDC->SelectObject (&penCurve);		//将画笔选入
				//取数并归一化
				for(i=0; i<1024; i++)
				{
					if(nMaxValue>0)
					{
						m_fDrawData[i] = float(m_arraySamplingResult[nDrawIndex]->AverageSamplingData[i])/nMaxValue;
					}
					else
					{
						m_fDrawData[i] = 0.0f;
					}
				}
				pDC->MoveTo(DrawRect.left+0, DrawRect.top+(int)(DrawRect.Height()-m_fDrawData[0]*m_Ratio));
				// 逐个像素进行绘制
				for(i=1; i<1024; i++)
				{
					pDC->LineTo(int(DrawRect.left+i*m_PixelValue) , DrawRect.top+(int)(DrawRect.Height()-m_fDrawData[i]*m_Ratio));
				}
			}
		}
	}
	
/****恢复环境****/
	pDC->SelectObject (pOldPen);	// 选择旧画笔

	font.DeleteObject();
	penScale.DeleteObject();
	penThreshold.DeleteObject();
	penGridding.DeleteObject();
	penCurve.DeleteObject();

	//返回操作结果
	return;
}
