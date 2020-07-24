#if !defined(AFX_MFCINTELLISENSEWND_H__2D884D5A_B4C1_4EE0_A5D6_3D8703CA99D1__INCLUDED_)
#define AFX_MFCINTELLISENSEWND_H__2D884D5A_B4C1_4EE0_A5D6_3D8703CA99D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//*******************************************************************************
// COPYRIGHT NOTES
// ---------------
// This is a part of MFCControlBar Library Professional Edition
// Copyright (C) 1998-2007 MFCSoft Ltd.
// All rights reserved.
//
// This source code can be used, distributed or modified
// only under terms and conditions 
// of the accompanying license agreement.
//*******************************************************************************
//
// MFCIntelliSenseWnd.h : header file
//

#ifndef MFC_EXCLUDE_EDIT_CTRL

#include "MFCIntelliSenseLB.h"

class CMFCEditCtrl;

/////////////////////////////////////////////////////////////////////////////
// CMFCIntelliSenseWnd frame

class  CMFCIntelliSenseWnd : public CMiniFrameWnd
{
	DECLARE_DYNCREATE(CMFCIntelliSenseWnd)
public:
	CMFCIntelliSenseWnd();           // protected constructor used by dynamic creation

	virtual BOOL Create (CObList& lstData, DWORD dwStyle, CPoint pt, 
						 CWnd* pParentWnd, CFont* pLBFont, CImageList* pImageList = NULL, 
						 CRuntimeClass* pLBDataRTC = NULL);
// Attributes
public:
	CMFCEditCtrl* GetParentEditCtrl () {return m_pParentEditCtrl;}
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMFCIntelliSenseWnd)
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMFCIntelliSenseWnd();

	// Generated message map functions
	//{{AFX_MSG(CMFCIntelliSenseWnd)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg LRESULT DeleteItem (WPARAM, LPARAM);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnSelchangeList(); 
	DECLARE_MESSAGE_MAP()

	CMFCBaseIntelliSenseLB*	m_pLstBoxData;
	CFont						m_lbFont;
	CMFCEditCtrl*				m_pParentEditCtrl;
};

#endif	// MFC_EXCLUDE_EDIT_CTRL

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFCINTELLISENSEWND_H__2D884D5A_B4C1_4EE0_A5D6_3D8703CA99D1__INCLUDED_)
