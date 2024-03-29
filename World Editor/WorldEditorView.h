// Supported with union (c) 2020 Union team
// Union HEADER file

using namespace GOTHIC_ENGINE;

	class CEditorView : public CView
	{
	protected:
		CEditorView() ;
		DECLARE_DYNCREATE(CEditorView)
	public:
		POINT				cur;
		BOOL				isStartScreen;
		zCVob*				pickedWP;
		zCVob*				pickedWP2nd;
		zCWorld*			world;
		CHomeSettings		homeSettings;
		CSettings			settings;
		static CEditorView* view;
	public:
		CEditorDoc*		GetDocument() const;
		void			RemoveObject(zCVob* vob);
		zCObject*		CreateNewObject(CString className);
		zCVob*			CreateNewVob(zCObject* obj);
		void			SelectObject(zCVob* vob);
	public:
		virtual void	OnDraw(CDC* pDC);
		virtual BOOL	PreCreateWindow(CREATESTRUCT& cs);
		void			OnNewpreset();
		void			OnDrawStartScreen();
		void			VobGoto(zCVob* vob);
		afx_msg void	OnLButtonDown(UINT nFlags, CPoint point);
		afx_msg void	OnLButtonDblClk(UINT nFlags, CPoint point);
		afx_msg void	OnObjInsert();
	public:
		virtual			~CEditorView();
	protected:
		afx_msg void	OnRButtonUp(UINT nFlags, CPoint point);
		afx_msg void	OnContextMenu(CWnd* pWnd, CPoint point);
		
		DECLARE_MESSAGE_MAP()
	public:
		virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
		afx_msg void OnUpdateObjInsert(CCmdUI* pCmdUI);
	};

#ifndef _DEBUG 
	inline CEditorDoc* CEditorView::GetDocument() const
	{
		return reinterpret_cast<CEditorDoc*>(m_pDocument);
	}
#endif

