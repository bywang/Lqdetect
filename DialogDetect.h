#if !defined(AFX_DIALOGDETECT_H__9B6AA295_CB31_4B08_8E0E_A325AA5DB5E8__INCLUDED_)
#define AFX_DIALOGDETECT_H__9B6AA295_CB31_4B08_8E0E_A325AA5DB5E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogDetect.h : header file
//

#include "PSerialPort.h"
#include "PSerialPortX.h"

/////////////////////////////////////////////////////////////////////////////
// CDialogDetect dialog

class CDialogDetect : public CDialog
{
// Construction
public:
	CDialogDetect(CWnd* pParent = NULL);   // standard constructor

public:
	CLQDetectApp	*pApp;

	//预热线程函数
	static DWORD WINAPI AutoWarmUpThread(LPVOID lpParameter);
	HANDLE	m_hWarmUpThread;	//预热线程handle
	BOOL	m_bWarmUp;			//预热
	float	m_X_fT;				//X射线机温度

	BOOL	m_bChannel1;
	BOOL	m_bChannel2;

	BOOL	m_bMethodRatio;		//比值法
	BOOL	m_bMethodCount;		//计数法


	BOOL	m_bStartDetect;		//开始测量
	BOOL	m_bSafeLightOpen;	//安全灯开启
	BOOL	m_bAlarmLightOpen;	//报警灯开户

	UINT	m_nSetXA;
	UINT	m_nSetXV;

	void	SetControls();
	void	DisableControls();
	void	EnableControls();
	BOOL	SampleIsSafe(UINT nSamplingCount, UINT nSelChannelInterface);	//样品是否安全
	
	//串口类变量
	CPSerialPort	m_Serial;		//控制板通讯口（COM1）
	CPSerialPortX	m_SerialX;		//X射线机通讯口（COM2）
	BOOL			m_bOpen;

// Dialog Data
	//{{AFX_DATA(CDialogDetect)
	enum { IDD = IDD_DIALOG_DETECT };
	CSpinButtonCtrl	m_SpinSetXV;
	CSpinButtonCtrl	m_SpinSetXA;
	CString	m_sContainer;
	CString	m_sIsSafe;
	CString	m_sValue;
	CString	m_sInfo;
	UINT	m_nTimes;
	CString	m_X_A;
	CString	m_X_T;
	CString	m_X_V;
	CString	m_sSetXA;
	CString	m_sSetXV;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogDetect)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
//	CWinThread * m_hThread;		//线程

	// Generated message map functions
	//{{AFX_MSG(CDialogDetect)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnRadioChannel1();
	afx_msg void OnRadioChannel2();
	afx_msg void OnRadioMethodRatio();
	afx_msg void OnRadioMethodCount();
	virtual void OnOK();
	afx_msg void OnButtonStart();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonReadX();
	afx_msg void OnButtonSetX();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnButtonWarmUp();
	afx_msg void OnButtonDemarcate();
	afx_msg void OnButtonTest();
	afx_msg void OnButtonClearTimes();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGDETECT_H__9B6AA295_CB31_4B08_8E0E_A325AA5DB5E8__INCLUDED_)
