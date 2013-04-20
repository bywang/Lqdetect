#if !defined(AFX_DIALOGOKCANCEL_H__EAA34581_44D6_435D_A4C6_C2E21F4EA9E2__INCLUDED_)
#define AFX_DIALOGOKCANCEL_H__EAA34581_44D6_435D_A4C6_C2E21F4EA9E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogOKCANCEL.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogOKCANCEL dialog

class CDialogOKCANCEL : public CDialog
{
// Construction
public:
	CDialogOKCANCEL(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogOKCANCEL)
	enum { IDD = IDD_DIALOG_OKCANCEL };
	CString	m_sText;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogOKCANCEL)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogOKCANCEL)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGOKCANCEL_H__EAA34581_44D6_435D_A4C6_C2E21F4EA9E2__INCLUDED_)
