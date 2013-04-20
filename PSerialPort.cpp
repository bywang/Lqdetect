// PSerialPort.cpp: implementation of the CPSerialPort class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PSerialPort.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPSerialPort::CPSerialPort()
{
	m_hComm=INVALID_HANDLE_VALUE;
	m_hReadThread=NULL;
	m_bReceiving=FALSE;
	m_nBufferSize=5000; //�����С

	m_bEchoReceiveSuccess		= FALSE;
	m_DataState					= DataReceiveWait;
	m_nThresholdSamplingCount	= 0;	//����ֵ�����ڲ�������ֵ

	pApp=(CLQDetectApp *)AfxGetApp();

}

CPSerialPort::~CPSerialPort()
{
	ClosePort();
}

DWORD WINAPI CPSerialPort::ReadPortThread(LPVOID lpParameter)
{
	CPSerialPort* m_pSerial;

	m_pSerial=(CPSerialPort*)lpParameter;

	BOOL fReadState;
	DWORD dwLength;
	
	unsigned char* buf=new unsigned char[m_pSerial->m_nBufferSize];

	CString sMsg;

	while((m_pSerial->m_hComm!=INVALID_HANDLE_VALUE)&&(m_pSerial->m_bReceiving))
	{		
		fReadState=ReadFile(m_pSerial->m_hComm, buf, m_pSerial->m_nBufferSize, &dwLength, NULL);
		if(!fReadState)
		{
//			AfxMessageBox(_T("�޷��Ӵ��ڶ�ȡ���ݣ�"));
		}
		else
		{
//			sMsg.Format(_T("%d"),dwLength);
//			AfxMessageBox(sMsg);
			if(dwLength==10)
			{
//				sMsg.Format(_T("%2X "),buf[0]);
//				AfxMessageBox(sMsg);
//				sMsg.Format(_T("%X "),buf[1]);
//				AfxMessageBox(sMsg);
//				sMsg.Format(_T("%X "),buf[9]);
//				AfxMessageBox(sMsg);
				if(buf[0]==0xC0 && buf[1]==0x80 && buf[9]==0xC0)
				{
//					AfxMessageBox(_T("OK"));
					m_pSerial->m_bEchoReceiveSuccess	= TRUE;
				}
			}
			else if(dwLength==2054)
			{
//				AfxMessageBox(_T("OK"));
				if(buf[0]==0xC0 && buf[1]==0x13 && buf[2053]==0xC0)
				{
					//����ȫ������
					for(int i=0; i<1024; i++)
					{
						m_pSerial->m_ReceiveDataBuf[i] = buf[i*2+3] + buf[i*2+4]*256;
					}
					m_pSerial->m_nThresholdSamplingCount	= 0;
					for(UINT j=(m_pSerial->pApp->m_para_thresholdbegin-1); j<m_pSerial->pApp->m_para_thresholdend; j++)
					{
						m_pSerial->m_nThresholdSamplingCount += m_pSerial->m_ReceiveDataBuf[j];
					}
					m_pSerial->m_DataState	= DataReceiveOK;	//�ɹ�
				}
				else
				{
//					AfxMessageBox(L"DataError");
					m_pSerial->m_DataState	= DataReceiveError;	//����
				}
			}
			else if(dwLength!=0)
			{
//				AfxMessageBox(L"LengthError");
				m_pSerial->m_DataState	= DataReceiveLengthError;		//���ȴ���
//				sMsg.Format(_T("%d"), dwLength);
//				AfxMessageBox(sMsg);
				//��������
//				if(m_pSerial->m_lpDataArriveProc!=NULL)
//				{
//					m_pSerial->m_lpDataArriveProc(buf,dwLength,m_pSerial->m_dwUserData);
//				}
			}
		}		
	}

	delete[] buf;

	return 0;
}

