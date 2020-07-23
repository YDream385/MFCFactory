#pragma once

// Forward class declarations.
class CControlWnd;

// CFatoryControlWnd 对话框

class CFatoryControlWnd : public CDialogEx
{
	DECLARE_DYNAMIC(CFatoryControlWnd)

public:
	CFatoryControlWnd(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFatoryControlWnd();

	void OpenXmlFile(const CString& xmlFile);
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CONTROL_WND };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()

private:
	CControlWnd* m_controlWnd;
public:
	virtual BOOL OnInitDialog();
};
