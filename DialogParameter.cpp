// DialogParameter.cpp : implementation file
//

#include "stdafx.h"
#include "LQDetect.h"
#include "DialogParameter.h"
#include "DialogSetDisplay.h"
#include "DialogOtherParameter.h"
#include "DialogContainer.h"
#include "DialogLiquid.h"
#include "..\_Common\color.h"	//��ɫ����

//#include "CeFileFind.h"
#include "sipapi.h"				//����̿���

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
	
	//����Paint
	m_bUpdateData		= FALSE;
	m_bPaint			= FALSE;
	//���ڽ������
	m_nCurrentMaxCount			= 0;	//��ǰ����������ֵ
	m_nCurrentMinCount			= 0;	//��ǰ������С����ֵ
	m_nCurrentAverageCount		= 0;	//��ǰ����ƽ������ֵ
	m_nCurrentSamplingNumber	= 0;	//��ǰ������������
	
	for(int i=0; i<1024; i++)
	{
		m_fDrawData[i]=0.0f;
	}
	m_nDrawIndexSS	= 0;			//���������λ�����ţ�Ĭ��Ϊ0�����һ��

	m_bKeyboardEnable	= FALSE;	//��������ʾ����
	m_bKeyboardShowed	= FALSE;

	m_bWaitThread		= FALSE;
	
}

CDialogParameter::~CDialogParameter()
{
	ClearArrayData();
}

void CDialogParameter::ClearArrayData()
{
	//�������
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

	//�򿪴���
	m_bOpen	= m_Serial.OpenPort(_T("COM1:"),115200,8,ONESTOPBIT,NOPARITY);
	if(!m_bOpen || !m_Serial.Activate())
	{
		pApp->m_dlgOK.m_sText = _T("���ư�ͨѶ�ڳ�ʼ��ʧ�ܣ��޷����в��ԣ�");
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
	
	OnRadioChannel1();	//ѡ��ͨ��1	
	OnRadioDrawSS();	//����������

	m_sInfo = _T("�뽫���궨��������ѡ��ͨ�������в�����");
	MessageUpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogParameter::SetControls()
{
	//���������б�
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

	//����list
	m_listDetectValue.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	i=0;
	m_listDetectValue.InsertColumn(i++, L"����", LVCFMT_CENTER, 40);
	m_listDetectValue.InsertColumn(i++, L"���ڼ���ֵ", LVCFMT_CENTER, 80);
	
	m_listSwatch.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	i=0;
	m_listSwatch.InsertColumn(i++, L"�ӿ�", LVCFMT_CENTER, 40);
	m_listSwatch.InsertColumn(i++, L"��������", LVCFMT_CENTER, 135);
	m_listSwatch.InsertColumn(i++, L"Һ������", LVCFMT_CENTER, 80);
	m_listSwatch.InsertColumn(i++, L"������", LVCFMT_CENTER, 60);
	m_listSwatch.InsertColumn(i++, L"��С����", LVCFMT_CENTER, 60);
	m_listSwatch.InsertColumn(i++, L"ƽ������", LVCFMT_CENTER, 60);
	m_listSwatch.InsertColumn(i++, L"��ֵ��Χ", LVCFMT_CENTER, 60);
	m_listSwatch.InsertColumn(i++, L"��������", LVCFMT_CENTER, 60);
	
	MessageUpdateData(FALSE);
}

void CDialogParameter::EnableControls()
{
	//ͼ����ʾ��
	GetDlgItem(IDC_RADIO_DRAWSS)->EnableWindow(TRUE);					//��������
	GetDlgItem(IDC_RADIO_DRAWSM)->EnableWindow(TRUE);					//�������
	GetDlgItem(IDC_RADIO_DRAWMS)->EnableWindow(TRUE);					//��������
	GetDlgItem(IDC_BUTTON_SET_DISPLAY)->EnableWindow(TRUE);				//��ʾ����
	
	//����������
	GetDlgItem(IDC_COMBO_SECOND)->EnableWindow(TRUE);					//�������
	GetDlgItem(IDC_RADIO_CHANNEL1)->EnableWindow(TRUE);					//��һͨ��
	GetDlgItem(IDC_RADIO_CHANNEL2)->EnableWindow(TRUE);					//�ڶ�ͨ��
	GetDlgItem(IDC_BUTTON_CONTAINER)->EnableWindow(TRUE);				//������ť
	GetDlgItem(IDC_COMBO_CONTAINER)->EnableWindow(TRUE);				//����ѡ��
	GetDlgItem(IDC_BUTTON_LIQUID)->EnableWindow(TRUE);					//Һ�尴ť
	GetDlgItem(IDC_COMBO_LIQUID)->EnableWindow(TRUE);					//Һ��ѡ��
	GetDlgItem(IDC_BUTTON_MANUAL_DETECT)->EnableWindow(TRUE);			//�ֶ�����
	GetDlgItem(IDC_BUTTON_AUTO_DETECT)->EnableWindow(TRUE);				//�Զ�����

	GetDlgItem(IDC_BUTTON_SAVE_SAMPLINGDATA)->EnableWindow(TRUE);		//��������
	GetDlgItem(IDC_BUTTON_LOAD_SAMPLINGDATA)->EnableWindow(TRUE);		//��������
	GetDlgItem(IDC_BUTTON_OTHER_PARAMETER)->EnableWindow(TRUE);			//��������
	GetDlgItem(IDOK)->EnableWindow(TRUE);								//�˳�
	
/*
	//�������
	GetDlgItem(IDC_BUTTON_DELETE_DETECT_VALUE)->EnableWindow(TRUE);		//ɾ������ֵ
	GetDlgItem(IDC_BUTTON_DELETEALL_DETECT_VALUE)->EnableWindow(TRUE);	//ɾ��ȫ������ֵ
	GetDlgItem(IDC_BUTTON_UPDATE_SWATCH)->EnableWindow(TRUE);			//��������

	//��������
	GetDlgItem(IDC_BUTTON_SAVE_SWATCH_MODIFY)->EnableWindow(TRUE);		//�����޸ı���
	GetDlgItem(IDC_BUTTON_DELETE_SWATCH)->EnableWindow(TRUE);			//ɾ������
	((CEdit*)GetDlgItem(IDC_EDIT_COUNTUP))->SetReadOnly(FALSE);			//��������
	((CEdit*)GetDlgItem(IDC_EDIT_COUNTDOWN))->SetReadOnly(FALSE);		//��������
	((CEdit*)GetDlgItem(IDC_EDIT_COUNTAVERAGE))->SetReadOnly(FALSE);	//����ƽ��
*/
}

void CDialogParameter::DisableControls()
{
	//ͼ����ʾ��
	GetDlgItem(IDC_RADIO_DRAWSS)->EnableWindow(FALSE);					//��������
	GetDlgItem(IDC_RADIO_DRAWSM)->EnableWindow(FALSE);					//�������
	GetDlgItem(IDC_RADIO_DRAWMS)->EnableWindow(FALSE);					//��������
	GetDlgItem(IDC_BUTTON_SET_DISPLAY)->EnableWindow(FALSE);			//��ʾ����

	//����������
	GetDlgItem(IDC_COMBO_SECOND)->EnableWindow(FALSE);					//�������
	GetDlgItem(IDC_RADIO_CHANNEL1)->EnableWindow(FALSE);				//��һͨ��
	GetDlgItem(IDC_RADIO_CHANNEL2)->EnableWindow(FALSE);				//�ڶ�ͨ��
	GetDlgItem(IDC_BUTTON_CONTAINER)->EnableWindow(FALSE);				//������ť
	GetDlgItem(IDC_COMBO_CONTAINER)->EnableWindow(FALSE);				//����ѡ��
	GetDlgItem(IDC_BUTTON_LIQUID)->EnableWindow(FALSE);					//Һ�尴ť
	GetDlgItem(IDC_COMBO_LIQUID)->EnableWindow(FALSE);					//Һ��ѡ��
	GetDlgItem(IDC_BUTTON_MANUAL_DETECT)->EnableWindow(FALSE);			//�ֶ�����
	GetDlgItem(IDC_BUTTON_AUTO_DETECT)->EnableWindow(FALSE);			//�Զ�����

	GetDlgItem(IDC_BUTTON_OTHER_PARAMETER)->EnableWindow(FALSE);		//��������
	GetDlgItem(IDOK)->EnableWindow(FALSE);								//�˳�
	
	//�������
	GetDlgItem(IDC_BUTTON_DELETE_DETECT_VALUE)->EnableWindow(FALSE);	//ɾ������ֵ
	GetDlgItem(IDC_BUTTON_DELETEALL_DETECT_VALUE)->EnableWindow(FALSE);	//ɾ��ȫ������ֵ
	GetDlgItem(IDC_BUTTON_UPDATE_SWATCH)->EnableWindow(FALSE);			//��������
	GetDlgItem(IDC_BUTTON_SAVE_SAMPLINGDATA)->EnableWindow(FALSE);		//��������
	GetDlgItem(IDC_BUTTON_LOAD_SAMPLINGDATA)->EnableWindow(FALSE);		//��������

	//��������
	GetDlgItem(IDC_BUTTON_SAVE_SWATCH_MODIFY)->EnableWindow(FALSE);		//�����޸ı���
	GetDlgItem(IDC_BUTTON_DELETE_SWATCH)->EnableWindow(FALSE);			//ɾ������
	((CEdit*)GetDlgItem(IDC_EDIT_COUNTUP))->SetReadOnly(TRUE);			//��������
	((CEdit*)GetDlgItem(IDC_EDIT_COUNTDOWN))->SetReadOnly(TRUE);		//��������
	((CEdit*)GetDlgItem(IDC_EDIT_COUNTAVERAGE))->SetReadOnly(TRUE);		//����ƽ��
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
		//����ͨ��������
		if(pApp->m_arraySwatch[i]->channel==sChannel && pApp->m_arraySwatch[i]->con_name==m_sContainer)
		{
			nRow=m_listSwatch.InsertItem(m_listSwatch.GetItemCount(), pApp->m_arraySwatch[i]->channel);	//ͨ��
			j=1;
			m_listSwatch.SetItemText(nRow, j++, pApp->m_arraySwatch[i]->con_name);						//��������
			m_listSwatch.SetItemText(nRow, j++, pApp->m_arraySwatch[i]->liq_name);						//Һ������
			m_listSwatch.SetItemText(nRow, j++, pApp->m_arraySwatch[i]->countup);						//������
			m_listSwatch.SetItemText(nRow, j++, pApp->m_arraySwatch[i]->countdown);						//��С����
			m_listSwatch.SetItemText(nRow, j++, pApp->m_arraySwatch[i]->countaverage);					//ƽ������
			m_listSwatch.SetItemText(nRow, j++, pApp->m_arraySwatch[i]->countrange);					//��ֵ��Χ
			if(pApp->m_arraySwatch[i]->issafe==L"1")
				strData = L"��ȫƷ";
			else
				strData	= L"����Ʒ";
			m_listSwatch.SetItemText(nRow, j++, strData);												//��Ʒ����
		}
	}
	UpdateData(FALSE);
}

void CDialogParameter::OnRadioChannel1() 
{
	m_bChannel1	= TRUE;
	m_bChannel2	= FALSE;
	((CButton *)GetDlgItem(IDC_RADIO_CHANNEL1))->SetCheck(m_bChannel1);//ѡ��
	((CButton *)GetDlgItem(IDC_RADIO_CHANNEL2))->SetCheck(m_bChannel2);
	//ȡͨ����Ӧ�ӿ�
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
	((CButton *)GetDlgItem(IDC_RADIO_CHANNEL2))->SetCheck(m_bChannel2);//ѡ��
	//ȡͨ����Ӧ�ӿ�
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
		SipShowIM(SIPF_ON);  //�����ʾ
		m_bKeyboardShowed	= TRUE;
	}
}

