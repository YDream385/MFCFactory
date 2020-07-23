
// MFCFactoryView.cpp : CMFCFactoryView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "MFCFactory.h"
#endif

#include "MFCFactoryDoc.h"
#include "MFCFactoryView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCFactoryView

IMPLEMENT_DYNCREATE(CMFCFactoryView, CView)

BEGIN_MESSAGE_MAP(CMFCFactoryView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMFCFactoryView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CMFCFactoryView 构造/析构

CMFCFactoryView::CMFCFactoryView()
{
}

CMFCFactoryView::~CMFCFactoryView()
{
}

BOOL CMFCFactoryView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CMFCFactoryView 绘制

void CMFCFactoryView::OnDraw(CDC* /*pDC*/)
{
	CMFCFactoryDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CMFCFactoryView 打印


void CMFCFactoryView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMFCFactoryView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CMFCFactoryView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CMFCFactoryView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CMFCFactoryView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMFCFactoryView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMFCFactoryView 诊断

#ifdef _DEBUG
void CMFCFactoryView::AssertValid() const
{
	CView::AssertValid();
}

void CMFCFactoryView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCFactoryDoc* CMFCFactoryView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCFactoryDoc)));
	return (CMFCFactoryDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFCFactoryView 消息处理程序
int CMFCFactoryView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_wndFatoryControl.Create(IDD_CONTROL_WND, this);

	return 0;
}

void CMFCFactoryView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// Resize and reposition the control dialog.
	if (m_wndFatoryControl.GetSafeHwnd() != NULL && 
		::IsWindow(m_wndFatoryControl.GetSafeHwnd()))
	{
		CRect rect;
		GetClientRect(&rect);

		m_wndFatoryControl.SetWindowPos(NULL, rect.left, rect.top,
			rect.Width(), rect.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
	}
}