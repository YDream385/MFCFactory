
// MFCFactoryView.h : CMFCFactoryView 类的接口
//

#pragma once
#include "FatoryControlWnd.h"

class CMFCFactoryView : public CView
{
protected: // 仅从序列化创建
	CMFCFactoryView();
	DECLARE_DYNCREATE(CMFCFactoryView)

// 特性
public:
	CMFCFactoryDoc* GetDocument() const;

// 操作
public:

// 重写
public:	
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnInitialUpdate(); // called first time after construct
// 实现
public:
	virtual ~CMFCFactoryView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CFatoryControlWnd m_wndFatoryControl;
// 生成的消息映射函数
protected:	
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // MFCFactoryView.cpp 中的调试版本
inline CMFCFactoryDoc* CMFCFactoryView::GetDocument() const
   { return reinterpret_cast<CMFCFactoryDoc*>(m_pDocument); }
#endif

