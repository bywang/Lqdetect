#if !defined(AFX_DIALOGLIQUID_H__C136FBA6_DFAD_4138_B58C_2CA43B2221C2__INCLUDED_)
#define AFX_DIALOGLIQUID_H__C136FBA6_DFAD_4138_B58C_2CA43B2221C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogLiquid.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogLiquid dialog

class CDialogLiquid : public CDialog
{
// Construction
public:
	CDialogLiquid(CWnd* pParent = NULL);   // standard constructor

public:
	CLQDetectApp	*pApp;
	HACCEL			m_hAccel;

	BOOL		m_bKeyboardShowed;
	BOOL		m_bAddNew;
	COLORREF	m_Color;

	void	SetControls();
	void	LoadDataFromArray();
	
// Dialog Data
	//{{AFX_DATA(CDialogLiquid)
	enum { IDD = IDD_DIALOG_LIQUID };
	CListCtrl	m_list;
	CString	m_name;
	CString	m_id;
	BOOL	m_display;
	int		m_issafe;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogLiquid)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogLiquid)
	virtual BOOL OnInitDialog();
	afx_msg void OnEnter();
	afx_msg void OnButtonColorDis();
	afx_msg void OnPaint();
	afx_msg void OnCheckDis();
	afx_msg void OnSetfocusEditId();
	afx_msg void OnKillfocusEditId();
	afx_msg void OnSetfocusEditName();
	afx_msg void OnKillfocusEditName();
	afx_msg void OnButtonAddnew();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGLIQUID_H__C136FBA6_DFAD_4138_B58C_2CA43B2221C2__INCLUDED_)
