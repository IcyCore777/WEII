// Supported with union (c) 2020 Union team
// Union SOURCE file

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

	const char* SPC_VERSION = "ver 0.2";
	const char* SPC_APPLICATION_DESCRIPTION = "The Editor for the ZenGin";
	const char* SPC_COMPANY_NAME = "300$";
	const char* SPC_AUTHOR_NAMES = "Gratt, Saturas";
	const char* SPC_YEAR_START = "2021";
	const char* SPC_YEAR_END = "2022";
	const char* SPC_COMPILE_DATE = __DATE__;
	const char* SPC_COMPILE_TIME = __TIME__;

	const char* engineObjects = "Engine objects";
	const char* invalidSrcFile = "INVALID_SOURCE_FILE.3DS";

	CEditorView* CEditorView::view = 0;

	IMPLEMENT_DYNCREATE(CEditorView, CView)

	BEGIN_MESSAGE_MAP(CEditorView, CView)

		ON_WM_CONTEXTMENU()
		ON_WM_RBUTTONUP()
		ON_WM_LBUTTONDOWN()
		ON_WM_LBUTTONDBLCLK()
		ON_COMMAND(ID_OBJ_INSERT, &CEditorView::OnObjInsert)
	END_MESSAGE_MAP()

	CEditorView::CEditorView() 
	{
		isStartScreen	= FALSE;
		view			= this;
		world			= Null;
		pickedWP		= Null;
		pickedWP2nd		= Null;
	}

	CEditorView::~CEditorView()
	{
		
	}

	BOOL CEditorView::PreCreateWindow(CREATESTRUCT& cs)
	{
		return CView::PreCreateWindow(cs);
	}



	//-----------------------------------------------------------------
	// View operations
	//=================================================================
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
		if(isStartScreen)	OnDrawStartScreen(NULL);
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


	//-----------------------------------------------------------------
	// Object manipulations
	//=================================================================
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

			SelectObject(vob);
		}

		CView::OnLButtonDown(nFlags, point);
	}

	void CEditorView::OnLButtonDblClk(UINT nFlags, CPoint point)
	{
		SelectObject(Null);
		CView::OnLButtonDblClk(nFlags, point);
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
			dynamic_cast<zCTrigger*>	(newvob)||
			dynamic_cast<zCMover*>		(newvob)||
			dynamic_cast<zCVobWaypoint*>(newvob)||
			dynamic_cast<zCVobSpot*>	(newvob)
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
			PRESETLIB.LoadPreset(pickedVob->GetClassDef()->className.ToChar());
		}
	}

	void CEditorView::OnObjInsert()
	{
		uString objTypeName = ClassTree.GetItemText(ClassTree.GetSelectedItem());

		bool enabled = (MESHLIBDLG.mode == ITEM) || (!objTypeName.IsEmpty() && objTypeName != engineObjects && objTypeName.Search("Virtual"));


		zCVob* parent = CEditorDoc::doc->GetParentVob(GetSelectedVob());

		zCVob* newvob = 0;
		uString visualName;
		zVEC3 pos = _camera->GetPositionWorld();
		zVEC3 dir = _camera->GetAtVectorWorld();



		if (PRESETLIB.meshListBox.mode == PRESETS)
		{
			zCObject* obj = 0;
			zSTRING presetName = PRESETLIB.meshListBox.GetPreset();

			zCObject* refObject = CMainFrame::mainframe->presetLib.GetObjectByName(presetName);
			if (refObject)
				obj = refObject->CreateCopy();
			else
			{
				OutputError("Wrong preset name, please reselct it.");
				return;
			}

			newvob = dynamic_cast<zCVob*>(obj);
			if (newvob) newvob->SetVobPresetName(presetName);
		}

		if (PRESETLIB.meshListBox.mode == MESHLIB)
		{
			if (MESHLIBDLG.meshListBox.GetCurSel() != LB_ERR)
				visualName = MESHLIBDLG.meshListBox.GetSelectedMesh().ToChar();
			else
				visualName = invalidSrcFile;
			newvob = CreateNewVob(CreateNewObject(ClassTree.GetItemText(ClassTree.GetSelectedItem())));
		}

		if (MESHLIBDLG.mode == ITEM)
		{
			zSTRING itemInst = MESHLIBDLG.meshListBox.GetSelectedMesh();
			newvob = dynamic_cast<oCObjectFactory*>(zfactory)->CreateItem(parser->GetIndex(itemInst.PickWord(1, ".", ".")));
			if (!newvob) return;
		}

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
			CEditorView::view->SelectObject(newvob);

			if (dynamic_cast<zCTriggerBase*>(newvob) || dynamic_cast<zCZone*>(newvob))
			{
				if (!newvob->visual)
					newvob->SetVisual(visualName.ToChar());
				return;
			}


			if (dynamic_cast<zCVobWaypoint*>	(newvob) ||
				dynamic_cast<zCVobSpot*>		(newvob) ||
				dynamic_cast<zCVobStartpoint*>	(newvob) ||
				dynamic_cast<oCItem*>			(newvob) ||
				dynamic_cast<zCCamTrj_KeyFrame*>(newvob) ||
				dynamic_cast<zCCSCamera*>		(newvob) ||
				dynamic_cast<zCVobLight*>		(newvob))
			{
				zRELEASE(newvob);
				return;
			}

			if (!newvob->visual)
				newvob->SetVisual(visualName.ToChar());
		}
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

	void CEditorView::RemoveObject(zCVob* vob)
	{

		if (vob == pickedWP)	pickedWP = NULL;
		if (vob == pickedWP2nd) pickedWP2nd = NULL;

		SelectObject(NULL);
		CEditorDoc::doc->RemoveVob(vob);
	}



	//-----------------------------------------------------------------
	// Message handling & Export Ribbon interface
	//=================================================================
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
}


