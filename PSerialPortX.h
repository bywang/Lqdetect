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

	BOOL OpenPort(LPCTSTR Port,int Baudrate,int DataBits,int StopBits,int Parity); //�򿪴���
	BOOL ClosePort(); //�رմ���
	
	//������رճ�������
	BOOL Activate();
	BOOL Deactivate();
	BOOL IsActive();
	
	//���������̺߳���
	static DWORD WINAPI ReadPortThread(LPVOID lpParameter);
	
	DWORD ReadPort(char *data,int length);			//��ȡһ�����ȵ�����
	DWORD WritePort(char *data,int length);			//��������
	DWORD WriteFileToPort(LPCTSTR FileName);		//�����ļ�
	
	//���Ӱ�����ͨѶ����
	CLQDetectApp	*pApp;

	BOOL	m_bEchoReceiveSuccess;			//�ظ����ճɹ�
	int		m_DataState;					//����״̬������ǰ��ʼΪ-1���յ���Ϊ1������Ϊ0��
	UINT	m_ReceiveDataBuf[50];			//�յ����ݵĻ���
	CString	m_sV;							//��ѹ
	CString	m_sA;							//����
	CString	m_sT;							//�¶�
	float	m_fT;							//�¶�ֵ
	
	BOOL	ReadXRadial();					//��ȡX����״̬
	BOOL	SetXV(UINT nSetValue);			//����X���߻���ѹ
	BOOL	SetXA(UINT nSetValue);			//����X���߻�����

private:
	HANDLE	m_hComm;						//�����豸handle
	HANDLE	m_hReadThread;					//�������߳�handle
	BOOL	m_bReceiving;					//�Ƿ��������
	UINT	m_nBufferSize;					//�����С

	
//	LPDataArriveProc m_lpDataArriveProc;
//	DWORD m_dwUserData;

	//���������Լ���ʱ����
	DCB dcb;
	COMMTIMEOUTS CommTimeOuts;	
	
};

