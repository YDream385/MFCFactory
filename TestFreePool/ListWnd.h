// Filename: ListWnd.h
// 12-Dec-2006 nschan Initial revision. 

#ifndef LIST_WND_INCLUDED
#define LIST_WND_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// User message. 
const UINT WM_LISTWND_SELECTION_CHANGE = WM_USER + 0x101;

// CListWnd class.
class CListWnd : public CWnd
{
    DECLARE_DYNCREATE(CListWnd)

public:
    CListWnd();
    virtual ~CListWnd();

    void AddItem(const CString& item);
    void RemoveAllItems();
    int  GetNumItems() const;

    void SetNotifyWnd(CWnd* notifyWnd);

protected:
    //{{AFX_MSG(CListWnd)
    afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
    void         OnListBoxSelChanged(NMHDR* pNMHDR, LRESULT* pResult);

private:
    void GetListRect(CRect& rect);

    CListBox  m_listBox;    
    int       m_listMargin;
    CFont     m_listFont;

    CWnd*     m_notifyWnd;
};

#endif // LIST_WND_INCLUDED

// END


