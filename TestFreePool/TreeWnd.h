// Filename: TreeWnd.h
// 27-Nov-2006 nschan Initial revision. 

#ifndef TREE_WND_INCLUDED
#define TREE_WND_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>

// Forward class declarations.
class CControlGroup;

// User message. 
const UINT WM_TREEWND_SELECTION_CHANGE = WM_USER + 0x100;

// CTreeWnd class.
class CTreeWnd : public CWnd
{
    DECLARE_DYNCREATE(CTreeWnd)

public:
    CTreeWnd();
    virtual ~CTreeWnd();

    void           AddControlGroup(CControlGroup* controlGroup);
    void           ResetTree();

    CControlGroup* GetSelectedGroup();
    void           SetSelectedGroup(CControlGroup* controlGroup);

    void           SetNotifyWnd(CWnd* notifyWnd);

protected:
    virtual void PostNcDestroy();

    //{{AFX_MSG(CTreeWnd)
    afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
    void         OnTreeCtrlSelChanged(NMHDR* pNMHDR, LRESULT* pResult);

private:
    bool CreateTreeCtrl();
    void GetTreeRect(CRect& rect);

    CControlGroup* MapItemToGroup(HTREEITEM htiItem);
    HTREEITEM      MapGroupToItem(CControlGroup* controlGroup);

    void       AddTreeItems(HTREEITEM htiParent, CControlGroup* controlGroup);

    CTreeCtrl* m_treeCtrl;
    HTREEITEM  m_htiSelected;
    HTREEITEM  m_htiFirst;
    int        m_treeMargin;

    std::map<HTREEITEM,CControlGroup*> m_itemToGroupMap;
    std::map<CControlGroup*,HTREEITEM> m_groupToItemMap;

    CWnd*      m_notifyWnd;
};

#endif // TREE_WND_INCLUDED

// END


