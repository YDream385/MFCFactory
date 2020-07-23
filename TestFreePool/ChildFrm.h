// Filename: ChildFrm.h
// 20-May-2006 nschan Initial revision.

#ifndef CHILD_FRM_INCLUDED
#define CHILD_FRM_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Includes.
#include "ChildView.h"

// CChildFrame class.
class CChildFrame : public CMDIChildWnd
{
    DECLARE_DYNCREATE(CChildFrame)
        
public:
    // Constructor / destructor.
    CChildFrame();
    virtual ~CChildFrame();
    
    // ClassWizard generated virtual function overrides.
    //{{AFX_VIRTUAL(CChildFrame)
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
    //}}AFX_VIRTUAL
    
    // Access the child view.
    CChildView* GetViewWnd();

    #ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
    #endif
    
protected:
    // Generated message map functions.
    //{{AFX_MSG(CChildFrame)
    afx_msg void OnFileClose();
    afx_msg void OnSetFocus(CWnd* pOldWnd);
    afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
        
private:
    CChildView m_wndView;
    
};

#endif // CHILD_FRM_INCLUDED

// END