void CDialogParameter::OnSetfocusEditCountdown() 
{
	if(m_bKeyboardEnable && !m_bKeyboardShowed)
	{
		SipShowIM(SIPF_ON);  //�����ʾ
		m_bKeyboardShowed	= TRUE;
	}
}

void CDialogParameter::OnSetfocusEditCountaverage() 
{
	if(m_bKeyboardEnable && !m_bKeyboardShowed)
	{
		SipShowIM(SIPF_ON);  //�����ʾ
		m_bKeyboardShowed	= TRUE;
	}
}

void CDialogParameter::OnKillfocusEditCountup() 
{
	if(m_bKeyboardShowed)
	{
		SipShowIM(SIPF_OFF);//�������
		m_bKeyboardShowed	= FALSE;
	}
}

void CDialogParameter::OnKillfocusEditCountdown() 
{
	if(m_bKeyboardShowed)
	{
		SipShowIM(SIPF_OFF);//�������
		m_bKeyboardShowed	= FALSE;
	}
}

void CDialogParameter::OnKillfocusEditCountaverage() 
{
	if(m_bKeyboardShowed)
	{
		SipShowIM(SIPF_OFF);//�������
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

		OnRadioChannel1();	//ѡ��ͨ��1	
		OnRadioDrawSS();	//����������
	}
}

void CDialogParameter::OnButtonLiquid() 
{
	CDialogLiquid dlg;
	dlg.DoModal();
	if(dlg.m_bAddNew)
	{
		//���������б�
		CComboBox*		pCombo;
		pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_LIQUID);
		pCombo->ResetContent();
		for(int i=0; i<pApp->m_arrayLiquid.GetSize(); i++)
		{
			pCombo->AddString(pApp->m_arrayLiquid[i]->liq_name);
		}
		m_sLiquid	= pApp->m_arrayLiquid[0]->liq_name;

		OnRadioChannel1();	//ѡ��ͨ��1	
		OnRadioDrawSS();	//����������
	}
}

void CDialogParameter::OnButtonSaveSamplingData() 
{
/*
	//������
	structSamplingData* pSamplingData		= new structSamplingData;
	pSamplingData->channel					= 1;					//ͨ���ӿں�
	pSamplingData->con_name					= "����";				//��������
	pSamplingData->liq_name					= "ˮ";					//Һ������
	for(UINT n=0; n<1024; n++)
	{
		//����ȫ����������
		pSamplingData->SamplingData[n]	= n;
	}
	m_arraySamplingData.Add(pSamplingData);

	pSamplingData		= new structSamplingData;
	pSamplingData->channel					= 1;					//ͨ���ӿں�
	pSamplingData->con_name					= "����";				//��������
	pSamplingData->liq_name					= "ˮ";					//Һ������
	for(n=0; n<1024; n++)
	{
		//����ȫ����������
		pSamplingData->SamplingData[n]	= n;
	}
	m_arraySamplingData.Add(pSamplingData);	
	//����������*/

	
	if(m_arraySamplingData.GetSize()<=0)
	{
		pApp->m_dlgOK.m_sText = _T("�޲���������ݣ��޷����棡");
		pApp->m_dlgOK.DoModal();
		return;
	}
	CString StrFilePath;
	CString StrFilter;

	// �ļ���չ��
	StrFilter = L"dat(*.dat)|*.dat||";
	// �򿪱���Ի���
	CFileDialog dlg(FALSE,NULL,NULL,OFN_HIDEREADONLY | 
		            OFN_OVERWRITEPROMPT | OFN_EXPLORER,StrFilter);
	if (dlg.DoModal() != IDOK) 
		return;

	StrFilePath = dlg.GetPathName();

	if(StrFilePath.Right(4)!=L".dat")
		StrFilePath += L".dat";

	//�����������������ݿ�
	CString sSQL, sData, s;
	CppSQLite3DB db;

	int i,j;
//	unsigned char data[2048];
//	AfxMessageBox(L"creat db");
	BeginWaitCursor();
	try
	{
		//�򿪻��½�һ�����ݿ�
		db.open(StrFilePath);

		db.execDML(L"begin transaction;");
		if(db.tableExists(L"Sampling"))
		{
			db.execDML(L"DROP TABLE Sampling");
		}
		//�½���
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
		pApp->m_dlgOK.m_sText = _T("����������ݱ���ʧ�ܣ�");
		pApp->m_dlgOK.DoModal();
		return;
	}
	
 	EndWaitCursor();
	pApp->m_dlgOK.m_sText = _T("����������ݱ���ɹ���");
	pApp->m_dlgOK.DoModal();
}

void CDialogParameter::OnButtonLoadSamplingData() 
{
	pApp->m_dlgOKCANCEL.m_sText = _T("�������������ݽ���յ�ǰ�������ݣ��Ƿ������");
	if(pApp->m_dlgOKCANCEL.DoModal()==IDCANCEL)
		return;
	
	CString StrFilePath;
	CString StrFilter;

	// �ļ���չ��
	StrFilter = L"dat(*.dat)|*.dat||";
	// �򿪱���ͼ��Ի���
	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY | 
		            OFN_OVERWRITEPROMPT | OFN_EXPLORER, StrFilter);
	if (dlg.DoModal() != IDOK) 
		return;

	StrFilePath=dlg.GetPathName();
	if(StrFilePath.Right(4)!=L".dat")
		StrFilePath += L".dat";
