// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {

	bool bMeshLibLoaded = false;
#ifdef __G1
	bool& logStat = *(bool*)0x0085EB00;
#endif
	
	HOOK Hook_oCGame_LoadGame AS(&oCGame::LoadGame, &oCGame::LoadGame_Union);

	IMPLEMENT_DYNCREATE(CEditorDoc, CDocument)

	BEGIN_MESSAGE_MAP(CEditorDoc, CDocument)
		ON_COMMAND(ID_PARSEMSH, &CEditorDoc::OnParsemsh)
		ON_UPDATE_COMMAND_UI(ID_FILE_NEW, &CEditorDoc::OnUpdateFileNew)
		ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, &CEditorDoc::OnUpdateFileSaveAs)
		ON_COMMAND(ID_FILE_NEW, &CEditorDoc::OnFileNew)
		ON_COMMAND(ID_TEST, &CEditorDoc::OnTest)
	END_MESSAGE_MAP()
	CEditorDoc* CEditorDoc::doc = Null;
	CEditorDoc::CEditorDoc() 
	{
		bWorldLoaded = FALSE;
		doc			= this;
		bChilds		= TRUE;
		bRemoveAsk	= TRUE;
	}

	CEditorDoc::~CEditorDoc()
	{
		
	}

	BOOL CEditorDoc::OnNewDocument()
	{
		if (!CDocument::OnNewDocument())
			return FALSE;
		return TRUE;
	}

	void CEditorDoc::Serialize(CArchive& ar)
	{
		if (ar.IsStoring())
		{
		}
		else
		{
		}
	}

	void CEditorDoc::VobTreeCorrX(zCTree<zCVob>* node, zCArray<TFlagMem>& flagMem)
	{
		flagMem.Insert(TFlagMem(node->GetData()));
		zCTree<zCVob>* tree = node->GetFirstChild();
		while (tree)
		{
			VobTreeCorrX(tree, flagMem);
			tree = tree->GetNextChild();
		}
	}

	void CEditorDoc::VobTreeCorrection(CString& matName)
	{
		if (!ogame->world) return;

		zCVob* transVob = ogame->world->SearchVobByName("SPACER_LCMOVE_VOB");

		zCTree<zCVob>* tree = ogame->world->globalVobTree.GetFirstChild();
		while (tree)
		{
			zCVobLevelCompo* lev = dynamic_cast<zCVobLevelCompo*>(tree->GetData());
			if (lev) {

				zCMesh* levMesh = dynamic_cast<zCMesh*>(lev->GetVisual());
				if (levMesh == 0) { tree = tree->GetNextChild(); continue; }

				int z = 0;
				BOOL found = FALSE;
				zCPolygon* lpoly = 0;
				while (z < levMesh->numPoly && !found)
				{
					lpoly = levMesh->Poly(z);
					found = (lpoly->GetMaterial()->GetName() == zSTRING(matName.GetBuffer(0)));
					z++;
				}

				zMAT4 polyMat, vobMat;
				if (found) {
					polyMat = CalcMatrixFromPoly(lpoly);
					vobMat = CalcMatrixFromVob(transVob);
				}
				if (found && !transVob)
				{
					transVob = new zCVob();
					transVob->SetVobName("SPACER_LCMOVE_VOB");
					ogame->world->AddVob(transVob);
					transVob->SetTrafoObjToWorld(polyMat);
					zRELEASE(transVob);
					found = false;
				}
				zMAT4 transMat;
				if (found && CalcMatrixDifference(vobMat, polyMat, transMat))
				{
					zCArray<TFlagMem> flagMem;
					zCTree<zCVob>* vobTree = tree->GetFirstChild();
					while (vobTree)
					{
						VobTreeCorrX(vobTree, flagMem);
						zCVob* vobi = vobTree->GetData();
						vobi->SetTrafoObjToWorld(transMat * vobi->trafoObjToWorld);
						vobTree = vobTree->GetNextChild();
					}
					for (int c = 0; c < flagMem.GetNumInList(); c++) flagMem[c].Put();
					transVob->SetTrafoObjToWorld(polyMat);
				} 
			} 
			tree = tree->GetNextChild();
		} 
	} 

	void CEditorDoc::HandleWorldAfterLoad()
	{
#ifdef __G1A
		zCVob::s_showHelperVisuals = TRUE;
#else
		zCVob::SetShowHelperVisuals(TRUE);
#endif// _G1A
		VobTreeCorrection(CString("TRA_HE_POLY"));
		oCNpc::SetNpcAIDisabled(TRUE);
		oCNpc::player = (oCNpc*)ogame->GetGameWorld()->CreateVob(zVOB_TYPE_NSC, parser->GetIndex("PC_HERO"));
		player->dontWriteIntoArchive = true;
		player->SetSleeping(TRUE);
		player->SetPhysicsEnabled(FALSE);
		ogame->GetSpawnManager()->SetSpawningEnabled(FALSE);
	}

	void oCGame::LoadGame_Union(int slot, const zSTRING& name)
	{
		VirtualVobTree_Portals::GetInstance().DeleteChilds();
		VirtualVobTree_Globals::GetInstance().DeleteChilds();
#ifdef __G1
		logStat = false;
#endif
		Log("::Loading ZEN file...");
		if (slot == 0 || slot == 1)
		{
			zoptions->ChangeDir(DIR_WORLD);

			CMainFrame::mainframe->ShowProgressBar("Loading world " + CString(name.ToChar()), TRUE);
			CProgressBar* progressBar = 0;
			progressBar = new CProgressBar();
			progressBar->Init(CMainFrame::mainframe->m_progressBar);
			ogame->world->SetProgressBar(progressBar);
			Log("::Progress bar initialised");
		}

		switch (slot)
		{
		case LOAD_3DS:
			zoptions->ChangeDir(DIR_MESHES);
			ogame->world->LoadWorld(name, zCWorld::zWLD_LOAD_EDITOR_UNCOMPILED);
			break;
		case LOAD_ZEN:
			zoptions->ChangeDir(DIR_WORLD);
			Log("::Load World ZEN");
			ogame->world->LoadWorld(name, zCWorld::zWLD_LOAD_EDITOR_COMPILED);
			Log("::Load world compleate!");
			break;
		case LOAD_ZEN_UC:
			zoptions->ChangeDir(DIR_WORLD);
			ogame->world->LoadWorld(name, zCWorld::zWLD_LOAD_EDITOR_UNCOMPILED);
			break;
		}

		if (slot == 0 || slot == 1)
		{
			Log("::Handling World after load");
			CEditorDoc::doc->HandleWorldAfterLoad();
			CMainFrame::mainframe->m_wndVobList.BuildVobTree();
		}
		Log("::Initialisation camera");
		CEditorDoc::doc->InitCamera(zVEC3(0, 0, 0));
		Log("::World handleCamera ready!");
		if (slot == 0 || slot == 1)
		{
			Log("::Remove progressbar");
			ogame->world->SetProgressBar(0);
			delete progressBar; progressBar = 0;
			Log("progress bar rmoved");
			
			CEditorView::view->world = ogame->world;
		}


		ControllerEvents.GizmoScale = 1;
		ControllerEvents.PickMode = zPM_VOBS;
		ogame->GetWorldTimer()->SetTime(12, 00);
		zCVob::s_showHelperVisuals = TRUE;
		GetCombo(ID_GIZMO_HEADING)->SelectItem((int)ControllerEvents.MotionHeading);
		
		BuildItemList();
		Log("oCGame::LoadGame_Union::MeshLib::	BuildItemList();");
		SaveTumbsByMask(MSH);
		Log("oCGame::LoadGame_Union::MeshLib::	SaveTumbsByMask(MSH);");
		SaveTumbsByMask(ASC);
		Log("oCGame::LoadGame_Union::MeshLib::	SaveTumbsByMask(ASC);");
		SaveTumbsByMask(MDS);
		Log("oCGame::LoadGame_Union::MeshLib::	SaveTumbsByMask(MDS);");
		SaveTumbsByMask(MMS);
		Log("oCGame::LoadGame_Union::MeshLib::	SaveTumbsByMask(MMS);");
		if (!bMeshLibLoaded)
		{
			Log("oCGame::LoadGame_Union::MeshLib::BuildItemList();");
			BuildItemList();
			Log("oCGame::LoadGame_Union::MeshLib::	BuildItemList();");
			SaveTumbsByMask(MSH);
			Log("oCGame::LoadGame_Union::MeshLib::	SaveTumbsByMask(MSH);");
			SaveTumbsByMask(ASC);
			Log("oCGame::LoadGame_Union::MeshLib::	SaveTumbsByMask(ASC);");
			SaveTumbsByMask(MDS);
			Log("oCGame::LoadGame_Union::MeshLib::	SaveTumbsByMask(MDS);");
			SaveTumbsByMask(MMS);
			Log("oCGame::LoadGame_Union::MeshLib::	SaveTumbsByMask(MMS);");

			CMainFrame::mainframe->m_wndMeshLib.mode = (ELibMode)theApp.GetProfileInt(settings, _meshLib, MSH);
			Log("oCGame::LoadGame_Union::MeshLib::CMainFrame::mainframe->m_wndMeshLib.mode");
			switch (CMainFrame::mainframe->m_wndMeshLib.mode)
			{
			case MSH:CMainFrame::mainframe->m_wndMeshLib.On3ds();
				Log("oCGame::LoadGame_Union::MeshLib::On3ds();");
				break;
			case ASC:CMainFrame::mainframe->m_wndMeshLib.OnAsc();
				Log("oCGame::LoadGame_Union::MeshLib::OnAsc()");
				break;
			case MDS:CMainFrame::mainframe->m_wndMeshLib.OnMds();
				Log("oCGame::LoadGame_Union::MeshLib::OnMds()");
				break;
			case MMS:CMainFrame::mainframe->m_wndMeshLib.OnMms();
				Log("oCGame::LoadGame_Union::MeshLib::OnMms()");
				break;
			case ITEM:CMainFrame::mainframe->m_wndMeshLib.OnItems();
				Log("oCGame::LoadGame_Union::MeshLib::OnItems()");
				break;
			}

			bMeshLibLoaded = true;
		}

		if (slot == 0 || slot == 1)
		{
			ogame->game_showwaynet = TRUE;
			CMainFrame::mainframe->ShowProgressBar("Done", FALSE);
			Log("progress bar rmoved 2");
			ogame->world->wayNet->CreateVobDependencies(ogame->world);
			Log("Create way net!");
		}
		CEditorDoc::doc->bWorldLoaded = TRUE;
	}

	void CEditorDoc::InitCamera(zVEC3 pos)
	{
		
		if (ogame->GetCamera() && ogame->GetCamera()->connectedVob)
			ogame->world->RemoveVob(ogame->GetCamera()->connectedVob);

		ogame->CamInit();

		ogame->GetCamera()->connectedVob->SetVobName("PIDOR_CAMERA_VOB");
		ogame->GetCamera()->connectedVob->SetAI(0);
		ogame->GetCamera()->connectedVob->SetPositionWorld(pos);
		ogame->EnvironmentInit();
		theApp.EnableDrawing();
	}

	zCVob* CEditorDoc::GetParentVob(zCVob* candidate)
	{
		
		if (candidate) return candidate;

		zCTree<zCVob>* tree = ogame->world->globalVobTree.GetFirstChild();
		while (tree)
		{
			if (tree->GetData() && tree->GetData()->GetVobType() == zVOB_TYPE_LEVEL_COMPONENT)
				return tree->GetData();
			tree = tree->GetNextChild();
		}
		return 0;
	}

	BOOL CEditorDoc::OnOpenDocument(LPCTSTR lpszPathName)
	{

		if (!CDocument::OnOpenDocument(lpszPathName))
			return FALSE;
		ogame->LoadGame(0, lpszPathName);
		return TRUE;
	}

	bool CEditorDoc::InsertVob(zCVob* vob, zVEC3* pos, zVEC3* dir, zCVob* parent)
	{

		zBOOL				phys = vob->GetPhysicsEnabled();
		zTVobSleepingMode	mode = vob->GetSleepingMode();

		parent = GetParentVob(parent);

		if (!bChilds)
			ogame->GetGameWorld()->AddVobAsChild1(vob, parent);
		else
			ogame->GetGameWorld()->AddVobAsChild1(vob, GetParentVob());

		if (dynamic_cast<zCTrigger*>(vob) || !dynamic_cast<zCZone*>(vob))
			dir = 0;

		if (pos)
		{
			zBOOL cdDyn = vob->GetCollDetDyn();
			zBOOL cdStat = vob->GetCollDetStat();

			vob->SetCollDetDyn(FALSE);
			vob->SetCollDetStat(FALSE);

			vob->SetPositionWorld(*pos);
			if (dir) vob->SetHeadingWorld((*pos) + (*dir) * 200);

			vob->SetCollDetDyn(cdDyn);
			vob->SetCollDetStat(cdStat);
		}

		vob->ResetXZRotationsWorld();

		if (
			(vob->GetCharacterClass() != zCVob::zVOB_CHAR_CLASS_PC) &&
			(vob->GetCharacterClass() != zCVob::zVOB_CHAR_CLASS_NPC)
			)
		{
			vob->SetPhysicsEnabled(phys);
			vob->SetSleepingMode(mode);
		};


		if (dynamic_cast<zCVobWaypoint*>(vob))
		{
			zCWayNet* waynet = ogame->world->GetWayNet();
			if (waynet)
			{
				zCWaypoint* wp = zfactory->CreateWaypoint();
				wp->Init(dynamic_cast<zCVobWaypoint*>(vob));
				waynet->InsertWaypoint(wp);
				waynet->CorrectHeight();
				zCVob* cam = ogame->GetCameraVob();

				zVEC3 vecSrc = _camera->GetAtVectorWorld();
				zVEC3 vecDst = zVEC3(vecSrc[VX], 0.0f, vecSrc[VZ]).Normalize();

				vob->SetHeadingAtWorld(vecDst);
			}
		}
		return false;
	}

	void CEditorDoc::RemoveVob(zCVob* vob)
	{
		CString ask;

		int result;

		if (vob == ogame->GetCameraVob()) return;

		if (dynamic_cast<zCVobLevelCompo*>(vob))
		{
			ask = "ATTENTION! This is a level-component!\r\nDo you really want to delete it?";
			result = AfxMessageBox(ask, MB_YESNO);
			if (result == IDYES)
			{
				ask = "Hey, really?\r\nTHIS IS A LEVEL-COMPONENT!\r\nDo you wanna delete it?\r\nAre you sure?";
				result = AfxMessageBox(ask, MB_YESNO);
			};
		}
		else if (bRemoveAsk)
		{
			zSTRING vobName;
			vobName = vob->GetVobName();
			if (vobName.IsEmpty()) vobName = vob->GetClassName_();
			ask = CString("Do you really want to remove\r\nthe item ") + CString(vobName.ToChar()) + CString(" ?");
			result = AfxMessageBox(ask, MB_YESNO);
		}
		else result = IDYES;

		if (result != IDYES) return;

		if (vob) vob->AddRef();

		zCVobWaypoint* wpvob = dynamic_cast<zCVobWaypoint*>(vob);
		if (wpvob)
		{
			zCWayNet* waynet = ogame->world->GetWayNet();
			zCWaypoint* wp = waynet->SearchWaypoint(wpvob);
			if (!wp) return;
			waynet->DeleteWaypoint(wp);
		}
		else ogame->world->RemoveVobSubtree(vob);
	}

	void CEditorDoc::OnParsemsh()
	{
		RenderItems();
		ParseMeshThumbs();
	}

	BOOL CEditorDoc::OnSaveDocument(LPCTSTR lpszPathName)
	{
		if (ogame->GetCamera()->connectedVob)
			ogame->world->RemoveVob(ogame->GetCamera()->connectedVob);
		
		CMainFrame::mainframe->ShowProgressBar("Saving world " + CString(lpszPathName), TRUE);
		CProgressBar* progressBar = 0;
		progressBar = new CProgressBar();
		progressBar->Init(CMainFrame::mainframe->m_progressBar);
		ogame->world->SetProgressBar(progressBar);

		ogame->SaveWorld(lpszPathName, zCWorld::zWLD_SAVE_EDITOR_COMPILED, false, true); // noMesh, Binary

		CMainFrame::mainframe->ShowProgressBar("Done", FALSE);
		if (ogame->GetCamera()->connectedVob)
			ogame->world->AddVob(ogame->GetCamera()->connectedVob);
		return TRUE;
	}

	void CEditorDoc::OnUpdateFileNew(CCmdUI* pCmdUI)
	{
		if (!CEditorView::view->world)
			pCmdUI->Enable(FALSE);
		else
			pCmdUI->Enable(TRUE);
	}

	void CEditorDoc::OnUpdateFileSaveAs(CCmdUI* pCmdUI)
	{
		if (!CEditorView::view->world)
			pCmdUI->Enable(FALSE);
		else
			pCmdUI->Enable(TRUE);
	}

	void CEditorDoc::OnFileNew()
	{
		if (CEditorView::view->world)
		{
			bWorldLoaded = FALSE;
			OutputBuild("oCWorld::Disposing world...");
			if (ogame->world) ogame->GetGameWorld()->DisposeWorld();
			CEditorView::view->world = NULL;
			CEditorView::view->RedrawWindow();


			CMainFrame::mainframe->m_wndVobList.m_wndVobList.SetRedraw(FALSE);
			CMainFrame::mainframe->m_wndVobList.m_wndVobList.DeleteAllItems();
			CMainFrame::mainframe->m_wndVobList.m_wndVobList.SetRedraw(TRUE);
			CMainFrame::mainframe->m_wndVobList.m_wndVobList.Invalidate(FALSE);
			OutputBuild("oCWorld::Ready to load world!");

			OnNewDocument();
			SetTitle("");
			CEditorView::view->world = Null;
		}
	}

	void CEditorDoc::OnTest()
	{
		MessageBox(0, "TEST TEST TEST", 0, 0);
	}
}














