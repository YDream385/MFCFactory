// Filename: ControlWnd.h
// 13-May-2006 nschan Initial revision. 

#ifndef CONTROL_WND_INCLUDED
#define CONTROL_WND_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WndEvent.h"
#include <list>

// Forward class declarations.
class CControlDlg;
class CControlGroup;
class CListWnd;
class CTreeWnd;
class CWndControl;

// CControlWnd class.
class CControlWnd : public CWnd, public IWndEventHandler
{
    DECLARE_DYNCREATE(CControlWnd)

public:
    CControlWnd();
    virtual ~CControlWnd();

    void BuildFromXml(const CString& xmlFile);
    void ClearEventList();

    // IWndEventHandler overrides.
    virtual void HandleWndEvent(const CWndEvent& ev);

protected:
    virtual void PostNcDestroy();

    //{{AFX_MSG(CControlWnd)
    afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg int  OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
    afx_msg void OnSetFocus(CWnd* pOldWnd);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
    LRESULT OnTreeWndSelectionChange(WPARAM wParam, LPARAM lParam);

private:
    bool CreateTreeWnd();
    bool CreateListWnd();
    bool CreateControlDlg();
    void UpdateControlDlg();

    void SaveFocus();
    void RestoreFocus();

    void DeleteGroups();
    void DeleteControls();

    // Current window with focus.
    HWND m_hWndFocus;

    // Top-level control groups.
    std::list<CControlGroup*> m_topLevelGroupsList;

    // All controls.
    std::list<CWndControl*> m_allControlsList;

    // Tree window.
    CTreeWnd* m_treeWnd;
    int       m_minTreeSize;

    // List window.
    CListWnd* m_listWnd;
    int       m_minListSize;

    // Control dialog.
    CControlDlg* m_controlDlg;
};

#endif // CONTROL_WND_INCLUDED

// END


