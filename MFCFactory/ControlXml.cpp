// Filename: ControlXml.cpp
// 29-Nov-2006 nschan Initial revision. 
//
// Note: The ReadFile() method is derived from Ben Bryant's MarkupDlg.cpp class
//       from the markup demo.

#include "stdafx.h"
#include "ControlXml.h"
#include "ControlGroup.h"
#include "WndControl.h"
#include "WndFactory.h"
#include "Markup.h"

using namespace std;

//////////////////////////////////////////////////////////////////////////////
// Conversion helper functions

static bool StringToBool(const CString& s, bool& flag)
{
    if ( s.IsEmpty() )
        return false;

    if ( s.CompareNoCase(_T("true")) == 0 )
        flag = true;
    else
        flag = false;

    return true;
}

static bool StringToInteger(const CString& s, int& val)
{
    if ( s.IsEmpty() )
        return false;

    val = _ttoi(s);

    return true;
}

static bool StringToItems(const CString& s, std::list<CString>& itemList)
{
    if ( s.IsEmpty() )
        return false;

   TokenizeString(s, _T("|"), itemList);

   return true;
}

static bool StringToLocation(const CString& s, const CString& t, CPoint& location)
{
    if ( s.IsEmpty() || t.IsEmpty() )
        return false;

    location.x = _ttoi(s);
    location.y = _ttoi(t);

    return true;
}

static bool StringToSize(const CString& s, const CString& t, CSize& size)
{
    if ( s.IsEmpty() || t.IsEmpty() )
        return false;

    size.cx = _ttoi(s);
    size.cy = _ttoi(t);

    return true;
}

//////////////////////////////////////////////////////////////////////////////
// CControlXml

CControlXml::CControlXml()
{
    m_controlFactory = new CWndFactory;

    // Register static parse control functions.
    RegisterParseControlFunction(_T("Button"),      ParseControl_Button);
    RegisterParseControlFunction(_T("CheckBox"),    ParseControl_CheckBox);
    RegisterParseControlFunction(_T("ComboBox"),    ParseControl_ComboBox);
    RegisterParseControlFunction(_T("GroupBox"),    ParseControl_GroupBox);
    RegisterParseControlFunction(_T("IpAddress"),   ParseControl_IpAddress);
    RegisterParseControlFunction(_T("Label"),       ParseControl_Label);
    RegisterParseControlFunction(_T("ListBox"),     ParseControl_ListBox);
    RegisterParseControlFunction(_T("RadioButton"), ParseControl_RadioButton);
    RegisterParseControlFunction(_T("SpinButton"),  ParseControl_SpinButton);
    RegisterParseControlFunction(_T("TextBox"),     ParseControl_TextBox);
    RegisterParseControlFunction(_T("TrackBar"),    ParseControl_TrackBar);
    RegisterParseControlFunction(_T("WebBrowser"),  ParseControl_WebBrowser);
}

CControlXml::~CControlXml()
{
    delete m_controlFactory;
}

bool CControlXml::ReadFile(const CString& xmlFile, CString& xmlText)
{
    // This code derived from Ben Bryant's MarkupDlg.cpp file.

    // Open file.
    CFile file;
    if ( !file.Open(xmlFile, CFile::modeRead) )
    {
        return false;
    }
    int nFileLen = (int)file.GetLength();

    // Allocate buffer for binary file data.
    unsigned char* pBuffer = new unsigned char[nFileLen + 2];
    nFileLen = file.Read( pBuffer, nFileLen );
    file.Close();
    pBuffer[nFileLen] = '\0';
    pBuffer[nFileLen+1] = '\0'; // in case 2-byte encoded

    // Windows Unicode file is detected if starts with FEFF.
    if ( pBuffer[0] == 0xff && pBuffer[1] == 0xfe )
    {
        // Contains byte order mark, so assume wide char content.
        // Non _UNICODE builds should perform UCS-2 (wide char) to UTF-8 conversion here.
        xmlText = (LPCWSTR)(&pBuffer[2]);
    }
    else
    {
        // _UNICODE builds should perform UTF-8 to UCS-2 (wide char) conversion here.
        xmlText = (LPCSTR)pBuffer;
    }
    delete [] pBuffer;

    // If it is too short, assume it got truncated due to non-text content.
    if ( xmlText.GetLength() < nFileLen / 2 - 20 )
    {
        return false;
    }

    return true;
}

