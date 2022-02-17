// Supported with union (c) 2020 Union team
// Union HEADER file

namespace GOTHIC_ENGINE {

	class COutputList : public CListBox
	{
	public:
		COutputList() ;
		int Insert(zSTRING);
	public:
		virtual ~COutputList();

	protected:
		afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
		afx_msg void OnEditCopy();
		afx_msg void OnEditClear();
		afx_msg void OnViewOutput();
		afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
		virtual BOOL PreTranslateMessage(MSG* pMsg);
		DECLARE_MESSAGE_MAP()
	};

	class COutputWnd : public CDockablePane
	{

	public:
		COutputWnd() ;

		void UpdateFonts();

	public:
		CMFCTabCtrl		m_wndTabs;

		COutputList		m_wndOutputBuild;
		COutputList		m_wndOutputFind;
		CColorListBox	m_wndOutputDebug;
		CColorListBox	m_wndOutputSpy;

	protected:
		void AdjustHorzScroll(CListBox& wndListBox);

	public:
		virtual ~COutputWnd();

	protected:
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg void OnSize(UINT nType, int cx, int cy);

		DECLARE_MESSAGE_MAP()
	};



#define OutputBuild(str)		CMainFrame::mainframe->m_wndOutput.m_wndOutputBuild.SetTopIndex(CMainFrame::mainframe->m_wndOutput.m_wndOutputBuild.Insert(str))
#define ClearOutput()			CMainFrame::mainframe->m_wndOutput.m_wndOutputBuild.ResetContent();
#define ClearDebug()			CMainFrame::mainframe->m_wndOutput.m_wndOutputDebug.ResetContent();
#define ClearFind()				CMainFrame::mainframe->m_wndOutput.m_wndOutputFind.ResetContent();
#define OutputReport(str)		CMainFrame::mainframe->m_wndOutput.m_wndOutputSpy.SetTopIndex(CMainFrame::mainframe->m_wndOutput.m_wndOutputSpy.AddString(str,RGB(0,0,0)))
#define OutputDebug(str)		CMainFrame::mainframe->m_wndOutput.m_wndOutputDebug.SetTopIndex(CMainFrame::mainframe->m_wndOutput.m_wndOutputDebug.AddString(str,RGB(0,255,0)))
#define OutputError(str)		CMainFrame::mainframe->m_wndOutput.m_wndOutputDebug.SetTopIndex(CMainFrame::mainframe->m_wndOutput.m_wndOutputDebug.AddString(str,RGB(255,0,0)))
#define OutputWarning(str)		CMainFrame::mainframe->m_wndOutput.m_wndOutputDebug.SetTopIndex(CMainFrame::mainframe->m_wndOutput.m_wndOutputDebug.AddString(str,RGB(255,180,0)))
}