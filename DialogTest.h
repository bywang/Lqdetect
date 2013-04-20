#if !defined(AFX_DIALOGTEST_H__46AE605C_BE3C_4203_9769_C0C793C0AA1E__INCLUDED_)
#define AFX_DIALOGTEST_H__46AE605C_BE3C_4203_9769_C0C793C0AA1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogTest.h : header file
//

#include "PSerialPort.h"

/////////////////////////////////////////////////////////////////////////////
// CDialogTest dialog

class CDialogTest : public CDialog
{
// Construction
public:
	CDialogTest(CWnd* pParent = NULL);   // standard constructor

public:
	CLQDetectApp	*pApp;

	BOOL	m_bChannel1;
	BOOL	m_bChannel2;

	UINT	m_nSecond;		//间隔秒数

	void	DisableControls();
	void	EnableControls();
	void	SetControls();
	
	//串口类变量
	CPSerialPort*	m_pSerial;

	BOOL	m_bReadValueTest;			//读数据

	void	MessageUpdateData(BOOL bUpdateData = TRUE);
	BOOL	m_bUpdateData;				//用于Paint;
	BOOL	m_bPaint;

	//接收数据线程函数
	static DWORD WINAPI ReadTestThread(LPVOID lpParameter);
	
	HANDLE m_hReadThread; //读串口线程handle
//	CWinThread * m_hThread;		//线程

//	static void OnDataArrive(char *data,int length,DWORD userdata);


// Dialog Data
	//{{AFX_DATA(CDialogTest)
	enum { IDD = IDD_DIALOG_TEST };
	CString	m_sValue1;
	CString	m_sValue10;
	CString	m_sValue11;
	CString	m_sValue12;
	CString	m_sValue13;
	CString	m_sValue14;
	CString	m_sValue15;
	CString	m_sValue16;
	CString	m_sValue17;
	CString	m_sValue18;
	CString	m_sValue19;
	CString	m_sValue2;
	CString	m_sValue20;
	CString	m_sValue3;
	CString	m_sValue4;
	CString	m_sValue5;
	CString	m_sValue6;
	CString	m_sValue7;
	CString	m_sValue8;
	CString	m_sValue9;
	CString	m_sSecond;
	CString	m_sInfo;
	CString	m_sContainer;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogTest)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:

	// Generated message map functions
	//{{AFX_MSG(CDialogTest)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioChannel1();
	afx_msg void OnRadioChannel2();
	afx_msg void OnButtonStart();
	afx_msg void OnSelchangeComboSecond();
	virtual void OnOK();
	afx_msg void OnPaint();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGTEST_H__46AE605C_BE3C_4203_9769_C0C793C0AA1E__INCLUDED_)