bool CControlXml::RegisterParseControlFunction(const CString& strType, ParseControlFn parseControlFn)
{
    ASSERT( !strType.IsEmpty() );
    ASSERT( parseControlFn != NULL );
    
    // Check for existing entry in map.
    map<CString,ParseControlFn>::const_iterator result = m_parseControlFunctions.find(strType);
    if ( result != m_parseControlFunctions.end() )
        return false;
    
    // Insert new entry.  
    m_parseControlFunctions.insert(make_pair(strType,parseControlFn));
    
    return true;
}

bool CControlXml::ParseXml(const CString& xmlFile, std::list<CControlGroup*>& topLevelGroupsList, std::list<CWndControl*>& allControlsList)
{
    // Read the XML file into a CString.
    CString xmlText;
    if ( !ReadFile(xmlFile, xmlText) )
        return false;

    // Initialize the markup object (parser).
    CMarkup xml;
    xml.SetDoc(xmlText);
    xml.ResetPos();

    // There must be a <ControlWnd> element.
    if ( !xml.FindElem(_T("ControlWnd")) )
        return false;

    // Look for and parse the <ControlGroups> element.
    if ( xml.IntoElem() )
    {            
        ParseGroups(xml, NULL, topLevelGroupsList, allControlsList);

        xml.OutOfElem();
    }

    return true;
}

void CControlXml::ParseGroups(CMarkup& xml, CControlGroup* parentGroup, std::list<CControlGroup*>& groupList, std::list<CWndControl*>& controlList)
{
    // Look for <ControlGroups> element.
    if ( xml.FindElem(_T("ControlGroups")) )
    {
        if ( xml.IntoElem() )
        {
            while( true )
            {
                // Parse each control group (recursive method).
                if ( !ParseGroup(xml, parentGroup, groupList, controlList) )
                    break;
            }
            xml.OutOfElem();
        }
    }
}

bool CControlXml::ParseGroup(CMarkup& xml, CControlGroup* parentGroup, std::list<CControlGroup*>& groupList, std::list<CWndControl*>& controlList)
{
    // Look for <ControlGroup> element.
    if ( !xml.FindElem(_T("ControlGroup")) )
        return false;

    // Extract the group name.
    CString groupName = xml.GetAttrib(_T("Name"));
    if ( groupName.IsEmpty() )
        return false;
    
    // Create a new control group and add it to list.
    CControlGroup* group = new CControlGroup;
    group->SetName(groupName);
    if ( parentGroup == NULL )
        groupList.push_back(group);
    else
        parentGroup->AddGroup(group);
    
    // See if this control group has any child groups.
    if ( xml.IntoElem() )
    {
        ParseGroups(xml, group, groupList, controlList);
        xml.OutOfElem();
    }

    // See if this control group has any controls.
    if ( xml.IntoElem() )
    {
        ParseControls(xml, group, controlList);
        xml.OutOfElem();
    }

    return true;
}

void CControlXml::ParseControls(CMarkup& xml, CControlGroup* parentGroup, std::list<CWndControl*>& controlList)
{
    // Look for <Controls> element.
    if ( !xml.FindElem(_T("Controls")) )
        return;

    if ( xml.IntoElem() )
    {
        while( xml.FindElem() )
        {
            // Get the element tagname.
            CString strType = xml.GetTagName();
            if ( strType.IsEmpty() )
                continue;

            // Lookup the corresponding parse control function.
            map<CString,ParseControlFn>::const_iterator result = m_parseControlFunctions.find(strType);
            if ( result == m_parseControlFunctions.end() )
                continue;

            // Parse the control.
            ParseControlFn fn = result->second;
            CWndControl* pControl = (*fn)(xml, m_controlFactory);
 
            // Perform linking to other controls.
            PerformLinking(xml, pControl, controlList);

            // Add control to parent group and control list.
            parentGroup->AddControl(pControl);
            controlList.push_back(pControl);
        }

        xml.OutOfElem();
    }
}

void CControlXml::ParseControl_Common(CMarkup& xml, CWndControl* pControl)
{
    bool   flag;
    CPoint location;
    CSize  size;

    pControl->SetName(xml.GetAttrib(_T("Name")));        

    if ( StringToBool(xml.GetAttrib(_T("Visible")), flag) )
        pControl->SetVisible(flag);

    if ( StringToBool(xml.GetAttrib(_T("Enabled")), flag) )
        pControl->SetEnabled(flag);    

    if ( StringToBool(xml.GetAttrib(_T("ReadOnly")), flag) )
        pControl->SetReadOnly(flag);   
   
    if ( StringToLocation(xml.GetAttrib(_T("LocationX")), xml.GetAttrib(_T("LocationY")), location) )
        pControl->SetLocation(location);

    if ( StringToSize(xml.GetAttrib(_T("SizeWidth")), xml.GetAttrib(_T("SizeHeight")), size) )
        pControl->SetSize(size);
}

