// Filename: WndControl.h
// 08-Mar-2006 nschan Initial revision.

#ifndef WND_CONTROL_INCLUDED
#define WND_CONTROL_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WndEvent.h"

#include <list>
#include <vector>

// Forward class declarations.
class CWndEvent;
class IWndEventHandler;

// Exported functions.
void ShowWnd(CWnd* pWnd);
void HideWnd(CWnd* pWnd);
void MoveWnd(CWnd* pWnd);
void TokenizeString(const CString& strInput, const CString& strDelim, std::list<CString>& tokenList);

// CWndControl base class (abstract).
class CWndControl : public IWndEventHandler
{
public:
    // Constructor / destructor.
    CWndControl();
    virtual ~CWndControl();
    
    // Type string.
    const CString& GetTypeName() const { return m_typeName; }
    
    // General purpose name identifier.
    const CString& GetName() const { return m_name; }
    void  SetName(const CString& name);
    
    // Visibility.
    bool IsVisible() const { return m_visible; }
    void SetVisible(bool visible);
    
    // Enabled state.
    bool IsEnabled() const { return m_enabled; }
    void SetEnabled(bool enabled);
    
    // Read-only state.
    bool IsReadOnly() const { return m_readOnly; }
    void SetReadOnly(bool readOnly);
    
    // Location.
    const CPoint& GetLocation() const { return m_location; }
    void  SetLocation(const CPoint& location);
    
    // Size. 
    const CSize& GetSize() const { return m_size; }
    void  SetSize(const CSize& size);
    CRect GetRect() const;
    
    // CWnd resource ID.
    UINT GetResourceId() const { return m_resourceId; }
    
    // CWnd attachment.
    void  AttachWnd(CWnd* pWnd);
    void  DetachWnd();
    CWnd* GetAttachWnd() { return m_attachWnd; }
    
    // CFont attachment.
    void AttachFont(CFont* pFont);
    
    // Events.
    void EnableEvents(bool enable);
    void SuspendEvents();
    void RestoreEvents();
    void AddEventHandler(IWndEventHandler* pEventHandler);
    void RemoveEventHandler(IWndEventHandler* pEventHandler);
    void RemoveAllEventHandlers();

    // Link to other CWndControls.
    void AddLinkedControl(CWndControl* pControl);
    void RemoveLinkedControl(CWndControl* pControl);
    void RemoveAllLinkedControls();

    // Pure virtual methods.
    virtual bool CreateWnd(CWnd* pParentWnd, UINT resourceId) = 0;
    virtual void UpdateWnd() = 0;
    virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) = 0;
    
    // IWndEventHandler overrides.
    virtual void HandleWndEvent(const CWndEvent& ev);

protected:
    bool NeedReposition();
    void NotifyEventHandlers(const CWndEvent& event);
    void UpdateEnabledState();
    void UpdateVisibleState();

    CString m_typeName;
    CString m_name;
    
    bool m_visible;
    bool m_enabled;
    bool m_readOnly;
    
    CPoint m_location;
    CSize  m_size;
    
    UINT   m_resourceId;
    CWnd*  m_attachWnd;
    CFont* m_attachFont;
    
    // Events.
    bool m_enableEvents;
    bool m_saveEnableEvents;
    std::list<IWndEventHandler*> m_eventHandlers;

    // Store references to other controls, to allow handling
    // of events from those controls. Used primarily by subclasses
    // which manage other control instances, such as radio buttons.
    std::list<CWndControl*> m_linkedControls;
};

//////////////////////////////////////////////////////////////////////////////////////

// CWndButton class.
class CWndButton : public CWndControl
{
public:
    CWndButton();
    virtual ~CWndButton();
    
    const CString& GetText() const { return m_text; }
    void           SetText(const CString& text);
    
    // CWndControl overrides.
    virtual bool CreateWnd(CWnd* pParentWnd, UINT resourceId);
    virtual void UpdateWnd();
    virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
    
private:
    CString m_text;
};

//////////////////////////////////////////////////////////////////////////////////////

// CWndCheckBox class.
class CWndCheckBox : public CWndControl
{
public:
    CWndCheckBox();
    virtual ~CWndCheckBox();
    
    const CString& GetText() const { return m_text; }
    void           SetText(const CString& text);
    
    void  SetChecked(bool checked);
    bool  IsChecked() const { return m_checked; }

    // CWndControl overrides.
    virtual bool CreateWnd(CWnd* pParentWnd, UINT resourceId);
    virtual void UpdateWnd();
    virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
    
private:
    CString m_text;
    bool    m_checked;
};

//////////////////////////////////////////////////////////////////////////////////////

// CWndComboBox class.
class CWndComboBox : public CWndControl
{
public:
    CWndComboBox();
    virtual ~CWndComboBox();

    void    AddItem(const CString& item);
    void    AddItems(const std::list<CString>& itemList);
    void    ClearItems();
    
    CString GetItem(int index) const;
    int     GetCount() const;
    void    SetItem(int index, const CString& item);

    int     GetSelectedIndex() const;
    void    SetSelectedIndex(int index);

    // CWndControl overrides.
    virtual bool CreateWnd(CWnd* pParentWnd, UINT resourceId);
    virtual void UpdateWnd();
    virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

private:
    std::vector<CString> m_items;

    int m_selectedIndex;
};

//////////////////////////////////////////////////////////////////////////////////////

// CWndGroupBox class.
class CWndGroupBox : public CWndControl
{
public:
    CWndGroupBox();
    virtual ~CWndGroupBox();
    
