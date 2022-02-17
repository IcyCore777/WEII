// Supported with union (c) 2020 Union team
// Union SOURCE file


/*
ÔÈÊÑ ÍÀÕÓÉ ÅÁÀÒÜ ÓÓÓ ÑÓÊÀ ÁËßÄÜ ÏÈÇÄÅÖ ÓÓÓ ÁËÄ ÑÓÊÀÀÀÀÛôàûâûàâûàâ

 bool TraceRay( const zCLine2D& line, zVEC2* intersec = Null ) const
	{
		zCLine2D line1 = *this;
		zCLine2D line2 = line;

		float angle = line1.GetAngle();
		line1.Rotate(-angle);
		line2.Rotate(-angle, line1[VA]);

		float vx_mid = line1[VA][VX];
		float vx_min = min(line2[VA][VX], line2[VB][VX]);
		float vx_max = max(line2[VA][VX], line2[VB][VX]);

		if (vx_mid >= vx_min && vx_mid <= vx_max) {
			float vx_length = vx_max - vx_min;
			float vx_collide = vx_mid - vx_min;
			float vx_multiplier = SafeDiv(1.0f, vx_length) * vx_collide;
			zCLine2D line3 = line2.Dot(vx_multiplier);

			float vy_mid = line3[VB][VY];
			float vy_min = min(line1[VA][VY], line1[VB][VY]);
			float vy_max = max(line1[VA][VY], line1[VB][VY]);

			if (vy_mid >= vy_min && vy_mid <= vy_max) {
				if (intersec)
					*intersec = line[VA] + line.GetVector() * vx_multiplier;

				return true;
			}
		}

		return false;
	}


*/



namespace GOTHIC_ENGINE {


#ifdef __G2A
	const char* SPC_APPLICATION_NAME = "Union Based World Editor for Gothic 2 Addon";
#endif
#ifdef __G1A
	const char* SPC_APPLICATION_NAME = "Union Based World Editor for Gothic 1 Addon";
#endif
#ifdef __G1
	const char* SPC_APPLICATION_NAME = "Union Based World Editor for Gothic";
#endif

	const char* SPC_VERSION = "ver 0.666";
	const char* SPC_APPLICATION_DESCRIPTION = "The Editor for the ZenGin";
	const char* SPC_COMPANY_NAME = "300$";
	const char* SPC_AUTHOR_NAMES = "Gratt, Saturas & Bratishka";
	const char* SPC_YEAR_START = "2021";
	const char* SPC_YEAR_END = "2021";
	const char* SPC_COMPILE_DATE = __DATE__;
	const char* SPC_COMPILE_TIME = __TIME__;




	CEditorView* CEditorView::view = 0;

	IMPLEMENT_DYNCREATE(CEditorView, CView)

