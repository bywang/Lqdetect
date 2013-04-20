#if !defined(AFX_DIALOGCONTAINER_H__DC7F1B65_E9BE_4EC7_A139_873B5554EFC0__INCLUDED_)
#define AFX_DIALOGCONTAINER_H__DC7F1B65_E9BE_4EC7_A139_873B5554EFC0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogContainer.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogContainer dialog

class CDialogContainer : public CDialog
{
// Construction
public:
	CDialogContainer(CWnd* pParent = NULL);   // standard constructor

public:
	CLQDetectApp	*pApp;
	HACCEL			m_hAccel;

	BOOL		m_bKeyboardShowed;
	BOOL		m_bAddNew;

	void	SetControls();
	void	LoadDataFromArray();
	
// Dialog Data
	//{{AFX_DATA(CDialogContainer)
	enum { IDD = IDD_DIALOG_CONTAINER };
	CListCtrl	m_list;
	CString	m_name;
	CString	m_id;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogContainer)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogContainer)
	virtual BOOL OnInitDialog();
	afx_msg void OnEnter();
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

#endif // !defined(AFX_DIALOGCONTAINER_H__DC7F1B65_E9BE_4EC7_A139_873B5554EFC0__INCLUDED_)
