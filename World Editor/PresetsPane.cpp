// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
#define IDC_SELCHG_CB 32213


	BEGIN_MESSAGE_MAP(CPresetPane, CDockablePane)
		ON_WM_CREATE()
		ON_WM_SIZE()
		ON_WM_PAINT()
		ON_WM_SETFOCUS()
		ON_COMMAND(ID_ADDPRESET, &CPresetPane::OnAddpreset)
		ON_COMMAND(ID_REMOVEPRESET, &CPresetPane::OnRemovepreset)
		ON_UPDATE_COMMAND_UI(ID_ADDPRESET, &CPresetPane::OnUpdateAddpreset)
		ON_UPDATE_COMMAND_UI(ID_REMOVEPRESET, &CPresetPane::OnUpdateRemovepreset)
	END_MESSAGE_MAP()

	CPresetPane::CPresetPane()
	{
		
		m_nCurrSort = ID_SORTING_GROUPBYTYPE;
	}


	CPresetPane::~CPresetPane()
	{
		
	}


	int CPresetPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		
		if (CDockablePane::OnCreate(lpCreateStruct) == -1)
			return -1;

		CRect rectDummy;
		rectDummy.SetRectEmpty();

		const DWORD dwViewStyle = WS_CHILDWINDOW | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | LBS_OWNERDRAWVARIABLE | LBS_HASSTRINGS | LBS_NOTIFY;

		if (!meshListBox.Create(dwViewStyle, rectDummy, this, 6))
		{
			TRACE0("Не удалось создать представление классов\n");
			return -1;
		}


		m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDB_PRESETS);
		m_wndToolBar.LoadToolBar(IDR_PRESETS, 0, 0, TRUE);
		m_wndToolBar.CleanUpLockedImages();
		m_wndToolBar.LoadBitmap(IDB_PRESETS, 0, 0, TRUE);

		m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
		m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
		m_wndToolBar.SetOwner(this);
		m_wndToolBar.SetRouteCommandsViaFrame(FALSE);
		OnChangeVisualStyle();
		return 0;
	}


	void CPresetPane::OnSize(UINT nType, int cx, int cy)
	{
		
		CDockablePane::OnSize(nType, cx, cy);
		if (IsWindow(meshListBox.GetSafeHwnd()))
			meshListBox.SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
		AdjustLayout();
	}


	void CPresetPane::AdjustLayout()
	{
		
		if (GetSafeHwnd() == NULL)
		{
			return;
		}
		CRect rectClient, rectCombo;
		GetClientRect(rectClient);
		int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

		m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
		meshListBox.SetWindowPos(NULL, rectClient.left, rectClient.top + cyTlb, rectClient.Width(), rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
	}


	BOOL CPresetPane::PreTranslateMessage(MSG* pMsg)
	{
		
		return CDockablePane::PreTranslateMessage(pMsg);
	}


	void CPresetPane::LoadPreset(CString filter)
	{
		
		meshListBox.ResetContent();
		meshListBox.LoadPreset(filter.GetBuffer(0));
		UpdateData(FALSE);
		RedrawWindow();
	}


	void CPresetPane::OnPaint()
	{
		
		CPaintDC dc(this); // контекст устройства для рисования

		CRect rectTree;
		meshListBox.GetWindowRect(rectTree);
		ScreenToClient(rectTree);

		rectTree.InflateRect(1, 1);
		dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
	}


	void CPresetPane::OnSetFocus(CWnd* pOldWnd)
	{
		
		CDockablePane::OnSetFocus(pOldWnd);
		meshListBox.mode = PRESETS;
		meshListBox.SetFocus();
	}


	void CPresetPane::OnChangeVisualStyle()
	{
		


	}


	void CPresetPane::OnAddpreset()
	{
		
		CEditorView::view->OnNewpreset(); 
	}


	void CPresetPane::OnRemovepreset()
	{
		
		zSTRING presetName = CMainFrame::mainframe->m_wndPresetLib.meshListBox.GetSelectedMesh().ToChar();
		int result = AfxMessageBox(CString("Really delete preset ") + presetName.ToChar() + "?", MB_YESNO);
		if (result == IDYES)
		{
			CMainFrame::mainframe->m_wndPresetLib.meshListBox.presets->RemovePresetByName(presetName);
			CMainFrame::mainframe->m_wndPresetLib.LoadPreset(CMainFrame::mainframe->m_wndPresetLib.meshListBox.presets->GetViewName().ToChar());
		};
	}


	void CPresetPane::OnUpdateAddpreset(CCmdUI* pCmdUI)
	{

		if(!CEditorView::view->world)
			pCmdUI->Enable(FALSE);
		else
			pCmdUI->Enable((BOOL)GetSelectedVob());

	}


	void CPresetPane::OnUpdateRemovepreset(CCmdUI* pCmdUI)
	{

		if (!CEditorView::view->world)
			pCmdUI->Enable(FALSE);
		else
			pCmdUI->Enable(CMainFrame::mainframe->m_wndPresetLib.meshListBox.GetSelectedMesh() != "BOX2.3DS");
	}
}