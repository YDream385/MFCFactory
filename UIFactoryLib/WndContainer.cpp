// Filename: WndContainer.cpp
// 08-Mar-2006 nschan Initial revision. 

#include "stdafx.h"
#include "WndContainer.h"
#include "WndFreePool.h"
#include "WndFactory.h"
#include "WndControl.h"

#include <algorithm>

using namespace std;

CWndContainer::CWndContainer()
{
    m_attachWnd = NULL;
    
    // Set the resource ID range.
    m_minResourceId = 10000;
    m_maxResourceId = 12000;
    m_currResourceId = m_minResourceId;
    
    // Create the default font used in dialogs.
    m_controlFont.CreatePointFont(80, _T("MS Sans Serif"));
    
    // Create the CWnd free pool.
    m_freePool = new CWndFreePool;
    
    // Create the control factory.
    m_controlFactory = new CWndFactory;
}

CWndContainer::~CWndContainer()
{
    m_attachWnd = NULL;
    
    // Clear the two indexing maps.
    m_controlNameMap.clear();
    m_resourceIdMap.clear();
    
    // Delete the CWnd instances attached to controls.
    list<CWndControl*>::iterator iter = m_controlList.begin();
    for( ; iter != m_controlList.end(); ++iter)
    {
        // Delete the CWnd only, not the control
        // as we do not own the control instances.
        // Control instances are managed externally
        // by the application code.
        CWndControl* pControl = *iter;
        delete pControl->GetAttachWnd();
    }
    m_controlList.clear();

    // Delete the control factory.
    delete m_controlFactory;
    m_controlFactory = NULL;
    
    // Delete the free pool of CWnds.
    delete m_freePool;
    m_freePool = NULL;
}

void CWndContainer::AttachWnd(CWnd* pWnd)
{
    m_attachWnd = pWnd;
}

void CWndContainer::DetachWnd()
{
    m_attachWnd = NULL;
}

void CWndContainer::SetResourceIdRange(UINT minResourceId, UINT maxResourceId)
{
    ASSERT( maxResourceId > minResourceId );
    m_minResourceId = minResourceId;
    m_maxResourceId = maxResourceId;
    m_currResourceId = m_minResourceId;
}

void CWndContainer::AddControl(CWndControl* pControl)
{
    ASSERT( m_attachWnd != NULL );
    ASSERT( ::IsWindow(m_attachWnd->m_hWnd) );
    
    // Check the control and type name.
    ASSERT( pControl != NULL );
    ASSERT( !pControl->GetName().IsEmpty() );
    ASSERT( !pControl->GetTypeName().IsEmpty() );
    if ( pControl->GetName().IsEmpty() )
        return;
    if ( pControl->GetTypeName().IsEmpty() )
        return;
    
    // Check if a control with the same name has already been added.
    map<CString,CWndControl*>::const_iterator result = m_controlNameMap.find(pControl->GetName());
    if ( result != m_controlNameMap.end() )
        return;
    
    // Try to acquire a CWnd from the free pool.
    CWnd* pWnd = m_freePool->GetWnd(pControl->GetTypeName());
    if ( pWnd == NULL )
    {
        // We have to create a new CWnd using the factory.
        pWnd = m_controlFactory->CreateWnd(pControl->GetTypeName());
    }
    ASSERT( pWnd != NULL );

    // Attach the CWnd to the control.
    pControl->AttachWnd(pWnd);
    
    // Attach the default font to the control.
    pControl->AttachFont(&m_controlFont);
    
    // Insert control into the list and 2 maps.
    m_controlList.push_back(pControl);
    UpdateControlNameMap(pControl->GetName(), pControl);
    if ( !::IsWindow(pControl->GetAttachWnd()->m_hWnd) )
        pControl->CreateWnd(m_attachWnd, GetNextResourceId());
    UpdateResourceIdMap(pControl->GetResourceId(), pControl);

    // Update the GUI instance of the control.
    pControl->UpdateWnd();

    // Update z-order.
    ::SetWindowPos(pControl->GetAttachWnd()->m_hWnd, HWND_BOTTOM, 0, 0, 0, 0,
        SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
}

void CWndContainer::AddControls(const std::list<CWndControl*>& controlList)
{
    list<CWndControl*>::const_iterator iter = controlList.begin();
    for( ; iter != controlList.end(); ++iter)
    {
        CWndControl* pControl = *iter;
        AddControl(pControl);
    }
}

void CWndContainer::RemoveControl(CWndControl* pControl)
{
    // Check if the control is in the list.
    list<CWndControl*>::const_iterator result = find(m_controlList.begin(), m_controlList.end(), pControl);
    if ( result == m_controlList.end() )
        return;
    
    // Remove from control list and the 2 maps.
    m_controlList.remove(pControl);
    m_controlNameMap.erase(pControl->GetName());
    m_resourceIdMap.erase(pControl->GetResourceId());
    
    // Return the control's attached wnd back to the free pool.
    ReturnToFreePool(pControl);
}

void CWndContainer::RemoveAllControls()
{
    // Clear the two indexing maps.
    m_controlNameMap.clear();
    m_resourceIdMap.clear();
    
    // Loop through all the controls in the list.
    list<CWndControl*>::iterator i = m_controlList.begin();
    for( ; i != m_controlList.end(); i++)
    {
        CWndControl* pControl = *i;
        
        // Return the control's attached wnd back to the free pool.
        ReturnToFreePool(pControl);
    }

    // Clear the control list.
    m_controlList.clear();
}

CWndControl* CWndContainer::GetControl(const CString& controlName)
{
    map<CString,CWndControl*>::const_iterator result = m_controlNameMap.find(controlName);
    if ( result != m_controlNameMap.end() )
    {
        return result->second;
    }

    return NULL;
}

CWndControl* CWndContainer::GetControl(UINT resourceId)
{
    map<UINT,CWndControl*>::const_iterator result = m_resourceIdMap.find(resourceId);
    if ( result != m_resourceIdMap.end() )
    {
        return result->second;
    }

    return NULL;
}

void CWndContainer::GetControls(std::list<CWndControl*>& controlList) const
{
    controlList = m_controlList;
}

BOOL CWndContainer::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
    BOOL isHandled = FALSE;

    // Find the control with resource ID == nID.
    map<UINT,CWndControl*>::const_iterator result = m_resourceIdMap.find(nID);
    if ( result != m_resourceIdMap.end() )
    {
        CWndControl* pControl = result->second;
        if ( pControl != NULL )
        {
            // Let the control handle the msg.
            isHandled = pControl->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
        }
    }

    return isHandled;
}

