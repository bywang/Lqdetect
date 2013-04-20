#if !defined(AFX_DIALOGPARAMETER_H__836E7D0B_3D5E_4FDA_AA0D_906397E45CE7__INCLUDED_)
#define AFX_DIALOGPARAMETER_H__836E7D0B_3D5E_4FDA_AA0D_906397E45CE7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogParameter.h : header file
//

#include "PSerialPort.h"

//采集结果
typedef struct
{
	UINT	channel;							//通道接口
	CString	con_name;							//容器名称
	CString	liq_name;							//液体名称
	UINT	CollectionTimes;					//采集次数（每次多个采样平均）
	UINT	MaxCount;							//最大计数
	UINT	MinCount;							//最小计数
	UINT	AverageCount;						//平均计数
	UINT	RangeCount;							//计数范围
	UINT	MaxValue;							//最大数值
	double	AverageSamplingData[1024];			//平均采样数组
} structSamplingResult;

//采样数据
typedef struct
{
	UINT	channel;					//通道接口
	CString	con_name;					//容器名称
	CString	liq_name;					//液体名称
//	UINT	ThresholdBegin;				//阈值起点
//	UINT	ThresholdEnd;				//阈值终点
	UINT	ThresholdSamplingCount;		//采样阈内计数值
	UINT	MaxValue;					//最大数值
	UINT	SamplingData[1024];			//采样数组
} structSamplingData;

typedef CArray<structSamplingResult*,	structSamplingResult*> CSamplingResultArray;
typedef CArray<structSamplingData*,	structSamplingData*> CSamplingDataArray;

/////////////////////////////////////////////////////////////////////////////
// CDialogParameter dialog
class CDialogParameter : public CDialog
{
// Construction
public:
	CDialogParameter(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDialogParameter();

public:
	CLQDetectApp	*pApp;
	HACCEL			m_hAccel;

	CSamplingResultArray	m_arraySamplingResult;	//采集结果
	CSamplingDataArray		m_arraySamplingData;	//采集详细数据

	BOOL	m_bChannel1;				//通道1
	BOOL	m_bChannel2;				//通道2
	UINT	m_nSelChannelInterface;		//通道对应接口
	BOOL	m_bDrawSS;					//单样本单次显示
	BOOL	m_bDrawSM;					//单样本多次显示
	BOOL	m_bDrawMS;					//多样本多次显示
	UINT	m_nTempSamplingData[1024];	//采样临时数据
	double	m_fAverageSamplingData[1024];	//采样平均数据

	UINT	m_nSecond;					//间隔秒数

	BOOL	m_bKeyboardEnable;			//是否显示键盘
	BOOL	m_bKeyboardShowed;			//显示键盘
	UINT	m_nCurrentMaxCount;			//当前样本最大计数值
	UINT	m_nCurrentMinCount;			//当前样本最小计数值
	UINT	m_nCurrentMaxValue;			//当前样本最大数值（用于绘图）
	UINT	m_nCurrentAverageCount;		//当前样本平均计数值
	UINT	m_nCurrentSamplingNumber;	//当前样本采样次数

	double	m_fDrawData[1024];			//画图数据

	void	SetControls();
	void	EnableControls();
	void	DisableControls();
	void	LoadSwatchFromArray();

	void	InsertSamplingDataToArray();				//采样数据存入数组，计算采样结果
	void	LoadDetectValueFromSamplingDataArray();		//循环查询采样数据，计算并显示采样结果
	void	UpdateSamplingResultToArray();				//采样结果更新或新增入数组
	void	RecountSamplingDataArray();					//按新的阈限重新计算采样数据

	void	ClearArrayData();
	
//	double	MiddleValue(double Data[],int n);
	
	//串口类变量
	CPSerialPort	m_Serial;
	BOOL			m_bOpen;


	void	MessageUpdateData(BOOL bUpdateData = TRUE);
	BOOL	m_bUpdateData;				//用于Paint;
	BOOL	m_bPaint;

	//接收数据线程函数
	static DWORD WINAPI AutoReadPortThread(LPVOID lpParameter);
	BOOL	m_bWaitThread;				//等待线程
	BOOL	m_bReadValueTest;			//读数据
	HANDLE	m_hReadThread;				//读串口线程handle

//	// 绘制颜色
//	COLORREF *m_Color;

	// 显示刷新
	// 绘图
	UINT	m_nDrawIndexSS;	//单样本单次画线序号（第1-20条）
	void	DrawImageSS();	//单样本单次
	void	DrawImageSM();	//单样本多次
	void	DrawImageMS();	//多样本单次

// Dialog Data
	//{{AFX_DATA(CDialogParameter)
	enum { IDD = IDD_DIALOG_PARAMETER };
	CListCtrl	m_listSwatch;
	CListCtrl	m_listDetectValue;
	CString	m_sLiquid;
	CString	m_sSecond;
	CString	m_sContainer;
	UINT	m_iCountDown;
	UINT	m_iCountUp;
	UINT	m_iCountAverage;
	CString	m_sInfo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogParameter)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:

	// Generated message map functions
	//{{AFX_MSG(CDialogParameter)
	virtual BOOL OnInitDialog();
	afx_msg void OnEnter();
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnRadioChannel1();
	afx_msg void OnRadioChannel2();
	afx_msg void OnRadioDrawSS();
	afx_msg void OnRadioDrawSM();
	afx_msg void OnRadioDrawMS();
	afx_msg void OnSelchangeComboSecond();
	virtual void OnOK();
	afx_msg void OnButtonAutoDetect();
	afx_msg void OnButtonSetDisplay();
	afx_msg void OnClickListDetectValue(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickListSwatch(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonDeleteSwatch();
	afx_msg void OnButtonSaveSwatchModify();
	afx_msg void OnButtonDeleteDetectValue();
	afx_msg void OnButtonUpdateSwatch();
	afx_msg void OnButtonOtherParameter();
	virtual void OnCancel();
	afx_msg void OnSelchangeComboContainer();
	afx_msg void OnButtonManualDetect();
	afx_msg void OnSetfocusEditCountup();
	afx_msg void OnSetfocusEditCountdown();
	afx_msg void OnSetfocusEditCountaverage();
	afx_msg void OnKillfocusEditCountup();
	afx_msg void OnKillfocusEditCountdown();
	afx_msg void OnKillfocusEditCountaverage();
	afx_msg void OnSelchangeComboLiquid();
	afx_msg void OnButtonLiquid();
	afx_msg void OnButtonContainer();
	afx_msg void OnButtonDeleteallDetectValue();
	afx_msg void OnButtonSaveSamplingData();
	afx_msg void OnButtonLoadSamplingData();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGPARAMETER_H__836E7D0B_3D5E_4FDA_AA0D_906397E45CE7__INCLUDED_)
