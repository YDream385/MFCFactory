// Filename: ControlWnd.cpp
// 13-May-2006 nschan Initial revision. 

#include "stdafx.h"
#include "ControlWnd.h"
#include "ControlDlg.h"
#include "ControlGroup.h"
#include "ControlXml.h"
#include "WndControl.h"
#include "TreeWnd.h"
#include "ListWnd.h"
#include "Resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;

IMPLEMENT_DYNCREATE(CControlWnd, CWnd)

BEGIN_MESSAGE_MAP(CControlWnd, CWnd)
    //{{AFX_MSG_MAP(CControlWnd)
    ON_WM_CREATE()
    ON_WM_MOUSEACTIVATE()
    ON_WM_SETFOCUS()
    ON_WM_SIZE()
    //}}AFX_MSG_MAP
    ON_MESSAGE(WM_TREEWND_SELECTION_CHANGE, OnTreeWndSelectionChange)
END_MESSAGE_MAP()

static const int MIN_TREE_SIZE = 50;
static const int MIN_LIST_SIZE = 50;

CControlWnd::CControlWnd()
{
    m_hWndFocus = 0;

    m_treeWnd = NULL;   
    m_minTreeSize = MIN_TREE_SIZE;

    m_listWnd = NULL;
    m_minListSize = MIN_LIST_SIZE;

    m_controlDlg = NULL;
}

CControlWnd::~CControlWnd()
{
    delete m_treeWnd;
    m_treeWnd = NULL;

    delete m_listWnd;
    m_listWnd = NULL;

    DeleteGroups();
    DeleteControls();
}

void CControlWnd::BuildFromXml(const CString& xmlFile)
{
    CControlXml controlXml;
    controlXml.ParseXml(xmlFile, m_topLevelGroupsList, m_allControlsList);

    list<CControlGroup*>::iterator i = m_topLevelGroupsList.begin();
    for( ; i != m_topLevelGroupsList.end(); i++)
    {
        m_treeWnd->AddControlGroup(*i);
    }

    list<CWndControl*>::iterator j = m_allControlsList.begin();
    for( ; j != m_allControlsList.end(); j++)
    {
        CWndControl* pControl = *j;
        pControl->AddEventHandler(this);
    }

    m_treeWnd->SetNotifyWnd(this);

    UpdateControlDlg();
}

void CControlWnd::ClearEventList()
{
    if ( m_listWnd != NULL && ::IsWindow(m_listWnd->m_hWnd) )
    {
        m_listWnd->RemoveAllItems();
    }  
}

void CControlWnd::HandleWndEvent(const CWndEvent& ev)
{
    if ( m_listWnd != NULL && ::IsWindow(m_listWnd->m_hWnd) )
    {
        CString msg;
        msg.Format(_T("%04d, %s, %s"), m_listWnd->GetNumItems()+1, ev.GetSender()->GetName(), ev.GetText());
        m_listWnd->AddItem(msg);
    }
}

void CControlWnd::PostNcDestroy()
{
    // Delete the C++ instance.
    delete this;
}

int CControlWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if ( !CreateTreeWnd() )
    {
        return -1;
    }

    if ( !CreateListWnd() )
    {
        return -1;
    }

    if ( !CreateControlDlg() )
    {
        return -1;
    }

    return 0;
}

int CControlWnd::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
    int status = CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);

    SetFocus();

    return status;
}

void CControlWnd::OnSetFocus(CWnd* pOldWnd)
{
    ASSERT( m_controlDlg != NULL );

    CWnd::OnSetFocus(pOldWnd);

    if ( ::IsWindow(m_controlDlg->m_hWnd) )
    {
        m_controlDlg->SetFocus();
    }
}

