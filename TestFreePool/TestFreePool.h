// Filename: TestFreePool.h
// 20-May-2006 nschan Initial revision.

#ifndef TEST_FREE_POOL_INCLUDED
#define TEST_FREE_POOL_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif

//#include "resource.h"

// CTestFreePoolApp class.
class CTestFreePoolApp : public CWinApp
{
public:
    // Constructor.
    CTestFreePoolApp();
    
    // ClassWizard generated virtual function overrides.
    //{{AFX_VIRTUAL(CTestFreePoolApp)
    virtual BOOL InitInstance();
    virtual int  ExitInstance();
    //}}AFX_VIRTUAL
    
    //{{AFX_MSG(CTestFreePoolApp)
    afx_msg void OnAppAbout();
    afx_msg void OnFileOpen();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

protected:
    HMENU  m_hMDIMenu;
    HACCEL m_hMDIAccel;
};

#endif // TEST_FREE_POOL_INCLUDED

// END

