// Filename: EditEx.cpp
// 27-May-2006 nschan Initial revision.

#include "stdafx.h"
#include "EditEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CEditEx, CEdit)
	//{{AFX_MSG_MAP(CEditEx)
	ON_WM_KEYUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CEditEx::CEditEx()
{
}

CEditEx::~CEditEx()
{
}

BOOL CEditEx::PreCreateWindow(CREATESTRUCT& cs) 
{
	// Add 3D border.
	cs.dwExStyle = WS_EX_CLIENTEDGE;
	
	return CEdit::PreCreateWindow(cs);
}

void CEditEx::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if ( nChar == VK_RETURN )
    {
		int nID = GetDlgCtrlID();
	
		LPARAM lParam = MAKELPARAM(m_hWnd, 0);
		WPARAM wParam = MAKEWPARAM(nID, EN_ENTER_KEY);

		// Notify parent when enter key has been pressed.
		CWnd* pWnd = GetParent();
        if ( pWnd != NULL && ::IsWindow(pWnd->m_hWnd) ) 
            pWnd->PostMessage(WM_COMMAND, wParam, lParam);
	}

	CEdit::OnKeyUp(nChar, nRepCnt, nFlags);
}

// END
