// LQDetect.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "LQDetect.h"
#include "LQDetectDlg.h"
#include "DialogSplash.h"
#include "DialogDetect.h"

#include "CppSQLite3U.h"		//SQLite3ͷ�ļ�

//��������
#include "DialogPassword.h"
#include "DialogParameter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLQDetectApp

BEGIN_MESSAGE_MAP(CLQDetectApp, CWinApp)
	//{{AFX_MSG_MAP(CLQDetectApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLQDetectApp construction

CLQDetectApp::CLQDetectApp()
	: CWinApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CLQDetectApp object

CLQDetectApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CLQDetectApp initialization
#ifdef VERSION_SET_PARAMETER
CMutex g_mutex(FALSE, _T("LQParameter"));	//��������
#else
CMutex g_mutex(FALSE, _T("LQDetect"));		//Һ����
#endif

BOOL CLQDetectApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.
	
//	m_dlgOK.m_sText = L"ע�⣺���ӵ��������ݽ��޷�ɾ����������ϸ�˶����ݣ�ȷ��׼ȷ�����Ƿ����ִ�����Ӳ�����";
//	m_dlgOK.DoModal();
//	m_dlgOKCANCEL.m_sText = L"ע�⣺���ӵ��������ݽ��޷�ɾ����������ϸ�˶����ݣ�ȷ��׼ȷ�����Ƿ����ִ�����Ӳ�����";
//	if(m_dlgOKCANCEL.DoModal()==IDCANCEL) return FALSE;

	if(!g_mutex.Lock(1000))
	{
		return FALSE;
	}

	GetCurrentDirectory(m_CurrentDir);
	m_strDbPath = m_CurrentDir+FILE_DB_NAME;

	LoadDataFromLiquid();
	LoadDataFromContainer();
	LoadDataFromSwatch();
	LoadDataFromParameter();

	CDialogSplash	dlgSplash;
	dlgSplash.DoModal();

    // open the serial port
    bool isOpen=false;

	isOpen	= m_Serial.OpenPort(_T("COM1:"),115200,8,ONESTOPBIT,NOPARITY);
	if(!isOpen || !m_Serial.Activate())
	{
		m_dlgOK.m_sText = _T("���ư�ͨѶ�ڳ�ʼ��ʧ�ܣ��޷����в��ԣ�");
		m_dlgOK.DoModal();
        m_serialIsOk = false;
        m_SerialP = NULL;
    } else {
        m_SerialP = &m_Serial;
        m_serialIsOk = true;
    }
	
	isOpen = m_SerialX.OpenPort(_T("COM2:"),9600,8,ONESTOPBIT,NOPARITY);
	if(!m_bOpen || !m_SerialX.Activate())
	{
		m_dlgOK.m_sText = _T("X���߻�ͨѶ�ڳ�ʼ��ʧ�ܣ��޷���ȡ���߻�״ֵ̬��");
		m_dlgOK.DoModal();
        m_serialXIsOk = false;
        m_SerialXP = NULL;
    } else {
        m_SerialXP = &m_SerialX;
        m_serialXIsOk = true;
    }
	
	int nResponse;

#ifdef VERSION_SET_PARAMETER
	CDialogPassword dlgPass;
	if(dlgPass.DoModal()==IDOK)
	{
		CDialogParameter dlg;
		m_pMainWnd = &dlg;
		nResponse = dlg.DoModal();
	}
#else
	CDialogGeneral dlgGeneral;
	m_pMainWnd = &dlgGeneral;
	nResponse = dlgGeneral.DoModal();
//	if (nResponse == IDOK)
//	{
//		// TODO: Place code here to handle when the dialog is
//		//  dismissed with OK
//	}
//	else if (nResponse == IDCANCEL)
//	{
//		CLQDetectDlg dlg;
//		m_pMainWnd = &dlg;
//		nResponse = dlg.DoModal();
//		// TODO: Place code here to handle when the dialog is
//		//  dismissed with Cancel
//	}
#endif
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	ClearArrayData();
	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

void CLQDetectApp::ClearArrayData()
{
	//�������
	for(int i=0; i<m_arrayLiquid.GetSize(); i++)
	{
		delete m_arrayLiquid[i];
	}
	m_arrayLiquid.RemoveAll();
	
	for(i=0; i<m_arrayContainer.GetSize(); i++)
	{
		delete m_arrayContainer[i];
	}
	m_arrayContainer.RemoveAll();

	for(i=0; i<m_arraySwatch.GetSize(); i++)
	{
		delete m_arraySwatch[i];
	}
	m_arraySwatch.RemoveAll();
}

