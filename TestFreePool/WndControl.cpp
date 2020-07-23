// Filename: WndControl.cpp
// 08-Mar-2006 nschan Initial revision.

#include "stdafx.h"
#include "WndControl.h"
#include "WndEvent.h"
#include "EditEx.h"
#include "webbrowser2.h"

#include <afxpriv2.h>
#include <algorithm>

using namespace std;

//////////////////////////////////////////////////////////////////////////////////////
// Show or hide a CWnd.

void ShowWnd(CWnd* pWnd)
{
    ASSERT( pWnd != NULL );
    ASSERT( ::IsWindow(pWnd->m_hWnd) );

    if ( !(pWnd->GetStyle() & WS_VISIBLE) )
    {
        pWnd->ShowWindow(SW_SHOW);       
    }    
}

void HideWnd(CWnd* pWnd)
{
    ASSERT( pWnd != NULL );
    ASSERT( ::IsWindow(pWnd->m_hWnd) );

    if ( pWnd->GetStyle() & WS_VISIBLE )
    {
        pWnd->ShowWindow(SW_HIDE);       
    }
}

void MoveWnd(CWnd* pWnd)
{
    ASSERT( pWnd != NULL );
    ASSERT( ::IsWindow(pWnd->m_hWnd) );

    if ( pWnd->GetStyle() & WS_VISIBLE )
    {
        // Move window offscreen.
        pWnd->MoveWindow(-100,-100,10,10);
    }
}

//////////////////////////////////////////////////////////////////////////////////////
// Tokenize a CString

void TokenizeString(const CString& strInput, const CString& strDelim, list<CString>& tokenList)
{
    // Clear the target list.
    tokenList.clear();

    // Check for empty input string.
    if ( strInput.IsEmpty() )
        return;

    // Check for empty delimiter.
    if ( strDelim.IsEmpty() && !strInput.IsEmpty() )
    {
        tokenList.push_back(strInput);
        return;
    }

    // Proceed with tokenizing.
    CString currToken = _T("");
    int nInputLen = strInput.GetLength();
    for(int i = 0; i < nInputLen; ++i)
    {
        // Check if there is a delimiter at position i.
        if ( i < (nInputLen - strDelim.GetLength() + 1) &&
             strInput.Mid(i, strDelim.GetLength()).Compare(strDelim) == 0 )
        {
            if ( !currToken.IsEmpty() )
                tokenList.push_back(currToken);
            currToken = _T("");

            i += (strDelim.GetLength() - 1);
        } 
        else
        {
            currToken += strInput.GetAt(i);
        }
    }
    if ( !currToken.IsEmpty() )
        tokenList.push_back(currToken);

}

//////////////////////////////////////////////////////////////////////////////////////
// CWndControl

CWndControl::CWndControl()
{
    m_typeName = _T("Control");
    m_name = _T("");
    
    m_visible = true;
    m_enabled = true;
    m_readOnly = false;
    
    m_location = CPoint(0,0);
    m_size = CSize(0,0);
    
    m_resourceId = 0;
    m_attachWnd = NULL;
    m_attachFont = NULL;

    m_enableEvents = true;
    m_saveEnableEvents = true;
}

CWndControl::~CWndControl()
{
    m_attachWnd = NULL;
    m_attachFont = NULL;

    m_eventHandlers.clear();
}

void CWndControl::SetName(const CString& name)
{
    m_name = name;
}
    
void CWndControl::SetVisible(bool visible)
{
    m_visible = visible; 
    UpdateWnd();
}
    
void CWndControl::SetEnabled(bool enabled)
{
    m_enabled = enabled;
    UpdateWnd();
}
    
void CWndControl::SetReadOnly(bool readOnly)
{
    m_readOnly = readOnly;
    UpdateWnd();
}
    
void CWndControl::SetLocation(const CPoint& location)
{
    m_location = location;
    UpdateWnd();
}
    
void CWndControl::SetSize(const CSize& size)
{
    m_size = size;
    UpdateWnd();
}

CRect CWndControl::GetRect() const
{
    CRect rect(GetLocation().x, GetLocation().y, 
        GetLocation().x + GetSize().cx,
        GetLocation().y + GetSize().cy);
    
    return rect;
}

void CWndControl::AttachWnd(CWnd* pWnd)
{
    m_attachWnd = pWnd;
    m_resourceId = 0;

    // Record the attached wnd resource ID.
    if ( m_attachWnd != NULL && ::IsWindow(m_attachWnd->m_hWnd) )
    {
        m_resourceId = m_attachWnd->GetDlgCtrlID();
    }
}

void CWndControl::DetachWnd()
{
    m_attachWnd = NULL;
    m_resourceId = 0;
}

void CWndControl::AttachFont(CFont* pFont)
{
    m_attachFont = pFont;
}

void CWndControl::EnableEvents(bool enable)
{
    m_enableEvents = enable;
}

void CWndControl::SuspendEvents()
{
    m_saveEnableEvents = m_enableEvents;
    m_enableEvents = false;
}

void CWndControl::RestoreEvents()
{
    m_enableEvents = m_saveEnableEvents;
}

void CWndControl::AddEventHandler(IWndEventHandler* pEventHandler)
{
    // Check if this handler has already been added.
    list<IWndEventHandler*>::const_iterator result = find(m_eventHandlers.begin(), m_eventHandlers.end(), pEventHandler);
    if ( result == m_eventHandlers.end() )
    {
        m_eventHandlers.push_back(pEventHandler);
    }
}

void CWndControl::RemoveEventHandler(IWndEventHandler* pEventHandler)
{
    // Check if this handler is present.
    list<IWndEventHandler*>::const_iterator result = find(m_eventHandlers.begin(), m_eventHandlers.end(), pEventHandler);
    if ( result != m_eventHandlers.end() )
    {
        m_eventHandlers.remove(pEventHandler);
    }
}

void CWndControl::RemoveAllEventHandlers()
{
    m_eventHandlers.clear();
}

void CWndControl::AddLinkedControl(CWndControl* pControl)
{
    // Check if this control has already been added.
    list<CWndControl*>::const_iterator result = find(m_linkedControls.begin(), m_linkedControls.end(), pControl);
    if ( result == m_linkedControls.end() )
    {
        m_linkedControls.push_back(pControl);
        pControl->AddEventHandler(this);
    }
}

