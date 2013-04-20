#if !defined(AFX_DIALOGSPLASH_H__3B96C5B6_0AF5_4D21_AA3A_3BCB4E174DAA__INCLUDED_)
#define AFX_DIALOGSPLASH_H__3B96C5B6_0AF5_4D21_AA3A_3BCB4E174DAA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogSplash.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogSplash dialog

class CDialogSplash : public CDialog
{
// Construction
public:
	CDialogSplash(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogSplash)
	enum { IDD = IDD_DIALOG_SPLASH };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogSplash)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogSplash)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGSPLASH_H__3B96C5B6_0AF5_4D21_AA3A_3BCB4E174DAA__INCLUDED_)
