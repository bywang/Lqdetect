// LQDetectDlg.h : header file
//

#if !defined(AFX_LQDETECTDLG_H__F072F666_FCA8_47A1_8990_A9B8D30ECC6C__INCLUDED_)
#define AFX_LQDETECTDLG_H__F072F666_FCA8_47A1_8990_A9B8D30ECC6C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CLQDetectDlg dialog

class CLQDetectDlg : public CDialog
{
// Construction
public:
	CLQDetectDlg(CWnd* pParent = NULL);	// standard constructor

public:

// Dialog Data
	//{{AFX_DATA(CLQDetectDlg)
	enum { IDD = IDD_LQDETECT_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLQDetectDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CLQDetectDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnButtonParameter();
	afx_msg void OnButtonDetect();
	afx_msg void OnButtonTest();
	afx_msg void OnButtonDemarcate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft eMbedded Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LQDETECTDLG_H__F072F666_FCA8_47A1_8990_A9B8D30ECC6C__INCLUDED_)