void CWndControl::RemoveLinkedControl(CWndControl* pControl)
{
    // Check if the control is in the list.
    list<CWndControl*>::const_iterator result = find(m_linkedControls.begin(), m_linkedControls.end(), pControl);
    if ( result != m_linkedControls.end() )
    {
        m_linkedControls.remove(pControl);
        pControl->RemoveEventHandler(this);
    }
}

void CWndControl::RemoveAllLinkedControls()
{
    while( true )
    {
        if ( m_linkedControls.size() == 0 )
            break;

        RemoveLinkedControl(m_linkedControls.front());
    }
}

bool CWndControl::NeedReposition()
{
    ASSERT( m_attachWnd != NULL );
    ASSERT( ::IsWindow(m_attachWnd->m_hWnd) );

    // Get current rect in screen coords.
    CRect currRect;
    m_attachWnd->GetWindowRect(&currRect);

    // Get the new rect and check if the size is different.
    CRect newRect = GetRect();
    if ( newRect.Width() != currRect.Width() )
        return true;
    if ( newRect.Height() != currRect.Height() )
        return true;

    // Get the parent of the attach wnd.
    CWnd* parentWnd = m_attachWnd->GetParent();
    if ( parentWnd != NULL )
    {
        // Convert the screen rect to client coords.
        parentWnd->ScreenToClient(&currRect);
        if ( currRect.left != newRect.left )
            return true;
        if ( currRect.top != newRect.top )
            return true;
    }

    return false;
}

void CWndControl::NotifyEventHandlers(const CWndEvent& ev)
{
    if ( !m_enableEvents )
        return;
    if ( m_eventHandlers.size() == 0 )
        return;

    list<IWndEventHandler*>::iterator i = m_eventHandlers.begin();
    for( ; i != m_eventHandlers.end(); i++)
    {
        IWndEventHandler* pHandler = *i;
        pHandler->HandleWndEvent(ev);
    }
} 

void CWndControl::UpdateEnabledState()
{
    ASSERT( m_attachWnd != NULL );
    ASSERT( ::IsWindow(m_attachWnd->m_hWnd) );

    bool isDisabled = false;
    if ( m_attachWnd->GetStyle() & WS_DISABLED )
        isDisabled = true;
    if ( isDisabled && IsEnabled() )
        m_attachWnd->EnableWindow(TRUE);
    else if ( !isDisabled && !IsEnabled() )
        m_attachWnd->EnableWindow(FALSE);
}

void CWndControl::UpdateVisibleState()
{
    ASSERT( m_attachWnd != NULL );
    ASSERT( ::IsWindow(m_attachWnd->m_hWnd) );

    if ( IsVisible() )
    {
        ShowWnd(m_attachWnd);
    }
    else
    {
        if ( m_typeName == _T("WebBrowser") )
        {
            // If web browser, we can move the wnd offscreen instead of hiding it
            // as this seems to destroy the HWND. However, this will leave the
            // web browser wnd active still (it may perform processing even though
            // it is not visible).
            MoveWnd(m_attachWnd);
        }
        else
        {
            if ( m_typeName == _T("Button") )
            {
                // If button, turn off the default pushbutton style.
                CButton* pButton = static_cast<CButton*>(m_attachWnd);
                pButton->SetButtonStyle(pButton->GetButtonStyle() & !BS_DEFPUSHBUTTON);
            }
            HideWnd(m_attachWnd);
        }
    }
}

void CWndControl::HandleWndEvent(const CWndEvent& ev)
{
    // Derived classes can override to handle linking to other controls.
}

//////////////////////////////////////////////////////////////////////////////////////
// CWndButton

CWndButton::CWndButton()
{
    m_typeName = _T("Button");
    m_text = _T("Button");
}

CWndButton::~CWndButton()
{
}

void CWndButton::SetText(const CString& text)
{
    m_text = text;
    UpdateWnd();
}

bool CWndButton::CreateWnd(CWnd* pParentWnd, UINT resourceId)
{
    if ( m_attachWnd == NULL )
        return false;
    
    m_resourceId = resourceId;
    
    CRect rect = GetRect();
    CButton* pControlWnd = static_cast<CButton*>(m_attachWnd);
    BOOL status = pControlWnd->Create(
        NULL,
        WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON,
        rect,
        pParentWnd,
        resourceId);
    if ( !status )
        return false;
    
    pControlWnd->SetFont(m_attachFont, FALSE);
    pControlWnd->SetWindowText(GetText());
        
    return true;
}

void CWndButton::UpdateWnd()
{
    if ( m_attachWnd == NULL )
        return;
    if ( !::IsWindow(m_attachWnd->m_hWnd) )
        return;

    // Update control location and size if needed.
    if ( NeedReposition() )
    {
        CRect rect = GetRect();
        m_attachWnd->SetWindowPos(
            NULL,
            rect.left,
            rect.top,
            rect.Width(),
            rect.Height(),
            SWP_NOZORDER | SWP_NOACTIVATE);
    }

    // Update enabled state.
    UpdateEnabledState();

    // Update text.
    CString currText;
    m_attachWnd->GetWindowText(currText);
    if ( currText.Compare(GetText()) != 0 )
        m_attachWnd->SetWindowText(GetText());

    // Update visibility state.
    UpdateVisibleState();
}

