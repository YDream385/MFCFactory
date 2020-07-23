// Filename: MainFrm.h
// 20-May-2006 nschan Initial revision.

#ifndef MAIN_FRM_INCLUDED
#define MAIN_FRM_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// CMainFrame class.
class CMainFrame : public CMDIFrameWnd
{
    DECLARE_DYNAMIC(CMainFrame)
        
public:
    // Constructor / destructor.
    CMainFrame();
    virtual ~CMainFrame();
    
    // ClassWizard generated virtual function overrides.
    //{{AFX_VIRTUAL(CMainFrame)
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    //}}AFX_VIRTUAL
    
    #ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
    #endif
    
protected:  
    // Control bar embedded members.
    CStatusBar  m_wndStatusBar;
    CToolBar    m_wndToolBar;
    
    // Generated message map functions.
    //{{AFX_MSG(CMainFrame)
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

#endif // MAIN_FRM_INCLUDED

// END