void CLQDetectApp::LoadDataFromLiquid()
{
	for(int i=0; i<m_arrayLiquid.GetSize(); i++)
	{
		delete m_arrayLiquid[i];
	}
	m_arrayLiquid.RemoveAll();

	CppSQLite3DB db;
	try
	{
		db.open(m_strDbPath);		//�򿪻��½�һ�����ݿ�

		CppSQLite3Query q;
		//��ȡҺ�嶨��
		q = db.execQuery(L"select * from Liquid order by liquid_id");
		while (!q.eof())
        {
			structLiquid* pLiquid	= new structLiquid;
			pLiquid->liquid_id		= q.fieldValue(0);		//Һ��ID
			pLiquid->liq_name		= q.fieldValue(1);		//Һ������
			pLiquid->showcolor		= q.fieldValue(2);		//��ʾ��ɫ
			pLiquid->showenable		= q.fieldValue(3);		//�Ƿ���ʾ
			pLiquid->issafe			= q.fieldValue(4);		//���ԣ��Ƿ�ȫƷ��
			m_arrayLiquid.Add(pLiquid);
			q.nextRow();
		}
		q.finalize();
		
		db.close();
	}
	catch(CppSQLite3Exception ex)
	{
		m_dlgOK.m_sText = ex.errorMessage();
		m_dlgOK.DoModal();
		m_dlgOK.m_sText = _T("����Һ������ʧ�ܣ�");
		m_dlgOK.DoModal();
		return;
	}
}

void CLQDetectApp::LoadDataFromContainer()
{
	for(int i=0; i<m_arrayContainer.GetSize(); i++)
	{
		delete m_arrayContainer[i];
	}
	m_arrayContainer.RemoveAll();

	CppSQLite3DB db;
	try
	{
		db.open(m_strDbPath);		//�򿪻��½�һ�����ݿ�

		CppSQLite3Query q;
		//��ȡ��������
		q = db.execQuery(L"select * from Container order by container_id");
		while (!q.eof())
        {
			structContainer* pContainer	= new structContainer;
			pContainer->container_id	= q.fieldValue(0);	//����ID
			pContainer->con_name		= q.fieldValue(1);	//��������
			pContainer->thresholddown	= q.fieldValue(2);	//�����ֵ����
			pContainer->thresholdup		= q.fieldValue(3);	//�����ֵ����
			pContainer->time			= q.fieldValue(4);	//����ʱ��
			m_arrayContainer.Add(pContainer);
			q.nextRow();
		}
		q.finalize();
		
		db.close();
	}
	catch(CppSQLite3Exception ex)
	{
		m_dlgOK.m_sText = ex.errorMessage();
		m_dlgOK.DoModal();
		m_dlgOK.m_sText = _T("������������ʧ�ܣ�");
		m_dlgOK.DoModal();
		return;
	}
}

void CLQDetectApp::LoadDataFromSwatch()
{
	for(int i=0; i<m_arraySwatch.GetSize(); i++)
	{
		delete m_arraySwatch[i];
	}
	m_arraySwatch.RemoveAll();

	CppSQLite3DB db;
	try
	{
		db.open(m_strDbPath);		//�򿪻��½�һ�����ݿ�

/*
		//������������
		CString sSQL;
		db.execDML(L"delete from swatch");
//		AfxMessageBox(_T("������������"));
		db.execDML(L"begin transaction;");
//		for(int i=0; i<2; i++)
//		{
//			for(int j=0; j<m_arrayLiquid.GetSize(); j++)
//			{
//				for(int k=0; k<m_arrayContainer.GetSize(); k++)
//				{
//					sSQL.Format(_T("insert into swatch values(%d, '%s', '%s', 0, 0, 0, 0, %s)"), i+1, m_arrayLiquid[j]->liq_name, m_arrayContainer[k]->con_name, m_arrayLiquid[j]->issafe);
//					db.execDML(sSQL);
//				}
//			}
//		}
		for(int i=0; i<4; i++)
		{
			for(int j=0; j<m_arrayContainer.GetSize(); j++)
			{
				for(int k=0; k<m_arrayLiquid.GetSize(); k++)
				{
					sSQL.Format(_T("insert into swatch values(%d, '%s', '%s', 0, 0, 0, 0, %s)"), i+1, m_arrayLiquid[k]->liq_name, m_arrayContainer[j]->con_name, m_arrayLiquid[k]->issafe);
					db.execDML(sSQL);
				}
			}
		}
		db.execDML(L"commit transaction;");
*/

		CppSQLite3Query q;
		//��ȡ��������
		q = db.execQuery(L"select * from Swatch order by channel,con_name,liq_name");
		while (!q.eof())
        {
			structSwatch* pSwatch	= new structSwatch;
			pSwatch->channel		= q.fieldValue(0);	//ͨ��
			pSwatch->liq_name		= q.fieldValue(1);	//Һ������
			pSwatch->con_name		= q.fieldValue(2);	//��������
			pSwatch->countup		= q.fieldValue(3);	//��������
			pSwatch->countdown		= q.fieldValue(4);	//��������
			pSwatch->countaverage	= q.fieldValue(5);	//����ƽ��
			pSwatch->countrange		= q.fieldValue(6);	//������Χ
			pSwatch->issafe			= q.fieldValue(7);	//���ԣ���ȫƷ/����Ʒ��
			m_arraySwatch.Add(pSwatch);
			q.nextRow();
		}
		q.finalize();
		
		db.close();
	}
	catch(CppSQLite3Exception ex)
	{
		m_dlgOK.m_sText = ex.errorMessage();
		m_dlgOK.DoModal();
		m_dlgOK.m_sText = _T("������������ʧ�ܣ�");
		m_dlgOK.DoModal();
		return;
	}
}

