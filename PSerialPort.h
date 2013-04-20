// PSerialPort.h: interface for the CPSerialPort class.
//
//////////////////////////////////////////////////////////////////////
#include "LQDetect.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef void (*LPDataArriveProc)(char *data,int length,DWORD userdata);

//#define WM_NEW_DATA_ARRIVE WM_USER+1

//00-���� 01-�� ����-�ر�
#define ControlCommandKeepState		0x00	//����
#define ControlCommandOpen			0x01	//��
#define ControlCommandClose			0x02	//�ر�

class CPSerialPort  
{
public:
	CPSerialPort();
	virtual ~CPSerialPort();

	BOOL OpenPort(LPCTSTR Port,int Baudrate,int DataBits,int StopBits,int Parity,LPDataArriveProc proc=NULL,DWORD userdata=0); //�򿪴���
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
	UINT	m_ReceiveDataBuf[1024];			//�յ����ݵĻ���
	UINT	m_nThresholdSamplingCount;		//����ֵ�����ڲ�������ֵ
	
	BOOL	ControlSampling(int nChannel=1, char cTimes=0x01, int nSamplingTime=1000);	//���Ʋ�����ͨ��1-4������0-60������ʱ��ms��

	BOOL	ControlXRadial(char cCommand=ControlCommandKeepState);		//����X����:00-���� 01-�� ����-�ر�
	BOOL	ControlAlarmLight(char cCommand=ControlCommandKeepState);	//���Ʊ�����:00-���� 01-�� ����-�ر�
	BOOL	ControlSafeLight(char cCommand=ControlCommandKeepState);	//���ư�ȫ��:00-���� 01-�� ����-�ر�

private:
	HANDLE	m_hComm; //�����豸handle
	HANDLE	m_hReadThread; //�������߳�handle
	BOOL	m_bReceiving; //�Ƿ��������
	UINT	m_nBufferSize; //�����С

//	char *Buffer; //������
	
	LPDataArriveProc m_lpDataArriveProc;
	DWORD m_dwUserData;

	//���������Լ���ʱ����
	DCB dcb;
	COMMTIMEOUTS CommTimeOuts;	
	
};

