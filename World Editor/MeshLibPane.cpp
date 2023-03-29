// Supported with union (c) 2020 Union team
// Union SOURCE file

using namespace GOTHIC_ENGINE;
#define IDC_SELCHG_CB 32212

	BEGIN_MESSAGE_MAP(CMeshLibPane, CDockablePane)
		ON_WM_CREATE()
		ON_WM_SIZE()
		ON_WM_PAINT()
		ON_WM_SETFOCUS()
		ON_CBN_SELCHANGE(IDC_SELCHG_CB, &CMeshLibPane::OnSelChangeCat)
		ON_COMMAND(ID_3DS, &CMeshLibPane::On3ds)
		ON_UPDATE_COMMAND_UI(ID_3DS, &CMeshLibPane::OnUpdate3ds)
		ON_COMMAND(ID_ASC, &CMeshLibPane::OnAsc)
		ON_UPDATE_COMMAND_UI(ID_ASC, &CMeshLibPane::OnUpdateAsc)
		ON_COMMAND(ID_PFX, &CMeshLibPane::OnPfx)
		ON_UPDATE_COMMAND_UI(ID_PFX, &CMeshLibPane::OnUpdatePfx)
		ON_COMMAND(ID_MDS, &CMeshLibPane::OnMds)
		ON_UPDATE_COMMAND_UI(ID_MDS, &CMeshLibPane::OnUpdateMds)
		ON_COMMAND(ID_ITEMS, &CMeshLibPane::OnItems)
		ON_UPDATE_COMMAND_UI(ID_ITEMS, &CMeshLibPane::OnUpdateItems)
		ON_COMMAND(ID_MMS, &CMeshLibPane::OnMms)
		ON_UPDATE_COMMAND_UI(ID_MMS, &CMeshLibPane::OnUpdateMms)
		ON_WM_ACTIVATE()
	END_MESSAGE_MAP()

	CMeshLibPane::CMeshLibPane()
	{
		
	}

	CMeshLibPane::~CMeshLibPane()
	{
		
	}


	int CMeshLibPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		
		if (CDockablePane::OnCreate(lpCreateStruct) == -1)
			return -1;

		CRect rectDummy;
		rectDummy.SetRectEmpty();

		m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_MESHCAT);
		m_wndToolBar.LoadToolBar(IDR_MESHCAT, 0, 0, TRUE /* Заблокирован */);
		m_wndToolBar.CleanUpLockedImages();
		m_wndToolBar.LoadBitmap(/*theApp.m_bHiColorIcons ? IDB_PROPERTIES_HC : */IDR_MESHCAT, 0, 0, TRUE /* Заблокирован */);

		m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
		m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
		m_wndToolBar.SetOwner(this);

		// Все команды будут перенаправлены через этот элемент управления, а не через родительскую рамку:
		m_wndToolBar.SetRouteCommandsViaFrame(FALSE);
		// Создать поле со списком:
		const DWORD dwViewStyle1 = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER | CBS_SORT | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VSCROLL;

		if (!category.Create(dwViewStyle1, rectDummy, this, IDC_SELCHG_CB))
		{
			TRACE0("Не удалось создать поле со списком \"Свойства\" \n");
			return -1;
		}

		// Создание представлений:
		const DWORD dwViewStyle = WS_CHILDWINDOW | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | LBS_OWNERDRAWVARIABLE | LBS_HASSTRINGS | LBS_NOTIFY;

		if (!meshListBox.Create(dwViewStyle, rectDummy, this, 6))
		{
			TRACE0("Не удалось создать представление классов\n");
			return -1;
		}

		OnChangeVisualStyle();
		return 0;
	}

	void CMeshLibPane::OnSize(UINT nType, int cx, int cy)
	{
		
		CDockablePane::OnSize(nType, cx, cy);
		if (IsWindow(meshListBox.GetSafeHwnd()))
			meshListBox.SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
		AdjustLayout();
	}

	void CMeshLibPane::AdjustLayout()
	{
		
		if (GetSafeHwnd() == NULL)
		{
			return;
		}

		CRect rectClient, rectCombo;
		GetClientRect(rectClient);

		category.GetWindowRect(&rectCombo);

		int cyCmb = rectCombo.Size().cy;
		int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

		m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
		category.SetWindowPos(NULL, rectClient.left, rectClient.top + cyTlb, rectClient.Width(), 200, SWP_NOACTIVATE | SWP_NOZORDER);
		meshListBox.SetWindowPos(NULL, rectClient.left, rectClient.top + cyCmb + cyTlb, rectClient.Width(), rectClient.Height() - (cyCmb + cyTlb), SWP_NOACTIVATE | SWP_NOZORDER);
	}

	BOOL CMeshLibPane::PreTranslateMessage(MSG* pMsg)
	{
		
		return CDockablePane::PreTranslateMessage(pMsg);
	}

	void CMeshLibPane::OnPaint()
	{
		
		CPaintDC dc(this); // контекст устройства для рисования

		CRect rectTree;
		meshListBox.GetWindowRect(rectTree);
		ScreenToClient(rectTree);

		rectTree.InflateRect(1, 1);
		dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
	}

	void CMeshLibPane::OnSetFocus(CWnd* pOldWnd)
	{
		
		CDockablePane::OnSetFocus(pOldWnd);
		CMainFrame::mainframe->m_wndPresetLib.meshListBox.mode = MESHLIB;
		meshListBox.SetFocus();
	}

	void CMeshLibPane::OnChangeVisualStyle()
	{
		

	}

	void CMeshLibPane::OnSelChangeCat()
	{
		
		int index = category.GetCurSel();
		if (index != CB_ERR) category.GetLBText(index, filter);

		meshListBox.ResetContent();
		meshListBox.CreateList(filter, mode);
		UpdateData(FALSE);
		RedrawWindow();
	}


	void CMeshLibPane::On3ds()
	{
		
		mode = ELibMode::MSH;
		category.ResetContent();
		for (size_t i = 0; i < Groups3ds.GetNum(); i++)
		{
			CMainFrame::mainframe->m_wndMeshLib.category.AddString(Groups3ds[i]->Name);
			CMainFrame::mainframe->m_wndMeshLib.category.SelectString(0, "TRASH");
		}
		OnSelChangeCat();
	}


	void CMeshLibPane::OnUpdate3ds(CCmdUI* pCmdUI)
	{
		
		pCmdUI->SetCheck(mode == ELibMode::MSH);
	}


	void CMeshLibPane::OnAsc()
	{
		
		mode = ELibMode::ASC;
		category.ResetContent();
		for (size_t i = 0; i < GroupsASC.GetNum(); i++)
		{
			CMainFrame::mainframe->m_wndMeshLib.category.AddString(GroupsASC[i]->Name);
			CMainFrame::mainframe->m_wndMeshLib.category.SelectString(0, "TRASH");
		}
		OnSelChangeCat();
	}


	void CMeshLibPane::OnUpdateAsc(CCmdUI* pCmdUI)
	{
		
		pCmdUI->SetCheck(mode == ELibMode::ASC);
	}


	void CMeshLibPane::OnPfx()
	{
		
		// TODO: добавьте свой код обработчика команд
	}


	void CMeshLibPane::OnUpdatePfx(CCmdUI* pCmdUI)
	{
		
		pCmdUI->Enable(FALSE);
	}


	void CMeshLibPane::OnMds()
	{
		
		mode = ELibMode::MDS;
		category.ResetContent();
		for (size_t i = 0; i < GroupsMDS.GetNum(); i++)
		{
			CMainFrame::mainframe->m_wndMeshLib.category.AddString(GroupsMDS[i]->Name);
			CMainFrame::mainframe->m_wndMeshLib.category.SelectString(0, "TRASH");
		}
		OnSelChangeCat();
	}


	void CMeshLibPane::OnUpdateMds(CCmdUI* pCmdUI)
	{
		
		pCmdUI->SetCheck(mode == ELibMode::MDS);
	}


	void CMeshLibPane::OnItems()
	{
		
		mode = ELibMode::ITEM;
		category.ResetContent();
		for (size_t i = 0; i < GroupsITEMS.GetNum(); i++)
		{
			CMainFrame::mainframe->m_wndMeshLib.category.AddString(GroupsITEMS[i]->Name);
			CMainFrame::mainframe->m_wndMeshLib.category.SelectString(0, "TRASH");
		}
		OnSelChangeCat();
	}


	void CMeshLibPane::OnUpdateItems(CCmdUI* pCmdUI)
	{
		
		pCmdUI->SetCheck(mode == ELibMode::ITEM);
	}


	void CMeshLibPane::OnMms()
	{
		
		mode = ELibMode::MMS;
		category.ResetContent();
		for (size_t i = 0; i < GroupsMMS.GetNum(); i++)
		{
			CMainFrame::mainframe->m_wndMeshLib.category.AddString(GroupsMMS[i]->Name);
			CMainFrame::mainframe->m_wndMeshLib.category.SelectString(0, "TRASH");
		}
		OnSelChangeCat();
	}


	void CMeshLibPane::OnUpdateMms(CCmdUI* pCmdUI)
	{
		
		pCmdUI->SetCheck(mode == ELibMode::MMS);
	}


	void CMeshLibPane::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
	{
		
		CDockablePane::OnActivate(nState, pWndOther, bMinimized);
	}

