// FatoryControlWnd.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFCFactory.h"
#include "FatoryControlWnd.h"
#include "afxdialogex.h"


// CFatoryControlWnd �Ի���

IMPLEMENT_DYNAMIC(CFatoryControlWnd, CDialogEx)

CFatoryControlWnd::CFatoryControlWnd(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CONTROL_WND, pParent)
{

}

CFatoryControlWnd::~CFatoryControlWnd()
{
}

void CFatoryControlWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFatoryControlWnd, CDialogEx)
END_MESSAGE_MAP()


// CFatoryControlWnd ��Ϣ�������
