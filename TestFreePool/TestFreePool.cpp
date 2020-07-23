// Filename: TestFreePool.cpp
// 20-May-2006 nschan Initial revision.

#include "stdafx.h"
#include "TestFreePool.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "ChildView.h"
#include "Resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// The one and only CTestFreePoolApp object.
CTestFreePoolApp theApp;

BEGIN_MESSAGE_MAP(CTestFreePoolApp, CWinApp)
    //{{AFX_MSG_MAP(CTestFreePoolApp)
    ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
    ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

CTestFreePoolApp::CTestFreePoolApp()
{
    // TODO: add construction code here,
    // Place all significant initialization in InitInstance.
}

BOOL CTestFreePoolApp::InitInstance()
{
    AfxEnableControlContainer();
    
    // Standard initialization
    // If you are not using these features and wish to reduce the size
    // of your final executable, you should remove from the following
    // the specific initialization routines you do not need.
    
    #ifdef _AFXDLL
    Enable3dControls();			// Call this when using MFC in a shared DLL
    #else
    Enable3dControlsStatic();	// Call this when linking to MFC statically
    #endif
    
    // Change the registry key under which our settings are stored.
    // TODO: You should modify this string to be something appropriate
    // such as the name of your company or organization.
    SetRegistryKey(_T("Local AppWizard-Generated Applications"));
        
    // To create the main window, this code creates a new frame window
    // object and then sets it as the application's main window object.    
    CMDIFrameWnd* pFrame = new CMainFrame;
    m_pMainWnd = pFrame;
    
    // Create main MDI frame window.
    if ( !pFrame->LoadFrame(IDR_MAINFRAME) )
        return FALSE;
    
    // Try to load shared MDI menus and accelerator table.
    // TODO: add additional member variables and load calls for
    // additional menu types your application may need.     
    HINSTANCE hInst = AfxGetResourceHandle();
    m_hMDIMenu  = ::LoadMenu(hInst, MAKEINTRESOURCE(IDR_TESTFRTYPE));
    m_hMDIAccel = ::LoadAccelerators(hInst, MAKEINTRESOURCE(IDR_TESTFRTYPE));        
    
    // The main window has been initialized, so show and update it.
    pFrame->ShowWindow(m_nCmdShow);
    pFrame->UpdateWindow();
    
    return TRUE;
}

int CTestFreePoolApp::ExitInstance() 
{
    // TODO: handle additional resources you may have added.
    if ( m_hMDIMenu != NULL )
        FreeResource(m_hMDIMenu);
    if ( m_hMDIAccel != NULL )
        FreeResource(m_hMDIAccel);
    
    return CWinApp::ExitInstance();
}

void CTestFreePoolApp::OnFileOpen()
{
    TCHAR szFilters[] =
        _T ("XML files (*.xml)|*.xml|All files (*.*)|*.*||");

    CFileDialog dlg (TRUE, _T ("xml"), _T ("*.xml"),
        OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters);
    if ( dlg.DoModal() == IDOK )
    {
        CString pathname = dlg.GetPathName();
        CString filename = dlg.GetFileName();
    
        CMainFrame* pMainFrame = STATIC_DOWNCAST(CMainFrame, m_pMainWnd);
    
        // Create a new MDI child window.
        CChildFrame* pChildFrame = static_cast<CChildFrame*>(pMainFrame->CreateNewChild(
            RUNTIME_CLASS(CChildFrame), IDR_TESTFRTYPE, m_hMDIMenu, m_hMDIAccel));
        if ( pChildFrame != NULL && ::IsWindow(pChildFrame->m_hWnd) )
        {
            CChildView* pChildView = pChildFrame->GetViewWnd();
            if ( pChildView != NULL && ::IsWindow(pChildView->m_hWnd) )
                pChildView->OpenXmlFile(pathname);

            pChildFrame->SetTitle(filename);
            pChildFrame->SetWindowText(filename);
        }
    }
}

//////////////////////////////////////////////////////////////////////////////

// CAboutDlg dialog used for App About.
class CAboutDlg : public CDialog
{
public:
    // Constructor.
    CAboutDlg();
    
    // Dialog data.
    //{{AFX_DATA(CAboutDlg)
    enum { IDD = IDD_ABOUTBOX };
    //}}AFX_DATA
  
protected:    
    // ClassWizard generated virtual function overrides.
    //{{AFX_VIRTUAL(CAboutDlg)
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL
    
    //{{AFX_MSG(CAboutDlg)
    // No message handlers
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
    //{{AFX_DATA_INIT(CAboutDlg)
    //}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CAboutDlg)
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
    //{{AFX_MSG_MAP(CAboutDlg)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog.
void CTestFreePoolApp::OnAppAbout()
{
    CAboutDlg aboutDlg;
    aboutDlg.DoModal();
}

// END