    const CString& GetText() const { return m_text; }
    void           SetText(const CString& text);
    
    // CWndControl overrides.
    virtual bool CreateWnd(CWnd* pParentWnd, UINT resourceId);
    virtual void UpdateWnd();
    virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
    
private:
    CString m_text;
};

//////////////////////////////////////////////////////////////////////////////////////

// CWndIpAddress class.
class CWndIpAddress : public CWndControl
{
public:
    CWndIpAddress();
    virtual ~CWndIpAddress();

    void SetIpAddress(BYTE field1, BYTE field2, BYTE field3, BYTE field4); 
    void GetIpAddress(BYTE& field1, BYTE& field2, BYTE& field3, BYTE& field4) const;

    void    SetIpAddress(const CString& ipAddress);
    CString GetIpAddress() const;

    // CWndControl overrides.
    virtual bool CreateWnd(CWnd* pParentWnd, UINT resourceId);
    virtual void UpdateWnd();
    virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

private:
    BYTE m_field1;
    BYTE m_field2;
    BYTE m_field3;
    BYTE m_field4;
};

//////////////////////////////////////////////////////////////////////////////////////

// CWndLabel class.
class CWndLabel : public CWndControl
{
public:
    CWndLabel();
    virtual ~CWndLabel();
    
    const CString& GetText() const { return m_text; }
    void           SetText(const CString& text);
    
    // CWndControl overrides.
    virtual bool CreateWnd(CWnd* pParentWnd, UINT resourceId);
    virtual void UpdateWnd();
    virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
    
private:
    CString m_text;
};

//////////////////////////////////////////////////////////////////////////////////////

// CWndListBox class.
class CWndListBox : public CWndControl
{
public:
    CWndListBox();
    virtual ~CWndListBox();

    void    AddItem(const CString& item);
    void    AddItems(const std::list<CString>& itemList);
    void    ClearItems();
    
    CString GetItem(int index) const;
    int     GetCount() const;
    void    SetItem(int index, const CString& item);

    int     GetSelectedIndex() const;
    void    SetSelectedIndex(int index);

    // CWndControl overrides.
    virtual bool CreateWnd(CWnd* pParentWnd, UINT resourceId);
    virtual void UpdateWnd();
    virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

private:
    std::vector<CString> m_items;

    int m_selectedIndex;
};

//////////////////////////////////////////////////////////////////////////////////////

// CWndRadioButton class.
class CWndRadioButton : public CWndControl
{
public:
    CWndRadioButton();
    virtual ~CWndRadioButton();
    
    const CString& GetText() const { return m_text; }
    void           SetText(const CString& text);
    
    void  SetChecked(bool checked);
    bool  IsChecked() const { return m_checked; }

    // CWndControl overrides.
    virtual bool CreateWnd(CWnd* pParentWnd, UINT resourceId);
    virtual void UpdateWnd();
    virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
    
    // IWndEventHandler overrides.
    virtual void HandleWndEvent(const CWndEvent& ev);

private:
    void    UncheckLinkedControls(const CString& exceptName);

    CString m_text;
    bool    m_checked;
};

//////////////////////////////////////////////////////////////////////////////////////

// CWndSpinButton class.
class CWndSpinButton : public CWndControl
{
public:
    CWndSpinButton();
    virtual ~CWndSpinButton();

    void SetMinimum(int minimum); 
    int  GetMinimum() const;

    void SetMaximum(int maximum); 
    int  GetMaximum() const;

    void SetValue(int value); 
    int  GetValue() const;

    // CWndControl overrides.
    virtual bool CreateWnd(CWnd* pParentWnd, UINT resourceId);
    virtual void UpdateWnd();
    virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

private:
    int m_minimum;
    int m_maximum;
    int m_value;
};

//////////////////////////////////////////////////////////////////////////////////////

// CWndTextBox class.
class CWndTextBox : public CWndControl
{
public:
    CWndTextBox();
    virtual ~CWndTextBox();
    
    const CString& GetText() const { return m_text; }
    void           SetText(const CString& text);
    
    // CWndControl overrides.
    virtual bool CreateWnd(CWnd* pParentWnd, UINT resourceId);
    virtual void UpdateWnd();
    virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
    
private:
    CString m_text;
};

//////////////////////////////////////////////////////////////////////////////////////

// CWndTrackBar class.
class CWndTrackBar : public CWndControl
{
public:
    CWndTrackBar();
    virtual ~CWndTrackBar();

    void SetMinimum(int minimum); 
    int  GetMinimum() const;

    void SetMaximum(int maximum); 
    int  GetMaximum() const;

    void SetValue(int value); 
    int  GetValue() const;

    void SetTickFrequency(int tickFrequency);
    int  GetTickFrequency() const;

    // CWndControl overrides.
    virtual bool CreateWnd(CWnd* pParentWnd, UINT resourceId);
    virtual void UpdateWnd();
    virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

private:
    int m_minimum;
    int m_maximum;
    int m_value;
    int m_tickFrequency;
};

//////////////////////////////////////////////////////////////////////////////////////

// CWndWebBrowser class.
class CWndWebBrowser : public CWndControl
{
public:
    CWndWebBrowser();
    virtual ~CWndWebBrowser();

    CString GetUrl() const;
    void    SetUrl(const CString& url);

    // CWndControl overrides.
    virtual bool CreateWnd(CWnd* pParentWnd, UINT resourceId);
    virtual void UpdateWnd();
    virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

private:
    CString m_url;
};

#endif // WND_CONTROL_INCLUDED

// END