void CControlXml::PerformLinking(CMarkup& xml, CWndControl* pControl, std::list<CWndControl*>& controlList)
{
    // Only radio buttons use linking for now.
    if ( pControl->GetTypeName() == _T("RadioButton") )
    {
        // Get the radio manager name.
        CString managerName = xml.GetAttrib(_T("RadioManager"));
        if ( !managerName.IsEmpty() )
        {
            // Look in the control list for a radio button with this manager name.
            list<CWndControl*>::iterator iter = controlList.begin();
            for( ; iter != controlList.end(); ++iter)
            {
                CWndControl* pLinkFrom = *iter;
                if ( pLinkFrom->GetTypeName() == _T("RadioButton") &&
                     pLinkFrom->GetName() == managerName )
                {
                    // Set up the linking.
                    pLinkFrom->AddLinkedControl(pControl);
                }
            }
        }
    }
}

CWndControl* CControlXml::ParseControl_Button(CMarkup& xml, CWndFactory* controlFactory)
{
    // Create control.
    CWndButton* pControl = static_cast<CWndButton*>(controlFactory->CreateControl(_T("Button")));

    // Read attribute values from XML and use to set control.
    ParseControl_Common(xml, pControl);       
    
    // Parse button-specific attributes.
    pControl->SetText(xml.GetAttrib(_T("Text")));

    return pControl;
}

CWndControl* CControlXml::ParseControl_CheckBox(CMarkup& xml, CWndFactory* controlFactory)
{
    // Create control.
    CWndCheckBox* pControl = static_cast<CWndCheckBox*>(controlFactory->CreateControl(_T("CheckBox")));

    // Read attribute values from XML and use to set control.
    ParseControl_Common(xml, pControl);       
    
    // Parse checkbox-specific attributes.
    pControl->SetText(xml.GetAttrib(_T("Text")));

    bool checked;
    if ( StringToBool(xml.GetAttrib(_T("Checked")), checked) )
        pControl->SetChecked(checked);        

    return pControl;
}

CWndControl* CControlXml::ParseControl_ComboBox(CMarkup& xml, CWndFactory* controlFactory)
{
    // Create control.
    CWndComboBox* pControl = static_cast<CWndComboBox*>(controlFactory->CreateControl(_T("ComboBox")));

    // Read attribute values from XML and use to set control.
    ParseControl_Common(xml, pControl);       
    
    // Parse combobox-specific attributes.        
    list<CString> itemList;
    if ( StringToItems(xml.GetAttrib(_T("Items")), itemList) )
        pControl->AddItems(itemList);        

    int val;
    if ( StringToInteger(xml.GetAttrib(_T("SelectedIndex")), val) )
        pControl->SetSelectedIndex(val);

    return pControl;
}

CWndControl* CControlXml::ParseControl_GroupBox(CMarkup& xml, CWndFactory* controlFactory)
{
    // Create control.
    CWndGroupBox* pControl = static_cast<CWndGroupBox*>(controlFactory->CreateControl(_T("GroupBox")));

    // Read attribute values from XML and use to set control.
    ParseControl_Common(xml, pControl);       
    
    // Parse groupbox-specific attributes.
    pControl->SetText(xml.GetAttrib(_T("Text")));

    return pControl;
}

CWndControl* CControlXml::ParseControl_IpAddress(CMarkup& xml, CWndFactory* controlFactory)
{
    // Create control.
    CWndIpAddress* pControl = static_cast<CWndIpAddress*>(controlFactory->CreateControl(_T("IpAddress")));

    // Read attribute values from XML and use to set control.
    ParseControl_Common(xml, pControl);       
    
    // Parse ipaddress-specific attributes.
    pControl->SetIpAddress(xml.GetAttrib(_T("IP")));

    return pControl;
}

