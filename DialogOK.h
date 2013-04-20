#if !defined(AFX_DIALOGOK_H__D0641DB9_D8A3_44B8_933A_9C9B65D21293__INCLUDED_)
#define AFX_DIALOGOK_H__D0641DB9_D8A3_44B8_933A_9C9B65D21293__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogOK.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogOK dialog

class CDialogOK : public CDialog
{
// Construction
public:
	CDialogOK(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogOK)
	enum { IDD = IDD_DIALOG_OK };
	CString	m_sText;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogOK)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogOK)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGOK_H__D0641DB9_D8A3_44B8_933A_9C9B65D21293__INCLUDED_)
