#if !defined(AFX_DIALOGOTHERPARAMETER_H__716C27C5_6CB4_4F95_96C1_171F11359563__INCLUDED_)
#define AFX_DIALOGOTHERPARAMETER_H__716C27C5_6CB4_4F95_96C1_171F11359563__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogOtherParameter.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogOtherParameter dialog

class CDialogOtherParameter : public CDialog
{
// Construction
public:
	CDialogOtherParameter(CWnd* pParent = NULL);   // standard constructor

	CLQDetectApp	*pApp;

	BOOL	m_bModifyAll;
	BOOL	m_bThresholdModified;	//ãÐÏÞ±ä¸ü

	void	SetControls();


// Dialog Data
	//{{AFX_DATA(CDialogOtherParameter)
	enum { IDD = IDD_DIALOG_OTHERPARAMETER };
	CSpinButtonCtrl	m_SpinTEnd;
	CSpinButtonCtrl	m_SpinTBegin;
	UINT	m_iThresholdBegin;
	UINT	m_iThresholdEnd;
	CString	m_sChannelInterface1;
	CString	m_sChannelInterface2;
	CString	m_sSamplingTimes;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogOtherParameter)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogOtherParameter)
	afx_msg void OnButtonSave();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGOTHERPARAMETER_H__716C27C5_6CB4_4F95_96C1_171F11359563__INCLUDED_)
