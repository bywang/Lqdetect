#if !defined(AFX_DIALOGPARAMETER_H__836E7D0B_3D5E_4FDA_AA0D_906397E45CE7__INCLUDED_)
#define AFX_DIALOGPARAMETER_H__836E7D0B_3D5E_4FDA_AA0D_906397E45CE7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogParameter.h : header file
//

#include "PSerialPort.h"

//�ɼ����
typedef struct
{
	UINT	channel;							//ͨ���ӿ�
	CString	con_name;							//��������
	CString	liq_name;							//Һ������
	UINT	CollectionTimes;					//�ɼ�������ÿ�ζ������ƽ����
	UINT	MaxCount;							//������
	UINT	MinCount;							//��С����
	UINT	AverageCount;						//ƽ������
	UINT	RangeCount;							//������Χ
	UINT	MaxValue;							//�����ֵ
	double	AverageSamplingData[1024];			//ƽ����������
} structSamplingResult;

//��������
typedef struct
{
	UINT	channel;					//ͨ���ӿ�
	CString	con_name;					//��������
	CString	liq_name;					//Һ������
//	UINT	ThresholdBegin;				//��ֵ���
//	UINT	ThresholdEnd;				//��ֵ�յ�
	UINT	ThresholdSamplingCount;		//�������ڼ���ֵ
	UINT	MaxValue;					//�����ֵ
	UINT	SamplingData[1024];			//��������
} structSamplingData;

typedef CArray<structSamplingResult*,	structSamplingResult*> CSamplingResultArray;
typedef CArray<structSamplingData*,	structSamplingData*> CSamplingDataArray;

/////////////////////////////////////////////////////////////////////////////
// CDialogParameter dialog
class CDialogParameter : public CDialog
{
// Construction
public:
	CDialogParameter(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDialogParameter();

public:
	CLQDetectApp	*pApp;
	HACCEL			m_hAccel;

	CSamplingResultArray	m_arraySamplingResult;	//�ɼ����
	CSamplingDataArray		m_arraySamplingData;	//�ɼ���ϸ����

	BOOL	m_bChannel1;				//ͨ��1
	BOOL	m_bChannel2;				//ͨ��2
	UINT	m_nSelChannelInterface;		//ͨ����Ӧ�ӿ�
	BOOL	m_bDrawSS;					//������������ʾ
	BOOL	m_bDrawSM;					//�����������ʾ
	BOOL	m_bDrawMS;					//�����������ʾ
	UINT	m_nTempSamplingData[1024];	//������ʱ����
	double	m_fAverageSamplingData[1024];	//����ƽ������

	UINT	m_nSecond;					//�������

	BOOL	m_bKeyboardEnable;			//�Ƿ���ʾ����
	BOOL	m_bKeyboardShowed;			//��ʾ����
	UINT	m_nCurrentMaxCount;			//��ǰ����������ֵ
	UINT	m_nCurrentMinCount;			//��ǰ������С����ֵ
	UINT	m_nCurrentMaxValue;			//��ǰ���������ֵ�����ڻ�ͼ��
	UINT	m_nCurrentAverageCount;		//��ǰ����ƽ������ֵ
	UINT	m_nCurrentSamplingNumber;	//��ǰ������������

	double	m_fDrawData[1024];			//��ͼ����

	void	SetControls();
	void	EnableControls();
	void	DisableControls();
	void	LoadSwatchFromArray();

	void	InsertSamplingDataToArray();				//�������ݴ������飬����������
	void	LoadDetectValueFromSamplingDataArray();		//ѭ����ѯ�������ݣ����㲢��ʾ�������
	void	UpdateSamplingResultToArray();				//����������»�����������
	void	RecountSamplingDataArray();					//���µ��������¼����������

	void	ClearArrayData();
	
//	double	MiddleValue(double Data[],int n);
	
	//���������
	CPSerialPort	m_Serial;
	BOOL			m_bOpen;


	void	MessageUpdateData(BOOL bUpdateData = TRUE);
	BOOL	m_bUpdateData;				//����Paint;
	BOOL	m_bPaint;

	//���������̺߳���
	static DWORD WINAPI AutoReadPortThread(LPVOID lpParameter);
	BOOL	m_bWaitThread;				//�ȴ��߳�
	BOOL	m_bReadValueTest;			//������
	HANDLE	m_hReadThread;				//�������߳�handle

//	// ������ɫ
//	COLORREF *m_Color;

	// ��ʾˢ��
	// ��ͼ
	UINT	m_nDrawIndexSS;	//���������λ�����ţ���1-20����
	void	DrawImageSS();	//����������
	void	DrawImageSM();	//���������
	void	DrawImageMS();	//����������

// Dialog Data
	//{{AFX_DATA(CDialogParameter)
	enum { IDD = IDD_DIALOG_PARAMETER };
	CListCtrl	m_listSwatch;
	CListCtrl	m_listDetectValue;
	CString	m_sLiquid;
	CString	m_sSecond;
	CString	m_sContainer;
	UINT	m_iCountDown;
	UINT	m_iCountUp;
	UINT	m_iCountAverage;
	CString	m_sInfo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogParameter)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:

	// Generated message map functions
	//{{AFX_MSG(CDialogParameter)
	virtual BOOL OnInitDialog();
	afx_msg void OnEnter();
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnRadioChannel1();
	afx_msg void OnRadioChannel2();
	afx_msg void OnRadioDrawSS();
	afx_msg void OnRadioDrawSM();
	afx_msg void OnRadioDrawMS();
	afx_msg void OnSelchangeComboSecond();
	virtual void OnOK();
	afx_msg void OnButtonAutoDetect();
	afx_msg void OnButtonSetDisplay();
	afx_msg void OnClickListDetectValue(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickListSwatch(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonDeleteSwatch();
	afx_msg void OnButtonSaveSwatchModify();
	afx_msg void OnButtonDeleteDetectValue();
	afx_msg void OnButtonUpdateSwatch();
	afx_msg void OnButtonOtherParameter();
	virtual void OnCancel();
	afx_msg void OnSelchangeComboContainer();
	afx_msg void OnButtonManualDetect();
	afx_msg void OnSetfocusEditCountup();
	afx_msg void OnSetfocusEditCountdown();
	afx_msg void OnSetfocusEditCountaverage();
	afx_msg void OnKillfocusEditCountup();
	afx_msg void OnKillfocusEditCountdown();
	afx_msg void OnKillfocusEditCountaverage();
	afx_msg void OnSelchangeComboLiquid();
	afx_msg void OnButtonLiquid();
	afx_msg void OnButtonContainer();
	afx_msg void OnButtonDeleteallDetectValue();
	afx_msg void OnButtonSaveSamplingData();
	afx_msg void OnButtonLoadSamplingData();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGPARAMETER_H__836E7D0B_3D5E_4FDA_AA0D_906397E45CE7__INCLUDED_)
