// Filename: ChildFrm.cpp
// 20-May-2006 nschan Initial revision.

#include "stdafx.h"
#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
    //{{AFX_MSG_MAP(CChildFrame)
    ON_COMMAND(ID_FILE_CLOSE, OnFileClose)
    ON_WM_SETFOCUS()
    ON_WM_CREATE()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

CChildFrame::CChildFrame()
{	
}

CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
    // TODO: Modify the Window class or styles here by modifying
    // the CREATESTRUCT cs.
    
    if ( !CMDIChildWnd::PreCreateWindow(cs) )
        return FALSE;
    
    cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
    cs.lpszClass = AfxRegisterWndClass(0);
    
    return TRUE;
}

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
    CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
    CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

void CChildFrame::OnFileClose() 
{
    // To close the frame, just send a WM_CLOSE, which is the equivalent
    // choosing close from the system menu.
    
    SendMessage(WM_CLOSE);
}

int CChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
    if ( CMDIChildWnd::OnCreate(lpCreateStruct) == -1 )
        return -1;
    
    // Create a view to occupy the client area of the frame.
    if ( !m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, 
         CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL) )
    {
        return -1;
    }
    
    return 0;
}

void CChildFrame::OnSetFocus(CWnd* pOldWnd) 
{
    CMDIChildWnd::OnSetFocus(pOldWnd);
    
    m_wndView.SetFocus();
}

BOOL CChildFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
    // Let the view have first crack at the command.
    if ( m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo) )
        return TRUE;
    
    // Otherwise, do default handling.
    return CMDIChildWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

CChildView* CChildFrame::GetViewWnd()
{
    return &m_wndView;
}

// END
