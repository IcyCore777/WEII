// Supported with union (c) 2020 Union team
// Union HEADER file

using namespace GOTHIC_ENGINE;

	class CMainFrame : public CFrameWndEx
	{

	protected:
		CMainFrame() ;
		DECLARE_DYNCREATE(CMainFrame)
	public:
		virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	public:
		virtual ~CMainFrame();
#ifdef _DEBUG
		virtual void AssertValid() const;
		virtual void Dump(CDumpContext& dc) const;
#endif
		static CMainFrame*			mainframe;


		zCObjPresetLib				presetLib;
		CMeshLibPane				m_wndMeshLib;
		CPresetPane					m_wndPresetLib;
		CVobList					m_wndVobList;
		CClassView					m_wndClassView;
		COutputWnd					m_wndOutput;
		CPropertiesWnd				m_wndProperties;

		CMFCRibbonBar				m_wndRibbonBar;
		CMFCRibbonApplicationButton m_MainButton;
		CMFCToolBarImages			m_PanelImages;
		CMFCRibbonStatusBar			m_wndStatusBar;

		CMFCRibbonProgressBar*		m_progressBar;
	public:
		void ShowProgressBar(CString text, BOOL bShow);
		void ApplyVideoSettings();
		void ResizeToView(CView* view, int x, int y);
		BOOL CreateDockingWindows();
		void SetDockingWindowIcons(BOOL bHiColorIcons);
		void CopyToClipBoard(CString str);
	protected:
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg void OnApplicationLook(UINT id);
		afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);

		afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
		DECLARE_MESSAGE_MAP()
	public:
		afx_msg void OnDestroy();
	};
