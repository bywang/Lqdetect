// DialogSetDisplay.cpp : implementation file
//

#include "stdafx.h"
#include "LQDetect.h"
#include "DialogSetDisplay.h"
#include "CppSQLite3U.h"		//SQLite3ͷ�ļ�

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogSetDisplay dialog


CDialogSetDisplay::CDialogSetDisplay(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogSetDisplay::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogSetDisplay)
	m_bDisplay = FALSE;
	//}}AFX_DATA_INIT
}


void CDialogSetDisplay::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogSetDisplay)
	DDX_Control(pDX, IDC_LIST_SAMPLE, m_ListSample);
	DDX_Check(pDX, IDC_CHECK_DIS, m_bDisplay);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogSetDisplay, CDialog)
	//{{AFX_MSG_MAP(CDialogSetDisplay)
	ON_BN_CLICKED(IDC_BUTTON_DISPLAYALL, OnButtonDisplayAll)
	ON_BN_CLICKED(IDC_BUTTON_HIDEALL, OnButtonHideAll)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_DIS, OnButtonColorDis)
	ON_NOTIFY(NM_CLICK, IDC_LIST_SAMPLE, OnClickListSample)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CHECK_DIS, OnCheckDis)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogSetDisplay message handlers

BOOL CDialogSetDisplay::OnInitDialog() 
{
	CDialog::OnInitDialog();

	pApp=(CLQDetectApp *)AfxGetApp();
	
	m_ListSample.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	int i=0;
	m_ListSample.InsertColumn(i++, L"��Ʒ����", LVCFMT_CENTER, 110);
	m_ListSample.InsertColumn(i++, L"��ʾ��ɫ", LVCFMT_CENTER, 100);
	m_ListSample.InsertColumn(i++, L"�Ƿ���ʾ", LVCFMT_CENTER, 80);

	int nRow, j;
	for(i=0; i<pApp->m_arrayLiquid.GetSize(); i++)
	{
		nRow=m_ListSample.InsertItem(m_ListSample.GetItemCount(), pApp->m_arrayLiquid[i]->liq_name);	//Һ������
		j=1;
		m_ListSample.SetItemText(nRow, j++, pApp->m_arrayLiquid[i]->showcolor);							//��ʾ��ɫ
		if(pApp->m_arrayLiquid[i]->showenable==L"1")
			m_ListSample.SetItemText(nRow, j++, L"��");						//�Ƿ���ʾ
		else
			m_ListSample.SetItemText(nRow, j++, L"��");						//�Ƿ���ʾ
	}

	//ѡ���һ������
	m_CulSelect	= 0;
	m_ListSample.EnsureVisible(m_CulSelect, FALSE);
	m_ListSample.SetSelectionMark(m_CulSelect);
	m_ListSample.SetItemState(m_CulSelect,LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
	
	if(m_ListSample.GetItemText(m_CulSelect, 2)==L"��")
		m_bDisplay	= TRUE;
	else
		m_bDisplay	= FALSE;
	
	m_Color	= _ttoi(m_ListSample.GetItemText(m_CulSelect, 1));

//	CString sMsg;
//	sMsg.Format(L"%d", m_Color);
//	AfxMessageBox(sMsg);

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogSetDisplay::OnButtonDisplayAll() 
{
	for(int i=0; i<m_ListSample.GetItemCount(); i++)
	{
		m_ListSample.SetItemText(i, 2, L"��");
	}
	m_bDisplay	= TRUE;
	UpdateData(FALSE);
}

void CDialogSetDisplay::OnButtonHideAll() 
{
	for(int i=0; i<m_ListSample.GetItemCount(); i++)
	{
		m_ListSample.SetItemText(i, 2, L"��");
	}
	m_bDisplay	= FALSE;
	UpdateData(FALSE);
}

void CDialogSetDisplay::OnButtonColorDis() 
{
	CColorDialog ColorDlg(m_Color);
	if(ColorDlg.DoModal()==IDOK)
	{
		m_Color=ColorDlg.GetColor();
		CString s;
		s.Format(L"%d", m_Color);
		m_ListSample.SetItemText(m_CulSelect, 1, s);
	}
	UpdateData(FALSE);
	Invalidate();
}

void CDialogSetDisplay::OnClickListSample(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UpdateData(TRUE);

	POSITION pos = m_ListSample.GetFirstSelectedItemPosition();
	m_CulSelect = m_ListSample.GetNextSelectedItem(pos);  // �õ���Ŀ����
	
	if(m_CulSelect<0)
	{
		pApp->m_dlgOK.m_sText = _T("��ѡ��Һ��������");
		pApp->m_dlgOK.DoModal();
		return;
	}
	m_Color	= _ttoi(m_ListSample.GetItemText(m_CulSelect, 1));
	if(m_ListSample.GetItemText(m_CulSelect, 2)==L"��")
		m_bDisplay	= TRUE;
	else
		m_bDisplay	= FALSE;

	UpdateData(FALSE);
	
	Invalidate();
	*pResult = 0;
}

void CDialogSetDisplay::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// ��ʶ��ɫ��ʾ
	CDC *pDC;
	CRect DrawRect;
	GetDlgItem(IDC_STATIC_LABELCOLOR)->GetWindowRect(&DrawRect);
	ScreenToClient(&DrawRect);
	pDC=GetDC();
//	pDC->SetBkColor(RGB(255,255,255));
	pDC->FillSolidRect(DrawRect.left,DrawRect.top,DrawRect.Width(),DrawRect.Height(),m_Color); 
}

void CDialogSetDisplay::OnCheckDis() 
{
	if(((CButton *)GetDlgItem(IDC_CHECK_DIS))->GetCheck())
		m_ListSample.SetItemText(m_CulSelect, 2, L"��");
	else
		m_ListSample.SetItemText(m_CulSelect, 2, L"��");
}

void CDialogSetDisplay::OnOK() 
{
	CppSQLite3DB db;
	try
	{
		db.open(pApp->m_strDbPath);		//�򿪻��½�һ�����ݿ�
		CString sSQL;
		db.execDML(L"begin transaction;");
		for(int i=0; i<m_ListSample.GetItemCount(); i++)
		{
			if(m_ListSample.GetItemText(i,2)==L"��")
				sSQL.Format(_T("update Liquid set showcolor=%s, showenable=1 where liq_name='%s'"),m_ListSample.GetItemText(i,1),m_ListSample.GetItemText(i,0));
			else
				sSQL.Format(_T("update Liquid set showcolor=%s, showenable=0 where liq_name='%s'"),m_ListSample.GetItemText(i,1),m_ListSample.GetItemText(i,0));
			db.execDML(sSQL);
		}
		db.execDML(L"commit transaction;");
	}
	catch(CppSQLite3Exception ex)
	{
		pApp->m_dlgOK.m_sText = ex.errorMessage();
		pApp->m_dlgOK.DoModal();
		pApp->m_dlgOK.m_sText = _T("��ʾ���ñ���ʧ�ܣ�");
		pApp->m_dlgOK.DoModal();
		return;
	}
	for(int i=0; i<m_ListSample.GetItemCount(); i++)
	{
		pApp->m_arrayLiquid[i]->showcolor = m_ListSample.GetItemText(i,1);
		if(m_ListSample.GetItemText(i,2)==L"��")
			pApp->m_arrayLiquid[i]->showenable = L"1";
		else
			pApp->m_arrayLiquid[i]->showenable = L"0";
	}
	pApp->m_dlgOK.m_sText = _T("��ʾ�����ѱ��棡");
	pApp->m_dlgOK.DoModal();

	CDialog::OnOK();
}
