// Supported with union (c) 2020 Union team
// Union HEADER file

using namespace GOTHIC_ENGINE;
	class CPropertiesToolBar : public CMFCToolBar
	{
	public:
		virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
		{
			CMFCToolBar::OnUpdateCmdUI((CFrameWnd*)GetOwner(), bDisableIfNoHndler);
		}

		virtual BOOL AllowShowOnList() const { return FALSE; }
	};

	class CPropertiesWnd : public CDockablePane
	{
	public:
		CPropertiesWnd() ;

		void AdjustLayout();

	public:
		void SetVSDotNetLook(BOOL bSet)
		{
			m_wndPropList.SetVSDotNetLook(bSet);
			m_wndPropList.SetGroupNameFullWidth(bSet);
		}

	public:
		CFont					m_fntPropList;
		CComboBox				m_wndObjectCombo;
		CPropertiesToolBar		m_wndToolBar;
		CMFCPropertyGridCtrl	m_wndPropList;


		CPropList				propList;
		zCVob*					vob;
		zCArchiver*				arch;
		zSTRING					objTypeDescriptor;
		int						startPos;

		void GetProperties(zSTRING& propString);
		void SetProperties(zSTRING& propString);
		void SetObject(zCVob*);
		short GetBool(COleVariant v);
		char* GetString(COleVariant v);

	public:
		virtual ~CPropertiesWnd();

	protected:
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg void OnSize(UINT nType, int cx, int cy);
		afx_msg void OnExpandAllProperties();
		afx_msg void OnUpdateExpandAllProperties(CCmdUI* pCmdUI);
		afx_msg void OnSortProperties();
		afx_msg void OnUpdateSortProperties(CCmdUI* pCmdUI);
		afx_msg void OnProperties1();
		afx_msg void OnUpdateProperties1(CCmdUI* pCmdUI);
		afx_msg void OnProperties2();
		afx_msg void OnUpdateProperties2(CCmdUI* pCmdUI);
		afx_msg void OnSetFocus(CWnd* pOldWnd);
		afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
		afx_msg LRESULT OnPropertyChanged(__in WPARAM wparam, __in LPARAM lparam);
		DECLARE_MESSAGE_MAP()

		void InitPropList();
		void SetPropListFont();

		int m_nComboHeight;
	};

