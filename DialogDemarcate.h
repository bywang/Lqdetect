#if !defined(AFX_DIALOGDEMARCATE_H__2380F6BD_5C46_461A_8C19_4D59E1548675__INCLUDED_)
#define AFX_DIALOGDEMARCATE_H__2380F6BD_5C46_461A_8C19_4D59E1548675__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogDemarcate.h : header file
//

#include "PSerialPort.h"

/////////////////////////////////////////////////////////////////////////////
// CDialogDemarcate dialog

class CDialogDemarcate : public CDialog
{
// Construction
public:
	CDialogDemarcate(CWnd* pParent = NULL);   // standard constructor

public:
	CLQDetectApp	*pApp;

	BOOL	m_bChannel1;
	BOOL	m_bChannel2;

	UINT	m_nOldValue1;		//原标准值
	UINT	m_nOldValue2;		//原标准值
	double	m_fRatio1;			//比例系数
	double	m_fRatio2;			//比例系数
	
	//串口类变量
	CPSerialPort*	m_pSerial;

	void	SetControls();
	void	DisableControls();
	void	EnableControls();
	BOOL	GetOldValue();		//取样本出厂标定值

// Dialog Data
	//{{AFX_DATA(CDialogDemarcate)
	enum { IDD = IDD_DIALOG_DEMARCATE };
	CString	m_sNewValue1;
	CString	m_sOldValue1;
	CString	m_sRatio1;
	CString	m_sNewValue2;
	CString	m_sOldValue2;
	CString	m_sRatio2;
	CString	m_sInfo;
	CString	m_sContainer;
	CString	m_sLiquid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogDemarcate)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogDemarcate)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioChannel1();
	afx_msg void OnRadioChannel2();
	virtual void OnOK();
	afx_msg void OnButtonStart();
	afx_msg void OnButtonSave();
	virtual void OnCancel();
	afx_msg void OnSelchangeComboContainer();
	afx_msg void OnSelchangeComboLiquid();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGDEMARCATE_H__2380F6BD_5C46_461A_8C19_4D59E1548675__INCLUDED_)
