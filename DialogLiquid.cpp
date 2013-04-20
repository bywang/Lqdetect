// DialogLiquid.cpp : implementation file
//

#include "stdafx.h"
#include "LQDetect.h"
#include "DialogLiquid.h"
#include "CppSQLite3U.h"		//SQLite3头文件
#include "sipapi.h"				//软键盘控制

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogLiquid dialog


CDialogLiquid::CDialogLiquid(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogLiquid::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogLiquid)
	m_name = _T("");
	m_id = _T("");
	m_display = FALSE;
	m_issafe = -1;
	//}}AFX_DATA_INIT
	m_Color	= RGB(255, 0, 0);
	m_bKeyboardShowed	= FALSE;
	m_bAddNew			= FALSE;
}

void CDialogLiquid::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogLiquid)
	DDX_Control(pDX, IDC_LIST_LIQUID, m_list);
	DDX_Text(pDX, IDC_EDIT_NAME, m_name);
	DDV_MaxChars(pDX, m_name, 50);
	DDX_Text(pDX, IDC_EDIT_ID, m_id);
	DDV_MaxChars(pDX, m_id, 2);
	DDX_Check(pDX, IDC_CHECK_DIS, m_display);
	DDX_CBIndex(pDX, IDC_COMBO_ISSAFE, m_issafe);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDialogLiquid, CDialog)
	//{{AFX_MSG_MAP(CDialogLiquid)
	ON_COMMAND(IDR_KEY_ENTER, OnEnter)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_DIS, OnButtonColorDis)
	ON_WM_PAINT()
	ON_EN_SETFOCUS(IDC_EDIT_ID, OnSetfocusEditId)
	ON_EN_KILLFOCUS(IDC_EDIT_ID, OnKillfocusEditId)
	ON_EN_SETFOCUS(IDC_EDIT_NAME, OnSetfocusEditName)
	ON_EN_KILLFOCUS(IDC_EDIT_NAME, OnKillfocusEditName)
	ON_BN_CLICKED(IDC_BUTTON_ADDNEW, OnButtonAddnew)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogLiquid message handlers
BOOL CDialogLiquid::PreTranslateMessage(MSG* pMsg) 
{
    if (WM_KEYFIRST<=pMsg->message && pMsg->message<=WM_KEYLAST && m_hAccel) 
    { 
        HACCEL hAccel = m_hAccel; 
        if (::TranslateAccelerator(m_hWnd, hAccel, pMsg)) 
            return TRUE; 
    } 
	return CDialog::PreTranslateMessage(pMsg);
}

void CDialogLiquid::OnEnter() 
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

BOOL CDialogLiquid::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_hAccel = ::LoadAccelerators(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_KEY_ENTER)); 
	ASSERT(m_hAccel);
	
	pApp=(CLQDetectApp *)AfxGetApp();

	SetControls();
	
	LoadDataFromArray();

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogLiquid::SetControls()
{
	CComboBox*		pCombo;
	pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_ISSAFE);
	pCombo->ResetContent();
	pCombo->AddString(_T("可疑品"));
	pCombo->AddString(_T("安全品"));
	m_issafe	= 0;

	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	int i=0;
	m_list.InsertColumn(i++, L"液体编号", LVCFMT_CENTER, 80);
	m_list.InsertColumn(i++, L"液体名称", LVCFMT_CENTER, 150);
	m_list.InsertColumn(i++, L"显示颜色", LVCFMT_CENTER, 100);
	m_list.InsertColumn(i++, L"是否显示", LVCFMT_CENTER, 80);
	m_list.InsertColumn(i++, L"液体特性", LVCFMT_CENTER, 80);
}

void CDialogLiquid::LoadDataFromArray()
{
	int nRow, i,j;
	CString strData;
	m_list.DeleteAllItems();
	for(i=0; i<pApp->m_arrayLiquid.GetSize(); i++)
	{
		nRow=m_list.InsertItem(m_list.GetItemCount(), pApp->m_arrayLiquid[i]->liquid_id);				//液体编号
		j=1;
		m_list.SetItemText(nRow, j++, pApp->m_arrayLiquid[i]->liq_name);	//液体名称
		m_list.SetItemText(nRow, j++, pApp->m_arrayLiquid[i]->showcolor);	//显示颜色
		if(pApp->m_arrayLiquid[i]->showenable==L"1")
			strData	= L"是";
		else
			strData	= L"否";
		m_list.SetItemText(nRow, j++, strData);								//是否显示
		if(pApp->m_arrayLiquid[i]->issafe==L"1")
			strData	= L"安全品";
		else
			strData	= L"可疑品";
		m_list.SetItemText(nRow, j++, strData);								//液体特性
	}
}

