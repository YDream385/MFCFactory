// Filename: WndFreePool.h
// 04-Mar-2006 nschan Initial revision.

#ifndef WND_FREE_POOL_INCLUDED
#define WND_FREE_POOL_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>

// CWndFreePool keeps references to CWnds which have been
// created but are unused (hidden). The pool maintains ownership
// of the CWnds which are still in the pool and deletes them in
// its destructor.
class CWndFreePool
{
public:
    // Constructor / destructor.
    CWndFreePool();
    ~CWndFreePool();
    
    // Public methods.
    CWnd* GetWnd(const CString& strType);
    void  AddWnd(const CString& strType, CWnd* pWnd);

private:
    // CWndFreePoolItem nested class.
    class CWndFreePoolItem
    {
    public:
        CWndFreePoolItem()
        {
            m_strType = _T("");
            m_pWnd = NULL;
        }

        CString m_strType;
        CWnd*   m_pWnd;
    };

    // List of free CWnds. 
    std::list<CWndFreePoolItem> m_freePool;
};

#endif // WND_FREE_POOL_INCLUDED

// END