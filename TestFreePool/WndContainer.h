// Filename: WndContainer.h
// 08-Mar-2006 nschan Initial revision. 

#ifndef WND_CONTAINER_INCLUDED
#define WND_CONTAINER_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>
#include <map>

// Forward class declarations.
class CWndFreePool;
class CWndFactory;
class CWndControl;

// CWndContainer manages a collection of CWndControl instances and
// is designed for attachment to a CDialog such as CControlDlg.
// When a control is added to the container, the free pool is used
// to acquire an appropriate CWnd for attachment to the control.
// If none is available, the container will create a new CWnd for
// it by using the factory class. When a control is removed from
// the container, its CWnd is detached and added to the free pool
// for later reuse.
class CWndContainer
{
public:
    CWndContainer();
    ~CWndContainer();
    
    // Attach to CDialog.
    void AttachWnd(CWnd* pWnd);
    void DetachWnd();
    
    // Set resource ID range for control CWnds.
    void SetResourceIdRange(UINT minResourceId, UINT maxResourceId);
    
    // Control management.
    void AddControl(CWndControl* pControl);
    void AddControls(const std::list<CWndControl*>& controlList);
    void RemoveControl(CWndControl* pControl);
    void RemoveAllControls();
    
    // Find controls.
    CWndControl* GetControl(const CString& controlName);
    CWndControl* GetControl(UINT resourceId);
    void         GetControls(std::list<CWndControl*>& controlList) const;

    // Message handling.
    BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
    
private:
    // Private methods.
    UINT GetNextResourceId();
    void UpdateControlNameMap(const CString& controlName, CWndControl* pControl);
    void UpdateResourceIdMap(UINT resourceId, CWndControl* pControl);
    void ReturnToFreePool(CWndControl* pControl);

    // For attachment to a CDialog.
    CWnd* m_attachWnd;
    
    // Resource ID range and current ID for creating the next CWnd.
    UINT m_minResourceId;
    UINT m_maxResourceId;
    UINT m_currResourceId;
    
    // Default font used in dialogs.
    CFont m_controlFont;
    
    // Free pool of CWnds.
    CWndFreePool* m_freePool;
    
    // Factory for creating either a new CWnd or CWndControl instance
    // based on control type name.
    CWndFactory* m_controlFactory;
    
    // Store all control instances in a list. We also use two maps
    // for quick access based on control name or resource ID.
    std::list<CWndControl*>         m_controlList;
    std::map<CString, CWndControl*> m_controlNameMap;
    std::map<UINT, CWndControl*>    m_resourceIdMap;
};

#endif // WND_CONTAINER_INCLUDED

// END