void CDialogLiquid::OnButtonColorDis() 
{
	UpdateData(TRUE);
	CColorDialog ColorDlg(m_Color);
	if(ColorDlg.DoModal()==IDOK)
	{
		m_Color=ColorDlg.GetColor();
	}
	UpdateData(FALSE);
	Invalidate();
}

void CDialogLiquid::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// 标识颜色显示
	CDC *pDC;
	CRect DrawRect;
	GetDlgItem(IDC_STATIC_LABELCOLOR)->GetWindowRect(&DrawRect);
	ScreenToClient(&DrawRect);
	pDC=GetDC();
//	pDC->SetBkColor(RGB(255,255,255));
	pDC->FillSolidRect(DrawRect.left,DrawRect.top,DrawRect.Width(),DrawRect.Height(),m_Color); 
}

void CDialogLiquid::OnSetfocusEditId() 
{
	if(!m_bKeyboardShowed)
	{
		SipShowIM(SIPF_ON);  //面板显示
		m_bKeyboardShowed	= TRUE;
	}
}

void CDialogLiquid::OnSetfocusEditName() 
{
	if(!m_bKeyboardShowed)
	{
		SipShowIM(SIPF_ON);  //面板显示
		m_bKeyboardShowed	= TRUE;
	}
}

void CDialogLiquid::OnKillfocusEditId() 
{
	if(m_bKeyboardShowed)
	{
		SipShowIM(SIPF_OFF);//面板隐藏
		m_bKeyboardShowed	= FALSE;
	}
}

void CDialogLiquid::OnKillfocusEditName() 
{
	if(m_bKeyboardShowed)
	{
		SipShowIM(SIPF_OFF);//面板隐藏
		m_bKeyboardShowed	= FALSE;
	}
}

void CDialogLiquid::OnButtonAddnew() 
{
	UpdateData(TRUE);
	m_id.TrimLeft();
	m_id.TrimRight();
	if(m_id==L"")
	{
		pApp->m_dlgOK.m_sText = _T("液体编号不能为空！");
		pApp->m_dlgOK.DoModal();
		return;
	}
	m_name.TrimLeft();
	m_name.TrimRight();
	if(m_name==L"")
	{
		pApp->m_dlgOK.m_sText = _T("液体名称不能为空！");
		pApp->m_dlgOK.DoModal();
		return;
	}
	for(int i=0; i<pApp->m_arrayLiquid.GetSize(); i++)
	{
		if(m_id==pApp->m_arrayLiquid[i]->liquid_id)
		{
			pApp->m_dlgOK.m_sText = _T("液体编号重复，请重新输入！");
			pApp->m_dlgOK.DoModal();
			return;
		}
		if(m_name==pApp->m_arrayLiquid[i]->liq_name)
		{
			pApp->m_dlgOK.m_sText = _T("液体名称重复，请重新输入！");
			pApp->m_dlgOK.DoModal();
			return;
		}
	}	
	pApp->m_dlgOKCANCEL.m_sText = _T("注意：增加的液体数据将无法删除，请您仔细核对数据，确保准确无误。是否继续执行增加操作？");
	if(pApp->m_dlgOKCANCEL.DoModal()==IDCANCEL) return;

	CppSQLite3DB db;
	try
	{
		db.open(pApp->m_strDbPath);		//打开
		CString sSQL;
		sSQL.Format(_T("insert into liquid(liquid_id, liq_name, showcolor, showenable, issafe) values(%s, '%s', %d, %d, %d)"),
				m_id,
				m_name,
				m_Color,
				m_display,
				m_issafe);
//		AfxMessageBox(sSQL);
		db.execDML(sSQL);
	}
	catch(CppSQLite3Exception ex)
	{
		pApp->m_dlgOK.m_sText = ex.errorMessage();
		pApp->m_dlgOK.DoModal();
		pApp->m_dlgOK.m_sText = _T("液体数据增加失败！");
		pApp->m_dlgOK.DoModal();
		return;
	}
	db.close();

	pApp->LoadDataFromLiquid();
	LoadDataFromArray();
	m_bAddNew	= TRUE;
}
