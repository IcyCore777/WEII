// Supported with union (c) 2020 Union team
// Union SOURCE file

using namespace GOTHIC_ENGINE;

	CViewTree::CViewTree() 
	{
		
	}

	CViewTree::~CViewTree()
	{
		
	}

	BEGIN_MESSAGE_MAP(CViewTree, CTreeCtrl)
	END_MESSAGE_MAP()


	BOOL CViewTree::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
	{
		
		BOOL bRes = CTreeCtrl::OnNotify(wParam, lParam, pResult);

		NMHDR* pNMHDR = (NMHDR*)lParam;
		ASSERT(pNMHDR != nullptr);

		if (pNMHDR && pNMHDR->code == TTN_SHOW && GetToolTips() != nullptr)
		{
			GetToolTips()->SetWindowPos(&wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOSIZE);
		}

		return bRes;
	}


	CClassTree::CClassTree()
	{
		
	}


	CClassTree::~CClassTree()
	{
		
	}


	BEGIN_MESSAGE_MAP(CClassTree, CTreeCtrl)
		ON_NOTIFY_REFLECT(NM_DBLCLK, &CClassTree::OnNMDblclk)
		ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CClassTree::OnTvnSelchanged)
	END_MESSAGE_MAP()


	BOOL CClassTree::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
	{
		
		BOOL bRes = CTreeCtrl::OnNotify(wParam, lParam, pResult);

		NMHDR* pNMHDR = (NMHDR*)lParam;
		ASSERT(pNMHDR != NULL);

		if (pNMHDR && pNMHDR->code == TTN_SHOW && GetToolTips() != NULL)
		{
			GetToolTips()->SetWindowPos(&wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOSIZE);
		}

		return bRes;
	}


	void CClassTree::OnNMDblclk(NMHDR* pNMHDR, LRESULT* pResult)
	{
		
		*pResult = 0;
	}


	void CClassTree::OnTvnSelchanged(NMHDR* pNMHDR, LRESULT* pResult)
	{
		
		LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
		*pResult = 0;
		CMainFrame::mainframe->m_wndPresetLib.LoadPreset(ClassTree.GetItemText(ClassTree.GetSelectedItem()));
	}

