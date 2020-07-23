// Filename: ControlGroup.h
// 22-Nov-2006 nschan Initial revision. 

#ifndef CONTROL_GROUP_INCLUDED
#define CONTROL_GROUP_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>

// Forward class declarations.
class CWndControl;
class CControlGroup;

// CControlGroup represents a "control group", which is analogous
// to a folder in a file system. A control group may contain other
// groups, and also may contain controls (analogous to files in
// a file system).
class CControlGroup
{
public:
    // Constructor / destructor.
    CControlGroup();
    ~CControlGroup();
    
    // Group attributes.
    void SetName(const CString& name);
    const CString& GetName() const;

    // Manage sub-groups.
    void AddGroup(CControlGroup* pGroup);
    void RemoveGroup(CControlGroup* pGroup);
    void RemoveAllGroups();
    void GetGroups(std::list<CControlGroup*>& groupList) const;
    int  GetNumGroups() const;

    // Manage controls.
    void AddControl(CWndControl* pControl);
    void RemoveControl(CWndControl* pControl);
    void RemoveAllControls();       
    void GetControls(std::list<CWndControl*>& controlList) const;
    int  GetNumControls() const;

private:
    CString m_groupName;

    // List of control groups.
    std::list<CControlGroup*> m_groupList;

    // List of controls.
    std::list<CWndControl*> m_controlList;
};

#endif // CONTROL_GROUP_INCLUDED

// END