void CLQDetectApp::LoadDataFromParameter()
{
	CppSQLite3DB db;
	try
	{
		db.open(m_strDbPath);		//�򿪻��½�һ�����ݿ�

		CppSQLite3Query q;

		CString sMsg, s;
		//��ȡ��������
		q = db.execQuery(L"select * from parameter where parameterid='channelinterface1'");
		m_para_channelinterface1	= _ttoi(q.fieldValue(1));		//ͨ����Ӧ�ӿ�(Ĭ��1��ѡ��1/3)
		q.finalize();

		q = db.execQuery(L"select * from parameter where parameterid='channelinterface2'");
		m_para_channelinterface2	= _ttoi(q.fieldValue(1));		//ͨ����Ӧ�ӿ�(Ĭ��2��ѡ��2/4)
		q.finalize();

		q = db.execQuery(L"select * from parameter where parameterid='standardratio1'");
		s = q.fieldValue(1);
		unsigned short *num1;
		m_para_standardratio1	= wcstod (s,&num1);					//ͨ��1��׼��궨��ֵϵ��(Ĭ��1)
		q.finalize();

		q = db.execQuery(L"select * from parameter where parameterid='standardratio2'");
		s = q.fieldValue(1);
		unsigned short *num2;
		m_para_standardratio2	= wcstod (s,&num2);					//ͨ��2��׼��궨��ֵϵ��(Ĭ��1)
		q.finalize();

		q = db.execQuery(L"select * from parameter where parameterid='thresholdbegin'");
		m_para_thresholdbegin	= _ttoi(q.fieldValue(1));			//������ֵ��ʼ��(Ĭ��100)
		q.finalize();

		q = db.execQuery(L"select * from parameter where parameterid='thresholdend'");
		m_para_thresholdend	= _ttoi(q.fieldValue(1));				//������ֵ������(Ĭ��400)
		q.finalize();

		q = db.execQuery(L"select * from parameter where parameterid='samplingtimes'");
		m_para_samplingtimes	= _ttoi(q.fieldValue(1));			//��������(Ĭ��3)
		q.finalize();
		
		q = db.execQuery(L"select * from parameter where parameterid='totaldetecttimes'");
		m_para_totaldetecttimes	= _ttoi(q.fieldValue(1));			//�ܼ�����
		q.finalize();

		db.close();
	}
	catch(CppSQLite3Exception ex)
	{
		m_dlgOK.m_sText = ex.errorMessage();
		m_dlgOK.DoModal();
		m_dlgOK.m_sText = _T("�����������ʧ�ܣ�");
		m_dlgOK.DoModal();
		return;
	}
}

//��ȡ����ǰ·��
void CLQDetectApp::GetCurrentDirectory(CString &szPath)
{
	wchar_t pBuf[256];
	GetModuleFileName(NULL,pBuf,sizeof(pBuf)/sizeof(wchar_t));
	szPath=pBuf;
	szPath = szPath.Left(szPath.ReverseFind('\\')+1);
}
