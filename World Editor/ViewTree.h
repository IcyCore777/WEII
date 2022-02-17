// Supported with union (c) 2020 Union team
// Union HEADER file

namespace GOTHIC_ENGINE {
#define ClassTree				CMainFrame::mainframe->m_wndClassView.m_wndClassView
	class CViewTree : public CTreeCtrl
	{
	public:
		CViewTree() ;
		virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
		virtual ~CViewTree();
	protected:
		DECLARE_MESSAGE_MAP()
	};


	class CClassTree : public CTreeCtrl
	{
	public:
		CClassTree();
		virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
		virtual ~CClassTree();
		DECLARE_MESSAGE_MAP()
	public:
		afx_msg void OnNMDblclk(NMHDR* pNMHDR, LRESULT* pResult);
		afx_msg void OnTvnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	};

}