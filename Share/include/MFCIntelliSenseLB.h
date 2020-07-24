#if !defined(AFX_MFCINTELLISENSELB_H__46A739E6_3403_4577_9EBE_E2E59A7D1A1C__INCLUDED_)
#define AFX_MFCINTELLISENSELB_H__46A739E6_3403_4577_9EBE_E2E59A7D1A1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//
// MFCIntelliSenseLB.h : header file
//

#ifndef MFC_EXCLUDE_EDIT_CTRL

class CMFCEditCtrl;

class  CMFCBaseIntelliSenseLB : public CListBox
{
	DECLARE_DYNCREATE (CMFCBaseIntelliSenseLB)

// Construction
public:
	CMFCBaseIntelliSenseLB();

// Attributes
public:
	virtual CSize GetMaxItemSize () const {return m_sizeMaxItem;}
	CFont*		m_pFont;

	static int m_nNumVisibleItems;

	CSize		m_sizeMaxItem;
	int			m_nLBHeight;

	static int	m_nImageToFocusRectSpacing;
	static int	m_nFocusRectToTextSpacing;
	static int	m_nRightSpacing;

	static COLORREF m_clrSelectedItemBkColor;
	static COLORREF m_clrSelectedItemTextColor;
	static COLORREF m_clrWindow;
	static COLORREF m_clrWindowText;

// Operations
public:
	void SetImageList (CImageList* pImageList) {m_pImageList = pImageList;}
	virtual CSize GetImageSize (int iIdx); 
	virtual CSize GetTextSize (CString& str); 

	virtual void SelectCurrentWord () {}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMFCBaseIntelliSenseLB)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual int CompareItem(LPCOMPAREITEMSTRUCT /*lpCompareItemStruct*/) {return -1;}
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DeleteItem(LPDELETEITEMSTRUCT lpDeleteItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMFCBaseIntelliSenseLB();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMFCBaseIntelliSenseLB)
	afx_msg void OnDblclk();
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnKillfocus();
	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnNcDestroy();
	afx_msg void OnSysColorChange();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

	CImageList* m_pImageList;

protected:
	CMFCEditCtrl* GetParentEditCtrl ();
	virtual void InsertDataToEditCtrl () {}
	void UpdateColors ();
};

/////////////////////////////////////////////////////////////////////////////
// CMFCIntelliSenseLB window

class  CMFCIntelliSenseLB : public CMFCBaseIntelliSenseLB
{
	DECLARE_DYNCREATE (CMFCIntelliSenseLB)
// Construction
public:
	CMFCIntelliSenseLB();

// Attributes
public:

	static BOOL m_bComparenoCase;

// Operations
public:
	virtual void SelectCurrentWord ();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMFCIntelliSenseLB)
	public:
	virtual int CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMFCIntelliSenseLB();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMFCIntelliSenseLB)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

	int			m_nInitOffset;
	int			m_nInitRow;

protected:
	virtual void InsertDataToEditCtrl ();
};

class  CMFCSymImagesLB : public CMFCBaseIntelliSenseLB
{
	DECLARE_DYNCREATE (CMFCSymImagesLB)
// Construction
public:
	CMFCSymImagesLB()
	{
	}

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMFCIntelliSenseLB)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMFCSymImagesLB()
	{
	}

	// Generated message map functions
protected:
	//{{AFX_MSG(CMFCSymImagesLB)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	virtual void InsertDataToEditCtrl ();

};

#endif	// MFC_EXCLUDE_EDIT_CTRL

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFCINTELLISENSELB_H__46A739E6_3403_4577_9EBE_E2E59A7D1A1C__INCLUDED_)
