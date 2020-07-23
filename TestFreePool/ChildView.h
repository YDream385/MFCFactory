// Filename: ChildView.h
// 20-May-2006 nschan Initial revision.

#ifndef CHILD_VIEW_INCLUDED
#define CHILD_VIEW_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Forward class declarations.
class CControlWnd;

// CChildView window.
class CChildView : public CWnd
{
public:
    CChildView();
    virtual ~CChildView();

    void OpenXmlFile(const CString& xmlFile);

protected:
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    
    // Generated message map functions.
    afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnPaint();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnShowCWndCount();
    afx_msg void OnClearEventList();
    DECLARE_MESSAGE_MAP()
        
private:
	CControlWnd* m_controlWnd;
};

#endif // CHILD_VIEW_INCLUDED

// END

