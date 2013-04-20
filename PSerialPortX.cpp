// PSerialPortX.cpp: implementation of the CPSerialPortX class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PSerialPortX.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPSerialPortX::CPSerialPortX()
{
	m_hComm=INVALID_HANDLE_VALUE;
	m_hReadThread=NULL;
	m_bReceiving=FALSE;
	m_nBufferSize=50; //缓冲大小

	m_bEchoReceiveSuccess	= FALSE;
	m_DataState				= DataReceiveWait;

	pApp=(CLQDetectApp *)AfxGetApp();

}

CPSerialPortX::~CPSerialPortX()
{
	ClosePort();
}

DWORD WINAPI CPSerialPortX::ReadPortThread(LPVOID lpParameter)
{
	CPSerialPortX* m_pSerial;

	m_pSerial=(CPSerialPortX*)lpParameter;

	BOOL fReadState;
	DWORD dwLength;
	
	unsigned char* buf=new unsigned char[m_pSerial->m_nBufferSize];

	CString sMsg;

	while((m_pSerial->m_hComm!=INVALID_HANDLE_VALUE)&&(m_pSerial->m_bReceiving))
	{		
		fReadState=ReadFile(m_pSerial->m_hComm, buf, m_pSerial->m_nBufferSize, &dwLength, NULL);
		if(!fReadState)
		{
//			AfxMessageBox(_T("无法从串口读取数据！"));
		}
		else
		{
			if(dwLength==8)
			{
				if(buf[0]==0x02)
				{
//					CString s;
//					s.Format(_T("返回：%02d%02d%02d%02d%02d%02d%02d"),buf[0],buf[1],buf[2],buf[3],buf[4],buf[5],buf[6],buf[7]);
//					AfxMessageBox(s);

					m_pSerial->m_bEchoReceiveSuccess = TRUE;
				}
			}
			if(dwLength==21)
			{
//				sMsg.Format(_T("%2X "),buf[0]);
//				AfxMessageBox(sMsg);
//				sMsg.Format(_T("%X "),buf[1]);
//				AfxMessageBox(sMsg);
//				sMsg.Format(_T("%X "),buf[9]);
//				AfxMessageBox(sMsg);
				if(buf[0]==0x02
					&& buf[1]>=0x30 && buf[1]<=0x39
					&& buf[2]>=0x30 && buf[2]<=0x39
					&& buf[3]>=0x30 && buf[3]<=0x39
					&& buf[4]>=0x30 && buf[4]<=0x39
					&& buf[6]>=0x30 && buf[6]<=0x39
					&& buf[7]>=0x30 && buf[7]<=0x39
					&& buf[8]>=0x30 && buf[8]<=0x39
					&& buf[9]>=0x30 && buf[9]<=0x39
					&& buf[11]>=0x30 && buf[11]<=0x39
					&& buf[12]>=0x30 && buf[12]<=0x39
					&& buf[13]>=0x30 && buf[13]<=0x39
					&& buf[14]>=0x30 && buf[14]<=0x39)
				{
					m_pSerial->m_sV.Format(_T("%d%d%d.%d"), buf[1]-0x30, buf[2]-0x30, buf[3]-0x30, buf[4]-0x30);
					m_pSerial->m_sA.Format(_T("%d.%d%d%d"), buf[6]-0x30, buf[7]-0x30, buf[8]-0x30, buf[9]-0x30);//uA转mA
//					m_pSerial->m_sT.Format(_T("%d%d%d.%d"), buf[11]-0x30, buf[12]-0x30, buf[13]-0x30, buf[14]-0x30);
					m_pSerial->m_fT = (buf[11]-0x30)*100+(buf[12]-0x30)*10+(buf[13]-0x30)+(buf[14]-0x30)/10.0f;
					m_pSerial->m_sT.Format(_T("%0.1f"),m_pSerial->m_fT);
					m_pSerial->m_DataState	= DataReceiveOK;	//成功
//					AfxMessageBox(m_pSerial->m_sV);
//					AfxMessageBox(m_pSerial->m_sA);
//					AfxMessageBox(m_pSerial->m_sT);
//					AfxMessageBox(_T("OK"));
				}
				else
				{
					m_pSerial->m_DataState	= DataReceiveError;	//错误
				}
			}
			else if(dwLength!=0)
			{
//				sMsg.Format(_T("%d"),dwLength);
//				AfxMessageBox(sMsg);
//				AfxMessageBox(L"LengthError");
				m_pSerial->m_DataState	= DataReceiveLengthError;		//长度错误
//				sMsg.Format(_T("%d"), dwLength);
//				AfxMessageBox(sMsg);
				//回送数据
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

BOOL CPSerialPortX::OpenPort(LPCTSTR Port,int BaudRate,int DataBits,int StopBits,int Parity)
{
//	m_lpDataArriveProc=proc;
//	m_dwUserData=userdata;

	if(m_hComm==INVALID_HANDLE_VALUE)
	{
		m_hComm=CreateFile(Port,GENERIC_READ|GENERIC_WRITE,0,0,OPEN_EXISTING,0,0);
		if(m_hComm==INVALID_HANDLE_VALUE )
		{
			pApp->m_dlgOK.m_sText = _T("无法打开X射线机通讯端口！请检查是否已被占用。");
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
		
		//设置状态参数
		SetCommMask(m_hComm,EV_RXCHAR);		
		SetupComm(m_hComm,16384,16384);		
		if(!SetCommState(m_hComm,&dcb))
		{
			pApp->m_dlgOK.m_sText = _T("无法按当前参数配置端口，请检查参数！");
			pApp->m_dlgOK.DoModal();
			PurgeComm(m_hComm,PURGE_TXCLEAR|PURGE_RXCLEAR);
			ClosePort();
			return FALSE;
		}
		
		//设置超时参数
		GetCommTimeouts(m_hComm,&CommTimeOuts);		
		CommTimeOuts.ReadIntervalTimeout=100;
		CommTimeOuts.ReadTotalTimeoutMultiplier=1;
		CommTimeOuts.ReadTotalTimeoutConstant=100;
		CommTimeOuts.WriteTotalTimeoutMultiplier=0;
		CommTimeOuts.WriteTotalTimeoutConstant=0;		
		if(!SetCommTimeouts(m_hComm,&CommTimeOuts))
		{
			pApp->m_dlgOK.m_sText = _T("无法设置超时参数！");
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

BOOL CPSerialPortX::ClosePort()
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

BOOL CPSerialPortX::Activate()
{
	if(m_hComm==INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	if(!m_bReceiving)
	{
		//开始接收线程
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

BOOL CPSerialPortX::Deactivate()
{
	if(m_hComm==INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	//停止接收线程
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

BOOL CPSerialPortX::IsActive()
{
	return m_bReceiving;
}

DWORD CPSerialPortX::WritePort(char *data,int length)
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

DWORD CPSerialPortX::ReadPort(char *data,int length)
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

DWORD CPSerialPortX::WriteFileToPort(LPCTSTR FileName)
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
//		AfxMessageBox(_T("无法打开Hex文件！"));
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
//			AfxMessageBox(_T("无法向端口写入数据！"));
		}
		delete[] buf;		
	}
	cf.Close();
	return dwBytesWritten;
}

/*
	X射线机通讯协议
	Command：\02MON\r
	Voltage/Current/Temperature/Filament Monitor
	<STX>VVVV<SP>CCCC<SP>TTTT<SP> FFFF<CR>
	VVVV = 000.0–Max KV
CCCC = 0000 –Max uA
TTTT = 000.0 – 070.0 DegC
FFFF = 0000 – 4095
*/
BOOL CPSerialPortX::ReadXRadial()
{
	m_DataState				= DataReceiveWait;

	DWORD dwBytesWritten=0;
	
	char buf[5];
	buf[0]	= 0x02;
	buf[1]	= 'M';
	buf[2]	= 'O';
	buf[3]	= 'N';
	buf[4]	= 0x0D;

	int nTry(3);
	for(int n=1; n<=nTry; n++)
	{
		dwBytesWritten = WritePort(buf,5);
		if(dwBytesWritten==0)
		{
			if(n==nTry)
			{
				pApp->m_dlgOK.m_sText = _T("采样控制命令发送失败！");
				pApp->m_dlgOK.DoModal();
				return FALSE;
			}
			else
			{
				Sleep(500);
				continue;
			}
		}
		else
		{
			for(int i=0; i<5; i++)
			{
				Sleep(500);
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
					pApp->m_dlgOK.m_sText = _T("X射线机读取状态数据接收错误！");
					pApp->m_dlgOK.DoModal();
					return FALSE;
				}
				else if(m_DataState==DataReceiveLengthError)
				{
					pApp->m_dlgOK.m_sText = _T("X射线机读取状态数据接收长度错误！");
					pApp->m_dlgOK.DoModal();
					return FALSE;
				}
				else
				{
					pApp->m_dlgOK.m_sText = _T("X射线机读取状态命令无应答！");
					pApp->m_dlgOK.DoModal();
					return FALSE;
				}
			}
		}	
	}
	return FALSE;
}

BOOL CPSerialPortX::SetXV(UINT nValue)
{
	m_bEchoReceiveSuccess	= FALSE;

	DWORD dwBytesWritten=0;
	
	char buf[8];
	buf[0]	= 0x02;
	buf[1]	= 'V';
	buf[2]	= 'P';
	buf[3]	= '0';
	buf[4]	= nValue/10+0x30;
	buf[5]	= nValue%10+0x30;
	buf[6]	= '0';
	buf[7]	= 0x0D;

//	CString s;
//	s.Format(_T("发送V：%02d%02d%02d%02d%02d%02d%02d"),buf[0],buf[1],buf[2],buf[3],buf[4],buf[5],buf[6],buf[7]);
//	AfxMessageBox(s);

	int nTry(3);
	for(int n=1; n<=nTry; n++)
	{
		dwBytesWritten = WritePort(buf,8);
		if(dwBytesWritten==0)
		{
			if(n==nTry)
			{
				pApp->m_dlgOK.m_sText = _T("X射线机设置电压控制命令发送失败！");
				pApp->m_dlgOK.DoModal();
			}
			else
			{
				Sleep(500);
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
				pApp->m_dlgOK.m_sText = _T("X射线机设置电压控制命令无应答！");
				pApp->m_dlgOK.DoModal();
			}
		}
	}
	return FALSE;
}

BOOL CPSerialPortX::SetXA(UINT nValue)
{
	m_bEchoReceiveSuccess	= FALSE;

	DWORD dwBytesWritten=0;
	
	char buf[8];
	buf[0]	= 0x02;
	buf[1]	= 'C';
	buf[2]	= 'P';
	buf[3]	= '0';
	buf[4]	= nValue/10+0x30;
	buf[5]	= nValue%10+0x30;
	buf[6]	= '0';
	buf[7]	= 0x0D;

//	CString s;
//	s.Format(_T("发送A：%02d%02d%02d%02d%02d%02d%02d"),buf[0],buf[1],buf[2],buf[3],buf[4],buf[5],buf[6],buf[7]);
//	AfxMessageBox(s);

	int nTry(3);
	for(int n=1; n<=nTry; n++)
	{
		dwBytesWritten = WritePort(buf,8);
		if(dwBytesWritten==0)
		{
			if(n==nTry)
			{
				pApp->m_dlgOK.m_sText = _T("X射线机设置电流控制命令发送失败！");
				pApp->m_dlgOK.DoModal();
			}
			else
			{
				Sleep(500);
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
				pApp->m_dlgOK.m_sText = _T("X射线机设置电流控制命令无应答！");
				pApp->m_dlgOK.DoModal();
			}
		}
	}
	return FALSE;
}