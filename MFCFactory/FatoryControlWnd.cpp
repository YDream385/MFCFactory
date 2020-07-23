// FatoryControlWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCFactory.h"
#include "FatoryControlWnd.h"
#include "afxdialogex.h"
#include "ControlWnd.h"

// CFatoryControlWnd 对话框

IMPLEMENT_DYNAMIC(CFatoryControlWnd, CDialogEx)

CFatoryControlWnd::CFatoryControlWnd(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CONTROL_WND, pParent)
{
	m_controlWnd = NULL;
}

CFatoryControlWnd::~CFatoryControlWnd()
{
	m_controlWnd = NULL;
}

void CFatoryControlWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFatoryControlWnd, CDialogEx)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CFatoryControlWnd 消息处理程序


BOOL CFatoryControlWnd::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Create the control window instance and GUI.
	CRect rect(0, 0, 0, 0);
	m_controlWnd = new CControlWnd;
	BOOL status = m_controlWnd->Create(NULL, NULL, WS_VISIBLE | WS_CHILD, rect,
		this, 1002);
	if (!status)
		return FALSE;

	OpenXmlFile(_T("D:\\work\\MFCFactory\\TestFreePool\\Example1.xml"));

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CFatoryControlWnd::OpenXmlFile(const CString& xmlFile)
{
	if (m_controlWnd != NULL && ::IsWindow(m_controlWnd->m_hWnd))
	{
		m_controlWnd->BuildFromXml(xmlFile);
	}
}

void CFatoryControlWnd::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// Resize and reposition the control dialog.
	if (m_controlWnd != NULL && ::IsWindow(m_controlWnd->m_hWnd))
	{
		CRect rect;
		GetClientRect(&rect);

		m_controlWnd->SetWindowPos(NULL, rect.left, rect.top,
			rect.Width(), rect.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
	}
}