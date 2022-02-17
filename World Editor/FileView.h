// Supported with union (c) 2020 Union team
// Union HEADER file

namespace GOTHIC_ENGINE {
	class CFileViewToolBar : public CMFCToolBar
	{
		virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
		{
			CMFCToolBar::OnUpdateCmdUI((CFrameWnd*)GetOwner(), bDisableIfNoHndler);
		}

		virtual BOOL AllowShowOnList() const { return FALSE; }
	};

	class CVobList : public CDockablePane
	{
	public:
		CVobList() ;

		void AdjustLayout();
		void OnChangeVisualStyle();

	

		CViewTree			m_wndVobList;
		CImageList			m_FileViewImages;
		CFileViewToolBar	m_wndToolBar;

	protected:
		void UpdateVobList();
		void			CreateVobTree(zCTree<zCVob>* node, HTREEITEM parent);

	public:
		virtual ~CVobList();
		void			BuildVobTree();
	protected:
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg void OnSize(UINT nType, int cx, int cy);
		afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
		afx_msg void OnProperties();
		afx_msg void OnFileOpen();
		afx_msg void OnFileOpenWith();
		afx_msg void OnDummyCompile();
		afx_msg void OnEditCut();
		afx_msg void OnEditCopy();
		afx_msg void OnEditClear();
		afx_msg void OnPaint();
		afx_msg void OnSetFocus(CWnd* pOldWnd);

		DECLARE_MESSAGE_MAP()
	};
}