BOOL CPSerialPort::OpenPort(LPCTSTR Port,int BaudRate,int DataBits,int StopBits,int Parity,LPDataArriveProc proc,DWORD userdata)
{
	m_lpDataArriveProc=proc;
	m_dwUserData=userdata;

	if(m_hComm==INVALID_HANDLE_VALUE)
	{
		m_hComm=CreateFile(Port,GENERIC_READ|GENERIC_WRITE,0,0,OPEN_EXISTING,0,0);
		if(m_hComm==INVALID_HANDLE_VALUE )
		{
			pApp->m_dlgOK.m_sText = _T("�޷��򿪶˿ڣ������Ƿ��ѱ�ռ�á�");
			pApp->m_dlgOK.DoModal();
			return FALSE;
		}
		GetCommState(m_hComm,&dcb);
		dcb.BaudRate=BaudRate;
		dcb.ByteSize=DataBits;
		dcb.Parity=Parity;
		dcb.StopBits=StopBits;
		dcb.fParity=FALSE;
		dcb.fBinary=TRUE;
		dcb.fDtrControl=0;
		dcb.fRtsControl=0;
		dcb.fOutX=dcb.fInX=dcb.fTXContinueOnXoff=0;
		
		//����״̬����
		SetCommMask(m_hComm,EV_RXCHAR);		
		SetupComm(m_hComm,16384,16384);		
		if(!SetCommState(m_hComm,&dcb))
		{
			pApp->m_dlgOK.m_sText = _T("�޷�����ǰ�������ö˿ڣ����������");
			pApp->m_dlgOK.DoModal();
			PurgeComm(m_hComm,PURGE_TXCLEAR|PURGE_RXCLEAR);
			ClosePort();
			return FALSE;
		}
		
		//���ó�ʱ����
		GetCommTimeouts(m_hComm,&CommTimeOuts);		
		CommTimeOuts.ReadIntervalTimeout=100;
		CommTimeOuts.ReadTotalTimeoutMultiplier=1;
		CommTimeOuts.ReadTotalTimeoutConstant=100;
		CommTimeOuts.WriteTotalTimeoutMultiplier=0;
		CommTimeOuts.WriteTotalTimeoutConstant=0;		
		if(!SetCommTimeouts(m_hComm,&CommTimeOuts))
		{
			pApp->m_dlgOK.m_sText = _T("�޷����ó�ʱ������");
			pApp->m_dlgOK.DoModal();
			PurgeComm(m_hComm,PURGE_TXCLEAR|PURGE_RXCLEAR);
			ClosePort();
			return FALSE;
		}
		
		PurgeComm(m_hComm,PURGE_TXCLEAR|PURGE_RXCLEAR);		
		return TRUE;		
	}
	
	return FALSE;
}

BOOL CPSerialPort::ClosePort()
{
	Deactivate();
	if(m_hComm!=INVALID_HANDLE_VALUE)
	{
		SetCommMask(m_hComm,0);		
		PurgeComm(m_hComm,PURGE_TXCLEAR|PURGE_RXCLEAR);
		CloseHandle(m_hComm);
		m_hComm=INVALID_HANDLE_VALUE;
		return TRUE;
	}
	
	return TRUE;	
}

