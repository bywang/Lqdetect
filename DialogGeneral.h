#if !defined(AFX_DIALOGGENERAL_H__742974DE_6C7A_45E6_BC6E_716C9F0F8233__INCLUDED_)
#define AFX_DIALOGGENERAL_H__742974DE_6C7A_45E6_BC6E_716C9F0F8233__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogGeneral.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogGeneral dialog

class CDialogGeneral : public CDialog
{
// Construction
public:
	CDialogGeneral(CWnd* pParent = NULL);   // standard constructor
	CLQDetectApp	*pApp;

// Dialog Data
	//{{AFX_DATA(CDialogGeneral)
	enum { IDD = IDD_DIALOG_GENERAL };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogGeneral)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual int DoModal();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogGeneral)
	afx_msg void OnButtonAlum();
	afx_msg void OnButtonCal();
	afx_msg void OnButtonManage();
	afx_msg void OnButtonOthers();
	afx_msg void OnButtonPlastic();
	afx_msg void OnButtonStart();
	afx_msg void OnButtonXray();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnStaticTemp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGGENERAL_H__742974DE_6C7A_45E6_BC6E_716C9F0F8233__INCLUDED_)
