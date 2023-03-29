// Supported with union (c) 2020 Union team
// Union HEADER file

using namespace GOTHIC_ENGINE;
	//-------------------------------------------------------------------
	//
	//	CColorListBox class - 
	//		A CListBox-derived class with optional colored items.
	//
	//		Version: 1.0	01/10/1998 Copyright © Patrice Godard
	//
	//		Version: 2.0	09/17/1999 Copyright © Paul M. Meidinger
	//
	//-------------------------------------------------------------------

	/////////////////////////////////////////////////////////////////////////////
	// CColorListBox window

	class CColorListBox : public CListBox
	{
		// Construction
	public:
		CColorListBox();

		// Attributes
	public:

		// Operations
	public:
		int AddString(LPCTSTR lpszItem);											// Adds a string to the list box
		int AddString(zSTRING lpszItem, COLORREF rgb);						// Adds a colored string to the list box
		int InsertString(int nIndex, LPCTSTR lpszItem);						// Inserts a string to the list box
		int InsertString(int nIndex, LPCTSTR lpszItem, COLORREF rgb);	// Inserts a colored string to the list box
		void SetItemColor(int nIndex, COLORREF rgb);							// Sets the color of an item in the list box
	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CColorListBox)
	public:
		virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
		virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
		//}}AFX_VIRTUAL
		virtual BOOL PreTranslateMessage(MSG* pMsg);
		// Implementation
	public:
		virtual ~CColorListBox();

		// Generated message map functions
	protected:
		//{{AFX_MSG(CColorListBox)
		//}}AFX_MSG

		DECLARE_MESSAGE_MAP()
		afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
		void OnEditCopy();
		void OnEditClear();
	};

	/////////////////////////////////////////////////////////////////////////////

