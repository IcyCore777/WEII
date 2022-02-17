// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
	HOOK Hook_CGameManager_Init		PATCH(&CGameManager::Init, &CGameManager::Init_Union);

	void CGameManager::Init_Union(HWND& wnd)
	{
		DestroyWindow(wnd);
		CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
		AFX_MANAGE_STATE(AfxGetStaticModuleState());

		theApp.bReady = true;
		theApp.InitInstance();
		theApp.Process();
	}




	void CGameManager::InitGothic()
	{
		THISCALL(Hook_CGameManager_Init)(CEditorView::view->m_hWnd);
		ControllerEvents.mainframe = CMainFrame::mainframe->m_hWnd;
		
#ifdef EXPORT_RIBBON
//		LoadRibbon();
#endif
		ControllerEvents.PickMode = zPM_VOBS;
		ControllerEvents.GizmoScale = 3;

		ogame->game_frameinfo = TRUE;
		if (dontStartGame) return;
		gameSession->game_testmode = TRUE;
		exitGame = FALSE;
		zSTRING inivalue = zoptions->ParmValue("ini");
		if (inivalue.IsEmpty())
			inivalue = "Gothic.ini";
		zoptions->Save(inivalue);
		ShowCursor(TRUE);

		CMainFrame::mainframe->ApplyVideoSettings();
		OutputBuild("ApplyVideoSettings");
		CMainFrame::mainframe->m_wndClassView.InitData();
		OutputBuild("Build class-tree");
		CEditorView::view->homeSettings.InitData();
		OutputBuild("Init home page");
		CMainFrame::mainframe->presetLib.Load();
		OutputBuild("Loading settings ");
		CEditorView::view->settings.InitData();
		CEditorView::view->isStartScreen = TRUE;
		ogame->UpdateScreenResolution();
		OutputBuild("Update screen res");
	}
}