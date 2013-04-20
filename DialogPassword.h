#if !defined(AFX_DIALOGPASSWORD_H__8703E912_7BAD_41BB_8A7D_9C67E8BB7EB6__INCLUDED_)
#define AFX_DIALOGPASSWORD_H__8703E912_7BAD_41BB_8A7D_9C67E8BB7EB6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogPassword.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogPassword dialog

class CDialogPassword : public CDialog
{
// Construction
public:
	CDialogPassword(CWnd* pParent = NULL);   // standard constructor

	CLQDetectApp	*pApp;

	BOOL		m_bKeyboardShowed;

// Dialog Data
	//{{AFX_DATA(CDialogPassword)
	enum { IDD = IDD_DIALOG_PASSWORD };
	CString	m_password;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogPassword)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogPassword)
	virtual void OnOK();
	afx_msg void OnSetfocusEditPassword();
	afx_msg void OnKillfocusEditPassword();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGPASSWORD_H__8703E912_7BAD_41BB_8A7D_9C67E8BB7EB6__INCLUDED_)