BOOL CWndButton::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
    if ( !m_enableEvents )
        return FALSE;
    if ( m_attachWnd == NULL || !::IsWindow(m_attachWnd->m_hWnd) )
        return FALSE;
    if ( nID != m_resourceId )
        return FALSE;

    // Check command code from winuser.h.
    if ( nCode == BN_CLICKED )
    {
        CWndEvent ev(this, _T("Button.Click"));
        NotifyEventHandlers(ev);
        return TRUE;
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////////////////
// CWndCheckBox

CWndCheckBox::CWndCheckBox()
{
    m_typeName = _T("CheckBox");
    m_text = _T("CheckBox");
    m_checked = false;
}

CWndCheckBox::~CWndCheckBox()
{
}

void CWndCheckBox::SetText(const CString& text)
{
    m_text = text;
    UpdateWnd();
}

void CWndCheckBox::SetChecked(bool checked)
{
    m_checked = checked;
    UpdateWnd();
}

bool CWndCheckBox::CreateWnd(CWnd* pParentWnd, UINT resourceId)
{
    if ( m_attachWnd == NULL )
        return false;

    m_resourceId = resourceId;

    CRect rect = GetRect();

    CButton* pControlWnd = static_cast<CButton*>(m_attachWnd);
    BOOL status = pControlWnd->Create(
                     NULL,
                     WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX,
                     rect,
                     pParentWnd,
                     resourceId);
    if ( !status )
        return false;

    pControlWnd->SetFont(m_attachFont, FALSE);
    pControlWnd->SetWindowText(GetText());
    pControlWnd->SetCheck(IsChecked() ? 1 : 0);

    return true;
}

void CWndCheckBox::UpdateWnd()
{
    if ( m_attachWnd == NULL )
        return;
    if ( !::IsWindow(m_attachWnd->m_hWnd) )
        return;

    // Update control location and size if needed.
    if ( NeedReposition() )
    {
        CRect rect = GetRect();
        m_attachWnd->SetWindowPos(
            NULL,
            rect.left,
            rect.top,
            rect.Width(),
            rect.Height(),
            SWP_NOZORDER | SWP_NOACTIVATE);
    }

    // Update enabled state.
    UpdateEnabledState();

    // Update text and checked state.
    CString currText;
    m_attachWnd->GetWindowText(currText);
    if ( currText.Compare(GetText()) != 0 )
        m_attachWnd->SetWindowText(GetText());

    CButton* pControlWnd = static_cast<CButton*>(m_attachWnd);
    pControlWnd->SetCheck(IsChecked() ? 1 : 0);

    // Update visibility state.
    UpdateVisibleState();
}

BOOL CWndCheckBox::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
    if ( !m_enableEvents )
        return FALSE;
    if ( m_attachWnd == NULL || !::IsWindow(m_attachWnd->m_hWnd) )
        return FALSE;
    if ( nID != m_resourceId )
        return FALSE;

     // Check command code from winuser.h.
    if ( nCode == BN_CLICKED )
    {
        CButton* pControlWnd = static_cast<CButton*>(m_attachWnd);
        if ( pControlWnd->GetCheck() > 0 )
            m_checked = true;
        else
            m_checked = false;

        CWndEvent ev(this, _T("CheckBox.CheckedChanged"));
        NotifyEventHandlers(ev);
        return TRUE;
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////////////////
// CWndComboBox

CWndComboBox::CWndComboBox()
{
    m_typeName = _T("ComboBox");    
    m_selectedIndex = -1;
}

CWndComboBox::~CWndComboBox()
{
}

void CWndComboBox::AddItem(const CString& item)
{
    m_items.push_back(item);
    UpdateWnd();
}

void CWndComboBox::AddItems(const std::list<CString>& itemList)
{
    list<CString>::const_iterator iter = itemList.begin();
    for( ; iter != itemList.end(); ++iter)
    {
        AddItem(*iter);
    }
    UpdateWnd();
}

void CWndComboBox::ClearItems()
{
    m_items.clear();
    m_selectedIndex = -1;
    UpdateWnd();
}

CString CWndComboBox::GetItem(int index) const
{
    if ( 0 <= index && index < (int)m_items.size() )
    {
        return m_items[index];
    }

    return _T("");
}

int CWndComboBox::GetCount() const
{
    return (int)m_items.size();
}

void CWndComboBox::SetItem(int index, const CString& item)
{
    if ( 0 <= index && index < (int)m_items.size() )
    {
        m_items[index] = item;
        UpdateWnd();
    }
}

int CWndComboBox::GetSelectedIndex() const
{
    return m_selectedIndex;
}

void CWndComboBox::SetSelectedIndex(int index)
{
    if ( -1 <= index && index < (int)m_items.size() )
    {
        m_selectedIndex = index;
        UpdateWnd();
    }
}

bool CWndComboBox::CreateWnd(CWnd* pParentWnd, UINT resourceId)
{
    if ( m_attachWnd == NULL )
        return false;

    m_resourceId = resourceId;

    CRect rect = GetRect();
    CComboBox* pControlWnd = static_cast<CComboBox*>(m_attachWnd);
    BOOL status = pControlWnd->Create(
                     WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | WS_VSCROLL | CBS_DROPDOWNLIST,
                     rect,
                     pParentWnd,
                     resourceId);
    if ( !status )
        return false;

    pControlWnd->SetFont(m_attachFont, FALSE);
    
    // Load items into combo box.
    pControlWnd->ResetContent();
    for(int i = 0; i < (int)m_items.size(); i++)
    {      
        pControlWnd->InsertString(-1, m_items[i]);
    }
    pControlWnd->SetCurSel(m_selectedIndex);

    return true;
}

void CWndComboBox::UpdateWnd()
{
    if ( m_attachWnd == NULL )
        return;
    if ( !::IsWindow(m_attachWnd->m_hWnd) )
        return;

    CComboBox* pControlWnd = static_cast<CComboBox*>(m_attachWnd);

    // Update control location and size if needed.
    if ( NeedReposition() )
    {
        CRect rect = GetRect();
        pControlWnd->SetWindowPos(
            NULL,
            rect.left,
            rect.top,
            rect.Width(),
            rect.Height(),
            SWP_NOZORDER | SWP_NOACTIVATE);
    }

    // Update enabled state.
    UpdateEnabledState();

    // Load items into combo box.
    pControlWnd->ResetContent();
    for(int i = 0; i < (int)m_items.size(); i++)
    {      
        pControlWnd->InsertString(-1, m_items[i]);
    }
    pControlWnd->SetCurSel(m_selectedIndex);

    // Update visibility state.
    UpdateVisibleState();
}

BOOL CWndComboBox::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
    if ( !m_enableEvents )
        return FALSE;
    if ( m_attachWnd == NULL || !::IsWindow(m_attachWnd->m_hWnd) )
        return FALSE;
    if ( nID != m_resourceId )
        return FALSE;

    // Check command code from winuser.h.
    if ( nCode == CBN_SELCHANGE )
    {   
        CComboBox* pControlWnd = static_cast<CComboBox*>(m_attachWnd);

        int oldSel = m_selectedIndex;
        int curSel = pControlWnd->GetCurSel();
        if ( oldSel != curSel )
        {
            m_selectedIndex = curSel;
        
            CWndEvent ev(this, _T("ComboBox.SelectedIndexChanged"));
            NotifyEventHandlers(ev);
        }

        return TRUE;
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////////////////
// CWndGroupBox

CWndGroupBox::CWndGroupBox()
{
    m_typeName = _T("GroupBox");
    m_text = _T("GroupBox");
}

CWndGroupBox::~CWndGroupBox()
{
}

void CWndGroupBox::SetText(const CString& text)
{
    m_text = text;
    UpdateWnd();
}

bool CWndGroupBox::CreateWnd(CWnd* pParentWnd, UINT resourceId)
{
    if ( m_attachWnd == NULL )
        return false;

    m_resourceId = resourceId;

    CRect rect = GetRect();

    CButton* pControlWnd = static_cast<CButton*>(m_attachWnd);
    BOOL status = pControlWnd->Create(
                     NULL,
                     WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
                     rect,
                     pParentWnd,
                     resourceId);
    if ( !status )
        return false;

    pControlWnd->ModifyStyleEx(0, WS_EX_TRANSPARENT);
    pControlWnd->SetFont(m_attachFont, FALSE);
    pControlWnd->SetWindowText(GetText());

    return true;
}

void CWndGroupBox::UpdateWnd()
{
    if ( m_attachWnd == NULL )
        return;
    if ( !::IsWindow(m_attachWnd->m_hWnd) )
        return;

    // Update the control location and size if needed.
    if ( NeedReposition() )
    {
        CRect rect = GetRect();
        m_attachWnd->SetWindowPos(
            NULL,
            rect.left,
            rect.top,
            rect.Width(),
            rect.Height(),
            SWP_NOZORDER | SWP_NOACTIVATE);
    }

    // Update enabled state.
    UpdateEnabledState();

    // Update text.
    CString currText;
    m_attachWnd->GetWindowText(currText);
    if ( currText.Compare(GetText()) != 0 )
        m_attachWnd->SetWindowText(GetText());

    // Update visibility state.
    UpdateVisibleState();
}

BOOL CWndGroupBox::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
    // This control does not support events.
    return FALSE;
}

//////////////////////////////////////////////////////////////////////////////////////
// CWndIpAddress

CWndIpAddress::CWndIpAddress()
{
    m_typeName = _T("IpAddress");
    m_field1 = 0;
    m_field2 = 0;
    m_field3 = 0;
    m_field4 = 0;
}

CWndIpAddress::~CWndIpAddress()
{
}

void CWndIpAddress::SetIpAddress(BYTE field1, BYTE field2, BYTE field3, BYTE field4)
{
    m_field1 = field1;
    m_field2 = field2;
    m_field3 = field3;
    m_field4 = field4;
    UpdateWnd();
}

void CWndIpAddress::GetIpAddress(BYTE& field1, BYTE& field2, BYTE& field3, BYTE& field4) const
{
    field1 = m_field1;
    field2 = m_field2;
    field3 = m_field3;
    field4 = m_field4;
}

void CWndIpAddress::SetIpAddress(const CString& ipAddress)
{
    if ( ipAddress.IsEmpty() )
        return;

    BYTE b0 = 0, b1 = 0, b2 = 0, b3 = 0;

    list<CString> tokenList;
    TokenizeString(ipAddress, _T("."), tokenList);
    if ( tokenList.size() > 0 )
    {
        b0 = _ttoi(tokenList.front());
        tokenList.pop_front();
    }
    if ( tokenList.size() > 0 )
    {
        b1 = _ttoi(tokenList.front());
        tokenList.pop_front();
    }
    if ( tokenList.size() > 0 )
    {
        b2 = _ttoi(tokenList.front());
        tokenList.pop_front();
    }
    if ( tokenList.size() > 0 )
    {
        b3 = _ttoi(tokenList.front());
    }

    SetIpAddress(b0, b1, b2, b3);
}

CString CWndIpAddress::GetIpAddress() const
{
    CString ipAddress;
    ipAddress.Format(_T("%d.%d.%d.%d"), m_field1, m_field2, m_field3, m_field4);

    return ipAddress;
}

bool CWndIpAddress::CreateWnd(CWnd* pParentWnd, UINT resourceId)
{
    if ( m_attachWnd == NULL )
        return false;

    m_resourceId = resourceId;

    CRect rect = GetRect();

    CIPAddressCtrl* pControlWnd = static_cast<CIPAddressCtrl*>(m_attachWnd);
    BOOL status = pControlWnd->Create(
         WS_CHILD | WS_VISIBLE | WS_TABSTOP,
         rect,
         pParentWnd,
         resourceId);
    if ( !status )
        return false;

    // Set font.
    pControlWnd->SetFont(m_attachFont, FALSE);

    // Update IP fields.
    pControlWnd->SetAddress(m_field1, m_field2, m_field3, m_field4);

    return true;
}

void CWndIpAddress:: UpdateWnd()
{
    if ( m_attachWnd == NULL )
        return;
    if ( !::IsWindow(m_attachWnd->m_hWnd) )
        return;

    CIPAddressCtrl* pControlWnd = static_cast<CIPAddressCtrl*>(m_attachWnd);

    // Update control location and size if needed.
    if ( NeedReposition() )
    {
        CRect rect = GetRect();
        pControlWnd->SetWindowPos(
            NULL,
            rect.left,
            rect.top,
            rect.Width(),
            rect.Height(),
            SWP_NOZORDER | SWP_NOACTIVATE);
    }

    // Update enabled state.
    UpdateEnabledState();

    // Update IP fields.
    pControlWnd->SetAddress(m_field1, m_field2, m_field3, m_field4);

    // Update visibility state.
    UpdateVisibleState();
}

BOOL CWndIpAddress::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
    if ( !m_enableEvents )
        return FALSE;
    if ( m_attachWnd == NULL || !::IsWindow(m_attachWnd->m_hWnd) )
        return FALSE;
    if ( nID != m_resourceId )
        return FALSE;

	if ( nCode == EN_CHANGE )
    {
        CIPAddressCtrl* pControlWnd = static_cast<CIPAddressCtrl*>(m_attachWnd);
        BYTE b0 = 0, b1 = 0, b2 = 0, b3 = 0;
        pControlWnd->GetAddress(b0, b1, b2, b3);
        
        // Drop update if nothing was changed.
        if ( b0 == m_field1 && b1 == m_field2 && b2 == m_field3 && b3 == m_field4 )
            return FALSE;
        m_field1 = b0;
        m_field2 = b1;
        m_field3 = b2;
        m_field4 = b3;
        
        CWndEvent ev(this, _T("IpAddress.TextChanged"));
        NotifyEventHandlers(ev);

        return TRUE;
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////////////////
// CWndLabel

CWndLabel::CWndLabel()
{
    m_typeName = _T("Label");
    m_text = _T("Label");
}

CWndLabel::~CWndLabel()
{
}

void CWndLabel::SetText(const CString& text)
{
    m_text = text;
    UpdateWnd();
}

bool CWndLabel::CreateWnd(CWnd* pParentWnd, UINT resourceId)
{
    if ( m_attachWnd == NULL )
        return false;

    m_resourceId = resourceId;

    CRect rect = GetRect();

    CStatic* pControlWnd = static_cast<CStatic*>(m_attachWnd);
    BOOL status = pControlWnd->Create(
                     NULL,
                     WS_CHILD | WS_VISIBLE | SS_LEFT,
                     rect,
                     pParentWnd,
                     resourceId);
    if ( !status )
        return false;

    pControlWnd->SetFont(m_attachFont, FALSE);
    pControlWnd->SetWindowText(GetText());

    return true;
}

void CWndLabel::UpdateWnd()
{
    if ( m_attachWnd == NULL )
        return;
    if ( !::IsWindow(m_attachWnd->m_hWnd) )
        return;

    // Update the control location and size if needed.
    if ( NeedReposition() )
    {
        CRect rect = GetRect();
        m_attachWnd->SetWindowPos(
            NULL,
            rect.left,
            rect.top,
            rect.Width(),
            rect.Height(),
            SWP_NOZORDER | SWP_NOACTIVATE);
    }

    // Update enabled state.
    UpdateEnabledState();

    // Update text.
    CString currText;
    m_attachWnd->GetWindowText(currText);
    if ( currText.Compare(GetText()) != 0 )
        m_attachWnd->SetWindowText(GetText());

    // Update visibility state.
    UpdateVisibleState();
}

BOOL CWndLabel::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
    // This control does not support events.
    return FALSE;
}

//////////////////////////////////////////////////////////////////////////////////////
// CWndListBox

CWndListBox::CWndListBox()
{
    m_typeName = _T("ListBox");    
    m_selectedIndex = -1;
}

CWndListBox::~CWndListBox()
{
}

void CWndListBox::AddItem(const CString& item)
{
    m_items.push_back(item);
    UpdateWnd();
}

void CWndListBox::AddItems(const std::list<CString>& itemList)
{
    list<CString>::const_iterator iter = itemList.begin();
    for( ; iter != itemList.end(); ++iter)
    {
        AddItem(*iter);
    }
    UpdateWnd();
}

void CWndListBox::ClearItems()
{
    m_items.clear();
    m_selectedIndex = -1;
    UpdateWnd();
}

CString CWndListBox::GetItem(int index) const
{
    if ( 0 <= index && index < (int)m_items.size() )
    {
        return m_items[index];
    }

    return _T("");
}

int CWndListBox::GetCount() const
{
    return (int)m_items.size();
}

void CWndListBox::SetItem(int index, const CString& item)
{
    if ( 0 <= index && index < (int)m_items.size() )
    {
        m_items[index] = item;
        UpdateWnd();
    }
}

int CWndListBox::GetSelectedIndex() const
{
    return m_selectedIndex;
}

void CWndListBox::SetSelectedIndex(int index)
{
    if ( -1 <= index && index < (int)m_items.size() )
    {
        m_selectedIndex = index;
        UpdateWnd();
    }
}

bool CWndListBox::CreateWnd(CWnd* pParentWnd, UINT resourceId)
{
    if ( m_attachWnd == NULL )
        return false;

    m_resourceId = resourceId;

    CRect rect = GetRect();
    CListBox* pControlWnd = static_cast<CListBox*>(m_attachWnd);
    BOOL status = pControlWnd->CreateEx(WS_EX_CLIENTEDGE, _T("LISTBOX"), NULL,
                     WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | LBS_NOTIFY,
                     rect,
                     pParentWnd,
                     resourceId);
    if ( !status )
        return false;

    pControlWnd->SetFont(m_attachFont, FALSE);
    
    // Load items into list box.
    pControlWnd->ResetContent();
    for(int i = 0; i < (int)m_items.size(); i++)
    {      
        pControlWnd->InsertString(-1, m_items[i]);
    }
    pControlWnd->SetCurSel(m_selectedIndex);

    return true;
}

void CWndListBox::UpdateWnd()
{
    if ( m_attachWnd == NULL )
        return;
    if ( !::IsWindow(m_attachWnd->m_hWnd) )
        return;

    CListBox* pControlWnd = static_cast<CListBox*>(m_attachWnd);

    // Update control location and size if needed.
    if ( NeedReposition() )
    {
        CRect rect = GetRect();
        pControlWnd->SetWindowPos(
            NULL,
            rect.left,
            rect.top,
            rect.Width(),
            rect.Height(),
            SWP_NOZORDER | SWP_NOACTIVATE);
    }

    // Update enabled state.
    UpdateEnabledState();

    // Load items into list box.
    pControlWnd->ResetContent();
    for(int i = 0; i < (int)m_items.size(); i++)
    {      
        pControlWnd->InsertString(-1, m_items[i]);
    }
    pControlWnd->SetCurSel(m_selectedIndex);

    // Update visibility state.
    UpdateVisibleState();
}

BOOL CWndListBox::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
    if ( !m_enableEvents )
        return FALSE;
    if ( m_attachWnd == NULL || !::IsWindow(m_attachWnd->m_hWnd) )
        return FALSE;
    if ( nID != m_resourceId )
        return FALSE;

    // Check command code from winuser.h.
    if ( nCode == LBN_SELCHANGE )
    {   
        CListBox* pControlWnd = static_cast<CListBox*>(m_attachWnd);

        int oldSel = m_selectedIndex;        
        int curSel = pControlWnd->GetCurSel();
        if ( oldSel != curSel )
        {
            m_selectedIndex = curSel;
        
            CWndEvent ev(this, _T("ListBox.SelectedIndexChanged"));
            NotifyEventHandlers(ev);
        }

        return TRUE;
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////////////////
// CWndRadioButton

CWndRadioButton::CWndRadioButton()
{
    m_typeName = _T("RadioButton");    
    m_text = _T("RadioButton");
    m_checked = false;
}

CWndRadioButton::~CWndRadioButton()
{
}

void CWndRadioButton::SetText(const CString& text)
{
    m_text = text;
    UpdateWnd();
}

void CWndRadioButton::SetChecked(bool checked)
{
    m_checked = checked;
    UpdateWnd();
}

bool CWndRadioButton::CreateWnd(CWnd* pParentWnd, UINT resourceId)
{
    if ( m_attachWnd == NULL )
        return false;

    m_resourceId = resourceId;

    CRect rect = GetRect();

    CButton* pControlWnd = static_cast<CButton*>(m_attachWnd);
    BOOL status = pControlWnd->Create(
                     NULL,
                     WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_RADIOBUTTON,
                     rect,
                     pParentWnd,
                     resourceId);
    if ( !status )
        return false;

    pControlWnd->SetFont(m_attachFont, FALSE);
    pControlWnd->SetWindowText(GetText());
    pControlWnd->SetCheck(IsChecked() ? 1 : 0);

    return true;
}

void CWndRadioButton::UpdateWnd()
{
    if ( m_attachWnd == NULL )
        return;
    if ( !::IsWindow(m_attachWnd->m_hWnd) )
        return;

    // Update control location and size if needed.
    if ( NeedReposition() )
    {
        CRect rect = GetRect();
        m_attachWnd->SetWindowPos(
            NULL,
            rect.left,
            rect.top,
            rect.Width(),
            rect.Height(),
            SWP_NOZORDER | SWP_NOACTIVATE);
    }

    // Update enabled state.
    UpdateEnabledState();

    // Update text.
    CString currText;
    m_attachWnd->GetWindowText(currText);
    if ( currText.Compare(GetText()) != 0 )
        m_attachWnd->SetWindowText(GetText());

    CButton* pControlWnd = static_cast<CButton*>(m_attachWnd);
    pControlWnd->SetCheck(IsChecked() ? 1 : 0);

    // Update visibility state.
    UpdateVisibleState();
}

BOOL CWndRadioButton::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
    if ( !m_enableEvents )
        return FALSE;
    if ( m_attachWnd == NULL || !::IsWindow(m_attachWnd->m_hWnd) )
        return FALSE;
    if ( nID != m_resourceId )
        return FALSE;

     // Check command code from winuser.h.
    if ( nCode == BN_CLICKED )
    {
        CButton* pControlWnd = static_cast<CButton*>(m_attachWnd);
        pControlWnd->SetCheck(1);

        if ( !m_checked )
        {
            m_checked = true;
            CWndEvent ev(this, _T("RadioButton.CheckedChanged"));
            NotifyEventHandlers(ev);

            UncheckLinkedControls(GetName());
        }

        return TRUE;
    }

    return FALSE;
}

void CWndRadioButton::HandleWndEvent(const CWndEvent& ev)
{
    CWndControl* pControl = ev.GetSender();
    if ( pControl->GetTypeName() == _T("RadioButton") )
    {
        CWndRadioButton* pRadioButton = static_cast<CWndRadioButton*>(pControl);
        if ( pRadioButton->IsChecked() )
        {
            // Uncheck ourselves.
            if ( IsChecked() )
            {
                SetChecked(false);

                CWndEvent ev(this, _T("RadioButton.CheckedChanged"));
                NotifyEventHandlers(ev);
            }

            // Uncheck linked controls except for the event sender.
            UncheckLinkedControls(pRadioButton->GetName());
        }
    }
}

void CWndRadioButton::UncheckLinkedControls(const CString& exceptName)
{
    list<CWndControl*>::iterator iter = m_linkedControls.begin();
    for( ; iter != m_linkedControls.end(); ++iter)
    {
        CWndControl* pControl = *iter;
        if ( pControl->GetName() == exceptName )
            continue;

        CWndRadioButton* pRadioButton = static_cast<CWndRadioButton*>(pControl);  
        if ( pRadioButton->IsChecked() )  
        {
            pRadioButton->SetChecked(false);  
            CWndEvent ev(pRadioButton, _T("RadioButton.CheckedChanged"));
            pRadioButton->NotifyEventHandlers(ev);
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////
// CWndSpinButton

CWndSpinButton::CWndSpinButton()
{
    m_typeName = _T("SpinButton");
    m_minimum = 0;
    m_maximum = 100;
    m_value = 0;
}

CWndSpinButton::~CWndSpinButton()
{
}

void CWndSpinButton::SetMinimum(int minimum)
{
    ASSERT(UD_MINVAL <= minimum && minimum <= UD_MAXVAL);

    m_minimum = minimum;
    UpdateWnd();
}

int CWndSpinButton::GetMinimum() const
{
    return m_minimum;
}

void CWndSpinButton::SetMaximum(int maximum)
{
    ASSERT(UD_MINVAL <= maximum && maximum <= UD_MAXVAL);

    m_maximum = maximum;
    UpdateWnd();
}

int CWndSpinButton::GetMaximum() const
{
    return m_maximum;
}

void CWndSpinButton::SetValue(int value)
{
    m_value = value;
    UpdateWnd();
}

int CWndSpinButton::GetValue() const
{
    return m_value;
}

bool CWndSpinButton::CreateWnd(CWnd* pParentWnd, UINT resourceId)
{
    if ( m_attachWnd == NULL )
        return false;

    m_resourceId = resourceId;

    CRect rect = GetRect();

    CSpinButtonCtrl* pControlWnd = static_cast<CSpinButtonCtrl*>(m_attachWnd);

    BOOL status = pControlWnd->Create(
        UDS_SETBUDDYINT | UDS_ARROWKEYS | UDS_NOTHOUSANDS,
        rect,
        pParentWnd,
        resourceId);
    if ( !status )
        return false;

    // Set font.
    pControlWnd->SetFont(m_attachFont, FALSE);

    // Set the range and position.
    pControlWnd->SetRange(GetMinimum(), GetMaximum());
    pControlWnd->SetPos(GetValue());

    return true;
}

void CWndSpinButton::UpdateWnd()
{
    if ( m_attachWnd == NULL )
        return;
    if ( !::IsWindow(m_attachWnd->m_hWnd) )
        return;

    CSpinButtonCtrl* pControlWnd = static_cast<CSpinButtonCtrl*>(m_attachWnd);

    // Update control location and size if needed.
    if ( NeedReposition() )
    {
        CRect rect = GetRect();
        pControlWnd->SetWindowPos(
            NULL,
            rect.left,
            rect.top,
            rect.Width(),
            rect.Height(),
            SWP_NOZORDER | SWP_NOACTIVATE);
    }

    // Update enabled state.
    UpdateEnabledState();

    // Update attributes.
    pControlWnd->SetRange(GetMinimum(), GetMaximum());
    pControlWnd->SetPos(GetValue());

    // Update visibility state.
    UpdateVisibleState();
}

BOOL CWndSpinButton::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
    if ( !m_enableEvents )
        return FALSE;
    if ( m_attachWnd == NULL || !::IsWindow(m_attachWnd->m_hWnd) )
        return FALSE;
    if ( nID != m_resourceId )
        return FALSE;

    CSpinButtonCtrl* pControlWnd = static_cast<CSpinButtonCtrl*>(m_attachWnd);
    m_value = pControlWnd->GetPos();
        
    CWndEvent ev(this, _T("SpinButton.Click"));
    NotifyEventHandlers(ev);

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////
// CWndTextBox

CWndTextBox::CWndTextBox()
{
    m_typeName = _T("TextBox");
    m_text = _T("");
}

CWndTextBox::~CWndTextBox()
{
}

void CWndTextBox::SetText(const CString& text)
{
    m_text = text;
    UpdateWnd();
}

bool CWndTextBox::CreateWnd(CWnd* pParentWnd, UINT resourceId)
{
    if ( m_attachWnd == NULL )
        return false;

    m_resourceId = resourceId;

    CRect rect = GetRect();

    // Remove WS_BORDER from style or else it doesn't look
    // correct using XP styles / manifest.
    CEditEx* pControlWnd = static_cast<CEditEx*>(m_attachWnd);
    BOOL status = pControlWnd->Create(
        WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL,
        rect,
        pParentWnd,
        resourceId);
    if ( !status )
        return false;

    // Set font.
    pControlWnd->SetFont(m_attachFont, FALSE);

    // Update read-only state.
    pControlWnd->SetReadOnly(IsReadOnly() ? TRUE : FALSE);

    // Update edit box text.
    pControlWnd->SetWindowText(GetText());

    return true;
}

void CWndTextBox::UpdateWnd()
{
    if ( m_attachWnd == NULL )
        return;
    if ( !::IsWindow(m_attachWnd->m_hWnd) )
        return;

    CEditEx* pControlWnd = static_cast<CEditEx*>(m_attachWnd);

    // Update control location and size if needed.
    if ( NeedReposition() )
    {
        CRect rect = GetRect();
        pControlWnd->SetWindowPos(
            NULL,
            rect.left,
            rect.top,
            rect.Width(),
            rect.Height(),
            SWP_NOZORDER | SWP_NOACTIVATE);
    }

    // Update enabled state.
    UpdateEnabledState();

    // Update read-only state.
    pControlWnd->SetReadOnly(IsReadOnly() ? TRUE : FALSE);

    // Update text.
    pControlWnd->SetWindowText(GetText());

    // Update visibility state.
    UpdateVisibleState();
}

BOOL CWndTextBox::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
    if ( !m_enableEvents )
        return FALSE;
    if ( m_attachWnd == NULL || !::IsWindow(m_attachWnd->m_hWnd) )
        return FALSE;
    if ( nID != m_resourceId )
        return FALSE;

    // Process lost focus or enter key press message.
	if ( nCode == EN_KILLFOCUS || nCode == EN_ENTER_KEY )
    {
        CEditEx* pControlWnd = static_cast<CEditEx*>(m_attachWnd);

        CString windowText;
        pControlWnd->GetWindowText(windowText);
        m_text = windowText;
        
        if ( nCode == EN_KILLFOCUS )
        {
            CWndEvent ev(this, _T("TextBox.LostFocus"));
            NotifyEventHandlers(ev);
        }
        else if ( nCode == EN_ENTER_KEY )
        {
            CWndEvent ev(this, _T("TextBox.EnterKeyPress"));
            NotifyEventHandlers(ev);
        }

        return TRUE;
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////////////////
// CWndTrackBar

CWndTrackBar::CWndTrackBar()
{
    m_typeName = _T("TrackBar");
    m_minimum = 0;
    m_maximum = 100;
    m_value = 50;
    m_tickFrequency = 10;
}

CWndTrackBar::~CWndTrackBar()
{
}

void CWndTrackBar::SetMinimum(int minimum)
{
    m_minimum = minimum;
    UpdateWnd();
}

int CWndTrackBar::GetMinimum() const
{
    return m_minimum;
}

void CWndTrackBar::SetMaximum(int maximum)
{
    m_maximum = maximum;
    UpdateWnd();
}

int CWndTrackBar::GetMaximum() const
{
    return m_maximum;
}

void CWndTrackBar::SetValue(int value)
{
    m_value = value;
    UpdateWnd();
}

int CWndTrackBar::GetValue() const
{
    return m_value;
}

void CWndTrackBar::SetTickFrequency(int tickFrequency)
{
    m_tickFrequency = tickFrequency;
    UpdateWnd();
}

int CWndTrackBar::GetTickFrequency() const
{
    return m_tickFrequency;
}

bool CWndTrackBar::CreateWnd(CWnd* pParentWnd, UINT resourceId)
{
    if ( m_attachWnd == NULL )
        return false;

    m_resourceId = resourceId;

    CRect rect = GetRect();

    CSliderCtrl* pControlWnd = static_cast<CSliderCtrl*>(m_attachWnd);

    BOOL status = pControlWnd->Create(
        WS_CHILD | WS_VISIBLE | WS_TABSTOP | TBS_AUTOTICKS,
        rect,
        pParentWnd,
        resourceId);
    if ( !status )
        return false;

    // Set font.
    pControlWnd->SetFont(m_attachFont, FALSE);

    // Set properties.
    pControlWnd->SetRange(GetMinimum(), GetMaximum());
    pControlWnd->SetTicFreq(GetTickFrequency());
    pControlWnd->SetPos(GetValue());

    return true;
}

void CWndTrackBar::UpdateWnd()
{
    if ( m_attachWnd == NULL )
        return;
    if ( !::IsWindow(m_attachWnd->m_hWnd) )
        return;

    CSliderCtrl* pControlWnd = static_cast<CSliderCtrl*>(m_attachWnd);

    // Update control location and size if needed.
    if ( NeedReposition() )
    {
        CRect rect = GetRect();
        pControlWnd->SetWindowPos(
            NULL,
            rect.left,
            rect.top,
            rect.Width(),
            rect.Height(),
            SWP_NOZORDER | SWP_NOACTIVATE);
    }

    // Update enabled state.
    UpdateEnabledState();

    // Update properties.
    pControlWnd->SetRange(GetMinimum(), GetMaximum());
    pControlWnd->SetTicFreq(GetTickFrequency());
    pControlWnd->SetPos(GetValue());
    if ( GetTickFrequency() <= 0 )
        pControlWnd->ModifyStyle(0, TBS_NOTICKS);
    else
        pControlWnd->ModifyStyle(TBS_NOTICKS, 0);

    // Update visibility state.
    UpdateVisibleState();
}

BOOL CWndTrackBar::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
    if ( !m_enableEvents )
        return FALSE;
    if ( m_attachWnd == NULL || !::IsWindow(m_attachWnd->m_hWnd) )
        return FALSE;
    if ( nID != m_resourceId )
        return FALSE;

    CSliderCtrl* pControlWnd = static_cast<CSliderCtrl*>(m_attachWnd);    
    if ( GetValue() != pControlWnd->GetPos() )
    {
        m_value = pControlWnd->GetPos();

        CWndEvent ev(this, _T("TrackBar.ValueChanged"));
        NotifyEventHandlers(ev);
    }

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////
// CWndWebBrowser

CWndWebBrowser::CWndWebBrowser()
{
    m_typeName = _T("WebBrowser");
    m_url = _T("");
}

CWndWebBrowser::~CWndWebBrowser()
{
}

CString CWndWebBrowser::GetUrl() const
{
    return m_url;
}

void CWndWebBrowser::SetUrl(const CString& url)
{
    m_url = url;
    UpdateWnd();
}
bool CWndWebBrowser::CreateWnd(CWnd* pParentWnd, UINT resourceId)
{
    if ( m_attachWnd == NULL )
        return false;

    m_resourceId = resourceId;

    CRect rect = GetRect();

    CWebBrowser2* pControlWnd = static_cast<CWebBrowser2*>(m_attachWnd);  

    BOOL status = pControlWnd->Create(
        NULL, 
        NULL, 
        WS_CHILD | WS_VISIBLE | WS_TABSTOP,
        rect,
        pParentWnd,
        resourceId);
    if ( !status )
        return false;

    // Set font.
    pControlWnd->SetFont(m_attachFont, FALSE);

    // Navigate to URL.
    if ( m_url.IsEmpty() )
    {
        pControlWnd->GoHome();
    }
    else
    {
        pControlWnd->Navigate(m_url, NULL, NULL, NULL, NULL);
    }

    return true;
}

void CWndWebBrowser::UpdateWnd()
{
    if ( m_attachWnd == NULL )
        return;
    if ( !::IsWindow(m_attachWnd->m_hWnd) )
        return;

    CWebBrowser2* pControlWnd = static_cast<CWebBrowser2*>(m_attachWnd);  

    // Update control location and size if needed.
    if ( NeedReposition() )
    {
        CRect rect = GetRect();
        pControlWnd->SetWindowPos(
            NULL,
            rect.left,
            rect.top,
            rect.Width(),
            rect.Height(),
            SWP_NOZORDER | SWP_NOACTIVATE);
    }

    // Update enabled state.
    UpdateEnabledState();

    // Navigate to URL.
    if ( m_url.IsEmpty() )
    {
        pControlWnd->GoHome();
    }
    else
    {
        pControlWnd->Navigate(m_url, NULL, NULL, NULL, NULL);
    }

    // Update visibility state.
    UpdateVisibleState();
}

BOOL CWndWebBrowser::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
    if ( !m_enableEvents )
        return FALSE;
    if ( m_attachWnd == NULL || !::IsWindow(m_attachWnd->m_hWnd) )
        return FALSE;
    if ( nID != m_resourceId )
        return FALSE;

    CWebBrowser2* pControlWnd = static_cast<CWebBrowser2*>(m_attachWnd);  

    // AFX_EVENT is a struct defined in MFC's afxpriv2.h include file.
	AFX_EVENT* pEvent = (AFX_EVENT *)pExtra;
	if ( pEvent->m_eventKind != 0 )
        return FALSE;

    // Process selected messages only - from exdispid.h in PlatformSDK\Include.
	// DISPID_NAVIGATECOMPLETE2: 252 = UIActivate new document
    if ( pEvent->m_dispid == 252 )
    {
        m_url = pControlWnd->GetLocationURL();

        CWndEvent ev(this, _T("WebBrowser.NavigateCompleted"));        
        NotifyEventHandlers(ev);
    }

    return FALSE;
}

// END
