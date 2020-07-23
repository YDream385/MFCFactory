// Filename: WndFreePool.cpp
// 04-Mar-2006 nschan Initial revision.

#include "stdafx.h"
#include "WndFreePool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;

CWndFreePool::CWndFreePool()
{
}

CWndFreePool::~CWndFreePool()
{
    // Delete all CWnds in the free pool.
    list<CWndFreePoolItem>::iterator iter = m_freePool.begin();
    for( ; iter != m_freePool.end(); ++iter)
    {
        CWndFreePoolItem& item = *iter;
        delete item.m_pWnd;
    }
    m_freePool.clear();
}

CWnd* CWndFreePool::GetWnd(const CString& strType)
{
    CWnd* pWnd = NULL;

    // Find the first entry in the free pool of the given type. 
    list<CWndFreePoolItem>::iterator iter = m_freePool.begin();
    for( ; iter != m_freePool.end(); ++iter)
    {
        CWndFreePoolItem& item = *iter;
        if ( item.m_strType == strType )
        {
            // Get the free CWnd.
            pWnd = item.m_pWnd;
            ASSERT( pWnd != NULL );

            // Remove the entry from the pool.
            m_freePool.erase(iter);
            break;
        }
    }
    
    return pWnd;
}

void CWndFreePool::AddWnd(const CString& strType, CWnd* pWnd)
{
    CWndFreePoolItem item;
    item.m_strType = strType;
    item.m_pWnd = pWnd;
    m_freePool.push_back(item);
}

// END
