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

#include "DialogOK.h"			//�Զ���ȷ�Ͽ�
#include "DialogOKCANCEL.h"		//�Զ���ѡ���
#include "CppSQLite3U.h"		//SQLite3ͷ�ļ�
#include "PSerialPort.h"
#include "PSerialPortX.h"

/////////////////////////////////////////////////////////////////////////////
// CLQDetectApp:
// See LQDetect.cpp for the implementation of this class
//
//#define SHOW_DEBUG_MESSAGE				//��ʾ������Ϣ

//#define VERSION_SET_PARAMETER				//���ò����汾������ʹ�ã�

#define FILE_DB_NAME	TEXT("LQData.db")	//���ݿ�����

#define DataReceiveLengthError		99		//���ݽ��ճ��ȴ���
#define DataReceiveError			0		//���ݽ��մ���
#define DataReceiveOK				1		//���ݽ��ճɹ�
#define DataReceiveWait				-1		//���ݽ��յȴ�

//Һ������
typedef struct
{
	CString	liquid_id;		//Һ��ID
	CString	liq_name;		//Һ������
	CString	showcolor;		//��ʾ��ɫ
	CString	showenable;		//�Ƿ���ʾ
	CString	issafe;			//���ԣ��Ƿ�ȫƷ��
} structLiquid;

//��������
typedef struct
{
	CString	container_id;	//����ID
	CString	con_name;		//��������
	CString thresholddown;	//�����ֵ����
	CString thresholdup;	//�����ֵ����
	CString time;			//����ʱ��
} structContainer;

//��������
typedef struct
{
	CString	channel;		//ͨ��
	CString	liq_name;		//Һ������
	CString	con_name;		//��������
	CString	countup;		//��������
	CString	countdown;		//��������
	CString	countaverage;	//����ƽ��
	CString	countrange;		//������Χ
	CString	issafe;			//���ԣ���ȫƷ/����Ʒ��
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
	CString			m_CurrentDir;				//��ǰ����Ŀ¼
	CString			m_strDbPath;				//���ݿ�����Ŀ¼

    // serial data

    bool m_serialIsOk;
    CPSerialPort *m_SerialP;
    CPSerialPort m_Serial;
    bool m_serialXIsOk;
    CPSerialPortX *m_SerialXP;
    CPSerialPortX m_SerialX;

	//��������
	UINT			m_para_channelinterface1;	//ͨ��1ʹ�ð��Ͻӿ�
	UINT			m_para_channelinterface2;	//ͨ��2ʹ�ð��Ͻӿ�
	double			m_para_standardratio1;		//ͨ��1��׼��궨��ֵϵ��;
	double			m_para_standardratio2;		//ͨ��2��׼��궨��ֵϵ��;
	UINT			m_para_thresholdbegin;		//������ֵ��ʼ��
	UINT			m_para_thresholdend;		//������ֵ������
	UINT			m_para_samplingtimes;		//��������;
	UINT			m_para_totaldetecttimes;	//�ܼ�����;

	CLiquidArray	m_arrayLiquid;				//Һ������
	CContainerArray	m_arrayContainer;			//��������
	CSwatchArray	m_arraySwatch;				//��������

	void	LoadDataFromLiquid();				//�����ݿ�������Һ������
	void	LoadDataFromContainer();			//�����ݿ���������������
	void	LoadDataFromSwatch();				//�����ݿ���������������
	void	LoadDataFromParameter();			//�����ݿ��������������
	void	ClearArrayData();					//���Array
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
