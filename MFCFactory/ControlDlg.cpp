// Filename: ControlDlg.cpp
// 13-May-2006 nschan Initial revision. 

#include "stdafx.h"
#include "ControlDlg.h"
#include "WndContainer.h"
#include "WndControl.h"
#include "Resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CControlDlg, CDialog)
    //{{AFX_MSG_MAP(CControlDlg)
    ON_WM_SIZE()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

CControlDlg::CControlDlg(CWnd* pParent)
    : CDialog(IDD_CONTROL_WND, pParent)
{
    m_wndContainer = new CWndContainer;
    m_wndContainer->AttachWnd(this);
    
    // Create the dialog.
    Create(IDD_CONTROL_WND, pParent);
}

CControlDlg::~CControlDlg()
{
    m_wndContainer->DetachWnd();
    delete m_wndContainer;
    m_wndContainer = NULL;    
}

void CControlDlg::PostNcDestroy()
{
    // Delete the C++ instance.
    delete this;    
}

void CControlDlg::AddControl(CWndControl* pControl)
{
    ASSERT( m_wndContainer != NULL );
    m_wndContainer->AddControl(pControl);
}

void CControlDlg::AddControls(std::list<CWndControl*>& controlList)
{
    ASSERT( m_wndContainer != NULL );
    m_wndContainer->AddControls(controlList);
}

void CControlDlg::RemoveControl(CWndControl* pControl)
{
    ASSERT( m_wndContainer != NULL );
    m_wndContainer->RemoveControl(pControl);
}

void CControlDlg::RemoveAllControls()
{
    ASSERT( m_wndContainer != NULL );
    m_wndContainer->RemoveAllControls();    
}

CWndControl* CControlDlg::GetControl(const CString& controlName)
{
    ASSERT( m_wndContainer != NULL );
    return m_wndContainer->GetControl(controlName);
}

void CControlDlg::GetControls(std::list<CWndControl*>& controlList) const
{
    ASSERT( m_wndContainer != NULL );
    m_wndContainer->GetControls(controlList);
}

void CControlDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    
    //{{AFX_DATA_MAP(CControlDlg)
    //}}AFX_DATA_MAP
}

BOOL CControlDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    
    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

void CControlDlg::OnOK()
{
}

void CControlDlg::OnCancel()
{
}

BOOL CControlDlg::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
    if ( m_wndContainer != NULL )
    {
        BOOL isHandled = m_wndContainer->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
        if ( isHandled )
            return TRUE;
    }
    
    return CDialog::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);    
}

void CControlDlg::OnSize(UINT nType, int cx, int cy)
{
    // Resizing support (none implemented).

    CDialog::OnSize(nType, cx, cy);
}

// END
