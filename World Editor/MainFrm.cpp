
// Supported with union (c) 2020 Union team
// Union SOURCE file

using namespace GOTHIC_ENGINE;
	// CMainFrame
	CMainFrame* CMainFrame::mainframe = Null;

	int viewy;
	int viewx;

	IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

	BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
		ON_WM_CREATE()
		ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
		ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
		ON_WM_SETTINGCHANGE()
		ON_WM_DESTROY()
	END_MESSAGE_MAP()


	CMainFrame::CMainFrame() : presetLib()
	{
		
		m_progressBar = NULL;
		mainframe = this;
		theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2008);

	}

	CMainFrame::~CMainFrame()
	{
	}
	
//#define EXPORT_RIBBON
#ifdef EXPORT_RIBBON
	typedef void(WINAPI* LOADRIBBON)();


	void LoadRibbon()
	{
		static HMODULE Controler = Null;
		static LOADRIBBON LoadRibbonProc = Null;
		for (int i = 0; i < UnionCore::CPlugin::GetPluginList().GetNumInList(); i++)
		{
			Controler = UnionCore::CPlugin::GetPluginList()[i].GetData()->GetModule();
			LoadRibbonProc = (LOADRIBBON)GetProcAddress(Controler, "?LoadRibbon@Gothic_II_Addon@@YGXXZ");
			if (LoadRibbonProc)
				LoadRibbonProc();
		}

	}
#endif


	int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		
		if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
			return -1;

		BOOL bNameValid;

		m_wndRibbonBar.Create(this);
		m_wndRibbonBar.LoadFromResource(IDR_RIBBON);

		if (!m_wndStatusBar.Create(this))
		{
			TRACE0("Не удалось создать строку состояния\n");
			return -1;      // не удалось создать
		}
#ifdef EXPORT_RIBBON
		LoadRibbon();
