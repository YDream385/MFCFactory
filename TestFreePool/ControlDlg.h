// Filename: ControlDlg.h
// 13-May-2006 nschan Initial revision. 

#ifndef CONTROL_DLG_INCLUDED
#define CONTROL_DLG_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>

// Forward class declarations.
class CWndContainer;
class CWndControl;

// CControlDlg class.
class CControlDlg : public CDialog
{
public:
    // Constructor.
    CControlDlg(CWnd* pParent = NULL);
    virtual ~CControlDlg();
    
    // Dialog data.
    //{{AFX_DATA(CControlDlg)
    //}}AFX_DATA
    
    // Managing dynamic controls.
    void         AddControl(CWndControl* pControl);
    void         AddControls(std::list<CWndControl*>& controlList);
    void         RemoveControl(CWndControl* pControl);
    void         RemoveAllControls();
    CWndControl* GetControl(const CString& controlName);
    void         GetControls(std::list<CWndControl*>& controlList) const;

protected:
    virtual void PostNcDestroy();
    
    // ClassWizard generated virtual function overrides.
    //{{AFX_VIRTUAL(CControlDlg)
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    virtual void OnCancel();
    virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
    //}}AFX_VIRTUAL
    
    // Generated message map functions.
    //{{AFX_MSG(CControlDlg)
    afx_msg void OnSize(UINT nType, int cx, int cy);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
        
private:
    // For dynamic controls.
    CWndContainer* m_wndContainer;
};

#endif // CONTROL_DLG_INCLUDED

// END


