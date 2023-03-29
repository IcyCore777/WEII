// Supported with union (c) 2020 Union team
// Union SOURCE file

using namespace GOTHIC_ENGINE;
	CPropertiesWnd::CPropertiesWnd() 
	{
		m_nComboHeight = 0;
		vob = Null;
		arch = Null;
	}

	CPropertiesWnd::~CPropertiesWnd()
	{
	}

	BEGIN_MESSAGE_MAP(CPropertiesWnd, CDockablePane)
		ON_WM_CREATE()
		ON_WM_SIZE()
		ON_COMMAND(ID_EXPAND_ALL, OnExpandAllProperties)
		ON_UPDATE_COMMAND_UI(ID_EXPAND_ALL, OnUpdateExpandAllProperties)
		ON_COMMAND(ID_SORTPROPERTIES, OnSortProperties)
		ON_UPDATE_COMMAND_UI(ID_SORTPROPERTIES, OnUpdateSortProperties)
		ON_COMMAND(ID_PROPERTIES1, OnProperties1)
		ON_UPDATE_COMMAND_UI(ID_PROPERTIES1, OnUpdateProperties1)
		ON_COMMAND(ID_PROPERTIES2, OnProperties2)
		ON_UPDATE_COMMAND_UI(ID_PROPERTIES2, OnUpdateProperties2)
		ON_WM_SETFOCUS()
		ON_WM_SETTINGCHANGE()
		ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChanged)
	END_MESSAGE_MAP()


	void CPropertiesWnd::AdjustLayout()
	{
		
		if (GetSafeHwnd() == nullptr || (AfxGetMainWnd() != nullptr && AfxGetMainWnd()->IsIconic()))
		{
			return;
		}

		CRect rectClient;
		GetClientRect(rectClient);

		int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

		m_wndObjectCombo.SetWindowPos(nullptr, rectClient.left, rectClient.top, rectClient.Width(), m_nComboHeight, SWP_NOACTIVATE | SWP_NOZORDER);
		m_wndToolBar.SetWindowPos(nullptr, rectClient.left, rectClient.top + m_nComboHeight, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
		m_wndPropList.SetWindowPos(nullptr, rectClient.left, rectClient.top + m_nComboHeight + cyTlb, rectClient.Width(), rectClient.Height() - (m_nComboHeight + cyTlb), SWP_NOACTIVATE | SWP_NOZORDER);
	}

	int CPropertiesWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		
		if (CDockablePane::OnCreate(lpCreateStruct) == -1)
			return -1;

		CRect rectDummy;
		rectDummy.SetRectEmpty();

		const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER | CBS_SORT | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

		if (!m_wndObjectCombo.Create(dwViewStyle, rectDummy, this, 1))
		{
			TRACE0("Не удалось создать поле со списком \"Свойства\" \n");
			return -1;
		}

		m_wndObjectCombo.AddString(_T("Приложение"));
		m_wndObjectCombo.AddString(_T("Окно свойств"));
		m_wndObjectCombo.SetCurSel(0);

		CRect rectCombo;
		m_wndObjectCombo.GetClientRect(&rectCombo);

		m_nComboHeight = rectCombo.Height();

		if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
		{
			TRACE0("Не удалось создать сетку свойств\n");
			return -1;
		}

		InitPropList();

		m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_PROPERTIES);
		m_wndToolBar.LoadToolBar(IDR_PROPERTIES, 0, 0, TRUE /* Заблокирован */);
		m_wndToolBar.CleanUpLockedImages();
		m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_PROPERTIES_HC : IDR_PROPERTIES, 0, 0, TRUE /* Заблокирован */);

		m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
		m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
		m_wndToolBar.SetOwner(this);

		m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

		AdjustLayout();
		return 0;
	}

	void CPropertiesWnd::OnSize(UINT nType, int cx, int cy)
	{
		
		CDockablePane::OnSize(nType, cx, cy);
		AdjustLayout();
	}

	void CPropertiesWnd::OnExpandAllProperties()
	{
		
		m_wndPropList.ExpandAll();
	}

	void CPropertiesWnd::OnUpdateExpandAllProperties(CCmdUI* /* pCmdUI */)
	{
		
	}

	void CPropertiesWnd::OnSortProperties()
	{
		
		m_wndPropList.SetAlphabeticMode(!m_wndPropList.IsAlphabeticMode());
	}

	void CPropertiesWnd::OnUpdateSortProperties(CCmdUI* pCmdUI)
	{
		
		pCmdUI->SetCheck(m_wndPropList.IsAlphabeticMode());
	}

	void CPropertiesWnd::OnProperties1()
	{
		
	}

	void CPropertiesWnd::OnUpdateProperties1(CCmdUI* /*pCmdUI*/)
	{
		
	}

	void CPropertiesWnd::OnProperties2()
	{
		
	}

	void CPropertiesWnd::OnUpdateProperties2(CCmdUI* /*pCmdUI*/)
	{
		
	}

	void CPropertiesWnd::InitPropList()
	{
		
		SetPropListFont();

		m_wndPropList.EnableHeaderCtrl(FALSE);
		m_wndPropList.EnableDescriptionArea();
		m_wndPropList.SetVSDotNetLook();
		m_wndPropList.MarkModifiedProperties();
	}

	void CPropertiesWnd::OnSetFocus(CWnd* pOldWnd)
	{
		
		CDockablePane::OnSetFocus(pOldWnd);
		m_wndPropList.SetFocus();
	}

	void CPropertiesWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
	{
		
		CDockablePane::OnSettingChange(uFlags, lpszSection);
		SetPropListFont();
	}

	void CPropertiesWnd::SetPropListFont()
	{
		
		::DeleteObject(m_fntPropList.Detach());

		LOGFONT lf;
		afxGlobalData.fontRegular.GetLogFont(&lf);

		NONCLIENTMETRICS info;
		info.cbSize = sizeof(info);

		afxGlobalData.GetNonClientMetrics(info);

		lf.lfHeight = info.lfMenuFont.lfHeight;
		lf.lfWeight = info.lfMenuFont.lfWeight;
		lf.lfItalic = info.lfMenuFont.lfItalic;

		m_fntPropList.CreateFontIndirect(&lf);

		m_wndPropList.SetFont(&m_fntPropList);
		m_wndObjectCombo.SetFont(&m_fntPropList);
	}



	//get string value
	char* CPropertiesWnd::GetString(COleVariant v)
	{
		
		return _com_util::ConvertBSTRToString(v.bstrVal);
	}

	//get bool value
	short CPropertiesWnd::GetBool(COleVariant v)
	{
		
		if (v.boolVal == -1)
			return 1;
		if (v.boolVal == 0)
			return 0;
	}

	LRESULT CPropertiesWnd::OnPropertyChanged(WPARAM wparam, LPARAM lparam)
	{
		
		CMFCPropertyGridProperty* pProp = (CMFCPropertyGridProperty*)lparam;
		CString ele = pProp->GetName();
		COleVariant value = pProp->GetValue();
		CMFCPropertyGridColorProperty* color = (CMFCPropertyGridColorProperty*)pProp;
		zCPropEle* pEle = propList.SearchElementByString(ele);

		switch (pEle->GetPropType())
		{
		case PETbool:
			pEle->SetByInt(GetBool(value));
			pProp->SetValue(value);
			break;
		case PETenum:
			pEle->SetByEnum(GetString(value));
			pProp->SetValue(value);
			break;
		case PETint:
			pEle->SetByInt(value.intVal);
			pProp->SetValue(value);
			break;
		case PETfloat:
			pEle->SetByStr(CString(zSTRING(value.fltVal).ToChar()));
			pProp->SetValue(value);
			break;
		case PETstring:
			pEle->SetByStr(GetString(value));
			pProp->SetValue(value);
			break;
		case PETvec3:
			pEle->SetByVec(String2Vec3(GetString(value)));
			pProp->SetValue(value);
			break;
		case PETcolor:

			pEle->SetByColor(color->GetColor());

			break;
		case PETraw:
			pEle->SetByStr(GetString(value));
			pProp->SetValue(value);
			break;
		}

		zSTRING propString;
		GetProperties(propString);
		zCBuffer buf(propString.ToChar(), propString.Length());
		arch = zarcFactory->CreateArchiverRead(&buf);
		arch->SetStringEOL(zSTRING("\n"));
		arch->ReadObject(vob);
		arch->Close();
		zRELEASE(arch);
		SAFE_DELETE(arch);
		return 0;
	}

	void CPropertiesWnd::GetProperties(zSTRING& propString)
	{
		
		propString.Clear();
		for (uint z = 0; z < propList.GetNum(); z++)
		{
			propString += zSTRING(propList[z]).ToChar();
			propString += zSTRING("\n").ToChar();
		}
	};

	void CPropertiesWnd::SetProperties(zSTRING& propString)
	{
		
		propList.Clear();
		propList.DeleteData();

		int z = 1;
		zSTRING w = propString.PickWord(z, "\n\t", "\n\t");
		while (w.Search("[") == -1)
		{
			zCPropEle* ele = new zCPropEle(w.ToChar());
			propList.Insert(ele);
			z++;
			w = propString.PickWord(z, "\n\t", "\n\t");
		}


		zCPropEle* ele = new zCPropEle("");
		propList.Insert(ele);

		startPos = propList.GetNum();

		ele = new zCPropEle(w.ToChar());
		propList.Insert(ele);
		objTypeDescriptor = ele->GetName();
		while (!w.IsEmpty())
		{
			z++;
			w = propString.PickWord(z, "\n\t", "\n\t");
			zCPropEle* ele = new zCPropEle(w.ToChar());
			propList.Insert(ele);

		}


		CString name;
		TPropEditType type;
		CMFCPropertyGridProperty* grp = 0;
		CMFCPropertyGridProperty* curGrp = 0;
		uArray< CMFCPropertyGridProperty*> groupsHieararchi;
		bool classInitializeStart = false;


		for (size_t i = 0; i < propList.GetNum(); i++)
		{
			name = propList[i]->GetName();
			type = propList[i]->GetPropType();
			if (name.IsEmpty())
				continue;


			int entries = 0;
			if (type == PETclass && curGrp != 0)
			{
				entries = 1;
				for (uint g = i + 1; g < propList.GetNum(); g++)
				{
					if (propList[g]->GetPropType() == PETgroup || propList[g]->GetPropType() == PETclass)
					{
						entries++;
					}
					else if (propList[g]->GetPropType() == PETchunkend)
					{
						entries--;
						if (entries <= 0)
						{
							i = g;
							goto nextFrame;
						}
					}
				}
			}

			if (type == PETgroup || (type == PETclass && !curGrp))
			{

				grp = new CMFCPropertyGridProperty(name);

				grp->Expand(FALSE);
				if (curGrp)
					curGrp->AddSubItem(grp);
				else
					m_wndPropList.AddProperty(grp);
				curGrp = grp;
				curGrp->Expand(FALSE);
				groupsHieararchi.Insert(grp);
			}
			else if (type == PETchunkend)
			{
				groupsHieararchi.Remove(curGrp);
				curGrp = NULL;
				if (groupsHieararchi.GetNum() > 0)
					curGrp = groupsHieararchi.GetLast();
			}
			else if (type == PETenum)
			{
				if (curGrp)
					curGrp->AddSubItem(propList[i]->GetEnumList());
			}
			else if (type == PETcolor)
			{
				if (curGrp)
					curGrp->AddSubItem(AddColor(name, propList[i]->GetValue(), NULL, _T("")));
			}
			else
			{
				if (curGrp)
					curGrp->AddSubItem(AddProp(name, propList[i]->GetValue(), _T("")));
			}
		nextFrame:
			continue;
		}
		m_wndPropList.ShowScrollBar(SB_VERT);
		m_wndPropList.ShowScrollBar(SB_VERT, FALSE);
		//m_wndPropList.SetCustomColors(RGB(228, 243, 254), RGB(46, 70, 165), RGB(200, 236, 209), RGB(33, 102, 49), RGB(255, 229, 216), RGB(128, 0, 0), RGB(159, 159, 255));
		m_wndPropList.RedrawWindow();
		
	}

	void CPropertiesWnd::SetObject(zCVob* object)
	{
		
		m_wndPropList.RemoveAll();
		m_wndPropList.RedrawWindow();
		vob = object;

		if (object == Null) return;

	
		arch = zarcFactory->CreateArchiverWrite(zARC_MODE_ASCII_PROPS);
		arch->SetStringEOL(zSTRING("\n"));
		arch->WriteObject(object);

		zSTRING arcString;
		arch->GetBufferString(arcString);

		SetProperties(arcString);

		arch->Close();
		zRELEASE(arch);
	}