//	AfxMessageBox(StrFilePath);
	sqlite3_stmt * stat = NULL; //Ԥ����ʹ�õ���һ������Ҫ�����ݽṹ

	CString sSQL, sData;
	int count;
	CppSQLite3DB db;
	unsigned char data[2048]; 
	BeginWaitCursor();
	try
	{
		//�򿪻��½�һ�����ݿ�
		db.open(StrFilePath);
		if(!db.tableExists(L"Sampling"))
		{
			pApp->m_dlgOK.m_sText = _T("����������ݲ����ڣ�������ѡ��");
			pApp->m_dlgOK.DoModal();
			return;
		}
		//�ж��Ƿ��������
		count = db.execScalar(L"select count(*) from Sampling");
		if(count<=0)
		{
			pApp->m_dlgOK.m_sText = _T("�޲���������ݣ��޷����룡");
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
				sSQL.Format(_T("��%d����������������ݲ���ȷ���޷����룡"), n);
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
			pSamplingData->channel	= _ttoi(q.fieldValue(1));		//ͨ���ӿں�
			pSamplingData->con_name	= q.fieldValue(2);				//��������
			pSamplingData->liq_name	= q.fieldValue(3);				//Һ������
			pSamplingData->ThresholdSamplingCount	= 0;			//�������ڼ���ֵ
			pSamplingData->MaxValue					= 0;			//�����ֵ

			for(i=0; i<1024; i++)
			{
				//����ȫ����������
				pSamplingData->SamplingData[i]	= data[i*2]*256+data[i*2+1];
				//�������ڼ���ֵ
				if(i>=(pApp->m_para_thresholdbegin-1) && i<pApp->m_para_thresholdend)
				{
					pSamplingData->ThresholdSamplingCount += pSamplingData->SamplingData[i];
				}
				//�����ֵ
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
		pApp->m_dlgOK.m_sText = _T("����������ݵ���ʧ�ܣ�");
		pApp->m_dlgOK.DoModal();
		return;
	}
//	sSQL.Format(L"%d", m_arraySamplingData.GetSize());
//	AfxMessageBox(sSQL);

	RecountSamplingDataArray();
	LoadDetectValueFromSamplingDataArray();
 	EndWaitCursor();
	pApp->m_dlgOK.m_sText = _T("����������ݵ���ɹ���");
	pApp->m_dlgOK.DoModal();
}

void CDialogParameter::RecountSamplingDataArray()	//���µ��������¼����������
{
	int i, j, n;
	BOOL bFound(FALSE);
	//���¼��������ϸ����
	for(i=0; i<m_arraySamplingData.GetSize(); i++)
	{
		m_arraySamplingData[i]->ThresholdSamplingCount	= 0;	//�������ڼ���ֵ
		m_arraySamplingData[i]->MaxValue	= 0;				//�������ڼ���ֵ
		for(j=0; j<1024; j++)
		{
			//�������ڼ���ֵ
			if(j>=(pApp->m_para_thresholdbegin-1) && j<pApp->m_para_thresholdend)
			{
				m_arraySamplingData[i]->ThresholdSamplingCount += m_arraySamplingData[i]->SamplingData[j];
			}
			//�����ֵ
			if(m_arraySamplingData[i]->MaxValue<m_arraySamplingData[i]->SamplingData[j])
			{
				m_arraySamplingData[i]->MaxValue=m_arraySamplingData[i]->SamplingData[j];
			}
		}
		//���ҽ�������Ƿ����
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
		//�����ڽ�����ݣ�����һ��
		if(!bFound)
		{
			structSamplingResult* pSamplingResult	= new structSamplingResult;
			pSamplingResult->channel				= m_arraySamplingData[i]->channel;		//ͨ���ӿں�
			pSamplingResult->con_name				= m_arraySamplingData[i]->con_name;		//��������
			pSamplingResult->liq_name				= m_arraySamplingData[i]->liq_name;		//Һ������
			pSamplingResult->CollectionTimes		= 0;									//�ɼ�������ÿ�ζ������ƽ����
			pSamplingResult->MaxCount				= 0;									//������
			pSamplingResult->MinCount				= 0x0fffffff;							//��С����
			pSamplingResult->AverageCount			= 0;									//ƽ������
			pSamplingResult->RangeCount				= 0;									//������Χ
			pSamplingResult->MaxValue				= 0;									//�����ֵ
			for(n=0; n<1024; n++)
			{
				pSamplingResult->AverageSamplingData[n]	= 0.0f;								//ƽ������
			}
			m_arraySamplingResult.Add(pSamplingResult);
		}
	}

	//����������
	for(i=0; i<m_arraySamplingResult.GetSize(); i++)
	{
		m_nCurrentMaxCount			= 0;			//��ǰ����������ֵ
		m_nCurrentMinCount			= 0x0fffffff;	//��ǰ������С����ֵ
		m_nCurrentAverageCount		= 0;			//��ǰ����ƽ������ֵ
		m_nCurrentSamplingNumber	= 0;			//��ǰ������������
		m_nCurrentMaxValue			= 0;			//�����ֵ
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
				//���������ֵ
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
		//�������λ�ͼƽ��ֵ
		for(n=0; n<1024; n++)
		{
			m_fAverageSamplingData[n] = m_fAverageSamplingData[n]/m_nCurrentSamplingNumber;
		}

		m_arraySamplingResult[i]->CollectionTimes	= m_nCurrentSamplingNumber;					//�ɼ�������ÿ�ζ������ƽ����
		m_arraySamplingResult[i]->MaxCount			= m_nCurrentMaxCount;						//������
		m_arraySamplingResult[i]->MinCount			= m_nCurrentMinCount;						//��С����
		m_arraySamplingResult[i]->AverageCount		= m_nCurrentAverageCount;					//ƽ������
		m_arraySamplingResult[i]->RangeCount		= m_nCurrentMaxCount-m_nCurrentMinCount;	//������Χ
		m_arraySamplingResult[i]->MaxValue			= m_nCurrentMaxValue;						//�����ֵ
		for(int n=0; n<1024; n++)
		{
			m_arraySamplingResult[i]->AverageSamplingData[n] = m_fAverageSamplingData[n];		//ƽ������
		}
	}
}

void CDialogParameter::InsertSamplingDataToArray()	//�������ݴ�������
{
//	UpdateData(TRUE);

	//���������ϸ����
	structSamplingData* pSamplingData		= new structSamplingData;
	pSamplingData->channel					= m_nSelChannelInterface;		//ͨ���ӿں�
	pSamplingData->con_name					= m_sContainer;					//��������
	pSamplingData->liq_name					= m_sLiquid;					//Һ������
//	pSamplingData->ThresholdBegin			= pApp->m_para_thresholdbegin;	//��ֵ���
//	pSamplingData->ThresholdEnd				= pApp->m_para_thresholdend;	//��ֵ�յ�
	pSamplingData->ThresholdSamplingCount	= 0;							//�������ڼ���ֵ
	pSamplingData->MaxValue					= 0;							//�����ֵ
	for(UINT i=0; i<1024; i++)
	{
		//����ȫ����������
		pSamplingData->SamplingData[i]	= m_nTempSamplingData[i];
		//�������ڼ���ֵ
		if(i>=(pApp->m_para_thresholdbegin-1) && i<pApp->m_para_thresholdend)
		{
			pSamplingData->ThresholdSamplingCount += m_nTempSamplingData[i];
		}
		//�����ֵ
		if(pSamplingData->MaxValue<pSamplingData->SamplingData[i])
		{
			pSamplingData->MaxValue=pSamplingData->SamplingData[i];
		}
	}
	m_arraySamplingData.Add(pSamplingData);
}

void CDialogParameter::LoadDetectValueFromSamplingDataArray()//ѭ����ѯ�������ݣ����㲢��ʾ�������
{
	m_nCurrentMaxCount			= 0;			//��ǰ����������ֵ
	m_nCurrentMinCount			= 0x0fffffff;	//��ǰ������С����ֵ
	m_nCurrentAverageCount		= 0;			//��ǰ����ƽ������ֵ
	m_nCurrentSamplingNumber	= 0;			//��ǰ������������
	m_nCurrentMaxValue			= 0;			//�����ֵ
	//������ʾ����б�
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
			//�������б�
			strData.Format(_T("%d"), m_nCurrentSamplingNumber);
			nRow=m_listDetectValue.InsertItem(m_listDetectValue.GetItemCount(), strData);	//����
			strData.Format(_T("%d"), m_arraySamplingData[i]->ThresholdSamplingCount);
			m_listDetectValue.SetItemText(nRow, 1, strData);							//���ڼ���ֵ
			//���������ֵ
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
	//�������λ�ͼƽ��ֵ
	for(j=0; j<1024; j++)
	{
		m_fAverageSamplingData[j] = m_fAverageSamplingData[j]/m_nCurrentSamplingNumber;
	}
}
	
void CDialogParameter::UpdateSamplingResultToArray()//����������»�����������
{
	BOOL bFound(FALSE);
	for(int i=0; i<m_arraySamplingResult.GetSize(); i++)
	{
		if(m_arraySamplingResult[i]->channel == m_nSelChannelInterface
			&& m_arraySamplingResult[i]->con_name == m_sContainer
			&& m_arraySamplingResult[i]->liq_name == m_sLiquid)
		{
			bFound = TRUE;
			m_arraySamplingResult[i]->CollectionTimes	= m_nCurrentSamplingNumber;					//�ɼ�������ÿ�ζ������ƽ����
			m_arraySamplingResult[i]->MaxCount			= m_nCurrentMaxCount;						//������
			m_arraySamplingResult[i]->MinCount			= m_nCurrentMinCount;						//��С����
			m_arraySamplingResult[i]->AverageCount		= m_nCurrentAverageCount;					//ƽ������
			m_arraySamplingResult[i]->RangeCount		= m_nCurrentMaxCount-m_nCurrentMinCount;	//������Χ
			m_arraySamplingResult[i]->MaxValue			= m_nCurrentMaxValue;						//�����ֵ
			for(int j=0; j<1024; j++)
			{
				m_arraySamplingResult[i]->AverageSamplingData[j] = m_fAverageSamplingData[j];	//ƽ������
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
		pSamplingResult->channel				= m_nSelChannelInterface;					//ͨ���ӿں�
		pSamplingResult->con_name				= m_sContainer;								//��������
		pSamplingResult->liq_name				= m_sLiquid;								//Һ������
		pSamplingResult->CollectionTimes		= m_nCurrentSamplingNumber;					//�ɼ�������ÿ�ζ������ƽ����
		pSamplingResult->MaxCount				= m_nCurrentMaxCount;						//������
		pSamplingResult->MinCount				= m_nCurrentMinCount;						//��С����
		pSamplingResult->AverageCount			= m_nCurrentAverageCount;					//ƽ������
		pSamplingResult->RangeCount				= m_nCurrentMaxCount-m_nCurrentMinCount;	//������Χ
		pSamplingResult->MaxValue				= m_nCurrentMaxValue;						//�����ֵ
		for(int j=0; j<1024; j++)
		{
			pSamplingResult->AverageSamplingData[j]	= m_fAverageSamplingData[j];			//ƽ������
		}
		m_arraySamplingResult.Add(pSamplingResult);
	}
}

void CDialogParameter::OnButtonManualDetect() 
{
	UpdateData(TRUE);
	DisableControls();
	m_sInfo		= _T("��X���߻�...");
	UpdateData(FALSE);

	BeginWaitCursor();
	if(!m_Serial.ControlXRadial(ControlCommandOpen))
	{
		m_sInfo	= _T("��X���߻�ʧ�ܣ�������ư��ͨѶ��·��");
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
		m_sInfo.Format(_T("��%s-%s�������У���%d�β����������Ժ�..."), m_sContainer, m_sLiquid, i+1);
		UpdateData(FALSE);
		if(!m_Serial.ControlSampling(m_nSelChannelInterface))
		{
			m_sInfo = _T("����ʧ�ܣ�������ư��ͨѶ��·��");
			UpdateData(FALSE);
			m_Serial.ControlXRadial(ControlCommandClose);
 			pApp->m_dlgOK.m_sText = _T("������ֹ��������ư��ͨѶ��·��");
			pApp->m_dlgOK.DoModal();
			EndWaitCursor();
			EnableControls();
			return;
		}
#ifdef SHOW_DEBUG_MESSAGE
		sTemp	= L"";
		sMsg.Format(L"�յ�����%d��", i+1);
		for(int n=0; n<1024; n++)
		{
//			if(n%64==0)	sMsg += L"\r\n";
			sTemp.Format(L"%d,",m_Serial.m_ReceiveDataBuf[n]);
			sMsg += sTemp;
		}
		AfxMessageBox(sMsg);
#endif
		if(i>0)//��1�β�ȡ
		{
			for(int j=0; j<1024; j++)
			{
				m_nTempSamplingData[j] += m_Serial.m_ReceiveDataBuf[j];
			}
#ifdef SHOW_DEBUG_MESSAGE
			sTemp	= L"";
			sMsg.Format(L"���Ƶ�����%d��", i);
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

	m_sInfo = _T("�ر�X���߻�...");
	UpdateData(FALSE);
	m_Serial.ControlXRadial(ControlCommandClose);

	m_sInfo.Format(L"��%s-%s��������������У����Ժ�...", m_sContainer, m_sLiquid);
	UpdateData(FALSE);
	for(int j=0; j<1024; j++)
	{
		m_nTempSamplingData[j] = UINT(float(m_nTempSamplingData[j])/pApp->m_para_samplingtimes+0.5);
	}
#ifdef SHOW_DEBUG_MESSAGE
	sTemp	= L"";
	sMsg.Format(L"ƽ��������");
	for(int n=0; n<1024; n++)
	{
//		if(n%64==0)	sMsg += L"\r\n";
		sTemp.Format(L"%d,",m_nTempSamplingData[n]);
		sMsg += sTemp;
	}
	AfxMessageBox(sMsg);
#endif

	/****������������ע��ʹ��˳��****/
	InsertSamplingDataToArray();				//�����������
	LoadDetectValueFromSamplingDataArray();		//ѭ����ѯ�������ݣ����㲢��ʾ�������
	UpdateSamplingResultToArray();				//����������»�����������

	m_sInfo = _T("�뽫���궨��������ѡ��ͨ�������в�����");
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
//		AfxMessageBox(_T("��ʼ����"));
		//�����Զ������߳�
		SetDlgItemText(IDC_BUTTON_AUTO_DETECT,_T("ֹͣ����"));
		DisableControls();
		GetDlgItem(IDC_BUTTON_AUTO_DETECT)->EnableWindow(TRUE);			//�Զ�����
		m_bReadValueTest	= TRUE;
		m_hReadThread = CreateThread(NULL,0,AutoReadPortThread,this,0, NULL);
	}
	else
	{
//		AfxMessageBox(_T("ֹͣ����"));
		BeginWaitCursor();
		GetDlgItem(IDC_BUTTON_AUTO_DETECT)->EnableWindow(FALSE);			//�Զ�����
		m_bReadValueTest	= FALSE;
		m_bWaitThread		= TRUE;
		if(m_hReadThread)
		{
			Sleep(2000);
			if(::WaitForSingleObject(m_hReadThread, 1000)   ==   WAIT_TIMEOUT)   
			{   
//				AfxMessageBox(_T("�ر��߳�"));
				//�����ʱ��   
				m_sInfo	= _T("�ر�X���߻�...");
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
		SetDlgItemText(IDC_BUTTON_AUTO_DETECT,_T("�Զ�����"));
		GetDlgItem(IDC_BUTTON_AUTO_DETECT)->EnableWindow(TRUE);			//�Զ�����
		m_sInfo = _T("�뽫���궨��������ѡ��ͨ�������в�����");
		EnableControls();
 		UpdateData(FALSE);
		EndWaitCursor();
	}
	CRect rcVal;
	GetDlgItem(IDC_STATIC_DRAW)->GetWindowRect(&rcVal);
	InvalidateRect(&rcVal);
}

//�������߳�
DWORD WINAPI CDialogParameter::AutoReadPortThread(LPVOID lpParameter)
{
	CDialogParameter *pDialog;
	pDialog=(CDialogParameter*)lpParameter;

	//��ʼ�������20��
	UINT i,j;
	UINT nMaxReadTimes=20;
	for(i=pDialog->m_listDetectValue.GetItemCount(); i<nMaxReadTimes; i++)
	{
		pDialog->m_sInfo	= _T("��X���߻�...");
		pDialog->MessageUpdateData(FALSE);
		if(!pDialog->m_Serial.ControlXRadial(ControlCommandOpen))	//��X���߻�
		{
			pDialog->m_sInfo	= _T("��X���߻�ʧ�ܣ�������ư��ͨѶ��·��");
			pDialog->MessageUpdateData(FALSE);
			pDialog->pApp->m_dlgOK.m_sText = _T("��X���߻�ʧ�ܣ��޷����ԣ�������ư��ͨѶ��·��");
			pDialog->pApp->m_dlgOK.DoModal();
			pDialog->OnButtonAutoDetect();	//�ж��̣߳��ָ���ť״̬
			return 0;
		}
		if(!pDialog->m_bReadValueTest)
		{
			pDialog->m_sInfo	= _T("�ر�X���߻�...");
			pDialog->MessageUpdateData(FALSE);
			pDialog->m_Serial.ControlXRadial(ControlCommandClose);
			pDialog->m_sInfo = _T("�뽫���궨��������ѡ��ͨ�������в�����");
			pDialog->MessageUpdateData(FALSE);
			return 0;
		}
		pDialog->m_sInfo.Format(_T("���ڶ�ȡ��%s-%s����%d������..."), pDialog->m_sContainer, pDialog->m_sLiquid, i+1);
		pDialog->MessageUpdateData(FALSE);
		
		for(j=0; j<1024; j++)
		{
			pDialog->m_nTempSamplingData[j]=0;
		}
		for(j=0; j<pDialog->pApp->m_para_samplingtimes+1; j++)
		{
			if(!pDialog->m_bReadValueTest)
			{
				pDialog->m_sInfo	= _T("�ر�X���߻�...");
				pDialog->MessageUpdateData(FALSE);
				pDialog->m_Serial.ControlXRadial(ControlCommandClose);
				pDialog->m_sInfo = _T("�뽫���궨��������ѡ��ͨ�������в�����");
				pDialog->MessageUpdateData(FALSE);
				return 0;
			}
			Sleep(1000);
			pDialog->m_sInfo.Format(_T("��%s-%s����%d������ȡ���У���%d�β����������Ժ�..."), pDialog->m_sContainer, pDialog->m_sLiquid, i+1,j+1);
			pDialog->MessageUpdateData(FALSE);
			if(!pDialog->m_Serial.ControlSampling(pDialog->m_nSelChannelInterface))
			{
				pDialog->m_sInfo	= _T("�ر�X���߻�...");
				pDialog->MessageUpdateData(FALSE);
				Sleep(1000);
				pDialog->m_Serial.ControlXRadial(ControlCommandClose);
				pDialog->m_sInfo	= _T("����ʧ�ܣ�������ư��ͨѶ��·��");
				pDialog->pApp->m_dlgOK.m_sText = _T("������ֹ��������ư��ͨѶ��·��");
				pDialog->pApp->m_dlgOK.DoModal();
				pDialog->OnButtonAutoDetect();	//�ж��̣߳��ָ���ť״̬
				return 0;
			}
			if(j>0)//��1�β�ȡ
			{
				for(int k=0; k<1024; k++)
				{
					pDialog->m_nTempSamplingData[k] += pDialog->m_Serial.m_ReceiveDataBuf[k];
				}
			}
		}

		//����ֵ����ȡֵ����ʾ
		for(j=0; j<1024; j++)
		{
			pDialog->m_nTempSamplingData[j] = UINT(float(pDialog->m_nTempSamplingData[j])/pDialog->pApp->m_para_samplingtimes+0.5);
		}
		/****������������ע��ʹ��˳��****/
		pDialog->InsertSamplingDataToArray();				//�����������
		pDialog->LoadDetectValueFromSamplingDataArray();	//ѭ����ѯ�������ݣ����㲢��ʾ�������
		pDialog->UpdateSamplingResultToArray();				//����������»�����������
		pDialog->MessageUpdateData(FALSE);
			
		pDialog->m_sInfo	= _T("�ر�X���߻�...");
		pDialog->MessageUpdateData(FALSE);
		if(!pDialog->m_Serial.ControlXRadial(ControlCommandClose))
		{
			pDialog->m_sInfo	= _T("�ر�X���߻�ʧ�ܣ�������ư��ͨѶ��·��");
			pDialog->pApp->m_dlgOK.m_sText = _T("������ֹ��������ư��ͨѶ��·��");
			pDialog->pApp->m_dlgOK.DoModal();
			pDialog->OnButtonAutoDetect();	//�ж��̣߳��ָ���ť״̬
			return 0;
		}

		//���������ʱ
		if(i<nMaxReadTimes-1)
		{
			for(UINT n=0; n<pDialog->m_nSecond; n++)
			{
				if(!pDialog->m_bReadValueTest)
				{
					pDialog->m_sInfo	= _T("�ر�X���߻�...");
					pDialog->MessageUpdateData(FALSE);
					pDialog->m_Serial.ControlXRadial(ControlCommandClose);
					pDialog->m_sInfo = _T("�뽫���궨��������ѡ��ͨ�������в�����");
					pDialog->MessageUpdateData(FALSE);
					return 0;
				}
				pDialog->m_sInfo.Format(_T("��%s-%s�����������ʱ%d�룬���Ժ�..."), pDialog->m_sContainer, pDialog->m_sLiquid, pDialog->m_nSecond-n);
				pDialog->MessageUpdateData(FALSE);
				Sleep(1000);
			}
		}
	}

	pDialog->SetDlgItemText(IDC_BUTTON_AUTO_DETECT,_T("�Զ�����"));
	pDialog->GetDlgItem(IDC_BUTTON_AUTO_DETECT)->EnableWindow(TRUE);			//�Զ�����
	pDialog->m_sInfo = _T("�뽫���궨��������ѡ��ͨ�������в�����");
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

//����UpdateData��Ϣ�������߳�UpdateDataʱ����
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
			//�����ʱ��   
			TerminateThread(m_hReadThread, NULL);
		}
	}
	
	CDialog::OnCancel();
}

void CDialogParameter::OnClickListDetectValue(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UpdateData(TRUE);

	POSITION pos = m_listDetectValue.GetFirstSelectedItemPosition();
	int nIndex = m_listDetectValue.GetNextSelectedItem(pos);  // �õ���Ŀ����
	
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
	int nIndex = m_listDetectValue.GetNextSelectedItem(pos);  // �õ���Ŀ����
	
	if(nIndex<0)
	{
		GetDlgItem(IDC_BUTTON_DELETE_DETECT_VALUE)->EnableWindow(FALSE);
		pApp->m_dlgOK.m_sText = _T("��ѡ��ɾ����Ŀ��");
		pApp->m_dlgOK.DoModal();
		return;
	}

	CString	s;
	s.Format(_T("ȷ��ɾ����%s�β������ֵ��"), m_listDetectValue.GetItemText(nIndex, 0));
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
	LoadDetectValueFromSamplingDataArray();		//ѭ����ѯ�������ݣ����㲢��ʾ�������
	UpdateSamplingResultToArray();				//����������»�����������
	UpdateData(FALSE);
}

void CDialogParameter::OnButtonDeleteallDetectValue() 
{
	pApp->m_dlgOKCANCEL.m_sText = _T("ȷ��ɾ�����в������ֵ��");
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
	LoadDetectValueFromSamplingDataArray();		//ѭ����ѯ�������ݣ����㲢��ʾ�������
	UpdateSamplingResultToArray();				//����������»�����������
	UpdateData(FALSE);
}

void CDialogParameter::OnButtonUpdateSwatch() 
{
	pApp->m_dlgOKCANCEL.m_sText = _T("�˲������������������궨���ݣ��Ƿ������");
	if(pApp->m_dlgOKCANCEL.DoModal()==IDCANCEL) return;

	UpdateData(TRUE);
	BOOL bFound(FALSE);
//	CString s;
//	s.Format(L"%d", m_arraySamplingResult.GetSize());
//	AfxMessageBox(s);
	CppSQLite3DB db;
	try
	{
		db.open(pApp->m_strDbPath);		//��
	}
	catch(CppSQLite3Exception ex)
	{
		pApp->m_dlgOK.m_sText = ex.errorMessage();
		pApp->m_dlgOK.DoModal();
		pApp->m_dlgOK.m_sText = _T("�����ļ���ʧ�ܣ��������ݸ���ʧ�ܣ�");
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
//			db.open(pApp->m_strDbPath);		//��
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
				//�������ݿ�
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
				//�������ݿ�
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
			pApp->m_dlgOK.m_sText = _T("�������ݸ���ʧ�ܣ�");
			pApp->m_dlgOK.DoModal();
			return;
		}
	}
	db.close();
	pApp->LoadDataFromSwatch();
	//��������Swatch
	LoadSwatchFromArray();
}

void CDialogParameter::OnClickListSwatch(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UpdateData(TRUE);

	POSITION pos = m_listSwatch.GetFirstSelectedItemPosition();
	int nIndex = m_listSwatch.GetNextSelectedItem(pos);  // �õ���Ŀ����
	
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
	m_bKeyboardEnable	= TRUE;	//������ʾ����

	UpdateData(FALSE);	
	*pResult = 0;
}

void CDialogParameter::OnButtonDeleteSwatch()
{
	UpdateData(TRUE);

	POSITION pos = m_listSwatch.GetFirstSelectedItemPosition();
	int nIndex = m_listSwatch.GetNextSelectedItem(pos);  // �õ���Ŀ����
	
	if(nIndex<0)
	{
		GetDlgItem(IDC_BUTTON_SAVE_SWATCH_MODIFY)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_DELETE_SWATCH)->EnableWindow(FALSE);

		((CEdit*)GetDlgItem(IDC_EDIT_COUNTUP))->SetReadOnly(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_COUNTDOWN))->SetReadOnly(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_COUNTAVERAGE))->SetReadOnly(TRUE);
		pApp->m_dlgOK.m_sText = _T("��ѡ����Ҫɾ�����������ұ궨ֵ���ݣ�");
		pApp->m_dlgOK.DoModal();
		return;
	}
	
	pApp->m_dlgOKCANCEL.m_sText = _T("ȷ��ɾ�������������ұ궨ֵ���ݣ�");
	if(pApp->m_dlgOKCANCEL.DoModal()==IDCANCEL) return;

	CString sChannel, sCon_name, sLiq_name;
	sChannel	= m_listSwatch.GetItemText(nIndex, 0);
	sCon_name	= m_listSwatch.GetItemText(nIndex, 1);
	sLiq_name	= m_listSwatch.GetItemText(nIndex, 2);
	CppSQLite3DB db;
	try
	{
		db.open(pApp->m_strDbPath);		//�򿪻��½�һ�����ݿ�
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
		pApp->m_dlgOK.m_sText = _T("�������ұ궨ֵ����ɾ��ʧ�ܣ�");
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
	//��������Swatch
	LoadSwatchFromArray();

//	pApp->m_dlgOK.m_sText = _T("�������ұ궨ֵ����ɾ���ɹ���");
//	pApp->m_dlgOK.DoModal();
}

void CDialogParameter::OnButtonSaveSwatchModify() 
{
	UpdateData(TRUE);
	if(m_iCountUp<m_iCountDown)
	{
		pApp->m_dlgOK.m_sText = _T("����������С����С������");
		pApp->m_dlgOK.DoModal();
		return;
	}
	if(m_iCountUp<=0 || m_iCountDown<=0 || m_iCountAverage<=0)
	{
		pApp->m_dlgOK.m_sText = _T("����ֵӦ�����㣡");
		pApp->m_dlgOK.DoModal();
		return;
	}

	UINT iCountRange=m_iCountUp-m_iCountDown;

	POSITION pos = m_listSwatch.GetFirstSelectedItemPosition();
	int nIndex = m_listSwatch.GetNextSelectedItem(pos);  // �õ���Ŀ����
	
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
		db.open(pApp->m_strDbPath);		//��
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
		pApp->m_dlgOK.m_sText = _T("�������ұ궨ֵ���ݸ���ʧ�ܣ�");
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
	//��������Swatch
	LoadSwatchFromArray();

//	pApp->m_dlgOK.m_sText = _T("�������ұ궨ֵ���ݸ��³ɹ���");
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
		GetDlgItem(IDC_BUTTON_UPDATE_SWATCH)->EnableWindow(TRUE);			//��������
		GetDlgItem(IDC_BUTTON_DELETEALL_DETECT_VALUE)->EnableWindow(TRUE);	//ɾ��ȫ������ֵ
	}
	else
	{
		GetDlgItem(IDC_BUTTON_UPDATE_SWATCH)->EnableWindow(FALSE);			//��������
		GetDlgItem(IDC_BUTTON_DELETEALL_DETECT_VALUE)->EnableWindow(FALSE);	//ɾ��ȫ������ֵ
	}

	if(m_arraySamplingData.GetSize()>0 && !m_bReadValueTest && !m_bWaitThread)
	{
		LoadDetectValueFromSamplingDataArray();
		GetDlgItem(IDC_BUTTON_SAVE_SAMPLINGDATA)->EnableWindow(TRUE);		//��������
	}
	else
	{
		GetDlgItem(IDC_BUTTON_SAVE_SAMPLINGDATA)->EnableWindow(FALSE);		//��������
	}
	
	if(m_bDrawSS)	//������������ʾ
	{
		DrawImageSS();
	}
	else if(m_bDrawSM)	//�����������ʾ
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
/****���ƻ������****/
	CDC *pDC;
	CRect DrawRect;
	GetDlgItem(IDC_STATIC_DRAW)->GetWindowRect(&DrawRect);
	ScreenToClient(&DrawRect);
	pDC=GetDC();
	CFont font, *pOldFont;
	CPen penScale,penThreshold,penGridding,penCurve,*pOldPen;

	// ����ʾ����Ĵ�С���й���
	int m_GridV=DrawRect.Height()/20;
	DrawRect.bottom=DrawRect.top+DrawRect.Height()/20*20;
	int m_GridH=DrawRect.Width()/(22*5);
	DrawRect.right=DrawRect.left+DrawRect.Width()/(22*5)*(22*5);

	// ����
	int m_Ratio;
	m_Ratio	= DrawRect.Height();
	// ÿ����������Ҫ������
	double m_PixelValue;
	m_PixelValue	= DrawRect.Width()/(1.0*22*5*10);

	// ������������
	font.CreatePointFont(90,_T("����"),pDC);
	// ��������
	penScale.CreatePen (PS_SOLID,1,COLOR_RED);				//�̶�
	penThreshold.CreatePen (PS_SOLID,3,COLOR_YELLOW);		//����
	penGridding.CreatePen (PS_DASH,1,RGB(0,128,64));		//����
	//������ѡ��
	pOldFont=pDC->SelectObject(&font);
	//������ѡ��
	pOldPen=pDC->SelectObject (&penScale);
	//����λͼ����ģʽΪ��͸��  
	pDC->SetBkMode(TRANSPARENT);
	//����λͼ������ɫΪ��ɫ  
	pDC->SetBkColor(COLOR_WHITE);
	//���λͼ 
	pDC->FillSolidRect(DrawRect.left,DrawRect.top,DrawRect.Width(),DrawRect.Height(),COLOR_WHITE); 
	//����������ʾ��ɫ  
	pDC->SetTextColor(COLOR_BLACK);

	// ��������
	pOldPen=pDC->SelectObject (&penGridding);	//������ѡ��
	CString Str;
	int	i;
	for(i=0; i< DrawRect.Height(); i+=m_GridV)  // ����
	{
		pDC->MoveTo(DrawRect.left+0  , DrawRect.top+i);
		pDC->LineTo(DrawRect.right   , DrawRect.top+i);
	}
	for(i=0; i< DrawRect.Width(); i+=m_GridH*5)  // ����
	{
		pDC->MoveTo(DrawRect.left + i  , DrawRect.top+0);
		pDC->LineTo(DrawRect.left + i  , DrawRect.top+DrawRect.Height());
	}

	// ���ƶ������
	pOldPen=pDC->SelectObject (&penThreshold);	//������ѡ��
	pDC->MoveTo(int(DrawRect.left+pApp->m_para_thresholdbegin*m_PixelValue), DrawRect.top);
	pDC->LineTo(int(DrawRect.left+pApp->m_para_thresholdbegin*m_PixelValue), DrawRect.bottom);
	pDC->MoveTo(int(DrawRect.left+pApp->m_para_thresholdend*m_PixelValue), DrawRect.top);
	pDC->LineTo(int(DrawRect.left+pApp->m_para_thresholdend*m_PixelValue), DrawRect.bottom);
	
	// ���ƿ̶ȼ��ַ�
	pOldPen=pDC->SelectObject (&penScale);	//������ѡ��
	// ����
	pDC->MoveTo(DrawRect.left , DrawRect.bottom);
	pDC->LineTo(DrawRect.right, DrawRect.bottom);
	// ����
	pDC->MoveTo(DrawRect.left, DrawRect.top);
	pDC->LineTo(DrawRect.left, DrawRect.bottom);
	//�����λ�� 
	pDC->FillSolidRect(DrawRect.left,DrawRect.bottom+2,DrawRect.Width(),15,RGB(236,233,216)); 
	for(i=0; i< DrawRect.Width(); i+=m_GridH)  // ���ἰ�̶�
	{
		pDC->SetTextColor(COLOR_RED);

		// 10�̶ȱ��
		pDC->MoveTo(DrawRect.left+i,DrawRect.top+DrawRect.Height());
		pDC->LineTo(DrawRect.left+i,DrawRect.top+(DrawRect.Height()-5));
		

		if(i==0 || i%(10*m_GridH))
			continue;

		// 100�̶ȱ��
		if(!(i%(10*m_GridH)))
		{
			pDC->MoveTo(DrawRect.left+i,DrawRect.top+DrawRect.Height());
			pDC->LineTo(DrawRect.left+i,DrawRect.top+(DrawRect.Height()-10));
		}

		Str.Format(_T("%d"),i/m_GridH*10);
		pDC->ExtTextOut(DrawRect.left+i, DrawRect.bottom, NULL, DrawRect, Str, NULL);
	}
	pDC->SetTextColor(COLOR_BLACK);
	// �����λ�� 
	for(i=DrawRect.Height(); i>=0 ; i-=m_GridV)  // ����̶�
	{
		if(i%2)	continue;

		// 0.1 �̶�
		pDC->MoveTo(DrawRect.left+0 ,DrawRect.top+i);
		pDC->LineTo(DrawRect.left+10,DrawRect.top+i);

		// �̶�
		Str.Format(_T("%.1lf"),(DrawRect.Height()-i)/m_GridV*0.05);
		pDC->SetTextColor(RGB(255,0,0));
		pDC->ExtTextOut(DrawRect.left-20,i+5,NULL, DrawRect, Str, NULL);
	}
	pDC->SetTextColor(COLOR_BLACK);
	
/****������������****/
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
					m_nDrawIndexSS	= 0;	//Ϊ����ȡ���һ������
				}
				break;
			}
		}
	}
	//ȡ������һ��
	if(bFoundData)
	{
		if(m_nDrawIndexSS>0)
		{	
			UINT n(1);
			//ȡ�����ָ���ݽ������С������ң�
			for(int nDrawIndex=0; nDrawIndex<m_arraySamplingData.GetSize(); nDrawIndex++)
			{
				if(m_arraySamplingData[nDrawIndex]->channel == m_nSelChannelInterface
					&& m_arraySamplingData[nDrawIndex]->con_name == m_sContainer
					&& m_arraySamplingData[nDrawIndex]->liq_name == m_sLiquid)
				{
					if(m_nDrawIndexSS==n)	//��Χ��1-20��
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
		else//ȡ���һ�εĽ�����ݣ��Ӵ�С���ң�
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

	//ȡcolor
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
	penCurve.CreatePen (PS_SOLID,1,m_Color);	//���߻���
	pOldPen=pDC->SelectObject (&penCurve);		//������ѡ��
	//�����������ߣ��������ߣ�
	if(bDrawData)
	{
		pDC->MoveTo(DrawRect.left+0, DrawRect.top+(int)(DrawRect.Height()-m_fDrawData[0]*m_Ratio));
		// ������ؽ��л���
		for(i=1; i<1024; i++)
		{
			pDC->LineTo(int(DrawRect.left+i*m_PixelValue) , DrawRect.top+(int)(DrawRect.Height()-m_fDrawData[i]*m_Ratio));
		}
	}
	
/****�ָ�����****/
	pDC->SelectObject (pOldPen);	// ѡ��ɻ���

	font.DeleteObject();
	penScale.DeleteObject();
	penThreshold.DeleteObject();
	penGridding.DeleteObject();
	penCurve.DeleteObject();

	//���ز������
	return;
}

//���������
void CDialogParameter::DrawImageSM()
{
//	AfxMessageBox(L"SM");
/****���ƻ������****/
	CDC *pDC;
	CRect DrawRect;
	GetDlgItem(IDC_STATIC_DRAW)->GetWindowRect(&DrawRect);
	ScreenToClient(&DrawRect);
	pDC=GetDC();
	CFont font, *pOldFont;
	CPen penScale,penThreshold,penGridding,penCurve,*pOldPen;

	// ����ʾ����Ĵ�С���й���
	int m_GridV=DrawRect.Height()/20;
	DrawRect.bottom=DrawRect.top+DrawRect.Height()/20*20;
	int m_GridH=DrawRect.Width()/(22*5);
	DrawRect.right=DrawRect.left+DrawRect.Width()/(22*5)*(22*5);

	// ����
	int m_Ratio;
	m_Ratio	= DrawRect.Height();
	// ÿ����������Ҫ������
	double m_PixelValue;
	m_PixelValue	= DrawRect.Width()/(1.0*22*5*10);

	// ������������
	font.CreatePointFont(90,_T("����"),pDC);
	// ��������
	penScale.CreatePen (PS_SOLID,1,COLOR_RED);				//�̶�
	penThreshold.CreatePen (PS_SOLID,3,COLOR_YELLOW);		//����
	penGridding.CreatePen (PS_DASH,1,RGB(0,128,64));		//����
	//������ѡ��
	pOldFont=pDC->SelectObject(&font);
	//������ѡ��
	pOldPen=pDC->SelectObject (&penScale);
	//����λͼ����ģʽΪ��͸��  
	pDC->SetBkMode(TRANSPARENT);
	//����λͼ������ɫΪ��ɫ  
	pDC->SetBkColor(COLOR_WHITE);
	//���λͼ 
	pDC->FillSolidRect(DrawRect.left,DrawRect.top,DrawRect.Width(),DrawRect.Height(),COLOR_WHITE); 
	//����������ʾ��ɫ  
	pDC->SetTextColor(COLOR_BLACK);

	// ��������
	pOldPen=pDC->SelectObject (&penGridding);	//������ѡ��
	CString Str;
	int	i;
	for(i=0; i< DrawRect.Height(); i+=m_GridV)  // ����
	{
		pDC->MoveTo(DrawRect.left+0  , DrawRect.top+i);
		pDC->LineTo(DrawRect.right   , DrawRect.top+i);
	}
	for(i=0; i< DrawRect.Width(); i+=m_GridH*5)  // ����
	{
		pDC->MoveTo(DrawRect.left + i  , DrawRect.top+0);
		pDC->LineTo(DrawRect.left + i  , DrawRect.top+DrawRect.Height());
	}

	// ���ƶ������
	pOldPen=pDC->SelectObject (&penThreshold);	//������ѡ��
	pDC->MoveTo(int(DrawRect.left+pApp->m_para_thresholdbegin*m_PixelValue), DrawRect.top);
	pDC->LineTo(int(DrawRect.left+pApp->m_para_thresholdbegin*m_PixelValue), DrawRect.bottom);
	pDC->MoveTo(int(DrawRect.left+pApp->m_para_thresholdend*m_PixelValue), DrawRect.top);
	pDC->LineTo(int(DrawRect.left+pApp->m_para_thresholdend*m_PixelValue), DrawRect.bottom);
	
	// ���ƿ̶ȼ��ַ�
	pOldPen=pDC->SelectObject (&penScale);	//������ѡ��
	// ����
	pDC->MoveTo(DrawRect.left , DrawRect.bottom);
	pDC->LineTo(DrawRect.right, DrawRect.bottom);
	// ����
	pDC->MoveTo(DrawRect.left, DrawRect.top);
	pDC->LineTo(DrawRect.left, DrawRect.bottom);
	//�����λ�� 
	pDC->FillSolidRect(DrawRect.left,DrawRect.bottom+2,DrawRect.Width(),15,RGB(236,233,216)); 
	for(i=0; i< DrawRect.Width(); i+=m_GridH)  // ���ἰ�̶�
	{
		pDC->SetTextColor(COLOR_RED);

		// 10�̶ȱ��
		pDC->MoveTo(DrawRect.left+i,DrawRect.top+DrawRect.Height());
		pDC->LineTo(DrawRect.left+i,DrawRect.top+(DrawRect.Height()-5));
		

		if(i==0 || i%(10*m_GridH))
			continue;

		// 100�̶ȱ��
		if(!(i%(10*m_GridH)))
		{
			pDC->MoveTo(DrawRect.left+i,DrawRect.top+DrawRect.Height());
			pDC->LineTo(DrawRect.left+i,DrawRect.top+(DrawRect.Height()-10));
		}

		Str.Format(_T("%d"),i/m_GridH*10);
		pDC->ExtTextOut(DrawRect.left+i, DrawRect.bottom, NULL, DrawRect, Str, NULL);
	}
	pDC->SetTextColor(COLOR_BLACK);
	// �����λ�� 
	for(i=DrawRect.Height(); i>=0 ; i-=m_GridV)  // ����̶�
	{
		if(i%2)	continue;

		// 0.1 �̶�
		pDC->MoveTo(DrawRect.left+0 ,DrawRect.top+i);
		pDC->LineTo(DrawRect.left+10,DrawRect.top+i);

		// �̶�
		Str.Format(_T("%.1lf"),(DrawRect.Height()-i)/m_GridV*0.05);
		pDC->SetTextColor(RGB(255,0,0));
		pDC->ExtTextOut(DrawRect.left-20,i+5,NULL, DrawRect, Str, NULL);
	}
	pDC->SetTextColor(COLOR_BLACK);
	
/****������������****/
	BOOL	bFoundData(FALSE), bDrawData(FALSE);
	UINT	nMaxValue;	//�����ֵ�����ڹ�һ��
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
					m_nDrawIndexSS	= 0;	//Ϊ����ȡ���һ������
				}
				break;
			}
		}
	}
	//ȡcolor
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
	penCurve.CreatePen (PS_SOLID,1,m_Color);	//���߻���
	pOldPen=pDC->SelectObject (&penCurve);		//������ѡ��
	//ȡ������һ��
	if(bFoundData)
	{
		//������������
		//��С������ʾ
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
				// ������ؽ��л���
				for(i=1; i<1024; i++)
				{
					pDC->LineTo(int(DrawRect.left+i*m_PixelValue) , DrawRect.top+(int)(DrawRect.Height()-m_fDrawData[i]*m_Ratio));
				}
			}
		}
	}
	
