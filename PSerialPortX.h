// PSerialPortX.h: interface for the CPSerialPortX class.
//
//////////////////////////////////////////////////////////////////////
#include "LQDetect.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//typedef void (*LPDataArriveProc)(char *data,int length,DWORD userdata);

class CPSerialPortX  
{
public:
	CPSerialPortX();
	virtual ~CPSerialPortX();

	BOOL OpenPort(LPCTSTR Port,int Baudrate,int DataBits,int StopBits,int Parity); //打开串口
	BOOL ClosePort(); //关闭串口
	
	//激活与关闭持续接收
	BOOL Activate();
	BOOL Deactivate();
	BOOL IsActive();
	
	//接收数据线程函数
	static DWORD WINAPI ReadPortThread(LPVOID lpParameter);
	
	DWORD ReadPort(char *data,int length);			//读取一定长度的数据
	DWORD WritePort(char *data,int length);			//发送数据
	DWORD WriteFileToPort(LPCTSTR FileName);		//发送文件
	
	//增加安检仪通讯控制
	CLQDetectApp	*pApp;

	BOOL	m_bEchoReceiveSuccess;			//回复接收成功
	int		m_DataState;					//数据状态（收数前初始为-1，收到数为1，错误为0）
	UINT	m_ReceiveDataBuf[50];			//收到数据的缓冲
	CString	m_sV;							//电压
	CString	m_sA;							//电流
	CString	m_sT;							//温度
	float	m_fT;							//温度值
	
	BOOL	ReadXRadial();					//读取X射线状态
	BOOL	SetXV(UINT nSetValue);			//设置X射线机电压
	BOOL	SetXA(UINT nSetValue);			//设置X射线机电流

private:
	HANDLE	m_hComm;						//串口设备handle
	HANDLE	m_hReadThread;					//读串口线程handle
	BOOL	m_bReceiving;					//是否持续接收
	UINT	m_nBufferSize;					//缓冲大小

	
//	LPDataArriveProc m_lpDataArriveProc;
//	DWORD m_dwUserData;

	//串口设置以及超时参数
	DCB dcb;
	COMMTIMEOUTS CommTimeOuts;	
	
};

