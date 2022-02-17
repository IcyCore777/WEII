// Supported with union (c) 2020 Union team
// Union HEADER file

namespace GOTHIC_ENGINE {
	class CMeshLibToolBar : public CMFCToolBar
	{
		virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
		{
			CMFCToolBar::OnUpdateCmdUI((CFrameWnd*)GetOwner(), bDisableIfNoHndler);
		}

		virtual BOOL AllowShowOnList() const { return FALSE; }
	};

	class CMeshLibPane : public CDockablePane
	{
	public:
		CMeshLibPane();
		virtual ~CMeshLibPane();

		void AdjustLayout();
		void OnChangeVisualStyle();
		CMeshLibToolBar	m_wndToolBar;
		CComboBox category;
		CMeshListBox meshListBox;

		CString filter;

		ELibMode mode;

		// Переопределение
	public:
		virtual BOOL PreTranslateMessage(MSG* pMsg);

	protected:
		DECLARE_MESSAGE_MAP()
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg void OnSize(UINT nType, int cx, int cy);
		afx_msg void OnPaint();
		afx_msg void OnSetFocus(CWnd* pOldWnd);
	public:
		afx_msg void OnSelChangeCat();
		afx_msg void On3ds();
		afx_msg void OnUpdate3ds(CCmdUI* pCmdUI);
		afx_msg void OnAsc();
		afx_msg void OnUpdateAsc(CCmdUI* pCmdUI);
		afx_msg void OnPfx();
		afx_msg void OnUpdatePfx(CCmdUI* pCmdUI);
		afx_msg void OnMds();
		afx_msg void OnUpdateMds(CCmdUI* pCmdUI);
		afx_msg void OnItems();
		afx_msg void OnUpdateItems(CCmdUI* pCmdUI);
		afx_msg void OnMms();
		afx_msg void OnUpdateMms(CCmdUI* pCmdUI);
		afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	};


	class CPresetPane : public CDockablePane
	{
	public:
		CMeshListBox	meshListBox;
		CMeshLibToolBar m_wndToolBar;
		CImageList		m_PresetViewImages;
		UINT			m_nCurrSort;
	public:
		CPresetPane();
		virtual			~CPresetPane();
		void			AdjustLayout();
		void			OnChangeVisualStyle();
		virtual BOOL	PreTranslateMessage(MSG* pMsg);
		void			LoadPreset(CString filter);
		afx_msg int		OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg void	OnSize(UINT nType, int cx, int cy);
		afx_msg void	OnPaint();
		afx_msg void	OnSetFocus(CWnd* pOldWnd);
		afx_msg void	OnAddpreset();
		afx_msg void	OnRemovepreset();
		afx_msg void	OnUpdateAddpreset(CCmdUI* pCmdUI);
		afx_msg void	OnUpdateRemovepreset(CCmdUI* pCmdUI);
		DECLARE_MESSAGE_MAP()
	};

#define PRESETLIB CMainFrame::mainframe->m_wndPresetLib
#define MESHLIBDLG   CMainFrame::mainframe->m_wndMeshLib
}