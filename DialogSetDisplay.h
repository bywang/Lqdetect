#if !defined(AFX_DIALOGSETDISPLAY_H__64DCA3DF_3944_4B6A_AACC_C49D6310449D__INCLUDED_)
#define AFX_DIALOGSETDISPLAY_H__64DCA3DF_3944_4B6A_AACC_C49D6310449D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogSetDisplay.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogSetDisplay dialog

class CDialogSetDisplay : public CDialog
{
// Construction
public:
	CDialogSetDisplay(CWnd* pParent = NULL);   // standard constructor

public:
	CLQDetectApp	*pApp;

	COLORREF	m_Color;
	int			m_CulSelect;


// Dialog Data
	//{{AFX_DATA(CDialogSetDisplay)
	enum { IDD = IDD_DIALOG_SETDISPLAY };
	CListCtrl	m_ListSample;
	BOOL	m_bDisplay;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogSetDisplay)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogSetDisplay)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonDisplayAll();
	afx_msg void OnButtonHideAll();
	afx_msg void OnButtonColorDis();
	afx_msg void OnClickListSample(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPaint();
	afx_msg void OnCheckDis();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGSETDISPLAY_H__64DCA3DF_3944_4B6A_AACC_C49D6310449D__INCLUDED_)
