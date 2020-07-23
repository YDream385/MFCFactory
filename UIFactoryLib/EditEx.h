// Filename: EditEx.h
// 27-May-2006 nschan Initial revision.

#ifndef EDIT_EX_INCLUDED
#define EDIT_EX_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// EN_CHANGE = 0x300, is defined in winuser.h.
const int EN_ENTER_KEY = 0x310;

// CEditEx class.
// This is simply an edit box that supports enter key events.
class CEditEx : public CEdit
{
public:
    // Constructor / destructor.
    CEditEx();
    virtual ~CEditEx();
    
    // ClassWizard generated virtual function overrides.
    //{{AFX_VIRTUAL(CEditEx)
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    //}}AFX_VIRTUAL
    
protected:
    // Generated message map functions.
    //{{AFX_MSG(CEditEx)
    afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

#endif // EDIT_EX_INCLUDED

// END

