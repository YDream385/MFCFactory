// Filename: ChildView.cpp
// 20-May-2006 nschan Initial revision.

#include "stdafx.h"
#include "ChildView.h"
#include "ControlWnd.h"
#include "Resource.h"

#include <stack>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;

// This function counts the total number of child windows
// under the given window. The count includes the given
// window.
static int GetWindowCount(CWnd* pTopWnd)
{
    if ( pTopWnd == NULL || !::IsWindow(pTopWnd->m_hWnd) )
        return 0;

    stack<CWnd*> windowStack;
    windowStack.push(pTopWnd);

    int windowCount = 0;
    while( true )
    {
        if ( windowStack.size() == 0 )
            break;

        CWnd* pCurrWnd = windowStack.top();
        windowStack.pop();
        ++windowCount;

        CWnd* pChildWnd = pCurrWnd->GetWindow(GW_CHILD);
        while( pChildWnd != NULL && ::IsWindow(pChildWnd->m_hWnd) )
        {
            windowStack.push(pChildWnd);
            pChildWnd = pChildWnd->GetWindow(GW_HWNDNEXT);
        }
    }

    return windowCount;

}

//////////////////////////////////////////////////////////////////////////////////////
// CChildView

BEGIN_MESSAGE_MAP(CChildView, CWnd)
    ON_WM_CREATE()
    ON_WM_CONTEXTMENU()
    ON_WM_ERASEBKGND()
    ON_WM_PAINT()
    ON_WM_SIZE()
    ON_COMMAND(ID_CONTEXTMENU_SHOWCWNDCOUNT, OnShowCWndCount)
    ON_COMMAND(ID_CONTEXTMENU_CLEAREVENTLIST, OnClearEventList)
END_MESSAGE_MAP()

CChildView::CChildView()
{
    m_controlWnd = NULL;
}

CChildView::~CChildView()
{
    m_controlWnd = NULL;
}

void CChildView::OpenXmlFile(const CString& xmlFile)
{
    if ( m_controlWnd != NULL && ::IsWindow(m_controlWnd->m_hWnd) )
    {
        m_controlWnd->BuildFromXml(xmlFile);
    }
}

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
    if ( !CWnd::PreCreateWindow(cs) )
        return FALSE;
    
    cs.dwExStyle |= WS_EX_CLIENTEDGE;
    cs.style &= ~WS_BORDER;
    cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
        ::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);
    
    return TRUE;
}

int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if ( CWnd::OnCreate(lpCreateStruct) == -1 )
        return -1;
    
    // Create the control window instance and GUI.
    CRect rect(0,0,0,0);
    m_controlWnd = new CControlWnd;
    BOOL status = m_controlWnd->Create(NULL, NULL, WS_VISIBLE | WS_CHILD, rect,
        this, 1002);
    if ( !status )
        return -1;

    return 0;
}

void CChildView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
    // Get the submenu representing the context menu.
    CMenu menu;
    menu.LoadMenu(IDR_CONTEXT_MENU);
    CMenu* pContextMenu = menu.GetSubMenu(0);

    // Compute coords for top-left corner of context menu.
    CPoint ptWindow;
    CRect  rectWindow;
    if ( point.x == -1 && point.y == -1 )
    {
        // For SHIFT F10.
        GetWindowRect(&rectWindow);
        rectWindow.OffsetRect(2,2);
        ptWindow.x = rectWindow.left;
        ptWindow.y = rectWindow.top;
    } 
    else
    {
        // For mouse click.
        ptWindow.x = point.x;
        ptWindow.y = point.y;
    }

    // Popup the context menu.
    pContextMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,
        ptWindow.x, ptWindow.y, AfxGetMainWnd()); 
}

BOOL CChildView::OnEraseBkgnd(CDC* pDC)
{
    // Return TRUE to indicate further erasing is not needed.
    return TRUE;
}

void CChildView::OnPaint() 
{
    CPaintDC dc(this);
        
    // Do not call CWnd::OnPaint() for painting messages.
}

void CChildView::OnSize(UINT nType, int cx, int cy)
{
    CWnd::OnSize(nType, cx, cy);
    
    // Resize and reposition the control dialog.
    if ( m_controlWnd != NULL && ::IsWindow(m_controlWnd->m_hWnd) )
    {
        CRect rect;
        GetClientRect(&rect);
        
        m_controlWnd->SetWindowPos(NULL, rect.left, rect.top,
            rect.Width(), rect.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
    }
}

void CChildView::OnShowCWndCount()
{
    CString strCount;
    strCount.Format(_T("Total CWnd Count for CChildView: %d"), (int)GetWindowCount(this));
    MessageBox(strCount, _T("TestFreePool"), MB_OK);
}

void CChildView::OnClearEventList()
{
    if ( m_controlWnd != NULL && ::IsWindow(m_controlWnd->m_hWnd) )
    {
        m_controlWnd->ClearEventList();
    }    
}

// END
