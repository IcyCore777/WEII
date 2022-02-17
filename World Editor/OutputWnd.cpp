// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {

	BOOL zErrReportDummy = FALSE;


	inline zSTRING Characters(char ch, int anzahl)
	{
		
		return zSTRING((float)anzahl, ch);
	}


	COutputWnd::COutputWnd()
	{
		
	}

	COutputWnd::~COutputWnd()
	{
		
	}

	BEGIN_MESSAGE_MAP(COutputWnd, CDockablePane)
		ON_WM_CREATE()
		ON_WM_SIZE()
	END_MESSAGE_MAP()

	int COutputWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		
		if (CDockablePane::OnCreate(lpCreateStruct) == -1)
			return -1;

		CRect rectDummy;
		rectDummy.SetRectEmpty();

		if (!m_wndTabs.Create(CMFCTabCtrl::STYLE_FLAT, rectDummy, this, 1))
		{
			TRACE0("Ќе удалось создать окно вкладки вывода\n");
			return -1;
		}

		const DWORD dwStyle = LBS_EXTENDEDSEL | LBS_NOINTEGRALHEIGHT | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL;
		const DWORD dwStyleDebug = LBS_EXTENDEDSEL | LBS_OWNERDRAWFIXED | LBS_HASSTRINGS | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL;

		if (!m_wndOutputDebug.Create(dwStyleDebug, rectDummy, &m_wndTabs,	3))
		{
			TRACE0("Cannot create debug window\n");
			return -1;
		}
		if (!m_wndOutputBuild.Create(dwStyle, rectDummy, &m_wndTabs,		2) ||

			!m_wndOutputFind.Create(dwStyle, rectDummy, &m_wndTabs,			4) ||

			!m_wndOutputSpy.Create(dwStyleDebug, rectDummy, &m_wndTabs,		5))
		{
			TRACE0("Cannot create output window\n");
			return -1;
		}

		UpdateFonts();

		CString strTabName;
		BOOL bNameValid;

		bNameValid = strTabName.LoadString(IDS_BUILD_TAB);
		ASSERT(bNameValid);
		m_wndTabs.AddTab(&m_wndOutputBuild, strTabName, (UINT)BUILDTAB);
		bNameValid = strTabName.LoadString(IDS_DEBUG_TAB);
		ASSERT(bNameValid);
		m_wndTabs.AddTab(&m_wndOutputDebug, strTabName, (UINT)DEBUGTAB);
		bNameValid = strTabName.LoadString(IDS_FIND_TAB);
		ASSERT(bNameValid);
		m_wndTabs.AddTab(&m_wndOutputFind, strTabName, (UINT)FINDTAB);
		bNameValid = strTabName.LoadString(IDS_SPY_TAB);
		ASSERT(bNameValid);
		m_wndTabs.AddTab(&m_wndOutputSpy, strTabName, (UINT)SPYTAB);

		return 0;
	}

	void COutputWnd::OnSize(UINT nType, int cx, int cy)
	{
		
		CDockablePane::OnSize(nType, cx, cy);
		m_wndTabs.SetWindowPos(nullptr, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
	}

	void COutputWnd::AdjustHorzScroll(CListBox& wndListBox)
	{
		
		CClientDC dc(this);
		CFont* pOldFont = dc.SelectObject(&afxGlobalData.fontRegular);

		int cxExtentMax = 0;

		for (int i = 0; i < wndListBox.GetCount(); i++)
		{
			CString strItem;
			wndListBox.GetText(i, strItem);

			cxExtentMax = max(cxExtentMax, (int)dc.GetTextExtent(strItem).cx);
		}

		wndListBox.SetHorizontalExtent(cxExtentMax);
		dc.SelectObject(pOldFont);
	}

	void COutputWnd::UpdateFonts()
	{
		
		m_wndOutputBuild.SetFont(&afxGlobalData.fontTooltip);
		m_wndOutputDebug.SetFont(&afxGlobalData.fontTooltip);
		m_wndOutputFind.SetFont(&afxGlobalData.fontTooltip);
		m_wndOutputSpy.SetFont(&afxGlobalData.fontTooltip);
	}




	BEGIN_MESSAGE_MAP(COutputList, CListBox)
		ON_WM_CONTEXTMENU()
		ON_WM_WINDOWPOSCHANGING()
		ON_WM_LBUTTONDBLCLK()
		ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
		ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
		ON_COMMAND(ID_VIEW_OUTPUTWND, OnViewOutput)
	END_MESSAGE_MAP()

	COutputList::COutputList()
	{
		
	}

	COutputList::~COutputList()
	{
		
	}

	int COutputList::Insert(zSTRING str)
	{
		
		return AddString(str.ToChar());
	}


	void COutputList::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
	{
		
		CMenu menu;
		menu.LoadMenu(IDR_OUTPUT_POPUP);

		CMenu* pSumMenu = menu.GetSubMenu(0);


		CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

		if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
			return;

		((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
		UpdateDialogControls(this, FALSE);


		SetFocus();
	}

	void COutputList::OnEditCopy()
	{
		
		CString str;
		int         i, iSize, * pIndex;
		CString     strText;

		iSize = GetSelCount();
		if (iSize > 0)
		{
			pIndex = new int[iSize];
			GetSelItems(iSize, pIndex);

			for (i = 0; i < iSize; i++)
			{
				GetText(pIndex[i], strText);
				str += strText + "\n";
			}
			delete[] pIndex;
		}

		CMainFrame::mainframe->CopyToClipBoard(str);
	}

	void COutputList::OnEditClear()
	{
		
		ResetContent();
		AddString("Info output...");
	}

	void COutputList::OnViewOutput()
	{
		
		CDockablePane* pParentBar = DYNAMIC_DOWNCAST(CDockablePane, GetOwner());
		CMDIFrameWndEx* pMainFrame = DYNAMIC_DOWNCAST(CMDIFrameWndEx, GetTopLevelFrame());

		if (pMainFrame != nullptr && pParentBar != nullptr)
		{
			pMainFrame->SetFocus();
			pMainFrame->ShowPane(pParentBar, FALSE, FALSE, FALSE);
			pMainFrame->RecalcLayout();
		}
	}

	void COutputList::OnLButtonDblClk(UINT nFlags, CPoint point)
	{
		
		if (CMainFrame::mainframe->m_wndOutput.m_wndTabs.GetActiveTab() == FINDTAB && GetCount() > 1)
		{
			zCVob* foundVob = (zCVob*)GetItemDataPtr(GetCurSel());
			if (foundVob)
				CEditorView::view->VobGoto(foundVob);
		}

		CListBox::OnLButtonDblClk(nFlags, point);
	}

	BOOL COutputList::PreTranslateMessage(MSG* pMsg)
	{
		
		if (dynamic_cast<CMainFrame*>(theApp.GetMainWnd())->m_hAccelTable)
			if (::TranslateAccelerator(*this, dynamic_cast<CMainFrame*>(theApp.GetMainWnd())->m_hAccelTable, pMsg))
				return(TRUE);
		return CListBox::PreTranslateMessage(pMsg);
	}


	void zERROR::Init_Union(zSTRING parmlist)
	{
		
		ack_type = zERR_TYPE_FATAL;

		if (filter_level > 0)
			ack_type = zERR_TYPE_FAULT;
	}


	int zERROR::Report_Union(zERROR_TYPE   type, int id, const zSTRING& str_text, signed char levelPrio, unsigned int flag, int line, char* file, char* function)
	{
		
		if(CEditorView::view)
			zerr->filter_level = 3;
		if (zErrReportDummy) return 0;

		if ((filter_level < 0) && (type != zERR_FATAL)) return id;

		zBOOL isAuthor = (str_text.Length() >= 2) && (str_text.operator[](1) == ':');

		if	(
			(type != zERR_TYPE_INFO)
			|| (isAuthor && (levelPrio <= filter_level) && (filter_authors.Search(str_text.operator[](0), 1) >= 0))
			)
		{

			zSTRING str = "";

			switch (type)
			{
			case zERR_TYPE_INFO:	str += "Info: "; break;
			case zERR_TYPE_WARN:	str += "Warn: "; break;
			case zERR_TYPE_FAULT:	str += "Fault:"; break;
			case zERR_TYPE_FATAL:	str += "Fatal:"; break;
			default:				str += "?????:"; break;
			}


			str += zSTRING(levelPrio).Align(zSTR_RIGHT, 2, ' ');
			str += " ";
			str += str_text.Inserted(2, Characters(' ', indent_depth * 2));

			if (type == zERR_TYPE_INFO)
				CMainFrame::mainframe->m_wndOutput.m_wndOutputSpy.SetTopIndex(CMainFrame::mainframe->m_wndOutput.m_wndOutputSpy.AddString(str, RGB(0, 0, 0)));
			if (type == zERR_TYPE_WARN)
				CMainFrame::mainframe->m_wndOutput.m_wndOutputSpy.SetTopIndex(CMainFrame::mainframe->m_wndOutput.m_wndOutputSpy.AddString(str, RGB(255, 180, 0)));
			if (type == zERR_TYPE_FAULT)
				CMainFrame::mainframe->m_wndOutput.m_wndOutputSpy.SetTopIndex(CMainFrame::mainframe->m_wndOutput.m_wndOutputSpy.AddString(str, RGB(255, 0, 0)));
		};

		return id;
	}


	void zERROR::Separator_Union(zSTRING titel)
	{
		
		if (zErrReportDummy) return;
		if (filter_level <= 0) return;

		zSTRING str = "---------------";

		OutputReport(str);

		if (!titel.IsEmpty())
		{
			zERR_MESSAGE(zERR_LV_SECTION, 0, "          " + titel);
			Separator_Union("");
		};
	}
}