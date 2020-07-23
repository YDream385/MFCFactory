// Filename: ControlXml.h
// 29-Nov-2006 nschan Initial revision. 

#ifndef CONTROL_XML_INCLUDED
#define CONTROL_XML_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>
#include <map>

// Forward class declarations.
class CControlGroup;
class CWndControl;
class CWndRadioButton;
class CWndFactory;
class CMarkup;

// Function pointer type for parsing a control.
typedef CWndControl* (*ParseControlFn)(CMarkup& xml, CWndFactory* controlFactory);

// CControlXml class uses CMarkup to parse an XML file
// and build the control tree in the form of top-level
// control group instances.
class CControlXml
{
public:
    CControlXml();
    ~CControlXml();
    
    bool ParseXml(const CString& xmlFile,
                  std::list<CControlGroup*>& topLevelGroupsList,
                  std::list<CWndControl*>& allControlsList);

private:
    bool ReadFile(const CString& xmlFile, CString& xmlText);
    bool RegisterParseControlFunction(const CString& strType, ParseControlFn parseControlFn);

    void ParseGroups(CMarkup& xml, CControlGroup* parentGroup, std::list<CControlGroup*>& groupList, std::list<CWndControl*>& controlList);
    bool ParseGroup(CMarkup& xml, CControlGroup* parentGroup, std::list<CControlGroup*>& groupList, std::list<CWndControl*>& controlList);    

    void        ParseControls(CMarkup& xml, CControlGroup* parentGroup, std::list<CWndControl*>& controlList);
    static void ParseControl_Common(CMarkup& xml, CWndControl* pControl);    
    void        PerformLinking(CMarkup& xml, CWndControl* pControl, std::list<CWndControl*>& controlList);

    static CWndControl* ParseControl_Button(CMarkup& xml, CWndFactory* controlFactory);
    static CWndControl* ParseControl_CheckBox(CMarkup& xml, CWndFactory* controlFactory);
    static CWndControl* ParseControl_ComboBox(CMarkup& xml, CWndFactory* controlFactory);
    static CWndControl* ParseControl_GroupBox(CMarkup& xml, CWndFactory* controlFactory);
    static CWndControl* ParseControl_IpAddress(CMarkup& xml, CWndFactory* controlFactory);
    static CWndControl* ParseControl_Label(CMarkup& xml, CWndFactory* controlFactory);
    static CWndControl* ParseControl_ListBox(CMarkup& xml, CWndFactory* controlFactory);
    static CWndControl* ParseControl_RadioButton(CMarkup& xml, CWndFactory* controlFactory);
    static CWndControl* ParseControl_SpinButton(CMarkup& xml, CWndFactory* controlFactory);
    static CWndControl* ParseControl_TextBox(CMarkup& xml, CWndFactory* controlFactory);
    static CWndControl* ParseControl_TrackBar(CMarkup& xml, CWndFactory* controlFactory);
    static CWndControl* ParseControl_WebBrowser(CMarkup& xml, CWndFactory* controlFactory);

    // Factory class for creating CWndControl subclasses.
    CWndFactory* m_controlFactory;

    // Store function pointers for parsing controls.
    std::map<CString, ParseControlFn> m_parseControlFunctions;
};

#endif // CONTROL_XML_INCLUDED

// END