#endif
		CString strTitlePane1;
		CString strTitlePane2;
		bNameValid = strTitlePane1.LoadString(IDS_STATUS_PANE1);
		ASSERT(bNameValid);
		bNameValid = strTitlePane2.LoadString(IDS_STATUS_PANE2);
		ASSERT(bNameValid);
		m_wndStatusBar.AddElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE1, strTitlePane1, TRUE), strTitlePane1);
		m_wndStatusBar.AddExtendedElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE2, strTitlePane2, TRUE), strTitlePane2);

		// включить режим работы закрепляемых окон стилей Visual Studio 2005
		CDockingManager::SetDockingMode(DT_SMART);
		// включить режим работы автоматического скрытия закрепляемых окон стилей Visual Studio 2005
		EnableAutoHidePanes(CBRS_ALIGN_ANY);

		// Загрузить изображение элемента меню (не размещенное на каких-либо стандартных панелях инструментов):
		CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);

		// создать закрепляемые окна
		if (!CreateDockingWindows())
		{
			TRACE0("Не удалось создать закрепляемые окна\n");
			return -1;
		}

		m_wndVobList.EnableDocking(CBRS_ALIGN_ANY);
		m_wndClassView.EnableDocking(CBRS_ALIGN_ANY);
		m_wndMeshLib.EnableDocking(CBRS_ALIGN_ANY);
		m_wndPresetLib.EnableDocking(CBRS_ALIGN_ANY);



		DockPane(&m_wndVobList);
		CDockablePane* pTabbedBar = NULL;
		m_wndClassView.AttachToTabWnd(&m_wndVobList, DM_SHOW, TRUE, &pTabbedBar);


		m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
		DockPane(&m_wndOutput);
		m_wndProperties.EnableDocking(CBRS_ALIGN_ANY);
		DockPane(&m_wndProperties);
		CDockablePane* pTabbedBar1 = NULL;
		m_wndMeshLib.AttachToTabWnd(&m_wndProperties, DM_SHOW, TRUE, &pTabbedBar1);
		m_wndPresetLib.AttachToTabWnd(&m_wndProperties, DM_SHOW, TRUE, &pTabbedBar1);

		// установите наглядный диспетчер и стиль на основе постоянного значения
		OnApplicationLook(theApp.m_nAppLook);

		return 0;
	}

	BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
	{
		
		if (!CFrameWndEx::PreCreateWindow(cs))
			return FALSE;
		return TRUE;
	}

	BOOL CMainFrame::CreateDockingWindows()
	{
		
		BOOL bNameValid;

		// Создать представление классов
		CString strClassView;
		bNameValid = strClassView.LoadString(IDS_CLASS_VIEW);
		ASSERT(bNameValid);
		if (!m_wndClassView.Create(strClassView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_CLASSVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
		{
			TRACE0("Не удалось создать окно \"Представление классов\"\n");
			return FALSE; // не удалось создать
		}

		// Создать представление файлов
		CString strFileView;
		bNameValid = strFileView.LoadString(IDS_FILE_VIEW);
		ASSERT(bNameValid);
		if (!m_wndVobList.Create(strFileView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_FILEVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
		{
			TRACE0("Не удалось создать окно \"Представление файлов\"\n");
			return FALSE; // не удалось создать
		}

		// Создать окно вывода
		CString strOutputWnd;
		bNameValid = strOutputWnd.LoadString(IDS_OUTPUT_WND);
		ASSERT(bNameValid);
		if (!m_wndOutput.Create(strOutputWnd, this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_OUTPUTWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
		{
			TRACE0("Не удалось создать окно \"Вывод\"\n");
			return FALSE; // не удалось создать
		}

		// Создать окно свойств
		CString strPropertiesWnd;
		bNameValid = strPropertiesWnd.LoadString(IDS_PROPERTIES_WND);
		ASSERT(bNameValid);
		if (!m_wndProperties.Create(strPropertiesWnd, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_PROPERTIESWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
		{
			TRACE0("Не удалось создать окно \"Свойства\"\n");
			return FALSE; // не удалось создать
		}

		// Создать окно meshlib
		CString strMeshLibWnd;
		bNameValid = strMeshLibWnd.LoadString(IDS_STRING101);
		ASSERT(bNameValid);
		if (!m_wndMeshLib.Create(strMeshLibWnd, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_MESHLIB, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
		{
			TRACE0("Cannot create \"MeshLib\"\n");
			return FALSE;
		}

		CString strPfxLibWnd;
		bNameValid = strPfxLibWnd.LoadString(IDS_STRING102);
		ASSERT(bNameValid);
		if (!m_wndPresetLib.Create(strPfxLibWnd, this, CRect(0, 0, 0, 0), TRUE, ID_VIEW_PFXLIB, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
		{
			TRACE0("Cannot create \"Preset lib\"\n");
			return FALSE;
		}

		SetDockingWindowIcons(theApp.m_bHiColorIcons);
		return TRUE;
	}

	void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
	{
		
		//HICON hFileViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_FILE_VIEW_HC : IDI_FILE_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
		//m_wndVobList.SetIcon(hFileViewIcon, FALSE);

		//HICON hClassViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_CLASS_VIEW_HC : IDI_CLASS_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
		//m_wndClassView.SetIcon(hClassViewIcon, FALSE);

		HICON hOutputBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_OUTPUT_WND_HC : IDI_OUTPUT_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
		m_wndOutput.SetIcon(hOutputBarIcon, FALSE);

		HICON hPropertiesBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_PROPERTIES_WND_HC : IDI_PROPERTIES_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
		m_wndProperties.SetIcon(hPropertiesBarIcon, FALSE);

	}

	// Диагностика CMainFrame

#ifdef _DEBUG
	void CMainFrame::AssertValid() const
	{
		CFrameWndEx::AssertValid();
	}

	void CMainFrame::Dump(CDumpContext& dc) const
	{
		CFrameWndEx::Dump(dc);
	}
#endif //_DEBUG


	// Обработчики сообщений CMainFrame

	void CMainFrame::OnApplicationLook(UINT id)
	{
		
		CWaitCursor wait;

		theApp.m_nAppLook = id;

		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_WIN_2000:
			CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
			m_wndRibbonBar.SetWindows7Look(FALSE);
			break;

		case ID_VIEW_APPLOOK_OFF_XP:
			CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
			m_wndRibbonBar.SetWindows7Look(FALSE);
			break;

		case ID_VIEW_APPLOOK_WIN_XP:
			CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
			CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
			m_wndRibbonBar.SetWindows7Look(FALSE);
			break;

		case ID_VIEW_APPLOOK_OFF_2003:
			CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
			CDockingManager::SetDockingMode(DT_SMART);
			m_wndRibbonBar.SetWindows7Look(FALSE);
			break;

		case ID_VIEW_APPLOOK_VS_2005:
			CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
			CDockingManager::SetDockingMode(DT_SMART);
			m_wndRibbonBar.SetWindows7Look(FALSE);
			break;

		case ID_VIEW_APPLOOK_VS_2008:
			CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
			CDockingManager::SetDockingMode(DT_SMART);
			m_wndRibbonBar.SetWindows7Look(FALSE);
			break;

		case ID_VIEW_APPLOOK_WINDOWS_7:
			CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
			CDockingManager::SetDockingMode(DT_SMART);
			m_wndRibbonBar.SetWindows7Look(TRUE);
			break;

		default:
			switch (theApp.m_nAppLook)
			{
			case ID_VIEW_APPLOOK_OFF_2007_BLUE:
				CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
				break;

			case ID_VIEW_APPLOOK_OFF_2007_BLACK:
				CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
				break;

			case ID_VIEW_APPLOOK_OFF_2007_SILVER:
				CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
				break;

			case ID_VIEW_APPLOOK_OFF_2007_AQUA:
				CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
				break;
			}

			CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
			CDockingManager::SetDockingMode(DT_SMART);
			m_wndRibbonBar.SetWindows7Look(FALSE);
		}

		m_wndOutput.UpdateFonts();
		RedrawWindow(nullptr, nullptr, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

		theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
	}

	void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
	{
		
		pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
	}



	void CMainFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
	{
		
		CFrameWndEx::OnSettingChange(uFlags, lpszSection);
		m_wndOutput.UpdateFonts();
	}

	void CMainFrame::CopyToClipBoard(CString str)
	{
		
		OpenClipboard();
		EmptyClipboard();
		HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, str.GetLength() + 1);
		if (!hg) {
			CloseClipboard();
			return;
		}
		memcpy(GlobalLock(hg), str.GetBuffer(0), str.GetLength() + 1);
		GlobalUnlock(hg);
		SetClipboardData(CF_TEXT, hg);
		CloseClipboard();
		GlobalFree(hg);
	}

	void CMainFrame::ShowProgressBar(CString text, BOOL bShow)
	{
		
		if (bShow)
		{
			RECT statRect;
			m_wndStatusBar.GetClientRect(&statRect);
			int sizex = statRect.right - statRect.left;
			m_progressBar = new CMFCRibbonProgressBar(4513, sizex / 4);
			m_wndStatusBar.AddDynamicElement(m_progressBar);
			m_progressBar->SetRange(0, 100);
			m_wndStatusBar.GetElement(0)->SetText(text + " ");
			m_wndStatusBar.RecalcLayout();
			m_wndStatusBar.GetElement(0)->Redraw();
		}
		else if (m_progressBar)
		{
			m_wndStatusBar.RemoveElement(4513);  
			//delete m_progressBar; m_progressBar = 0;
			m_wndStatusBar.GetElement(0)->SetText(text);
			m_wndStatusBar.RecalcLayout();
			m_wndStatusBar.GetElement(0)->Redraw();
			m_wndStatusBar.RedrawWindow();
		}
	}



	void CMainFrame::ApplyVideoSettings()
	{
		zCView::SetMode(viewx, viewy, 32, 0);
		zCViewDraw::GetScreen().SetSize(zPOS(8192, 8192));
		CMainFrame::mainframe->ResizeToView(CEditorView::view, viewx, viewy);
	}

	void CMainFrame::ResizeToView(CView* view, int x, int y)
	{
		static int oldX = -1;
		static int oldY = -1;
		if (oldX == x && oldY == y) return;

		RECT winRect;
		RECT viewRect;
		view->GetWindowRect(&viewRect);
		GetWindowRect(&winRect);

		RECT b;
		b.left = viewRect.left - winRect.left;
		b.top = viewRect.top - winRect.top;
		b.right = winRect.right - viewRect.right;
		b.bottom = winRect.bottom - viewRect.bottom;
		winRect.left = viewRect.left - b.left;
		winRect.right = viewRect.left + x + b.right;
		winRect.top = viewRect.top - b.top;
		winRect.bottom = viewRect.top + y + b.bottom;

		MoveWindow(&winRect, TRUE);

		oldX = x;
		oldY = y;
	}



	void CMainFrame::OnDestroy()
	{

		zErrReportDummy = TRUE;
		presetLib.Save();
		CFrameWndEx::OnDestroy();
		gameMan->Done();
		gameMan->GameDone();
	}




