#pragma once


// CFatoryControlWnd �Ի���

class CFatoryControlWnd : public CDialogEx
{
	DECLARE_DYNAMIC(CFatoryControlWnd)

public:
	CFatoryControlWnd(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFatoryControlWnd();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CONTROL_WND };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
