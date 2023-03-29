// Supported with union (c) 2020 Union team
// Union SOURCE file

using namespace GOTHIC_ENGINE;
	class CClassViewMenuButton : public CMFCToolBarMenuButton
	{
		
		friend class CClassView;

		DECLARE_SERIAL(CClassViewMenuButton)

	public:
		CClassViewMenuButton(HMENU hMenu = nullptr)  : CMFCToolBarMenuButton((UINT)-1, hMenu, -1)
		{
			
		}

		virtual void OnDraw(CDC* pDC, const CRect& rect, CMFCToolBarImages* pImages, BOOL bHorz = TRUE,
			BOOL bCustomizeMode = FALSE, BOOL bHighlight = FALSE, BOOL bDrawBorder = TRUE, BOOL bGrayDisabledButtons = TRUE)
		{
			
			pImages = CMFCToolBar::GetImages();

			CAfxDrawState ds;
			pImages->PrepareDrawImage(ds);

			CMFCToolBarMenuButton::OnDraw(pDC, rect, pImages, bHorz, bCustomizeMode, bHighlight, bDrawBorder, bGrayDisabledButtons);

			pImages->EndDrawImage(ds);
		}
	};

	IMPLEMENT_SERIAL(CClassViewMenuButton, CMFCToolBarMenuButton, 1)


	CClassView::CClassView() 
	{
		
		m_nCurrSort = ID_SORTING_GROUPBYTYPE;
	}

	CClassView::~CClassView()
	{
		
	}

	BEGIN_MESSAGE_MAP(CClassView, CDockablePane)
		ON_WM_CREATE()
		ON_WM_SIZE()
		ON_WM_CONTEXTMENU()
		ON_COMMAND(ID_CLASS_ADD_MEMBER_FUNCTION, OnClassAddMemberFunction)
		ON_COMMAND(ID_CLASS_ADD_MEMBER_VARIABLE, OnClassAddMemberVariable)
		ON_COMMAND(ID_CLASS_DEFINITION, OnClassDefinition)
		ON_COMMAND(ID_CLASS_PROPERTIES, OnClassProperties)
		ON_COMMAND(ID_NEW_FOLDER, OnNewFolder)
		ON_WM_PAINT()
		ON_WM_SETFOCUS()
		ON_COMMAND_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnSort)
		ON_UPDATE_COMMAND_UI_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnUpdateSort)
	END_MESSAGE_MAP()


	int CClassView::OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		
		if (CDockablePane::OnCreate(lpCreateStruct) == -1)
			return -1;

		CRect rectDummy;
		rectDummy.SetRectEmpty();

		const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

		if (!m_wndClassView.Create(dwViewStyle, rectDummy, this, 2))
		{
			TRACE0("Не удалось создать представление классов\n");
			return -1;
		}


		m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_SORT);
		m_wndToolBar.LoadToolBar(IDR_SORT, 0, 0, TRUE /* Заблокирован */);

		OnChangeVisualStyle();

		m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
		m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

		m_wndToolBar.SetOwner(this);

		m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

		CMenu menuSort;
		menuSort.LoadMenu(IDR_POPUP_SORT);

		m_wndToolBar.ReplaceButton(ID_SORT_MENU, CClassViewMenuButton(menuSort.GetSubMenu(0)->GetSafeHmenu()));

		CClassViewMenuButton* pButton = DYNAMIC_DOWNCAST(CClassViewMenuButton, m_wndToolBar.GetButton(0));

		if (pButton != nullptr)
		{
			pButton->m_bText = FALSE;
			pButton->m_bImage = TRUE;
			pButton->SetImage(GetCmdMgr()->GetCmdImage(m_nCurrSort));
			pButton->SetMessageWnd(this);
		}
		return 0;
	}

	void CClassView::OnSize(UINT nType, int cx, int cy)
	{
		
		CDockablePane::OnSize(nType, cx, cy);
		AdjustLayout();
	}

	void CClassView::InitData()
	{
		
		HTREEITEM hRoot = m_wndClassView.InsertItem(_T("Engine objects"), 0, 0);
		m_wndClassView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);

		HTREEITEM newITEM1 = 0;

		int c = 0;

		zCClassDef* vobClass = zCVob::GetStaticClassDef();

		zSTRING s = vobClass->GetClassName_();
		newITEM1 = m_wndClassView.InsertItem(s.ToChar(), 1, 1, hRoot);
		FillClassView(newITEM1, vobClass);

		if (newITEM1) m_wndClassView.Expand(newITEM1, TVE_EXPAND);
	}

	void CClassView::FillClassView(HTREEITEM parent, zCClassDef* parentClassDef)
	{
		
		zSTRING baseName;
		zSTRING s;
		int Icon;
		int SelIcon;
		for (int i = 0; i < zCClassDef::GetNum(); i++)
		{
			zCClassDef* classDef = zCClassDef::GetClassDefByIndex(i);

			if (classDef->GetBaseClassDef() == parentClassDef) {

				zBOOL baseOK;
				if (classDef->GetBaseClassDef())
				{
					baseName = classDef->GetBaseClassDef()->GetClassName_();
					baseOK = (baseName == classDef->GetBaseClassName());
				}
				else {
					baseOK = (classDef->GetBaseClassName() == "NULL");
				};

				s = classDef->GetClassName_();
				Icon = 1;
				SelIcon = 3;
				CString postfix = "";
				if (!baseOK)
				{
					if (!postfix.IsEmpty()) postfix += ",";
					postfix += "!Corrupted!";
				}
				if (classDef->IsAbstractClass())
				{
					if (!postfix.IsEmpty()) postfix += ",";
					postfix += "Virtual";
					Icon = 6;
					SelIcon = 6;
				}
				if (!postfix.IsEmpty()) s = s + " (" + postfix.GetBuffer(0) + ")";

				HTREEITEM newITEM =  NULL;

				if ((s != "oCItem") && (s != "oCNpc") && (s != "zCVobWaypoint") && (s != "zCVobSpot") && (s != "zCVobLight") && (s != "zCCSCamera") && (s != "zCCamTrj_KeyFrame")/* && !zCObject::CheckInheritance(zCTriggerBase::classDef, classDef)*/)
					newITEM = m_wndClassView.InsertItem(s.ToChar(), Icon, SelIcon, parent);

				FillClassView(newITEM, classDef);
			};
		};
	}


	void CClassView::OnContextMenu(CWnd* pWnd, CPoint point)
	{
		
		CTreeCtrl* pWndTree = (CTreeCtrl*)&m_wndClassView;
		ASSERT_VALID(pWndTree);

		if (pWnd != pWndTree)
		{
			CDockablePane::OnContextMenu(pWnd, point);
			return;
		}

		if (point != CPoint(-1, -1))
		{
			CPoint ptTree = point;
			pWndTree->ScreenToClient(&ptTree);

			UINT flags = 0;
			HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
			if (hTreeItem != nullptr)
			{
				pWndTree->SelectItem(hTreeItem);
			}
		}

		pWndTree->SetFocus();
		CMenu menu;
		menu.LoadMenu(IDR_POPUP_SORT);

		CMenu* pSumMenu = menu.GetSubMenu(0);

		if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
		{
			CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

			if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
				return;

			((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
			UpdateDialogControls(this, FALSE);
		}
	}

	void CClassView::AdjustLayout()
	{
		
		if (GetSafeHwnd() == nullptr)
		{
			return;
		}

		CRect rectClient;
		GetClientRect(rectClient);

		int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

		m_wndToolBar.SetWindowPos(nullptr, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
		m_wndClassView.SetWindowPos(nullptr, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
	}

	BOOL CClassView::PreTranslateMessage(MSG* pMsg)
	{
		
		return CDockablePane::PreTranslateMessage(pMsg);
	}

	void CClassView::OnSort(UINT id)
	{
		
		if (m_nCurrSort == id)
		{
			return;
		}

		m_nCurrSort = id;

		CClassViewMenuButton* pButton = DYNAMIC_DOWNCAST(CClassViewMenuButton, m_wndToolBar.GetButton(0));

		if (pButton != nullptr)
		{
			pButton->SetImage(GetCmdMgr()->GetCmdImage(id));
			m_wndToolBar.Invalidate();
			m_wndToolBar.UpdateWindow();
		}
	}

	void CClassView::OnUpdateSort(CCmdUI* pCmdUI)
	{
		
		pCmdUI->SetCheck(pCmdUI->m_nID == m_nCurrSort);
	}

	void CClassView::OnClassAddMemberFunction()
	{
		
		AfxMessageBox(_T("Добавить функцию-член..."));
	}

	void CClassView::OnClassAddMemberVariable()
	{
		
	}

	void CClassView::OnClassDefinition()
	{
		
	}

	void CClassView::OnClassProperties()
	{
		
	}

	void CClassView::OnNewFolder()
	{
		
		AfxMessageBox(_T("Создать папку..."));
	}

	void CClassView::OnPaint()
	{
		
		CPaintDC dc(this);

		CRect rectTree;
		m_wndClassView.GetWindowRect(rectTree);
		ScreenToClient(rectTree);

		rectTree.InflateRect(1, 1);
		dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
	}

	void CClassView::OnSetFocus(CWnd* pOldWnd)
	{
		
		CDockablePane::OnSetFocus(pOldWnd);

		m_wndClassView.SetFocus();
	}

	void CClassView::OnChangeVisualStyle()
	{
		
		m_ClassViewImages.DeleteImageList();

		UINT uiBmpId =  theApp.m_bHiColorIcons ? IDB_CLASS_VIEW_24 : IDB_CLASS_VIEW;

		CBitmap bmp;
		if (!bmp.LoadBitmap(uiBmpId))
		{
			TRACE(_T("Не удается загрузить точечный рисунок: %x\n"), uiBmpId);
			ASSERT(FALSE);
			return;
		}

		BITMAP bmpObj;
		bmp.GetBitmap(&bmpObj);

		UINT nFlags = ILC_MASK;

		nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

		m_ClassViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
		m_ClassViewImages.Add(&bmp, RGB(255, 0, 0));

		m_wndClassView.SetImageList(&m_ClassViewImages, TVSIL_NORMAL);

		m_wndToolBar.CleanUpLockedImages();
		m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_SORT_24 : IDR_SORT, 0, 0, TRUE /* Заблокирован */);
	}

