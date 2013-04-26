// LQDetect.h : main header file for the LQDETECT application
//

#if !defined(AFX_LQDETECT_H__CB92A1C6_47BE_43A0_8291_44713051092D__INCLUDED_)
#define AFX_LQDETECT_H__CB92A1C6_47BE_43A0_8291_44713051092D__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

#include "DialogOK.h"			//自定义确认框
#include "DialogOKCANCEL.h"		//自定义选择框
#include "CppSQLite3U.h"		//SQLite3头文件
#include "PSerialPort.h"
#include "PSerialPortX.h"

/////////////////////////////////////////////////////////////////////////////
// CLQDetectApp:
// See LQDetect.cpp for the implementation of this class
//
//#define SHOW_DEBUG_MESSAGE				//显示调试信息

//#define VERSION_SET_PARAMETER				//设置参数版本（单独使用）

#define FILE_DB_NAME	TEXT("LQData.db")	//数据库名称

#define DataReceiveLengthError		99		//数据接收长度错误
#define DataReceiveError			0		//数据接收错误
#define DataReceiveOK				1		//数据接收成功
#define DataReceiveWait				-1		//数据接收等待

//液体数据
typedef struct
{
	CString	liquid_id;		//液体ID
	CString	liq_name;		//液体名称
	CString	showcolor;		//显示颜色
	CString	showenable;		//是否显示
	CString	issafe;			//特性（是否安全品）
} structLiquid;

//容器数据
typedef struct
{
	CString	container_id;	//容器ID
	CString	con_name;		//容器名称
	CString thresholddown;	//多道阈值下限
	CString thresholdup;	//多道阈值上限
	CString time;			//采样时间
} structContainer;

//样本数据
typedef struct
{
	CString	channel;		//通道
	CString	liq_name;		//液体名称
	CString	con_name;		//容器名称
	CString	countup;		//计数上限
	CString	countdown;		//计数下限
	CString	countaverage;	//计数平均
	CString	countrange;		//计数范围
	CString	issafe;			//特性（安全品/可疑品）
} structSwatch;

typedef CArray<structLiquid*,	structLiquid*> CLiquidArray;
typedef CArray<structContainer*,	structContainer*> CContainerArray;
typedef CArray<structSwatch*,	structSwatch*> CSwatchArray;

class CLQDetectApp : public CWinApp
{
public:
	CLQDetectApp();
	
	CDialogOK			m_dlgOK;
	CDialogOKCANCEL		m_dlgOKCANCEL;

public:
	CString			m_CurrentDir;				//当前运行目录
	CString			m_strDbPath;				//数据库所以目录

    // serial data

    bool m_serialIsOk;
    CPSerialPort *m_SerialP;
    CPSerialPort m_Serial;
    bool m_serialXIsOk;
    CPSerialPortX *m_SerialXP;
    CPSerialPortX m_SerialX;

	//共用数据
	UINT			m_para_channelinterface1;	//通道1使用板上接口
	UINT			m_para_channelinterface2;	//通道2使用板上接口
	double			m_para_standardratio1;		//通道1标准块标定比值系数;
	double			m_para_standardratio2;		//通道2标准块标定比值系数;
	UINT			m_para_thresholdbegin;		//采样阈值启始点
	UINT			m_para_thresholdend;		//采样阈值结束点
	UINT			m_para_samplingtimes;		//采样次数;
	UINT			m_para_totaldetecttimes;	//总检测次数;

	CLiquidArray	m_arrayLiquid;				//液体数据
	CContainerArray	m_arrayContainer;			//容器数据
	CSwatchArray	m_arraySwatch;				//样本数据

	void	LoadDataFromLiquid();				//从数据库中载入液体数据
	void	LoadDataFromContainer();			//从数据库中载入容器数据
	void	LoadDataFromSwatch();				//从数据库中载入样本数据
	void	LoadDataFromParameter();			//从数据库中载入参数数据
	void	ClearArrayData();					//清空Array
	void	GetCurrentDirectory(CString &szPath);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLQDetectApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CLQDetectApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft eMbedded Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LQDETECT_H__CB92A1C6_47BE_43A0_8291_44713051092D__INCLUDED_)