	BEGIN_MESSAGE_MAP(CEditorView, CView)
		ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
		ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
		ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CEditorView::OnFilePrintPreview)
		ON_WM_CONTEXTMENU()
		ON_WM_RBUTTONUP()
		ON_WM_LBUTTONDOWN()
		ON_WM_LBUTTONDBLCLK()
		ON_COMMAND(ID_OBJ_INSERT, &CEditorView::OnObjInsert)
	END_MESSAGE_MAP()

	CEditorView::CEditorView() 
	{
		isReady = FALSE;
		view = this;
		world = Null;
	}

	CEditorView::~CEditorView()
	{
		
	}

	BOOL CEditorView::PreCreateWindow(CREATESTRUCT& cs)
	{
		return CView::PreCreateWindow(cs);
	}

	void CEditorView::OnNewpreset()
	{
		zCVob* pickedVob = GetSelectedVob();
		if (!pickedVob) return;

		zCObjPresetLib* lib = &CMainFrame::mainframe->presetLib;

		zCClassDef* classDef = pickedVob->GetClassDef();
		if (classDef->IsScriptedClass())
		{
			MessageBox("You can't create presets of scripted objects");
			return;
		};
		if (classDef->IsAbstractClass())
		{
			MessageBox("Fatal.Fatal.Fatal\r\nHow does this object of an abstract class could exist?");
			zERR_FATAL("B: SPC: Object of abstract class found. What went wrong?");
			return;
		};

		CString className = classDef->GetClassName_().ToChar();
		zSTRING presetName;
		bool ready = false;
		int result;
		zCObject* obj;
		CInputDialog dlg;
		dlg.name = "Type name of new preset\r\nclass \"" + className + "\"";
		dlg.ctrl_input = pickedVob->GetVobName().ToChar();
		do
		{
			result = dlg.DoModal();
			if (result == IDOK)
			{
				presetName = dlg.GetInput().GetBuffer(0);
				obj = lib->GetObjectByName(presetName);
				ready = (obj == NULL);
				dlg.name = "Type another name! " + dlg.GetInput() + " is already in use.\r\nclass \"" + className + "\"";
			}
			else ready = true;

		} while (!ready);

		if (result == IDOK)
		{
			lib->AddPreset(pickedVob, presetName);
			CMainFrame::mainframe->m_wndPresetLib.LoadPreset(pickedVob->GetClassDef()->className.ToChar());
		}
	}



	void CEditorView::OnDrawStartScreen(CDC* pDC)
	{
		RECT rect;
		CClientDC ClientDC(this);
		ClientDC.GetClipBox(&rect);
		ClientDC.FillSolidRect(&rect, RGB(0, 0, 0));

		CSize size = ClientDC.GetTextExtent("something");
		int height = size.cy;
		int line = 4;


		ClientDC.SetTextColor(RGB(255, 255, 255));
		ClientDC.TextOut(40, line * height, CString(SPC_APPLICATION_NAME)); line++;
		ClientDC.TextOut(40, line * height, CString(SPC_APPLICATION_DESCRIPTION)); line++;

		line += 2;


		ClientDC.SetTextColor(RGB(196, 250, 250));
		ClientDC.TextOut(40, line * height, "Copyright © " + CString(SPC_YEAR_START) + "-" + CString(SPC_YEAR_END) + " by " + CString(SPC_COMPANY_NAME)); line++;
		ClientDC.TextOut(40, line * height, "developed by " + CString(SPC_AUTHOR_NAMES)); line++;

		line += 2;


		ClientDC.SetTextColor(RGB(128, 128, 128));
		ClientDC.TextOut(40, line * height, "compiled: " + CString(SPC_COMPILE_DATE) + " " + CString(SPC_COMPILE_TIME)); line++;

		line += 2;
		CString renderVersion = "Direct3D";

		CString s; s.Format("%s %ix%i", renderVersion, viewy, viewx);

		ClientDC.SetTextColor(RGB(0, 175, 0));
		ClientDC.TextOut(40, line * height, s); line++;


		line += 2;

		ClientDC.SetTextColor(RGB(196, 196, 196));
		ClientDC.TextOut(40, line * height, CString("Load and merge 3DS-Files and compile")); line++;
		ClientDC.TextOut(40, line * height, CString("or load a ZEN-File.")); line++;
		theApp.EnableDrawing(false);
	}


	POINT  cur;
	void CEditorView::OnDraw(CDC* /*pDC*/)
	{
		ASSERT(AfxCheckMemory());

		if (gameMan && world && zrenderer && gameMan->gameSession)
		{
			zCView::GetInput();

			if (zrenderer && gameMan->gameSession)
			{
				GetCursorPos(&cur);
				gameMan->gameSession->Render();
				ControllerEvents.CursorPosition = PickCursor(cur);
				gameMan->gameSession->RenderBlit();

				screen->DrawItems();
			}
		}else
		if(isReady)
			OnDrawStartScreen(NULL);
		ASSERT(AfxCheckMemory());
	}

	void CEditorView::OnRButtonUp(UINT, CPoint point)
	{
		ClientToScreen(&point);
		if ((ControllerEvents.CameraMoving && zKeyPressed(KEY_LSHIFT)) || !ControllerEvents.CameraMoving)
		OnContextMenu(this, point);
	}

	void CEditorView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
	{
		theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
	}

	void CEditorView::RemoveObject(zCVob* vob)
	{
		
		if (vob == pickedWP)	pickedWP = NULL;
		if (vob == pickedWP2nd) pickedWP2nd = NULL;

		SelectObject(NULL);
		CEditorDoc::doc->RemoveVob(vob);
	}

	zCObject* CEditorView::CreateNewObject(CString className)
	{
		
		if (className.IsEmpty()) return Null;

		zCClassDef* classDef = zCObject::GetClassDef(className.GetBuffer(0));
		if (!classDef || classDef->IsAbstractClass() || oCNpc::classDef == classDef) return 0;
		zCObject* obj = 0;
		obj = zCObject::CreateNewInstance(zSTRING(className.GetBuffer(0)));
		return obj;
	}

	zCVob* CEditorView::CreateNewVob(zCObject* obj)
	{
		zCVob* newvob = 0;

		newvob = dynamic_cast<zCVob*>(obj);

		if (
			dynamic_cast<zCTrigger*>(newvob) ||
			dynamic_cast<zCMover*>(newvob) ||
			dynamic_cast<zCVobWaypoint*>(newvob) ||
			dynamic_cast<zCVobSpot*>(newvob)
			)
		{
			if (!newvob->GetVobName().IsEmpty())
				return newvob;
			CInputDialog dlg;
			dlg.name = "This vob of type " + CString(newvob->GetClassName_().ToChar()) + "\r\nmust have a VobName:";
			dlg.ctrl_input = "";

			zBOOL	ready = false;
			zSTRING	name = "";
			int		result = 0;
			while (!ready)
			{
				result = dlg.DoModal();
				if (result == IDOK)
				{
					zCArchiver* arch = zarcFactory->CreateArchiverWrite(zARC_MODE_ASCII_PROPS);
					name = dlg.GetInput().GetBuffer(0);
					ready = !name.IsEmpty() && arch->IsStringValid(name.ToChar());
					arch->Close(); zRELEASE(arch);

					if (!dynamic_cast<zCMover*>(newvob))
					{
						zCVob* vob = world->SearchVobByName(name);
						ready = ready && !vob;
					}
				}
				else
				{
					ready = true;
					name.Clear();
				}
			}

			if (!name.IsEmpty())
				newvob->SetVobName(name);
			else
				zRELEASE(newvob);
		};

		return newvob;
	}



	void CEditorView::VobGoto(zCVob* vob)
	{
		zVEC3 dir = vob->GetAtVectorWorld();
#ifdef __G1A
		zVEC3 pos = vob->GetPositionWorld();
#else
		zVEC3 pos;  vob->GetPositionWorld(pos);
#endif
		zVEC3 vob_pos = vob->GetPositionWorld();
		zVEC3 cam_vec = ogame->GetCameraVob()->GetAtVectorWorld();

		ogame->GetCameraVob()->SetPositionWorld(vob_pos - cam_vec * (float)500);
	}