BOOL CPSerialPort::Activate()
{
	if(m_hComm==INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	if(!m_bReceiving)
	{
		//��ʼ�����߳�
		PurgeComm(m_hComm,PURGE_TXCLEAR|PURGE_RXCLEAR);
		m_bReceiving=TRUE;
		m_hReadThread=CreateThread(NULL,0,ReadPortThread,this,0,NULL);
	}
	if(m_hReadThread!=NULL)
	{		
		return TRUE;
	}
	else
	{
		m_bReceiving=FALSE;
		return FALSE;
	}

	return FALSE;
}

BOOL CPSerialPort::Deactivate()
{
	if(m_hComm==INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	//ֹͣ�����߳�
	if(m_bReceiving)
	{
		m_bReceiving=FALSE;
		WaitForSingleObject(m_hReadThread,500);
		CloseHandle(m_hReadThread);
		m_hReadThread=NULL;
		return TRUE;
	}

	return FALSE;
}

BOOL CPSerialPort::IsActive()
{
	return m_bReceiving;
}

DWORD CPSerialPort::WritePort(char *data,int length)
{
	if(m_hComm==INVALID_HANDLE_VALUE)
	{
		return 0;
	}

	BOOL fWriteState;
	DWORD dwBytesWritten=0;

	fWriteState=WriteFile(m_hComm,data,length*sizeof(char),&dwBytesWritten,NULL);
	
	return dwBytesWritten;
}

DWORD CPSerialPort::ReadPort(char *data,int length)
{
	BOOL fReadState;
	DWORD dwLength,dwBytesRead;
	int TimeOutCount;

	dwBytesRead=0;
	TimeOutCount=0;
	
	while(m_hComm!=INVALID_HANDLE_VALUE)
	{
//		char* buf=new char[length];
		fReadState=ReadFile(m_hComm,data,length,&dwLength,NULL);
		if(!fReadState)
		{
			break;
		}
		else
		{
			dwBytesRead+=dwLength;
			data+=dwLength;			
		}
		if(dwBytesRead==(unsigned long)length)
		{
			break;
		}
		if(dwLength!=0)
		{
			TimeOutCount=0;
		}
		else
		{
			TimeOutCount++;
			Sleep(5);
		}
		if(TimeOutCount==5)
		{
			break;
		}
	}
	return dwBytesRead;
}

DWORD CPSerialPort::WriteFileToPort(LPCTSTR FileName)
{
	if(m_hComm==INVALID_HANDLE_VALUE)
	{
		return 0;
	}

	CFile cf;

	BOOL fWriteState;
	DWORD dwBytesWritten;
	DWORD dwCharToWrite;

	dwCharToWrite=0;

	if(!cf.Open(FileName,CFile::modeRead))
	{
//		AfxMessageBox(_T("�޷���Hex�ļ���"));
		return 0;
	}
	dwCharToWrite=(DWORD)cf.GetLength();
	cf.Seek(0,CFile::begin);
	dwBytesWritten=0;
	
	if(m_hComm!=INVALID_HANDLE_VALUE&&dwCharToWrite!=0)
	{
		char* buf=new char[dwCharToWrite];
		cf.Read(buf,dwCharToWrite);

		fWriteState=WriteFile(m_hComm,buf,dwCharToWrite*sizeof(char),&dwBytesWritten,NULL);
		if(!fWriteState)
		{
//			AfxMessageBox(_T("�޷���˿�д�����ݣ�"));
		}
		delete[] buf;		
	}
	cf.Close();
	return dwBytesWritten;
}

/*
	C0 02 X���߿���(1BYTE) ���������ƿ���(1BYTE) ��ȫ�ƿ���(1BYTE) 00 00 CRC(2BYTE) C0
      00:״̬����      00:״̬����           00:״̬����
      01:��          01:��               01:��
      ����ֵ:�ر�      ����ֵ:�ر�           ����ֵ:�ر�

��:C0 02 01 01 00 00 00 00 00 C0 Ϊ��X����,�򿪱�������������
�ظ�֡��ʽ
C0 80 �������״̬(1BYTE) 00 00 00 00 CRC(2BYTE) C0
      01:���ճɹ�
      00:����ʧ��
*/
BOOL CPSerialPort::ControlXRadial(char cCommand)	//����X����
{
	m_bEchoReceiveSuccess	= FALSE;

	DWORD dwBytesWritten=0;
	
	char buf[10];
	buf[0]	= (char)0xC0;
	buf[1]	= 0x02;
	buf[2]	= cCommand;
	buf[3]	= 0x00;
	buf[4]	= 0x00;
	buf[5]	= 0x00;
	buf[6]	= 0x00;
	buf[7]	= 0x00;
	buf[8]	= 0x00;
	buf[9]	= (char)0xC0;

	int nTry(3);
	for(int n=1; n<=nTry; n++)
	{
		dwBytesWritten = WritePort(buf,10);
		if(dwBytesWritten==0)
		{
			if(n==nTry)
			{
				pApp->m_dlgOK.m_sText = _T("X���߿��������ʧ�ܣ�");
				pApp->m_dlgOK.DoModal();
			}
			else
			{
				Sleep(200);
				continue;
			}
		}
		else
		{
			for(int i=0; i<5; i++)
			{
				if(m_bEchoReceiveSuccess) return TRUE;
				Sleep(500);
			}
			
			if(n==nTry)
			{
				pApp->m_dlgOK.m_sText = _T("X���߿���������Ӧ��");
				pApp->m_dlgOK.DoModal();
			}
		}
	}
	return FALSE;
}

BOOL CPSerialPort::ControlAlarmLight(char cCommand)	//���Ʊ�����
{
	m_bEchoReceiveSuccess	= FALSE;

	DWORD dwBytesWritten=0;
	
	char buf[10];
	buf[0]	= (char)0xC0;
	buf[1]	= 0x02;
	buf[2]	= 0x00;
	buf[3]	= cCommand;
	buf[4]	= 0x00;
	buf[5]	= 0x00;
	buf[6]	= 0x00;
	buf[7]	= 0x00;
	buf[8]	= 0x00;
	buf[9]	= (char)0xC0;

	int nTry(3);
	for(int n=1; n<=nTry; n++)
	{
		dwBytesWritten = WritePort(buf,10);
		if(dwBytesWritten==0)
		{
			if(n==nTry)
			{
				pApp->m_dlgOK.m_sText = _T("�����ƿ��������ʧ�ܣ�");
				pApp->m_dlgOK.DoModal();
			}
			else
			{
				Sleep(200);
				continue;
			}
		}
		else
		{
			for(int i=0; i<5; i++)
			{
				if(m_bEchoReceiveSuccess) return TRUE;
				Sleep(500);
			}
			
			if(n==nTry)
			{
				pApp->m_dlgOK.m_sText = _T("�����ƿ���������Ӧ��");
				pApp->m_dlgOK.DoModal();
			}
		}
	}
	return FALSE;
}

BOOL CPSerialPort::ControlSafeLight(char cCommand)	//���ư�ȫ��
{
	m_bEchoReceiveSuccess	= FALSE;

	DWORD dwBytesWritten=0;
	
	char buf[10];
	buf[0]	= (char)0xC0;
	buf[1]	= 0x02;
	buf[2]	= 0x00;
	buf[3]	= 0x00;
	buf[4]	= cCommand;
	buf[5]	= 0x00;
	buf[6]	= 0x00;
	buf[7]	= 0x00;
	buf[8]	= 0x00;
	buf[9]	= (char)0xC0;

	int nTry(3);
	for(int n=1; n<=nTry; n++)
	{
		dwBytesWritten = WritePort(buf,10);
		if(dwBytesWritten==0)
		{
			if(n==nTry)
			{
				pApp->m_dlgOK.m_sText = _T("��ȫ�ƿ��������ʧ�ܣ�");
				pApp->m_dlgOK.DoModal();
			}
			else
			{
				Sleep(200);
				continue;
			}
		}
		else
		{
			for(int i=0; i<5; i++)
			{
				if(m_bEchoReceiveSuccess) return TRUE;
				Sleep(500);
			}
			
			if(n==nTry)
			{
				pApp->m_dlgOK.m_sText = _T("��ȫ�ƿ���������Ӧ��");
				pApp->m_dlgOK.DoModal();
			}
		}
	}
	return FALSE;
}

//���Ʋ�����ͨ��0-3������0-60������ʱ��ms��
BOOL CPSerialPort::ControlSampling(int nChannel, char cTimes, int nSamplingTime)
{
	m_bEchoReceiveSuccess	= FALSE;
	m_DataState				= DataReceiveWait;

	DWORD dwBytesWritten=0;
	
	char buf[10];
	buf[0]	= (char)0xC0;
	buf[1]	= 0x03;
	switch(nChannel) {
	case 1:
		buf[2]	= 0x00;
		break;
	case 2:
		buf[2]	= 0x01;
		break;
	case 3:
		buf[2]	= 0x02;
		break;
	case 4:
		buf[2]	= 0x03;
		break;
	default:
		buf[2]	= 0x00;
	}
	buf[3]	= cTimes;
	buf[4]	= (char)0xE8;
	buf[5]	= 0x03;
	buf[6]	= 0x00;
	buf[7]	= 0x00;
	buf[8]	= 0x00;
	buf[9]	= (char)0xC0;

	int nTry(3);
	for(int n=1; n<=nTry; n++)
	{
		dwBytesWritten = WritePort(buf,10);
		if(dwBytesWritten==0)
		{
			if(n==nTry)
			{
				pApp->m_dlgOK.m_sText = _T("�������������ʧ�ܣ�");
				pApp->m_dlgOK.DoModal();
				return FALSE;
			}
			else
			{
				Sleep(1000);
				continue;
			}
		}
		else
		{
			for(int i=0; i<10; i++)
			{
				Sleep(1500);
				if(m_DataState==DataReceiveOK)
				{
					return TRUE;
				}
				else if(m_DataState==DataReceiveError || m_DataState==DataReceiveLengthError)
				{
					break;
				}
			}
			if(n==nTry)
			{
				if(m_DataState==DataReceiveError)
				{
					pApp->m_dlgOK.m_sText = _T("���ݽ��մ���");
					pApp->m_dlgOK.DoModal();
					return FALSE;
				}
				else if(m_DataState==DataReceiveLengthError)
				{
					pApp->m_dlgOK.m_sText = _T("���ݽ��ճ��ȴ���");
					pApp->m_dlgOK.DoModal();
					return FALSE;
				}
				else
				{
					pApp->m_dlgOK.m_sText = _T("��������������Ӧ��");
					pApp->m_dlgOK.DoModal();
					return FALSE;
				}
			}
		}	
	}
	return FALSE;
}