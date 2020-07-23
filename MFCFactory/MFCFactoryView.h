
// MFCFactoryView.h : CMFCFactoryView ��Ľӿ�
//

#pragma once
#include "FatoryControlWnd.h"

class CMFCFactoryView : public CView
{
protected: // �������л�����
	CMFCFactoryView();
	DECLARE_DYNCREATE(CMFCFactoryView)

// ����
public:
	CMFCFactoryDoc* GetDocument() const;

// ����
public:

// ��д
public:	
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnInitialUpdate(); // called first time after construct
// ʵ��
public:
	virtual ~CMFCFactoryView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CFatoryControlWnd m_wndFatoryControl;
// ���ɵ���Ϣӳ�亯��
protected:	
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // MFCFactoryView.cpp �еĵ��԰汾
inline CMFCFactoryDoc* CMFCFactoryView::GetDocument() const
   { return reinterpret_cast<CMFCFactoryDoc*>(m_pDocument); }
#endif

