// Filename: WndFactory.h
// 04-Mar-2006 nschan Initial revision.

#ifndef WND_FACTORY_INCLUDED
#define WND_FACTORY_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>

// Forward class declarations.
class CWndControl;

// Function pointer type for creating a new CWnd instance (C++ instance only).
typedef CWnd* (*CreateWndFn)();

// Function pointer type for creating a new CWndControl instance. 
typedef CWndControl* (*CreateControlFn)();

// CWndFactoryItem is a simple data class to store the two function pointers.
class CWndFactoryItem
{
public:
    CWndFactoryItem()
    {
        m_createWndFn = NULL;
        m_createControlFn = NULL;
    }
    
    CreateWndFn     m_createWndFn;
    CreateControlFn m_createControlFn;
};

// CWndFactory can be used to create CWnd or CWndControl instances given a
// type string. If you want to support new types of CWnds or CWndControls,
// you need to call the RegisterCreateFunction() method either externally or
// from within the CWndFactory constructor.
class CWndFactory
{
public:
    // Constructor / destructor.
    CWndFactory();
    ~CWndFactory();
    
    // Register new create functions.
    bool RegisterCreateFunctions(const CString& strType, CreateWndFn createWndFn, CreateControlFn createControlFn);
    
    // Top-level factory methods.
    CWnd*        CreateWnd(const CString& strType);
    CWndControl* CreateControl(const CString& strType);
    
private:    
    // Store the registered function pointers.
    std::map<CString,CWndFactoryItem> m_createFunctions;
};

#endif // WND_FACTORY_INCLUDED

// END
