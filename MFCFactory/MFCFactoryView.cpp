
// MFCFactoryView.cpp : CMFCFactoryView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMFCFactoryView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CMFCFactoryView ����/����

CMFCFactoryView::CMFCFactoryView()
{
	// TODO: �ڴ˴���ӹ������

}

CMFCFactoryView::~CMFCFactoryView()
{
}

BOOL CMFCFactoryView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CMFCFactoryView ����

void CMFCFactoryView::OnDraw(CDC* /*pDC*/)
{
	CMFCFactoryDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CMFCFactoryView ��ӡ


void CMFCFactoryView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMFCFactoryView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CMFCFactoryView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CMFCFactoryView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
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


// CMFCFactoryView ���

#ifdef _DEBUG
void CMFCFactoryView::AssertValid() const
{
	CView::AssertValid();
}

void CMFCFactoryView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCFactoryDoc* CMFCFactoryView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCFactoryDoc)));
	return (CMFCFactoryDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFCFactoryView ��Ϣ�������
