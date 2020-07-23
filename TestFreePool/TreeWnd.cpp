// Filename: TreeWnd.cpp
// 27-Nov-2006 nschan Initial revision. 

#include "stdafx.h"
#include "TreeWnd.h"
#include "ControlGroup.h"
#include "Resource.h"

#include <list>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;

static const int TREE_MARGIN = 10;

IMPLEMENT_DYNCREATE(CTreeWnd, CWnd)

BEGIN_MESSAGE_MAP(CTreeWnd, CWnd)
    //{{AFX_MSG_MAP(CTreeWnd)
    ON_WM_CREATE()
    ON_WM_ERASEBKGND()
    ON_WM_SIZE()
    //}}AFX_MSG_MAP
    ON_NOTIFY(TVN_SELCHANGED, IDC_CONTROL_TREE_CTRL, OnTreeCtrlSelChanged)
END_MESSAGE_MAP()

CTreeWnd::CTreeWnd()
{
    m_treeCtrl = NULL;
    m_htiSelected = 0;
    m_htiFirst = 0;
    m_treeMargin = TREE_MARGIN;

    m_notifyWnd = NULL;
}

CTreeWnd::~CTreeWnd()
{
    delete m_treeCtrl;
    m_treeCtrl = NULL;

    m_notifyWnd = NULL;
}

void CTreeWnd::AddControlGroup(CControlGroup* controlGroup)
{
    ASSERT( controlGroup != NULL );
    ASSERT( m_treeCtrl != NULL );
    if ( !::IsWindow(m_treeCtrl->m_hWnd) )
        return;
    
    // Turn off redraw of tree ctrl.
    m_treeCtrl->SetRedraw(FALSE);

    // Add this group and its child groups recursively.
    AddTreeItems(TVI_ROOT, controlGroup);

    // Select the first tree item.
    if ( m_htiFirst != 0 && m_treeCtrl->GetSelectedItem() != m_htiFirst )
    {
        m_treeCtrl->SelectItem(m_htiFirst);
        m_htiSelected = m_htiFirst;
        m_treeCtrl->EnsureVisible(m_htiFirst);
    }

    // Re-enable redraw for the tree ctrl.
    m_treeCtrl->SetRedraw(TRUE);
    m_treeCtrl->RedrawWindow();
}

void CTreeWnd::ResetTree()
{
    ASSERT( m_treeCtrl != NULL );
    m_treeCtrl->DeleteAllItems();
    m_htiSelected = 0;
    m_htiFirst = 0;

    m_itemToGroupMap.clear();
    m_groupToItemMap.clear();
}

CControlGroup* CTreeWnd::GetSelectedGroup()
{
    if ( m_htiSelected == 0 )
        return NULL;

    CControlGroup* pGroup = MapItemToGroup(m_htiSelected);
   
    return pGroup; 
}

void CTreeWnd::SetSelectedGroup(CControlGroup* controlGroup)
{
    // Find the corresponding tree item.
    HTREEITEM htiSelected = MapGroupToItem(controlGroup);
    if ( htiSelected != 0 )
    {
        if ( m_treeCtrl->GetSelectedItem() != htiSelected )
            m_treeCtrl->SelectItem(htiSelected);
        m_treeCtrl->EnsureVisible(htiSelected);
        m_htiSelected = htiSelected;
    }
}

void CTreeWnd::SetNotifyWnd(CWnd* notifyWnd)
{
    m_notifyWnd = notifyWnd;
}

void CTreeWnd::PostNcDestroy()
{
    delete m_treeCtrl;
    m_treeCtrl = NULL;
}

int CTreeWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if ( !CreateTreeCtrl() )
    {
        return -1;
    }

    return 0;
}

BOOL CTreeWnd::OnEraseBkgnd(CDC* pDC)
{
	// Exclude the tree ctrl from the paintable area.
	CRect treeRect;
	GetTreeRect(treeRect);
	pDC->ExcludeClipRect(treeRect);

    // Fill the borders not covered by tree ctrl with the background color.
    CRect clientRect;
    GetClientRect(clientRect);
    COLORREF crBackground = ::GetSysColor(COLOR_BTNFACE);
    if ( m_treeMargin > 0 )
    {
        pDC->FillSolidRect(0, 0, clientRect.Width(), m_treeMargin, crBackground);
        pDC->FillSolidRect(0, 0, m_treeMargin, clientRect.Height(), crBackground);
        pDC->FillSolidRect(0, clientRect.Height() - m_treeMargin, clientRect.Width(), m_treeMargin, crBackground);
        pDC->FillSolidRect(clientRect.Width() - m_treeMargin, 0, m_treeMargin, clientRect.Height(), crBackground);
    }

    // Return TRUE to indicate further erasing is not needed.
    return TRUE;
}

void CTreeWnd::OnSize(UINT nType, int cx, int cy)
{
    CWnd::OnSize(nType, cx, cy);
    
    // Resize and reposition the tree ctrl.
    if ( m_treeCtrl != NULL && ::IsWindow(m_treeCtrl->m_hWnd) )
    {
        CRect rect;
        GetTreeRect(rect);
        m_treeCtrl->SetWindowPos(NULL, rect.left, rect.top,
            rect.Width(), rect.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
    }
}

void CTreeWnd::OnTreeCtrlSelChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
    // Check if a selection was really changed.
    HTREEITEM htiSelected = m_treeCtrl->GetSelectedItem();
    if ( htiSelected == m_htiSelected )
        return;
    m_htiSelected = htiSelected;

    // Get the corresponding control group.
    CControlGroup* pGroup = MapItemToGroup(m_htiSelected);
    if ( pGroup != NULL )
    {
        // Notify wnd.
        if ( m_notifyWnd != NULL && ::IsWindow(m_notifyWnd->m_hWnd) )
            m_notifyWnd->PostMessage(WM_TREEWND_SELECTION_CHANGE, 0, 0);
    }
} 

bool CTreeWnd::CreateTreeCtrl()
{
    // Create tree control object.
    if ( m_treeCtrl == NULL )
        m_treeCtrl = new CTreeCtrl;

    // Create the tree control GUI.
    if ( !m_treeCtrl->Create(

            TVS_SHOWSELALWAYS |
            TVS_HASBUTTONS |
            TVS_LINESATROOT |
            TVS_HASLINES |
            WS_CHILD | WS_VISIBLE,

            CRect(0, 0, 0, 0), 
            this,
            IDC_CONTROL_TREE_CTRL) )
    {

        return false;
    }

    // Show sunken border.
    m_treeCtrl->ModifyStyleEx(0, WS_EX_CLIENTEDGE);

    return true;
}

void CTreeWnd::GetTreeRect(CRect& rect)
{
    CRect clientRect;
    GetClientRect(clientRect);

    int treeWidth = clientRect.Width() - 2 * m_treeMargin;
    rect.SetRect(
        clientRect.left + m_treeMargin,
        clientRect.top + m_treeMargin,
        clientRect.left + m_treeMargin + treeWidth,
        clientRect.bottom - m_treeMargin);
}

CControlGroup* CTreeWnd::MapItemToGroup(HTREEITEM htiItem)
{
    map<HTREEITEM,CControlGroup*>::const_iterator result = m_itemToGroupMap.find(htiItem);
    if ( result == m_itemToGroupMap.end() )
        return NULL;
    else
        return result->second;
}

HTREEITEM CTreeWnd::MapGroupToItem(CControlGroup* controlGroup)
{
    map<CControlGroup*,HTREEITEM>::const_iterator result = m_groupToItemMap.find(controlGroup);
    if ( result == m_groupToItemMap.end() )
        return 0;
    else
        return result->second;
}

void CTreeWnd::AddTreeItems(HTREEITEM htiParent, CControlGroup* controlGroup)
{
    // Insert the tree ctrl item.    
    HTREEITEM htiNew = m_treeCtrl->InsertItem(controlGroup->GetName(), 0, 0, htiParent, TVI_LAST);
    if ( htiParent == TVI_ROOT && m_htiFirst == 0 )
        m_htiFirst = htiNew;

    // Update map entries.
    if ( htiNew != 0 )
    {
        m_groupToItemMap.insert(make_pair(controlGroup,htiNew));
        m_itemToGroupMap.insert(make_pair(htiNew,controlGroup));
    }

    // Call recursively on this group's children.
    list<CControlGroup*> childGroups;
    controlGroup->GetGroups(childGroups);
    list<CControlGroup*>::iterator iter = childGroups.begin();
    for( ; iter != childGroups.end(); ++iter)
    {
        AddTreeItems(htiNew, *iter);
    }
}

// END
