// Filename: WndEvent.cpp
// 21-May-2006 nschan Initial revision.

#include "stdafx.h"
#include "WndEvent.h"

using namespace std;

CWndEvent::CWndEvent(CWndControl* sender, const CString& text)
{
    m_sender = sender;
    m_text = text;
}

CWndEvent::~CWndEvent()
{
}

CWndControl* CWndEvent::GetSender() const
{
    return m_sender;
}

CString CWndEvent::GetText() const
{
    return m_text;
}

void CWndEvent::AddProperty(const CString& name, const CString& value)
{
    map<CString,CString>::iterator result = m_properties.find(name);
    if ( result == m_properties.end() )
    {
        m_properties.insert(make_pair(name,value));
    }
    else
    {
        result->second = value;
    }
}

bool CWndEvent::GetProperty(const CString& name, CString& value) const
{
    map<CString,CString>::const_iterator result = m_properties.find(name);
    if ( result != m_properties.end() )
    {
        value = result->second;
        return true;
    }

    return false;
}

// END