CWndControl* CControlXml::ParseControl_Label(CMarkup& xml, CWndFactory* controlFactory)
{
    // Create control.
    CWndLabel* pControl = static_cast<CWndLabel*>(controlFactory->CreateControl(_T("Label")));

    // Read attribute values from XML and use to set control.
    ParseControl_Common(xml, pControl);       
    
    // Parse label-specific attributes.
    pControl->SetText(xml.GetAttrib(_T("Text")));

    return pControl;
}

CWndControl* CControlXml::ParseControl_ListBox(CMarkup& xml, CWndFactory* controlFactory)
{
    // Create control.
    CWndListBox* pControl = static_cast<CWndListBox*>(controlFactory->CreateControl(_T("ListBox")));

    // Read attribute values from XML and use to set control.
    ParseControl_Common(xml, pControl);       
    
    // Parse listbox-specific attributes.       
    list<CString> itemList;
    if ( StringToItems(xml.GetAttrib(_T("Items")), itemList) )
        pControl->AddItems(itemList);        

    int val;
    if ( StringToInteger(xml.GetAttrib(_T("SelectedIndex")), val) )
        pControl->SetSelectedIndex(val);

    return pControl;
}

CWndControl* CControlXml::ParseControl_RadioButton(CMarkup& xml, CWndFactory* controlFactory)
{
    // Create control.
    CWndRadioButton* pControl = static_cast<CWndRadioButton*>(controlFactory->CreateControl(_T("RadioButton")));

    // Read attribute values from XML and use to set control.
    ParseControl_Common(xml, pControl);       
    
    // Parse radiobutton-specific attributes.
    pControl->SetText(xml.GetAttrib(_T("Text")));

    bool checked;
    if ( StringToBool(xml.GetAttrib(_T("Checked")), checked) )
        pControl->SetChecked(checked);

    return pControl;
}

CWndControl* CControlXml::ParseControl_SpinButton(CMarkup& xml, CWndFactory* controlFactory)
{
    // Create control.
    CWndSpinButton* pControl = static_cast<CWndSpinButton*>(controlFactory->CreateControl(_T("SpinButton")));

    // Read attribute values from XML and use to set control.
    ParseControl_Common(xml, pControl);       
    
    // Parse spinbutton-specific attributes.       
    int val;
    if ( StringToInteger(xml.GetAttrib(_T("Minimum")), val) )
        pControl->SetMinimum(val);

    if ( StringToInteger(xml.GetAttrib(_T("Maximum")), val) )
        pControl->SetMaximum(val);        

    if ( StringToInteger(xml.GetAttrib(_T("Value")), val) )
        pControl->SetValue(val);

    return pControl;
}

CWndControl* CControlXml::ParseControl_TextBox(CMarkup& xml, CWndFactory* controlFactory)
{
    // Create control.
    CWndTextBox* pControl = static_cast<CWndTextBox*>(controlFactory->CreateControl(_T("TextBox")));

    // Read attribute values from XML and use to set control.
    ParseControl_Common(xml, pControl);       
    
    // Parse textbox-specific attributes.
    pControl->SetText(xml.GetAttrib(_T("Text")));

    return pControl;
}

CWndControl* CControlXml::ParseControl_TrackBar(CMarkup& xml, CWndFactory* controlFactory)
{
    // Create control.
    CWndTrackBar* pControl = static_cast<CWndTrackBar*>(controlFactory->CreateControl(_T("TrackBar")));

    // Read attribute values from XML and use to set control.
    ParseControl_Common(xml, pControl);       
    
    // Parse trackbar-specific attributes.
    int val;
    if ( StringToInteger(xml.GetAttrib(_T("Minimum")), val) )
        pControl->SetMinimum(val);

    if ( StringToInteger(xml.GetAttrib(_T("Maximum")), val) )
        pControl->SetMaximum(val);        

    if ( StringToInteger(xml.GetAttrib(_T("TickFrequency")), val) )
        pControl->SetTickFrequency(val);

    if ( StringToInteger(xml.GetAttrib(_T("Value")), val) )
        pControl->SetValue(val);

    return pControl;
}

CWndControl* CControlXml::ParseControl_WebBrowser(CMarkup& xml, CWndFactory* controlFactory)
{
    // Create control.
    CWndWebBrowser* pControl = static_cast<CWndWebBrowser*>(controlFactory->CreateControl(_T("WebBrowser")));

    // Read attribute values from XML and use to set control.
    ParseControl_Common(xml, pControl);       
    
    // Parse webbrowser-specific attributes.
    pControl->SetUrl(xml.GetAttrib(_T("Url")));

    return pControl;
}

// END

