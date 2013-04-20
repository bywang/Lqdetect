// DialogContainer.cpp : implementation file
//

#include "stdafx.h"
#include "LQDetect.h"
#include "DialogContainer.h"
#include "CppSQLite3U.h"		//SQLite3头文件
#include "sipapi.h"				//软键盘控制

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogContainer dialog


CDialogContainer::CDialogContainer(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogContainer::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogContainer)
	m_name = _T("");
	m_id = _T("");
	//}}AFX_DATA_INIT
	m_bKeyboardShowed	= FALSE;
	m_bAddNew			= FALSE;
}


void CDialogContainer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogContainer)
	DDX_Control(pDX, IDC_LIST_LIQUID, m_list);
	DDX_Text(pDX, IDC_EDIT_NAME, m_name);
	DDV_MaxChars(pDX, m_name, 50);
	DDX_Text(pDX, IDC_EDIT_ID, m_id);
	DDV_MaxChars(pDX, m_id, 2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogContainer, CDialog)
	//{{AFX_MSG_MAP(CDialogContainer)
	ON_COMMAND(IDR_KEY_ENTER, OnEnter)
	ON_EN_SETFOCUS(IDC_EDIT_ID, OnSetfocusEditId)
	ON_EN_KILLFOCUS(IDC_EDIT_ID, OnKillfocusEditId)
	ON_EN_SETFOCUS(IDC_EDIT_NAME, OnSetfocusEditName)
	ON_EN_KILLFOCUS(IDC_EDIT_NAME, OnKillfocusEditName)
	ON_BN_CLICKED(IDC_BUTTON_ADDNEW, OnButtonAddnew)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogContainer message handlers
BOOL CDialogContainer::PreTranslateMessage(MSG* pMsg) 
{
    if (WM_KEYFIRST<=pMsg->message && pMsg->message<=WM_KEYLAST && m_hAccel) 
    { 
        HACCEL hAccel = m_hAccel; 
        if (::TranslateAccelerator(m_hWnd, hAccel, pMsg)) 
            return TRUE; 
    } 
	return CDialog::PreTranslateMessage(pMsg);
}

void CDialogContainer::OnEnter() 
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

BOOL CDialogContainer::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_hAccel = ::LoadAccelerators(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_KEY_ENTER)); 
	ASSERT(m_hAccel);
	
	::SetWindowPos(m_hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE |SWP_NOSIZE);
	pApp=(CLQDetectApp *)AfxGetApp();

	SetControls();
	
	LoadDataFromArray();

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogContainer::SetControls()
{
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	int i=0;
	m_list.InsertColumn(i++, L"容器编号", LVCFMT_CENTER, 80);
	m_list.InsertColumn(i++, L"容器名称", LVCFMT_CENTER, 200);
}

void CDialogContainer::LoadDataFromArray()
{
	int nRow, i,j;
	CString strData;
	m_list.DeleteAllItems();
	for(i=0; i<pApp->m_arrayContainer.GetSize(); i++)
	{
		nRow=m_list.InsertItem(m_list.GetItemCount(), pApp->m_arrayContainer[i]->container_id);	//容器编号
		j=1;
		m_list.SetItemText(nRow, j++, pApp->m_arrayContainer[i]->con_name);						//容器名称
	}
}

void CDialogContainer::OnSetfocusEditId() 
{
	if(!m_bKeyboardShowed)
	{
		SipShowIM(SIPF_ON);  //面板显示
		m_bKeyboardShowed	= TRUE;
	}
}

void CDialogContainer::OnSetfocusEditName() 
{
	if(!m_bKeyboardShowed)
	{
		SipShowIM(SIPF_ON);  //面板显示
		m_bKeyboardShowed	= TRUE;
	}
}

void CDialogContainer::OnKillfocusEditId() 
{
	if(m_bKeyboardShowed)
	{
		SipShowIM(SIPF_OFF);//面板隐藏
		m_bKeyboardShowed	= FALSE;
	}
}

void CDialogContainer::OnKillfocusEditName() 
{
	if(m_bKeyboardShowed)
	{
		SipShowIM(SIPF_OFF);//面板隐藏
		m_bKeyboardShowed	= FALSE;
	}
}

void CDialogContainer::OnButtonAddnew() 
{
	UpdateData(TRUE);
	m_id.TrimLeft();
	m_id.TrimRight();
	if(m_id==L"")
	{
		pApp->m_dlgOK.m_sText = L"容器编号不能为空！";
		pApp->m_dlgOK.DoModal();
		return;
	}
	m_name.TrimLeft();
	m_name.TrimRight();
	if(m_name==L"")
	{
		pApp->m_dlgOK.m_sText = L"容器名称不能为空！";
		pApp->m_dlgOK.DoModal();
		return;
	}
	for(int i=0; i<pApp->m_arrayContainer.GetSize(); i++)
	{
		if(m_id==pApp->m_arrayContainer[i]->container_id)
		{
			pApp->m_dlgOK.m_sText = L"容器编号重复，请重新输入！";
			pApp->m_dlgOK.DoModal();
			return;
		}
		if(m_name==pApp->m_arrayContainer[i]->con_name)
		{
			pApp->m_dlgOK.m_sText = L"容器名称重复，请重新输入！";
			pApp->m_dlgOK.DoModal();
			return;
		}
	}
	
	pApp->m_dlgOKCANCEL.m_sText = L"注意：增加的容器数据将无法删除，请您仔细核对数据，确保准确无误。是否继续执行增加操作？";
	if(pApp->m_dlgOKCANCEL.DoModal()==IDCANCEL) return;

	CppSQLite3DB db;
	try
	{
		db.open(pApp->m_strDbPath);		//打开
		CString sSQL;
		sSQL.Format(_T("insert into container(container_id, con_name, thresholddown, thresholdup, time) values(%s, '%s', 100, 400, 3)"),
				m_id,
				m_name);
//		AfxMessageBox(sSQL);
		db.execDML(sSQL);
	}
	catch(CppSQLite3Exception ex)
	{
		pApp->m_dlgOK.m_sText = ex.errorMessage();
		pApp->m_dlgOK.DoModal();
		pApp->m_dlgOK.m_sText = L"容器数据增加失败！";
		pApp->m_dlgOK.DoModal();
		return;
	}
	db.close();

	pApp->LoadDataFromContainer();
	LoadDataFromArray();
	m_bAddNew	= TRUE;
}