void CControlWnd::OnSize(UINT nType, int cx, int cy)
{
    CWnd::OnSize(nType, cx, cy);
    
    CRect clientRect;
    GetClientRect(clientRect);

    // Resize and reposition the tree wnd.
    int treeWidth = m_minTreeSize + clientRect.Width() / 4;
    int treeHeight = max(m_minTreeSize, clientRect.Height() * 2/3);
    if ( m_treeWnd != NULL && ::IsWindow(m_treeWnd->m_hWnd) )
    {
        m_treeWnd->SetWindowPos(NULL, 0, 0, treeWidth, treeHeight,
            SWP_NOZORDER | SWP_NOACTIVATE);
    }

    // Resize and reposition the list wnd.
    int listHeight = max(m_minListSize, clientRect.Height() - treeHeight);
    if ( m_listWnd != NULL && ::IsWindow(m_listWnd->m_hWnd) )
    {
        m_listWnd->SetWindowPos(NULL, 0, treeHeight, treeWidth, listHeight,
            SWP_NOZORDER | SWP_NOACTIVATE);
    }

    // Resize and reposition the control dialog.
    if ( m_controlDlg != NULL && ::IsWindow(m_controlDlg->m_hWnd) )
    {
        m_controlDlg->SetWindowPos(NULL, treeWidth, 0,
            clientRect.Width() - treeWidth, clientRect.Height(),
            SWP_NOZORDER | SWP_NOACTIVATE);
    }
}
    
LRESULT CControlWnd::OnTreeWndSelectionChange(WPARAM wParam, LPARAM lParam)
{
    UpdateControlDlg();

    return 0;
}

bool CControlWnd::CreateTreeWnd()
{
    // Create tree wnd instance.
    m_treeWnd = new CTreeWnd();

    // Create the tree control GUI.
    CRect rect(0,0,0,0);
    if ( !m_treeWnd->Create(NULL, NULL, WS_VISIBLE | WS_CHILD, rect, this, 1000) )
    {        
        return false;
    }

    return true;
}

bool CControlWnd::CreateListWnd()
{
    // Create list wnd instance.
    m_listWnd = new CListWnd();

    // Create the list control GUI.
    CRect rect(0,0,0,0);
    if ( !m_listWnd->Create(NULL, NULL, WS_VISIBLE | WS_CHILD, rect, this, 1001) )
    {        
        return false;
    }

    return true;
}

bool CControlWnd::CreateControlDlg()
{
    // Create the control dialog instance. This will create the GUI instance as well.
    m_controlDlg = new CControlDlg(this);

    return true;
}

void CControlWnd::UpdateControlDlg()
{
    // Record current focus wnd so that we can restore it later.    
    SaveFocus();

    // Remove the controls from control dlg.
    m_controlDlg->RemoveAllControls();

    // Get the currently selected group from tree wnd.
    CControlGroup* group = m_treeWnd->GetSelectedGroup();
    if ( group != NULL )
    {
        // Get the controls belonging to this control group.
        list<CWndControl*> controlList;
        group->GetControls(controlList);

        // Add these controls to the control dlg.
        if ( controlList.size() > 0 )
        {
            m_controlDlg->AddControls(controlList);            
        }
    }

    // Restore focus.
    RestoreFocus();
}

void CControlWnd::SaveFocus()
{
    // Record current focus wnd so that we can restore it later.    
    m_hWndFocus = ::GetFocus();
}

void CControlWnd::RestoreFocus()
{
    if ( m_hWndFocus != ::GetFocus() && ::IsWindow(m_hWndFocus) )
        ::SetFocus(m_hWndFocus);
}

void CControlWnd::DeleteGroups()
{
    list<CControlGroup*>::iterator iter = m_topLevelGroupsList.begin();
    for( ; iter != m_topLevelGroupsList.end(); ++iter)
    {
        delete *iter;
    }
    m_topLevelGroupsList.clear();
}

void CControlWnd::DeleteControls()
{
    list<CWndControl*>::iterator iter = m_allControlsList.begin();
    for( ; iter != m_allControlsList.end(); ++iter)
    {
        delete *iter;
    }
    m_allControlsList.clear();
}

// END
