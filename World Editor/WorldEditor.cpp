// Supported with union (c) 2020 Union team
// Union SOURCE file
#include <crtversion.h>

using namespace GOTHIC_ENGINE;

	CString strController = "Controller";
	CString strMoving = "MovingSpeed";
	CString strRotSpeed = "RotatingSpeed";
	CString strFPS = "fpsLimit";
	CString strSmooth = "SmoothFactor";
	CString _meshLib = "MeshLibMode";
	CString settings = "Settings";
	CString strRnd = "Renderer";
	CString strMode = "RenderMode";
	CString strFog = "Fogging";
	CString strFar = "Farclip";

	CString strRes = "Resolution";
	CString strHeight = "Height";
	CString strWidth = "Width";

	CString strLog = "Log";
	CString strSpy = "LogLevel";

	BEGIN_MESSAGE_MAP(CEditorApp, CWinAppEx)
		ON_COMMAND(ID_APP_ABOUT, &CEditorApp::OnAppAbout)
		ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
		ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	END_MESSAGE_MAP()


	CEditorApp::CEditorApp()  : bInitialised(false)
	{
		
		bReady = false;
		m_bHiColorIcons = TRUE;
		bDrawing = true;

		SetAppID(_T("WorldEditor.AppID.NoVersion"));
	}


	CEditorApp theApp;

	int CEditorApp::Process()
	{
		
		ASSERT_VALID(this);
		_AFX_THREAD_STATE* pState = AfxGetThreadState();

		BOOL bIdle = TRUE;
		LONG lIdleCount = 0;

		gameMan->InitGothic();
		bInitialised = true;
		for (;;)
		{
			while (bIdle &&
				!::PeekMessage(&(pState->m_msgCur), NULL, NULL, NULL, PM_NOREMOVE)) {
				OnIdle(lIdleCount);
			}
			do
			{
				if (!PumpMessage())
					return ExitInstance();

				if (IsIdleMessage(&(pState->m_msgCur)))
				{
					bIdle = TRUE;
					lIdleCount = 0;
				}

			} while (::PeekMessage(&(pState->m_msgCur), NULL, NULL, NULL, PM_NOREMOVE));
		}
	}



	BOOL CEditorApp::InitInstance()
	{
		
		if (!bReady) {
			Union.DefineCRTVersion(_VC_CRT_MAJOR_VERSION, _VC_CRT_MINOR_VERSION, _VC_CRT_BUILD_VERSION, _VC_CRT_RBUILD_VERSION);
			return TRUE;
		}
		Common::cmd << "theApp.InitInstance()::Define CRT version" << Common::endl;
		INITCOMMONCONTROLSEX InitCtrls;
		InitCtrls.dwSize = sizeof(InitCtrls);

		InitCtrls.dwICC = ICC_WIN95_CLASSES;
		InitCommonControlsEx(&InitCtrls);

		CWinAppEx::InitInstance();
		Common::cmd << "theApp.InitInstance::CWinAppEx::InitInstance();" << Common::endl;
		if (!AfxOleInit())
		{
			AfxMessageBox(IDP_OLE_INIT_FAILED);
			return FALSE;
		}

		AfxEnableControlContainer();
		EnableTaskbarInteraction(FALSE);
		SetRegistryKey(_T("Gothic 2 World Editor"));
		LoadStdProfileSettings(5);
		   

		InitContextMenuManager();

		InitKeyboardManager();

		InitTooltipManager();
		Common::cmd << "theApp.InitInstance::CWinAppEx::InitTooltipManager" << Common::endl;
		CMFCToolTipInfo ttParams;
		ttParams.m_bVislManagerTheme = TRUE;
		theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
			RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

		Common::cmd << "theApp.InitInstance::CWinAppEx::Init doc template" << Common::endl;
		CSingleDocTemplate* pDocTemplate;
		pDocTemplate = new CSingleDocTemplate(
			IDR_MAINFRAME,
			RUNTIME_CLASS(CEditorDoc),
			RUNTIME_CLASS(CMainFrame),
			RUNTIME_CLASS(CEditorView));
		if (!pDocTemplate)
			return FALSE;
		AddDocTemplate(pDocTemplate);

		Common::cmd << "theApp.InitInstance::CWinAppEx::Documet initialised" << Common::endl;
		CMFCToolBar::m_bExtCharTranslation = TRUE;

		
		CCommandLineInfo cmdInfo;
		ParseCommandLine(cmdInfo);

		EnableShellOpen();
		RegisterShellFileTypes(TRUE);
		Common::cmd << "theApp.InitInstance::CWinAppEx::Command line" << Common::endl;

		if (!ProcessShellCommand(cmdInfo))
			return FALSE;
	
		Common::cmd << "theApp.InitInstance::Showng window" << Common::endl;
		m_pMainWnd->ShowWindow(SW_SHOW);
		m_pMainWnd->UpdateWindow();
		m_pMainWnd->DragAcceptFiles();



		viewy = GetProfileInt(strRes, strHeight, 600);
		viewx = GetProfileInt(strRes, strWidth, 800);

		ControllerEvents.movingSpeed = GetProfileInt(strController, strMoving, 50);
		ControllerEvents.rotatingSpeed = GetProfileInt(strController, strRotSpeed, 50);
		ControllerEvents.smoothFactor = GetProfileInt(strController, strSmooth, 50);

		GetSlider(ID_MOVSPD)->SetPos(ControllerEvents.movingSpeed);
		GetSlider(ID_ROTSPD)->SetPos(ControllerEvents.rotatingSpeed);
		GetSlider(ID_SMOOTH)->SetPos(ControllerEvents.smoothFactor);

		Common::cmd << "theApp.InitInstance::CWinAppEx::Done!" << Common::endl;
		return TRUE;
	}

	int CEditorApp::ExitInstance()
	{
		WriteProfileInt(settings, _meshLib, CMainFrame::mainframe->m_wndMeshLib.mode);
		WriteProfileInt(strController, strMoving, ControllerEvents.movingSpeed);
		WriteProfileInt(strController, strRotSpeed, ControllerEvents.rotatingSpeed);
		WriteProfileInt(strController, strSmooth, ControllerEvents.smoothFactor);
		WriteProfileInt(strRes, strHeight, viewy);
		WriteProfileInt(strRes, strWidth, viewx);
		AfxOleTerm(FALSE);
		if (m_pRecentFileList != NULL)
			m_pRecentFileList->WriteList();
		return CWinAppEx::ExitInstance();
	}


	class CAboutDlg : public CDialogEx
	{
	public:
		CAboutDlg() ;

#ifdef AFX_DESIGN_TIME
		enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
		virtual void DoDataExchange(CDataExchange* pDX);

	protected:
		DECLARE_MESSAGE_MAP()
	};

	CAboutDlg::CAboutDlg()  : CDialogEx(IDD_ABOUTBOX)
	{
	}

	void CAboutDlg::DoDataExchange(CDataExchange* pDX)
	{
		CDialogEx::DoDataExchange(pDX);
	}

	BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	END_MESSAGE_MAP()

	void CEditorApp::OnAppAbout()
	{
		CAboutDlg aboutDlg;
		aboutDlg.DoModal();
	}

	void CEditorApp::PreLoadState()
	{
		BOOL bNameValid;
		CString strName;
		bNameValid = strName.LoadString(IDS_EDIT_MENU);
		ASSERT(bNameValid);
		GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
		bNameValid = strName.LoadString(IDS_EXPLORER);
		ASSERT(bNameValid);
		GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
	}

	void CEditorApp::LoadCustomState()
	{
	}

	void CEditorApp::SaveCustomState()
	{
	}

	void CEditorApp::EnableDrawing(bool enable)
	{

		bDrawing = enable;
	}


	BOOL CEditorApp::OnIdle(LONG lCount)
	{

		if (bDrawing)
			CEditorView::view->RedrawWindow(0, 0, RDW_INVALIDATE);
		return CWinAppEx::OnIdle(lCount);
	}




