#pragma once

// Forward class declarations.
class CControlWnd;

// CFatoryControlWnd �Ի���

class CFatoryControlWnd : public CDialogEx
{
	DECLARE_DYNAMIC(CFatoryControlWnd)

public:
	CFatoryControlWnd(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFatoryControlWnd();

	void OpenXmlFile(const CString& xmlFile);
// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CONTROL_WND };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()

private:
	CControlWnd* m_controlWnd;
public:
	virtual BOOL OnInitDialog();
};