UINT CWndContainer::GetNextResourceId()
{
    UINT nextId = m_currResourceId;
    ASSERT(nextId <= m_maxResourceId);
    ++m_currResourceId;
    
    return nextId;
}

void CWndContainer::UpdateControlNameMap(const CString& controlName, CWndControl* pControl)
{
    if ( controlName.IsEmpty() )
        return;

    map<CString,CWndControl*>::iterator result = m_controlNameMap.find(controlName);
    if ( result == m_controlNameMap.end() )
    {
        m_controlNameMap.insert(make_pair(controlName,pControl));
    }
    else
    {
        result->second = pControl;
    }
}

void CWndContainer::UpdateResourceIdMap(UINT resourceId, CWndControl* pControl)
{
    if ( resourceId == 0 )
        return;
    
    map<UINT,CWndControl*>::iterator result = m_resourceIdMap.find(resourceId);
    if ( result == m_resourceIdMap.end() )
    {
        m_resourceIdMap.insert(make_pair(resourceId,pControl));
    }
    else
    {
        result->second = pControl;
    }
}

void CWndContainer::ReturnToFreePool(CWndControl* pControl)
{
    // Record the CWnd instance before detaching so we can add
    // it to free pool.
    CWnd* pFreeWnd = pControl->GetAttachWnd();
    pControl->DetachWnd();

    // Hide the free CWnd.
    if ( pControl->GetTypeName() == _T("WebBrowser") )
    {
        // If web browser, we can move the wnd offscreen instead of hiding it
        // as this seems to destroy the HWND. However, this will leave the
        // web browser wnd active still (it may perform processing even though
        // it is not visible).
        MoveWnd(pFreeWnd);
    }    
    else
    {
        if ( pControl->GetTypeName() == _T("Button") )
        {
            // If button, turn off the default pushbutton style.
            CButton* pButton = static_cast<CButton*>(pFreeWnd);
            pButton->SetButtonStyle(pButton->GetButtonStyle() & !BS_DEFPUSHBUTTON);
        }
        HideWnd(pFreeWnd);
    }

    // Return the free wnd to the free pool.
    m_freePool->AddWnd(pControl->GetTypeName(), pFreeWnd);
}

// END
