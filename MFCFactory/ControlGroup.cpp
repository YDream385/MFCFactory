// Filename: ControlGroup.cpp
// 22-Nov-2006 nschan Initial revision. 

#include "stdafx.h"
#include "ControlGroup.h"
#include "WndControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;

CControlGroup::CControlGroup()
{
    m_groupName = _T("");
}

CControlGroup::~CControlGroup()
{
    // We do not own the controls, so just clear the list.
    m_controlList.clear();

    // We own the child groups, so delete them.
    list<CControlGroup*>::iterator iter = m_groupList.begin();
    for( ; iter != m_groupList.end(); ++iter)
    {
        delete *iter;
    }
    m_groupList.clear();
}

void CControlGroup::SetName(const CString& name)
{
    m_groupName = name;
}

const CString& CControlGroup::GetName() const
{
    return m_groupName;
}

void CControlGroup::AddGroup(CControlGroup* pGroup)
{
    ASSERT( pGroup != NULL );
    m_groupList.push_back(pGroup);
}

void CControlGroup::RemoveGroup(CControlGroup* pGroup)
{
    ASSERT( pGroup != NULL );
    try
    {
        m_groupList.remove(pGroup);
    }
    catch(...)
    {}
}

void CControlGroup::RemoveAllGroups()
{
    m_groupList.clear();
}

void CControlGroup::GetGroups(std::list<CControlGroup*>& groupList) const
{
    groupList = m_groupList;
}

int CControlGroup::GetNumGroups() const
{
    return (int)m_groupList.size();
}

void CControlGroup::AddControl(CWndControl* pControl)
{
    ASSERT( pControl != NULL );
    m_controlList.push_back(pControl);
}

void CControlGroup::RemoveControl(CWndControl* pControl)
{
    ASSERT( pControl != NULL );
    try
    {
        m_controlList.remove(pControl);
    }
    catch(...)
    {}
}

void CControlGroup::RemoveAllControls()
{
    m_controlList.clear();
}

void CControlGroup::GetControls(std::list<CWndControl*>& controlList) const
{
    controlList = m_controlList;
}

int CControlGroup::GetNumControls() const
{
    return (int)m_controlList.size();
}

// END