/****�ָ�����****/
	pDC->SelectObject (pOldPen);	// ѡ��ɻ���

	font.DeleteObject();
	penScale.DeleteObject();
	penThreshold.DeleteObject();
	penGridding.DeleteObject();
	penCurve.DeleteObject();

	//���ز������
	return;
}

//����������
void CDialogParameter::DrawImageMS()
{
/****���ƻ������****/
	CDC *pDC;
	CRect DrawRect;
	GetDlgItem(IDC_STATIC_DRAW)->GetWindowRect(&DrawRect);
	ScreenToClient(&DrawRect);
	pDC=GetDC();
	CFont font, *pOldFont;
	CPen penScale,penThreshold,penGridding,penCurve,*pOldPen;

	// ����ʾ����Ĵ�С���й���
	int m_GridV=DrawRect.Height()/20;
	DrawRect.bottom=DrawRect.top+DrawRect.Height()/20*20;
	int m_GridH=DrawRect.Width()/(22*5);
	DrawRect.right=DrawRect.left+DrawRect.Width()/(22*5)*(22*5);

	// ����
	int m_Ratio;
	m_Ratio	= DrawRect.Height();
	// ÿ����������Ҫ������
	double m_PixelValue;
	m_PixelValue	= DrawRect.Width()/(1.0*22*5*10);

	// ������������
	font.CreatePointFont(90,_T("����"),pDC);
	// ��������
	penScale.CreatePen (PS_SOLID,1,COLOR_RED);				//�̶�
	penThreshold.CreatePen (PS_SOLID,3,COLOR_YELLOW);		//����
	penGridding.CreatePen (PS_DASH,1,RGB(0,128,64));		//����
	//������ѡ��
	pOldFont=pDC->SelectObject(&font);
	//������ѡ��
	pOldPen=pDC->SelectObject (&penScale);
	//����λͼ����ģʽΪ��͸��  
	pDC->SetBkMode(TRANSPARENT);
	//����λͼ������ɫΪ��ɫ  
	pDC->SetBkColor(COLOR_WHITE);
	//���λͼ 
	pDC->FillSolidRect(DrawRect.left,DrawRect.top,DrawRect.Width(),DrawRect.Height(),COLOR_WHITE); 
	//����������ʾ��ɫ  
	pDC->SetTextColor(COLOR_BLACK);

	// ��������
	pOldPen=pDC->SelectObject (&penGridding);	//������ѡ��
	CString Str;
	int	i;
	for(i=0; i< DrawRect.Height(); i+=m_GridV)  // ����
	{
		pDC->MoveTo(DrawRect.left+0  , DrawRect.top+i);
		pDC->LineTo(DrawRect.right   , DrawRect.top+i);
	}
	for(i=0; i< DrawRect.Width(); i+=m_GridH*5)  // ����
	{
		pDC->MoveTo(DrawRect.left + i  , DrawRect.top+0);
		pDC->LineTo(DrawRect.left + i  , DrawRect.top+DrawRect.Height());
	}

	// ���ƶ������
	pOldPen=pDC->SelectObject (&penThreshold);	//������ѡ��
	pDC->MoveTo(int(DrawRect.left+pApp->m_para_thresholdbegin*m_PixelValue), DrawRect.top);
	pDC->LineTo(int(DrawRect.left+pApp->m_para_thresholdbegin*m_PixelValue), DrawRect.bottom);
	pDC->MoveTo(int(DrawRect.left+pApp->m_para_thresholdend*m_PixelValue), DrawRect.top);
	pDC->LineTo(int(DrawRect.left+pApp->m_para_thresholdend*m_PixelValue), DrawRect.bottom);
	
	// ���ƿ̶ȼ��ַ�
	pOldPen=pDC->SelectObject (&penScale);	//������ѡ��
	// ����
	pDC->MoveTo(DrawRect.left , DrawRect.bottom);
	pDC->LineTo(DrawRect.right, DrawRect.bottom);
	// ����
	pDC->MoveTo(DrawRect.left, DrawRect.top);
	pDC->LineTo(DrawRect.left, DrawRect.bottom);
	//�����λ�� 
	pDC->FillSolidRect(DrawRect.left,DrawRect.bottom+2,DrawRect.Width(),15,RGB(236,233,216)); 
	for(i=0; i< DrawRect.Width(); i+=m_GridH)  // ���ἰ�̶�
	{
		pDC->SetTextColor(COLOR_RED);

		// 10�̶ȱ��
		pDC->MoveTo(DrawRect.left+i,DrawRect.top+DrawRect.Height());
		pDC->LineTo(DrawRect.left+i,DrawRect.top+(DrawRect.Height()-5));
		

		if(i==0 || i%(10*m_GridH))
			continue;

		// 100�̶ȱ��
		if(!(i%(10*m_GridH)))
		{
			pDC->MoveTo(DrawRect.left+i,DrawRect.top+DrawRect.Height());
			pDC->LineTo(DrawRect.left+i,DrawRect.top+(DrawRect.Height()-10));
		}

		Str.Format(_T("%d"),i/m_GridH*10);
		pDC->ExtTextOut(DrawRect.left+i, DrawRect.bottom, NULL, DrawRect, Str, NULL);
	}
	pDC->SetTextColor(COLOR_BLACK);
	// �����λ�� 
	for(i=DrawRect.Height(); i>=0 ; i-=m_GridV)  // ����̶�
	{
		if(i%2)	continue;

		// 0.1 �̶�
		pDC->MoveTo(DrawRect.left+0 ,DrawRect.top+i);
		pDC->LineTo(DrawRect.left+10,DrawRect.top+i);

		// �̶�
		Str.Format(_T("%.1lf"),(DrawRect.Height()-i)/m_GridV*0.05);
		pDC->SetTextColor(RGB(255,0,0));
		pDC->ExtTextOut(DrawRect.left-20,i+5,NULL, DrawRect, Str, NULL);
	}
	pDC->SetTextColor(COLOR_BLACK);
	
/****������������****/
	UINT	nMaxValue(0);	//�����ֵ�����ڹ�һ��
	//ȡ�������ʾҺ������ֵ
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

	//ȡcolor
	COLORREF m_Color;
	m_Color	= COLOR_RED;
	//��������ƽ������
	BOOL bShowEnable(TRUE);
	for(int nDrawIndex=0; nDrawIndex<m_arraySamplingResult.GetSize(); nDrawIndex++)
	{
		if(m_arraySamplingResult[nDrawIndex]->channel == m_nSelChannelInterface
			&& m_arraySamplingResult[nDrawIndex]->con_name == m_sContainer)
		{
			//ȡ��ɫ
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
				penCurve.CreatePen (PS_SOLID,1,m_Color);	//���߻���
				pOldPen=pDC->SelectObject (&penCurve);		//������ѡ��
				//ȡ������һ��
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
				// ������ؽ��л���
				for(i=1; i<1024; i++)
				{
					pDC->LineTo(int(DrawRect.left+i*m_PixelValue) , DrawRect.top+(int)(DrawRect.Height()-m_fDrawData[i]*m_Ratio));
				}
			}
		}
	}
	
/****�ָ�����****/
	pDC->SelectObject (pOldPen);	// ѡ��ɻ���

	font.DeleteObject();
	penScale.DeleteObject();
	penThreshold.DeleteObject();
	penGridding.DeleteObject();
	penCurve.DeleteObject();

	//���ز������
	return;
}
