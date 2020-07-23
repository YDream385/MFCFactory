// Filename: WndEvent.h
// 21-May-2006 nschan Initial revision.

#ifndef WND_EVENT_INCLUDED
#define WND_EVENT_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>

// Forward class declarations.
class CWndControl;

// CWndEvent class.
class CWndEvent
{
public:
    // Constructor / destructor.
    CWndEvent(CWndControl* sender, const CString& text);
    ~CWndEvent();
    
    // Public methods.
    CWndControl* GetSender() const;
    CString      GetText() const;
    void         AddProperty(const CString& name, const CString& value);
    bool         GetProperty(const CString& name, CString& value) const;

private:
    // Event sender.
    CWndControl* m_sender;

    // Event text.
    CString m_text;

    // Event properties.
    std::map<CString,CString> m_properties;
};

// IWndEventHandler interface.
class IWndEventHandler
{
public:
    virtual void HandleWndEvent(const CWndEvent& ev) = 0;
};

#endif // WND_EVENT_INCLUDED

// END