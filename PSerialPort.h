// PSerialPort.h: interface for the CPSerialPort class.
//
//////////////////////////////////////////////////////////////////////
#include "LQDetect.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef void (*LPDataArriveProc)(char *data,int length,DWORD userdata);

//#define WM_NEW_DATA_ARRIVE WM_USER+1

//00-不变 01-打开 其它-关闭
#define ControlCommandKeepState		0x00	//保持
#define ControlCommandOpen			0x01	//打开
#define ControlCommandClose			0x02	//关闭

class CPSerialPort  
{
public:
	CPSerialPort();
	virtual ~CPSerialPort();

	BOOL OpenPort(LPCTSTR Port,int Baudrate,int DataBits,int StopBits,int Parity,LPDataArriveProc proc=NULL,DWORD userdata=0); //打开串口
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
	UINT	m_ReceiveDataBuf[1024];			//收到数据的缓冲
	UINT	m_nThresholdSamplingCount;		//在阈值区间内采样计数值
	
	BOOL	ControlSampling(int nChannel=1, char cTimes=0x01, int nSamplingTime=1000);	//控制采样（通道1-4，次数0-60，采样时间ms）

	BOOL	ControlXRadial(char cCommand=ControlCommandKeepState);		//控制X射线:00-不变 01-打开 其它-关闭
	BOOL	ControlAlarmLight(char cCommand=ControlCommandKeepState);	//控制报警灯:00-不变 01-打开 其它-关闭
	BOOL	ControlSafeLight(char cCommand=ControlCommandKeepState);	//控制安全灯:00-不变 01-打开 其它-关闭

private:
	HANDLE	m_hComm; //串口设备handle
	HANDLE	m_hReadThread; //读串口线程handle
	BOOL	m_bReceiving; //是否持续接收
	UINT	m_nBufferSize; //缓冲大小

//	char *Buffer; //缓冲区
	
	LPDataArriveProc m_lpDataArriveProc;
	DWORD m_dwUserData;

	//串口设置以及超时参数
	DCB dcb;
	COMMTIMEOUTS CommTimeOuts;	
	
};

