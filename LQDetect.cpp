// LQDetect.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "LQDetect.h"
#include "LQDetectDlg.h"
#include "DialogSplash.h"
#include "DialogDetect.h"

#include "CppSQLite3U.h"		//SQLite3头文件

//参数设置
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
CMutex g_mutex(FALSE, _T("LQParameter"));	//参数设置
#else
CMutex g_mutex(FALSE, _T("LQDetect"));		//液体检测
#endif

BOOL CLQDetectApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.
	
//	m_dlgOK.m_sText = L"注意：增加的容器数据将无法删除，请您仔细核对数据，确保准确无误。是否继续执行增加操作？";
//	m_dlgOK.DoModal();
//	m_dlgOKCANCEL.m_sText = L"注意：增加的容器数据将无法删除，请您仔细核对数据，确保准确无误。是否继续执行增加操作？";
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
		m_dlgOK.m_sText = _T("控制板通讯口初始化失败，无法进行测试！");
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
		m_dlgOK.m_sText = _T("X射线机通讯口初始化失败，无法读取射线机状态值！");
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
	//数组清空
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
		db.open(m_strDbPath);		//打开或新建一个数据库

		CppSQLite3Query q;
		//获取液体定义
		q = db.execQuery(L"select * from Liquid order by liquid_id");
		while (!q.eof())
        {
			structLiquid* pLiquid	= new structLiquid;
			pLiquid->liquid_id		= q.fieldValue(0);		//液体ID
			pLiquid->liq_name		= q.fieldValue(1);		//液体名称
			pLiquid->showcolor		= q.fieldValue(2);		//显示颜色
			pLiquid->showenable		= q.fieldValue(3);		//是否显示
			pLiquid->issafe			= q.fieldValue(4);		//特性（是否安全品）
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
		m_dlgOK.m_sText = _T("载入液体数据失败！");
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
		db.open(m_strDbPath);		//打开或新建一个数据库

		CppSQLite3Query q;
		//获取容器数据
		q = db.execQuery(L"select * from Container order by container_id");
		while (!q.eof())
        {
			structContainer* pContainer	= new structContainer;
			pContainer->container_id	= q.fieldValue(0);	//容器ID
			pContainer->con_name		= q.fieldValue(1);	//容器名称
			pContainer->thresholddown	= q.fieldValue(2);	//多道阈值下限
			pContainer->thresholdup		= q.fieldValue(3);	//多道阈值上限
			pContainer->time			= q.fieldValue(4);	//采样时间
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
		m_dlgOK.m_sText = _T("载入容器数据失败！");
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
		db.open(m_strDbPath);		//打开或新建一个数据库

/*
		//生成样本数据
		CString sSQL;
		db.execDML(L"delete from swatch");
//		AfxMessageBox(_T("生成样本数据"));
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
		//获取样本数据
		q = db.execQuery(L"select * from Swatch order by channel,con_name,liq_name");
		while (!q.eof())
        {
			structSwatch* pSwatch	= new structSwatch;
			pSwatch->channel		= q.fieldValue(0);	//通道
			pSwatch->liq_name		= q.fieldValue(1);	//液体名称
			pSwatch->con_name		= q.fieldValue(2);	//容器名称
			pSwatch->countup		= q.fieldValue(3);	//计数上限
			pSwatch->countdown		= q.fieldValue(4);	//计数下限
			pSwatch->countaverage	= q.fieldValue(5);	//计数平均
			pSwatch->countrange		= q.fieldValue(6);	//计数范围
			pSwatch->issafe			= q.fieldValue(7);	//特性（安全品/可疑品）
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
		m_dlgOK.m_sText = _T("载入样本数据失败！");
		m_dlgOK.DoModal();
		return;
	}
}

void CLQDetectApp::LoadDataFromParameter()
{
	CppSQLite3DB db;
	try
	{
		db.open(m_strDbPath);		//打开或新建一个数据库

		CppSQLite3Query q;

		CString sMsg, s;
		//获取参数数据
		q = db.execQuery(L"select * from parameter where parameterid='channelinterface1'");
		m_para_channelinterface1	= _ttoi(q.fieldValue(1));		//通道对应接口(默认1可选择1/3)
		q.finalize();

		q = db.execQuery(L"select * from parameter where parameterid='channelinterface2'");
		m_para_channelinterface2	= _ttoi(q.fieldValue(1));		//通道对应接口(默认2可选择2/4)
		q.finalize();

		q = db.execQuery(L"select * from parameter where parameterid='standardratio1'");
		s = q.fieldValue(1);
		unsigned short *num1;
		m_para_standardratio1	= wcstod (s,&num1);					//通道1标准块标定比值系数(默认1)
		q.finalize();

		q = db.execQuery(L"select * from parameter where parameterid='standardratio2'");
		s = q.fieldValue(1);
		unsigned short *num2;
		m_para_standardratio2	= wcstod (s,&num2);					//通道2标准块标定比值系数(默认1)
		q.finalize();

		q = db.execQuery(L"select * from parameter where parameterid='thresholdbegin'");
		m_para_thresholdbegin	= _ttoi(q.fieldValue(1));			//采样阈值启始点(默认100)
		q.finalize();

		q = db.execQuery(L"select * from parameter where parameterid='thresholdend'");
		m_para_thresholdend	= _ttoi(q.fieldValue(1));				//采样阈值结束点(默认400)
		q.finalize();

		q = db.execQuery(L"select * from parameter where parameterid='samplingtimes'");
		m_para_samplingtimes	= _ttoi(q.fieldValue(1));			//采样次数(默认3)
		q.finalize();
		
		q = db.execQuery(L"select * from parameter where parameterid='totaldetecttimes'");
		m_para_totaldetecttimes	= _ttoi(q.fieldValue(1));			//总检测次数
		q.finalize();

		db.close();
	}
	catch(CppSQLite3Exception ex)
	{
		m_dlgOK.m_sText = ex.errorMessage();
		m_dlgOK.DoModal();
		m_dlgOK.m_sText = _T("载入参数数据失败！");
		m_dlgOK.DoModal();
		return;
	}
}

//获取程序当前路径
void CLQDetectApp::GetCurrentDirectory(CString &szPath)
{
	wchar_t pBuf[256];
	GetModuleFileName(NULL,pBuf,sizeof(pBuf)/sizeof(wchar_t));
	szPath=pBuf;
	szPath = szPath.Left(szPath.ReverseFind('\\')+1);
}
