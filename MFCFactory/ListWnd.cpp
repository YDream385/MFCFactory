// Filename: ListWnd.cpp
// 12-Dec-2006 nschan Initial revision. 

#include "stdafx.h"
#include "ListWnd.h"
#include "Resource.h"

#include <list>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;

static const int LIST_MARGIN = 10;

IMPLEMENT_DYNCREATE(CListWnd, CWnd)

BEGIN_MESSAGE_MAP(CListWnd, CWnd)
    //{{AFX_MSG_MAP(CListWnd)
    ON_WM_CREATE()
    ON_WM_ERASEBKGND()
    ON_WM_SIZE()
    //}}AFX_MSG_MAP
    ON_NOTIFY(LBN_SELCHANGE, IDC_CONTROL_LIST_BOX, OnListBoxSelChanged)
END_MESSAGE_MAP()

CListWnd::CListWnd()
{
    m_listMargin = LIST_MARGIN;
    m_listFont.CreatePointFont(80, _T("MS Sans Serif"));

    m_notifyWnd = NULL;
}

CListWnd::~CListWnd()
{
    m_notifyWnd = NULL;
}

void CListWnd::AddItem(const CString& item)
{
    m_listBox.AddString(item);
}

void CListWnd::RemoveAllItems()
{
    m_listBox.ResetContent();
}

int CListWnd::GetNumItems() const
{
    return m_listBox.GetCount();
}

void CListWnd::SetNotifyWnd(CWnd* notifyWnd)
{
    m_notifyWnd = notifyWnd;
}

int CListWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    // Create the list box GUI.
    BOOL status = m_listBox.CreateEx(WS_EX_CLIENTEDGE, _T("LISTBOX"), NULL,
                      WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_VSCROLL | WS_HSCROLL |
                      LBS_NOTIFY | LBS_NOINTEGRALHEIGHT,
                      CRect(0,0,0,0),
                      this,
                      IDC_CONTROL_LIST_BOX);
    if ( !status )
        return -1;
    m_listBox.SetFont(&m_listFont);
    m_listBox.SetHorizontalExtent(400);

    return 0;
}

BOOL CListWnd::OnEraseBkgnd(CDC* pDC)
{
	// Exclude the listbox from the paintable area.
	CRect listRect;
	GetListRect(listRect);
	pDC->ExcludeClipRect(listRect);

    // Fill the borders not covered by listbox with the background color.
    CRect clientRect;
    GetClientRect(clientRect);
    COLORREF crBackground = ::GetSysColor(COLOR_BTNFACE);
    if ( m_listMargin > 0 )
    {
        pDC->FillSolidRect(0, 0, clientRect.Width(), m_listMargin, crBackground);
        pDC->FillSolidRect(0, 0, m_listMargin, clientRect.Height(), crBackground);
        pDC->FillSolidRect(0, clientRect.Height() - m_listMargin, clientRect.Width(), m_listMargin, crBackground);
        pDC->FillSolidRect(clientRect.Width() - m_listMargin, 0, m_listMargin, clientRect.Height(), crBackground);
    }

    // Return TRUE to indicate further erasing is not needed.
    return TRUE;
}

void CListWnd::OnSize(UINT nType, int cx, int cy)
{
    CWnd::OnSize(nType, cx, cy);
    
    // Resize and reposition the listbox.
    if ( ::IsWindow(m_listBox.m_hWnd) )
    {
        CRect rect;
        GetListRect(rect);
        m_listBox.SetWindowPos(NULL, rect.left, rect.top,
            rect.Width(), rect.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
    }
}

void CListWnd::OnListBoxSelChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
    // Notify wnd.
    if ( m_notifyWnd != NULL && ::IsWindow(m_notifyWnd->m_hWnd) )
        m_notifyWnd->PostMessage(WM_LISTWND_SELECTION_CHANGE, 0, 0);
} 

void CListWnd::GetListRect(CRect& rect)
{
    CRect clientRect;
    GetClientRect(clientRect);

    int listWidth = clientRect.Width() - 2 * m_listMargin;
    rect.SetRect(
        clientRect.left + m_listMargin,
        clientRect.top + m_listMargin,
        clientRect.left + m_listMargin + listWidth,
        clientRect.bottom - m_listMargin);
}

// END
