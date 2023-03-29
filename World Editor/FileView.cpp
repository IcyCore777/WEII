// Supported with union (c) 2020 Union team
// Union SOURCE file

using namespace GOTHIC_ENGINE;

	CVobList::CVobList() 
	{
		
	}

	CVobList::~CVobList()
	{
		
	}

	BEGIN_MESSAGE_MAP(CVobList, CDockablePane)
		ON_WM_CREATE()
		ON_WM_SIZE()
		ON_WM_CONTEXTMENU()
		ON_COMMAND(ID_PROPERTIES, OnProperties)
		ON_COMMAND(ID_OPEN, OnFileOpen)
		ON_COMMAND(ID_OPEN_WITH, OnFileOpenWith)
		ON_COMMAND(ID_DUMMY_COMPILE, OnDummyCompile)
		ON_COMMAND(ID_EDIT_CUT, OnEditCut)
		ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
		ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
		ON_WM_PAINT()
		ON_WM_SETFOCUS()
	END_MESSAGE_MAP()


	int CVobList::OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		
		if (CDockablePane::OnCreate(lpCreateStruct) == -1)
			return -1;

		CRect rectDummy;
		rectDummy.SetRectEmpty();

		const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;

		if (!m_wndVobList.Create(dwViewStyle, rectDummy, this, 4))
		{
			TRACE0("Не удалось создать представление файлов\n");
			return -1;
		}
		m_FileViewImages.Create(IDB_FILE_VIEW, 16, 0, RGB(255, 0, 255));
		m_wndVobList.SetImageList(&m_FileViewImages, TVSIL_NORMAL);

		m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_EXPLORER);
		m_wndToolBar.LoadToolBar(IDR_EXPLORER, 0, 0, TRUE /* Заблокирован */);

		OnChangeVisualStyle();

		m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);

		m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

		m_wndToolBar.SetOwner(this);

		
		m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

		UpdateVobList();
		AdjustLayout();

		return 0;
	}

	void CVobList::OnSize(UINT nType, int cx, int cy)
	{
		
		CDockablePane::OnSize(nType, cx, cy);
		AdjustLayout();
	}

	void CVobList::UpdateVobList()
	{

	}

	void CVobList::OnContextMenu(CWnd* pWnd, CPoint point)
	{
		
		CTreeCtrl* pWndTree = (CTreeCtrl*)&m_wndVobList;
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
		theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EXPLORER, point.x, point.y, this, TRUE);
	}

	void CVobList::AdjustLayout()
	{
		
		if (GetSafeHwnd() == nullptr)
		{
			return;
		}

		CRect rectClient;
		GetClientRect(rectClient);

		int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

		m_wndToolBar.SetWindowPos(nullptr, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
		m_wndVobList.SetWindowPos(nullptr, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
	}

	VirtualVobTree* FindVobInVirtualVobTree(HTREEITEM item)
	{
		VirtualVobTree* treeItem = VirtualVobTree_Portals::GetInstance().FindNodeByInterfaceTreeItem(item);
		if (treeItem)
			return treeItem;

		return VirtualVobTree_Globals::GetInstance().FindNodeByInterfaceTreeItem(item);
	}

	void CVobList::OnProperties()
	{
		
		if (FindVobInVirtualVobTree(m_wndVobList.GetSelectedItem())->TreeNode)
			CEditorView::view->SelectObject(FindVobInVirtualVobTree(m_wndVobList.GetSelectedItem())->TreeNode->GetData());

	}

	void CVobList::OnFileOpen()
	{
		

	}

	void CVobList::OnFileOpenWith()
	{
		

	}

	void CVobList::OnDummyCompile()
	{
		

	}


	void CVobList::OnEditCut()
	{
		CEditorView::view->homeSettings.OnCut();
	}

	void CVobList::OnEditCopy()
	{
		CEditorView::view->homeSettings.OnCopy();
	}

	void CVobList::OnEditClear()
	{
		HTREEITEM item = m_wndVobList.GetSelectedItem();
		VirtualVobTree* treeItem = FindVobInVirtualVobTree(item);
		if (!treeItem)
			return;

		if (treeItem->TreeNode)
			CEditorView::view->RemoveObject(treeItem->TreeNode->GetData());
		else
			treeItem->Remove(item);
	}

	void CVobList::OnPaint()
	{
		
		CPaintDC dc(this);

		CRect rectTree;
		m_wndVobList.GetWindowRect(rectTree);
		ScreenToClient(rectTree);

		rectTree.InflateRect(1, 1);
		dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
	}

	void CVobList::OnSetFocus(CWnd* pOldWnd)
	{
		
		CDockablePane::OnSetFocus(pOldWnd);

		m_wndVobList.SetFocus();
	}

	void CVobList::OnChangeVisualStyle()
	{
		
		m_wndToolBar.CleanUpLockedImages();
		m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_EXPLORER_24 : IDR_EXPLORER, 0, 0, TRUE /* Заблокирован */);

		m_FileViewImages.DeleteImageList();

		UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_FILE_VIEW_24 : IDB_FILE_VIEW;

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

		m_FileViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
		m_FileViewImages.Add(&bmp, RGB(255, 0, 255));

		m_wndVobList.SetImageList(&m_FileViewImages, TVSIL_NORMAL);
	}



	void CVobList::CreateVobTree(zCTree<zCVob>* node, HTREEITEM parent)
	{
		zCTree<zCVob>* tree = node->GetFirstChild();
		while (tree)
		{
			zCVob* vob = tree->GetData();
			const zSTRING* sectorName = vob->GetSectorNameVobIsIn();

			if (sectorName)
			{
				VirtualVobTree_Portals::GetInstance().Insert(tree);
			}
			else
			{
				VirtualVobTree_Globals::GetInstance().Insert(tree);
			}

			tree = tree->GetNextChild();
		}
	}

	void CVobList::BuildVobTree()
	{
		zCTree<zCVob>* tree = ogame->world->globalVobTree.GetFirstChild();

		while (tree)
		{
			CreateVobTree(tree, 0);
			tree = tree->GetNextChild();
		}
		m_wndVobList.Expand(0, TVE_EXPAND);
		m_wndVobList.RedrawWindow();
	}



