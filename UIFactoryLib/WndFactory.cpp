// Filename: WndFactory.cpp
// 04-Mar-2006 nschan Initial revision.

#include "stdafx.h"
#include "WndFactory.h"
#include "WndControl.h"
#include "EditEx.h"
#include "webbrowser2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;

//////////////////////////////////////////////////////////////////////////////
// Button 

static CWnd* CreateWnd_Button()
{
    CWnd* pWnd = new CButton;
    return pWnd;
}

static CWndControl* CreateControl_Button()
{
    CWndControl* pControl = new CWndButton;
    return pControl;
}

//////////////////////////////////////////////////////////////////////////////
// CheckBox

static CWnd* CreateWnd_CheckBox()
{
    CWnd* pWnd = new CButton;
    return pWnd;
}

static CWndControl* CreateControl_CheckBox()
{
    CWndControl* pControl = new CWndCheckBox;
    return pControl;
}

//////////////////////////////////////////////////////////////////////////////
// ComboBox

static CWnd* CreateWnd_ComboBox()
{
    CWnd* pWnd = new CComboBox;
    return pWnd;
}

static CWndControl* CreateControl_ComboBox()
{
    CWndControl* pControl = new CWndComboBox;
    return pControl;
}

//////////////////////////////////////////////////////////////////////////////
// GroupBox

static CWnd* CreateWnd_GroupBox()
{
    CWnd* pWnd = new CButton;
    return pWnd;
}

static CWndControl* CreateControl_GroupBox()
{
    CWndControl* pControl = new CWndGroupBox;
    return pControl;
}

//////////////////////////////////////////////////////////////////////////////
// IpAddress

static CWnd* CreateWnd_IpAddress()
{
    CWnd* pWnd = new CIPAddressCtrl;
    return pWnd;
}

static CWndControl* CreateControl_IpAddress()
{
    CWndControl* pControl = new CWndIpAddress;
    return pControl;
}

//////////////////////////////////////////////////////////////////////////////
// Label

static CWnd* CreateWnd_Label()
{
    CWnd* pWnd = new CStatic;
    return pWnd;
}

static CWndControl* CreateControl_Label()
{
    CWndControl* pControl = new CWndLabel;
    return pControl;
}

//////////////////////////////////////////////////////////////////////////////
// ListBox

static CWnd* CreateWnd_ListBox()
{
    CWnd* pWnd = new CListBox;
    return pWnd;
}

static CWndControl* CreateControl_ListBox()
{
    CWndControl* pControl = new CWndListBox;
    return pControl;
}

//////////////////////////////////////////////////////////////////////////////
// RadioButton

static CWnd* CreateWnd_RadioButton()
{
    CWnd* pWnd = new CButton;
    return pWnd;
}

static CWndControl* CreateControl_RadioButton()
{
    CWndControl* pControl = new CWndRadioButton;
    return pControl;
}

//////////////////////////////////////////////////////////////////////////////
// SpinButton

static CWnd* CreateWnd_SpinButton()
{
    CWnd* pWnd = new CSpinButtonCtrl;
    return pWnd;
}

static CWndControl* CreateControl_SpinButton()
{
    CWndControl* pControl = new CWndSpinButton;
    return pControl;
}

//////////////////////////////////////////////////////////////////////////////
// TextBox

static CWnd* CreateWnd_TextBox()
{
    CWnd* pWnd = new CEditEx;
    return pWnd;
}

static CWndControl* CreateControl_TextBox()
{
    CWndControl* pControl = new CWndTextBox;
    return pControl;
}

//////////////////////////////////////////////////////////////////////////////
// TrackBar

static CWnd* CreateWnd_TrackBar()
{
    CWnd* pWnd = new CSliderCtrl;
    return pWnd;
}

static CWndControl* CreateControl_TrackBar()
{
    CWndControl* pControl = new CWndTrackBar;
    return pControl;
}

//////////////////////////////////////////////////////////////////////////////
// WebBrowser

static CWnd* CreateWnd_WebBrowser()
{
    CWnd* pWnd = new CWebBrowser2;
    return pWnd;
}

static CWndControl* CreateControl_WebBrowser()
{
    CWndControl* pControl = new CWndWebBrowser;
    return pControl;
}

//////////////////////////////////////////////////////////////////////////////
// CWndFactory

CWndFactory::CWndFactory()
{
    // Register static Create functions.
    RegisterCreateFunctions(_T("Button"),      CreateWnd_Button,      CreateControl_Button);
    RegisterCreateFunctions(_T("CheckBox"),    CreateWnd_CheckBox,    CreateControl_CheckBox);
    RegisterCreateFunctions(_T("ComboBox"),    CreateWnd_ComboBox,    CreateControl_ComboBox);
    RegisterCreateFunctions(_T("GroupBox"),    CreateWnd_GroupBox,    CreateControl_GroupBox);
    RegisterCreateFunctions(_T("IpAddress"),   CreateWnd_IpAddress,   CreateControl_IpAddress);
    RegisterCreateFunctions(_T("Label"),       CreateWnd_Label,       CreateControl_Label);
    RegisterCreateFunctions(_T("ListBox"),     CreateWnd_ListBox,     CreateControl_ListBox);
    RegisterCreateFunctions(_T("RadioButton"), CreateWnd_RadioButton, CreateControl_RadioButton);
    RegisterCreateFunctions(_T("SpinButton"),  CreateWnd_SpinButton,  CreateControl_SpinButton);
    RegisterCreateFunctions(_T("TextBox"),     CreateWnd_TextBox,     CreateControl_TextBox);
    RegisterCreateFunctions(_T("TrackBar"),    CreateWnd_TrackBar,    CreateControl_TrackBar);
    RegisterCreateFunctions(_T("WebBrowser"),  CreateWnd_WebBrowser,  CreateControl_WebBrowser);
}

CWndFactory::~CWndFactory()
{
    m_createFunctions.clear();
}

bool CWndFactory::RegisterCreateFunctions(const CString& strType, CreateWndFn createWndFn, CreateControlFn createControlFn)
{
    ASSERT( !strType.IsEmpty() );
    ASSERT( createWndFn != NULL );
    ASSERT( createControlFn != NULL );
    
    // Check for existing entry in map.
    map<CString,CWndFactoryItem>::const_iterator result = m_createFunctions.find(strType);
    if ( result != m_createFunctions.end() )
        return false;
    
    // Insert new functions.
    CWndFactoryItem item;
    item.m_createWndFn = createWndFn;
    item.m_createControlFn = createControlFn;
    m_createFunctions.insert(make_pair(strType,item));
    
    return true;
}

CWnd* CWndFactory::CreateWnd(const CString& strType)
{
    CWnd* pWnd = NULL;
    
    map<CString,CWndFactoryItem>::iterator result = m_createFunctions.find(strType);
    if ( result != m_createFunctions.end() )
    {
        CreateWndFn fn = result->second.m_createWndFn;
        ASSERT( fn != NULL );
        
        pWnd = (*fn)();
    }
    
    return pWnd;
}

CWndControl* CWndFactory::CreateControl(const CString& strType)
{
    CWndControl* pControl = NULL;
    
    map<CString,CWndFactoryItem>::iterator result = m_createFunctions.find(strType);
    if ( result != m_createFunctions.end() )
    {
        CreateControlFn fn = result->second.m_createControlFn;
        ASSERT( fn != NULL );
        
        pControl = (*fn)();
    }
    
    return pControl;
}

// END