#ifdef EXPORT_RIBBON
	typedef BOOL(WINAPI* CMDMSGPROC)(UINT,int,void*,AFX_CMDHANDLERINFO*);
#endif

	BOOL CEditorView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
	{
#ifdef EXPORT_RIBBON
		static HMODULE Controler = Null;
		static CMDMSGPROC CmdMsg = Null;
		for (int i = 0; i < UnionCore::CPlugin::GetPluginList().GetNumInList(); i++)
		{
			Controler = UnionCore::CPlugin::GetPluginList()[i].GetData()->GetModule();
			CmdMsg = (CMDMSGPROC)GetProcAddress(Controler, "?OnCmdMsg@Gothic_II_Addon@@YGHIHPAXPAUAFX_CMDHANDLERINFO@@@Z");
			if (CmdMsg)
				if (CmdMsg(nID, nCode, pExtra, pHandlerInfo))
					return TRUE;
		}
#endif
		
		if (settings.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
			return TRUE;
		if (homeSettings.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
			return TRUE;
		return CView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
	}


	void CEditorView::SelectObject(zCVob* vob)
	{
		if (!vob) {

			CMainFrame::mainframe->m_wndProperties.SetObject(NULL);
			ControllerEvents.PickedVob = NULL;
			return;
		};


		const zSTRING* sectorName = vob->GetSectorNameVobIsIn();

		if (sectorName)
			FILEVIEW.SelectItem(VirtualVobTree_Portals::GetInstance().FindTreeItemForVob(vob));
		else
			FILEVIEW.SelectItem(VirtualVobTree_Globals::GetInstance().FindTreeItemForVob(vob));

		CMainFrame::mainframe->m_wndProperties.SetObject(vob);
	}

	void CEditorView::OnLButtonDown(UINT nFlags, CPoint point)
	{
		if (!world) return;
		POINT  cur;
		GetCursorPos(&cur);
		ScreenToClient(&cur);

		PickSingle();

		zCVob* vob = world->traceRayReport.foundVob;

		if (vob)
		{
			ControllerEvents.PickedVob = vob;

			SelectObject(vob); //OK_OK
		}

		CView::OnLButtonDown(nFlags, point);
	}


	void CEditorView::OnLButtonDblClk(UINT nFlags, CPoint point)
	{
		SelectObject(Null);
		CView::OnLButtonDblClk(nFlags, point);
	}


	void CEditorView::OnObjInsert()
	{
		uString objTypeName = ClassTree.GetItemText(ClassTree.GetSelectedItem());

		bool enabled = (CMainFrame::mainframe->m_wndMeshLib.mode == ITEM) || (!objTypeName.IsEmpty() && objTypeName != "Engine objects" && objTypeName.Search("Virtual"));

		
		zCVob* parent = CEditorDoc::doc->GetParentVob(GetSelectedVob());

		zCVob* newvob = 0;
		uString visualName;
		zVEC3 pos = _camera->GetPositionWorld();
		zVEC3 dir = _camera->GetAtVectorWorld();

		if (CMainFrame::mainframe->m_wndMeshLib.mode == ITEM)
		{
			zSTRING itemInst = CMainFrame::mainframe->m_wndMeshLib.meshListBox.GetSelectedMesh();
			newvob = dynamic_cast<oCObjectFactory*>(zfactory)->CreateItem(parser->GetIndex(itemInst.PickWord(1, ".", ".")));
			goto ITEM;
		}

		if (CMainFrame::mainframe->m_wndPresetLib.meshListBox.mode == PRESETS)
		{
			Common::cmd << "PRESETS" << Common::endl;
			zCObject* obj = 0;
			zSTRING name_of_preset = CMainFrame::mainframe->m_wndPresetLib.meshListBox.GetPreset();

			zCObject* refObject = CMainFrame::mainframe->presetLib.GetObjectByName(name_of_preset);
			if (refObject)
				obj = refObject->CreateCopy();
			else
				return;
			newvob = dynamic_cast<zCVob*>(obj);
			if (newvob) newvob->SetVobPresetName(name_of_preset);
		}

		if (CMainFrame::mainframe->m_wndPresetLib.meshListBox.mode == MESHLIB)
		{
			Common::cmd << "MESHLIB" << Common::endl;
			if (CMainFrame::mainframe->m_wndMeshLib.meshListBox.GetCurSel() != LB_ERR)
				visualName = CMainFrame::mainframe->m_wndMeshLib.meshListBox.GetSelectedMesh().ToChar();
			else
				visualName = "INVALID_SOURCE_FILE.3DS";
			newvob = CreateNewVob(CreateNewObject(ClassTree.GetItemText(ClassTree.GetSelectedItem())));
		}
	ITEM:
		if (newvob)
		{
			pos = pos + dir * 200;

			CEditorDoc::doc->InsertVob(newvob, &pos, &dir, GetSelectedVob());
			//Log("3");
			//if (!pfxName.IsEmpty())
			//{
			//	zCParticleFX* pfx = new zCParticleFX();
			//	newvob->SetVisual(pfx);
			//	_ASSERT(pfx);
			//	pfx->SetAndStartEmitter(pfxName.PickWord(1,".","."));
			//	MessageBox(pfxName.PickWord(1, ".", ".").ToChar(), 0, 0);
			//}
		}
		//else if (mode == MODE_CAMERA) newvob = csCameraSettings.InsertVob(pos, dir, SelectedVob);
		if (!newvob) return;

		zVEC3 vecSrc = _camera->GetAtVectorWorld();
		zVEC3 vecDst = zVEC3(vecSrc[VX], 0.0f, vecSrc[VZ]).Normalize();

		newvob->SetHeadingAtWorld(vecDst);

		if (newvob && dynamic_cast<zCVob*>(newvob))
		{

			if (dynamic_cast<zCTriggerBase*>(newvob) || dynamic_cast<zCZone*>(newvob))
			{
				if (!newvob->visual)
					newvob->SetVisual(visualName.ToChar());
				CEditorView::view->SelectObject(newvob);//OK_**
				return;
			}


			if (dynamic_cast<zCVobWaypoint*>(newvob) ||
				dynamic_cast<zCVobSpot*>(newvob) ||
				dynamic_cast<zCVobStartpoint*>(newvob) ||
				dynamic_cast<oCItem*>(newvob) ||
				//			dynamic_cast<zCEffect*>(newvob) ||
				dynamic_cast<zCCamTrj_KeyFrame*>(newvob) ||
				dynamic_cast<zCCSCamera*>(newvob) ||
				dynamic_cast<zCVobLight*>(newvob))
			{
				CEditorView::view->SelectObject(newvob);//OK_**
				zRELEASE(newvob);

				return;
			}

			if (!newvob->visual)
				newvob->SetVisual(visualName.ToChar());
			CEditorView::view->SelectObject(newvob);//OK_**
		}
	}
}


