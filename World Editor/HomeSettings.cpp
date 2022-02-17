// Supported with union (c) 2020 Union team
// Union SOURCE file

__declspec(dllimport) uint s_FpsLimit; //todo FPS lim
#pragma comment(lib, "zUnionUtils.lib")


namespace GOTHIC_ENGINE {
	BEGIN_MESSAGE_MAP(CHomeSettings, CCmdTarget)
		ON_COMMAND(ID_HOLDTIME, &CHomeSettings::OnHoldtime)
		ON_UPDATE_COMMAND_UI(ID_HOUR, &CHomeSettings::OnUpdateTime)
		ON_UPDATE_COMMAND_UI(ID_HOLDTIME, &CHomeSettings::OnUpdateHoldtime)
		ON_COMMAND(ID_EDIT_COPY, &CHomeSettings::OnCopy)
		ON_COMMAND(ID_EDIT_CUT, &CHomeSettings::OnCut)
		ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, &CHomeSettings::OnUpdateCut)
		ON_COMMAND(ID_EDIT_PASTE, &CHomeSettings::OnPaste)
		ON_COMMAND(ID_DELETE, &CHomeSettings::OnDelete)
		ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, &CHomeSettings::OnUpdatePaste)
		ON_COMMAND(ID_HOUR, &CHomeSettings::OnHour)
		ON_COMMAND(ID_MINUTE, &CHomeSettings::OnMinute)
		ON_COMMAND(ID_MOVE, &CHomeSettings::OnMove)
		ON_COMMAND(ID_ROTATE, &CHomeSettings::OnRotate)
		ON_UPDATE_COMMAND_UI(ID_MOVE, &CHomeSettings::OnUpdateMove)
		ON_UPDATE_COMMAND_UI(ID_ROTATE, &CHomeSettings::OnUpdateRotate)
		ON_COMMAND(ID_MOVSPD, &CHomeSettings::OnMovspd)
		ON_COMMAND(ID_ROTSPD, &CHomeSettings::OnRotspd)
		ON_COMMAND(ID_SMOOTH, &CHomeSettings::OnSmooth)
		ON_COMMAND(ID_GIZMO_HEADING, &CHomeSettings::OnGizmoHeading)
		ON_UPDATE_COMMAND_UI(ID_GIZMO_HEADING, &CHomeSettings::OnUpdateGizmoHeading)
		ON_UPDATE_COMMAND_UI(ID_SMOOTH, &CHomeSettings::OnUpdateSmooth)
		ON_UPDATE_COMMAND_UI(ID_ROTSPD, &CHomeSettings::OnUpdateRotspd)
		ON_UPDATE_COMMAND_UI(ID_MOVSPD, &CHomeSettings::OnUpdateMovspd)
		ON_COMMAND(ID_WINRESTORE, &CHomeSettings::OnWinrestore)
		ON_COMMAND(ID_INSERT, &CHomeSettings::OnInsert)
		ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, &CHomeSettings::OnUpdateCopy)
		ON_UPDATE_COMMAND_UI(ID_DELETE, &CHomeSettings::OnUpdateDelete)
		ON_UPDATE_COMMAND_UI(ID_INSERT, &CHomeSettings::OnUpdateInsert)
		ON_COMMAND(ID_FPS, &CHomeSettings::OnFps)
		ON_COMMAND(ID_FREECAM, &CHomeSettings::OnFreecam)
		ON_UPDATE_COMMAND_UI(ID_FREECAM, &CHomeSettings::OnUpdateFreecam)

		ON_COMMAND(ID_FINDVOBVIS, &CHomeSettings::OnFindvobvis)
		ON_COMMAND(ID_STOP, &CHomeSettings::OnStop)
		ON_UPDATE_COMMAND_UI(ID_FINDVOBVIS, &CHomeSettings::OnUpdateFindvobvis)
	END_MESSAGE_MAP()


	void FindVob(zSTRING str, zCVob* vob)
	{
		
		int idx = CMainFrame::mainframe->m_wndOutput.m_wndOutputFind.AddString(str.ToChar());
		CMainFrame::mainframe->m_wndOutput.m_wndOutputFind.SetTopIndex(idx);
		CMainFrame::mainframe->m_wndOutput.m_wndOutputFind.SetItemDataPtr(idx, vob);
	}



	CHomeSettings::CHomeSettings()
	{
		
		isCut = false;
		holdTime = false;
		copyBuffer = Null;
		objNum = 0;
		sliderMov = Null;
		sliderRot = Null;
		sliderSmooth = Null;
		sliderFps = Null;
		gizmoHeading = Null;
		hour = Null;
		minute = Null;
		modeMove = zMM_NONE;
	}

	void CHomeSettings::OnHoldtime()
	{
		
		holdTime = !holdTime;
	}

	bool CHomeSettings::UpdateTime(int& _hour, int& _min)
	{
		
		if (holdTime)
		{
			_hour = uString(hour->GetEditText()).ToInt32();
			_min = uString(minute->GetEditText()).ToInt32();

			return true;
		}
		else
		{
			if (_hour > 23) { _hour = 23; _min = 59; }
			if (_min > 59) _min = 59;
			char hr[10];
			sprintf(hr, "%i", _hour);
			char min[10];
			sprintf(min, "%i", _min);
			hour->SetEditText(hr);
			minute->SetEditText(min);
			return false;
		};
	}

	void CHomeSettings::InitData()
	{
		
		hour = GetEdit(ID_HOUR);
		minute = GetEdit(ID_MINUTE);
		sliderMov = GetSlider(ID_MOVSPD);
		sliderRot = GetSlider(ID_ROTSPD);
		sliderSmooth = GetSlider(ID_SMOOTH);
		sliderFps = GetSlider(ID_FPS);
		gizmoHeading = GetCombo(ID_GIZMO_HEADING);
	}

	void CHomeSettings::ClearCut()
	{
		
		for (uint32 i = 0; i < cuttedVobs.GetNum(); i++)
			cuttedVobs[i]->RestoreAlpha();
		cuttedVobs.Clear();
		isCut = false;
	}

	void CHomeSettings::OnUpdateTime(CCmdUI* pCmdUI)
	{
		
		UICheck
			static int hour = 12, minute = 0;
		if (ogame->GetWorldTimer())
		{
			ogame->GetWorldTimer()->GetTime(hour, minute);
			if (UpdateTime(hour, minute))
				ogame->GetWorldTimer()->SetTime(hour, minute);
		}
	}

	void CHomeSettings::OnUpdateHoldtime(CCmdUI* pCmdUI)
	{
		
		UICheck
			pCmdUI->SetCheck(holdTime);
	}

	void CHomeSettings::OnCopy()
	{
		
		ClearCut();
		Common::Array<zCVob*> vobs = ControllerEvents.SelectedVobs;
		delete copyBuffer;
		zCArchiver* arch = zarcFactory->CreateArchiverWrite(zARC_MODE_BINARY, TRUE);

		arch->WriteInt("count", vobs.GetNum()); //Типа считаем

		for (uint32 i = 0; i < vobs.GetNum(); i++)
		{
			if (vobs[i]->GetClassDef()->IsScriptedClass())
				continue;
			arch->WriteObject(vobs[i]);
		}

		copyBuffer = new zCBuffer(arch->GetBuffer()->GetSize());
		copyBuffer->Write(arch->GetBuffer()->GetBuffer(), arch->GetBuffer()->GetSize());
		copyBuffer->SetPosBegin();
		arch->Close();
		zRELEASE(arch);
	}

	void CHomeSettings::OnCut()
	{
		
		Common::Array<zCVob*> vobs = ControllerEvents.SelectedVobs;
		cuttedVobs.Clear();
		for (size_t i = 0; i < vobs.GetNum(); i++)
		{
			vobs[i]->SetVisualAlpha(0.5);
			vobs[i]->SetVisualAlphaEnabled(TRUE);
			vobs[i]->SetDrawBBox3D(FALSE);
			vobs[i]->SetCollDet(FALSE);
		}
		cuttedVobs += vobs;
		if (!isCut) isCut = true;

		ControllerEvents.ClearSelection();
	}

	void CHomeSettings::OnUpdateCut(CCmdUI* pCmdUI)
	{
		
		UICheck
			if ((BOOL)dynamic_cast<oCItem*>(GetSelectedVob()))
			{
				pCmdUI->Enable(FALSE); return;
			}


		CheckSel

			pCmdUI->SetCheck(isCut);
	}

	void CHomeSettings::OnPaste()
	{
		
		if (isCut)
		{
			for (size_t i = 0; i < cuttedVobs.GetNum(); i++)
			{
				zCVob* next = cuttedVobs[i];


				zVEC3 center;
				for (uint i = 0; i < cuttedVobs.GetNum(); i++)
					center += cuttedVobs[i]->GetPositionWorld();

				center /= cuttedVobs.GetNum();

				zVEC3 cameraOffset = _camera->GetAtVectorWorld() * 300.0f;
				zVEC3 movement = _camera->GetPositionWorld() - center;

				for (uint i = 0; i < cuttedVobs.GetNum(); i++)
				{
					cuttedVobs[i]->SetPositionWorld(cuttedVobs[i]->GetPositionWorld() + movement);
					cuttedVobs[i]->SetCollDet(TRUE);
				}
				next->RestoreAlpha();
			}
			cuttedVobs.Clear();
			isCut = false;
			return;
		}
		if (!copyBuffer) return;

		bool openProps = true;
		Common::Array<zCVob*> copiedVobs;
		zCArchiver* arch = zarcFactory->CreateArchiverRead(copyBuffer);
		objNum = arch->ReadInt("count");
		if (copyBuffer)
		{
			zCVob* newvob = 0;
			for (size_t i = 0; i < objNum; i++)
			{
				newvob = dynamic_cast<zCVob*>(arch->ReadObject(NULL));
				if (objNum < 2)
				{
					zBOOL cddyn;
					zBOOL cdstat;
					if (!newvob->GetClassDef()->IsScriptedClass())
					{
						cddyn = newvob->GetCollDetDyn();
						cdstat = newvob->GetCollDetStat();
					}

					zVEC3 pos = _camera->GetPositionWorld();
					zVEC3 dir = _camera->GetAtVectorWorld();
					pos = pos + dir * 200;

					CEditorDoc::doc->InsertVob(newvob, &pos, &dir, Null);
					if (!newvob->GetClassDef()->IsScriptedClass())
					{
						newvob->SetCollDetDyn(cddyn);
						newvob->SetCollDetStat(cdstat);
						if (openProps)
						{
							openProps = false;
							CEditorView::view->SelectObject(newvob); //OK_OK
						}
					}
					zRELEASE(newvob);
				}
				else
				{
					copiedVobs.Insert(newvob);
					CEditorDoc::doc->InsertVob(newvob, &newvob->GetPositionWorld(), &newvob->GetAtVectorWorld(), Null);
				}
			}
			if (objNum > 2)
			{
				zVEC3 center;
				for (uint i = 0; i < copiedVobs.GetNum(); i++)
					center += copiedVobs[i]->GetPositionWorld();

				center /= copiedVobs.GetNum();

				zVEC3 cameraOffset = _camera->GetAtVectorWorld() * 300.0f;
				zVEC3 movement = _camera->GetPositionWorld() - center;

				for (uint i = 0; i < copiedVobs.GetNum(); i++)
				{
					copiedVobs[i]->SetPositionWorld(copiedVobs[i]->GetPositionWorld() + movement);
					zRELEASE(copiedVobs[i]);
				}
			}
			arch->Close();
			zRELEASE(arch);
		}
	}

	void CHomeSettings::OnDelete()
	{
		if (!cuttedVobs.IsEmpty())cuttedVobs.Clear();

		Common::Array<zCVob*> selVobs = ControllerEvents.SelectedVobs;
		for (size_t i = 0; i < selVobs.GetNum(); i++)
			if (selVobs[i]) CEditorView::view->RemoveObject(selVobs[i]);

		ControllerEvents.ClearSelection();
	}


	void CHomeSettings::SetMoveMode(zEVobMotionMode newmode)
	{
		switch (newmode)
		{
		case zMM_ROTATE:	ControllerEvents.MotionMode = zMM_ROTATE;	ControllerEvents.GizmoScale = 4; 	break;
		case zMM_MOVE:		ControllerEvents.MotionMode = zMM_MOVE;		ControllerEvents.GizmoScale = 1; 	break;
		case zMM_SCALE:		ControllerEvents.MotionMode = zMM_SCALE;	break;
		case zMM_NONE:		ControllerEvents.MotionMode = zMM_NONE;		ControllerEvents.GizmoScale = 1; 	break;
		}

		modeMove = newmode;
	}


	void CHomeSettings::OnUpdatePaste(CCmdUI* pCmdUI)
	{
		UICheck
			pCmdUI->Enable((BOOL)copyBuffer || cuttedVobs.GetNum());
	}

	void CHomeSettings::OnMove()
	{
		
		if (modeMove != zMM_MOVE)
			SetMoveMode(zMM_MOVE);
		else
			SetMoveMode(zMM_NONE);
	}

	void CHomeSettings::OnRotate()
	{
		
		if (modeMove != zMM_ROTATE)
			SetMoveMode(zMM_ROTATE);
		else
			SetMoveMode(zMM_NONE);
	}

	void CHomeSettings::OnUpdateMove(CCmdUI* pCmdUI)
	{
		
		UICheck
			pCmdUI->Enable((BOOL)GetSelectedVob());
		pCmdUI->SetCheck(modeMove == zMM_MOVE);
	}

	void CHomeSettings::OnUpdateRotate(CCmdUI* pCmdUI)
	{
		
		UICheck
			pCmdUI->Enable((BOOL)GetSelectedVob());
		pCmdUI->SetCheck(modeMove == zMM_ROTATE);
	}


	void CHomeSettings::OnMovspd()
	{
		
		ControllerEvents.movingSpeed = sliderMov->GetPos();
		sliderMov->SetToolTipText(uString("Moving speed - " + uString(ControllerEvents.movingSpeed)).ToChar());
	}

	void CHomeSettings::OnRotspd()
	{
		
		ControllerEvents.rotatingSpeed = sliderRot->GetPos();
		sliderRot->SetToolTipText(uString("Rotating speed - " + uString(ControllerEvents.rotatingSpeed)).ToChar());
	}

	void CHomeSettings::OnSmooth()
	{
		
		ControllerEvents.smoothFactor = sliderSmooth->GetPos();
		sliderSmooth->SetToolTipText(uString("smooth factor - " + uString(ControllerEvents.smoothFactor)).ToChar());
	}

	void CHomeSettings::OnGizmoHeading()
	{
		
		ControllerEvents.MotionHeading = (zEVobMotionHeading)gizmoHeading->GetCurSel();
	}

	void CHomeSettings::OnUpdateGizmoHeading(CCmdUI* pCmdUI)
	{
		
		UICheck
			//GetCombo(ID_GIZMO_HEADING)->SelectItem((int)ControllerEvents.MotionHeading);
	}

	void CHomeSettings::OnUpdateSmooth(CCmdUI* pCmdUI)
	{
		
		UICheck
			//GetSlider(ID_SMOOTH)->SetPos(50);
	}

	void CHomeSettings::OnUpdateRotspd(CCmdUI* pCmdUI)
	{
		
		UICheck
			//GetSlider(ID_ROTSPD)->SetPos(50);
	}

	void CHomeSettings::OnUpdateMovspd(CCmdUI* pCmdUI)
	{
		
		UICheck

	}

	void CHomeSettings::OnWinrestore()
	{
		
		GetMainFrame()->ShowPane(&GetMainFrame()->m_wndClassView, TRUE, FALSE, FALSE);
		GetMainFrame()->ShowPane(&GetMainFrame()->m_wndVobList, TRUE, FALSE, FALSE);
		GetMainFrame()->ShowPane(&GetMainFrame()->m_wndProperties, TRUE, FALSE, FALSE);
		GetMainFrame()->ShowPane(&GetMainFrame()->m_wndOutput, TRUE, FALSE, FALSE);
	}

	void CHomeSettings::OnInsert()
	{
		
		CEditorView::view->OnObjInsert();
	}

	void CHomeSettings::OnUpdateCopy(CCmdUI* pCmdUI)
	{

		if ((BOOL)dynamic_cast<oCItem*>(GetSelectedVob()))
		{
			pCmdUI->Enable(FALSE); return;
		}
		CheckSel
	}

	void CHomeSettings::OnUpdateDelete(CCmdUI* pCmdUI)
	{
		
		CheckSel
	}

	void CHomeSettings::OnUpdateInsert(CCmdUI* pCmdUI)
	{
		
		UICheck
			CString objTypeName = ClassTree.GetItemText(ClassTree.GetSelectedItem());
		bool enabled = !objTypeName.IsEmpty() && objTypeName != "Engine objects";
		pCmdUI->Enable(enabled);
	}





	void CHomeSettings::OnFps()
	{
		s_FpsLimit = (uint)sliderFps->GetPos();
		sliderFps->SetToolTipText(uString("fps limit - " + uString(s_FpsLimit)).ToChar());
	}

	void CHomeSettings::OnFreecam()
	{
		ControllerEvents.CameraMoving = !ControllerEvents.CameraMoving;
	}

	void CHomeSettings::OnUpdateFreecam(CCmdUI* pCmdUI)
	{
		UICheck
			pCmdUI->SetCheck(ControllerEvents.CameraMoving);
	}
	int cnt = 0;
	void zCWorld::SearchVobByVisual(zSTRING& str, zCTree<zCVob>* vobNode)
	{
		//ogame->world->progressBar->SetPercent(cnt,"");
		if (vobNode == 0)
			vobNode = &globalVobTree;

		if (vobNode->GetData() && vobNode->GetData()->GetVisual()) {
			if (vobNode->GetData()->GetVisual()->GetVisualName().Search(str) != -1)
			{
				cnt++;
				FindVob(zSTRING(cnt) + " # " + vobNode->GetData()->GetVisual()->GetVisualName(), vobNode->GetData());
			}
		};

		zCTree<zCVob>* child = vobNode->GetFirstChild();
		while (child) {
			SearchVobByVisual(str, child);
			child = child->GetNextChild();
		};
	};


	class CMFCRibbonButtonEx : public CMFCRibbonButton
	{
	public:
		void SetIcon(HICON ico) { m_hIconSmall = ico; };
	};


	void CHomeSettings::OnFindvobvis()
	{
		CInputDialog dlg;
		zBOOL	ready = false;
		zSTRING	name = "";
		int		result = 0;
		cnt = 0;
		while (!ready)
		{
			result = dlg.DoModal();
			dlg.name = "Enter visual name";
			if (result == IDOK)
			{
				zCArchiver* arch = zarcFactory->CreateArchiverWrite(zARC_MODE_ASCII_PROPS);
				name = dlg.GetInput().GetBuffer(0);
				ready = !name.IsEmpty() && arch->IsStringValid(name.ToChar());
				arch->Close(); zRELEASE(arch);
			}
			else if (result == IDCANCEL)
				return;
		}

		if (name.IsEmpty()) return;
		CMainFrame::mainframe->m_wndOutput.m_wndOutputFind.ResetContent();
		CMainFrame::mainframe->m_wndOutput.m_wndOutputFind.AddString(zSTRING("Search results for \"" + name + "\"...").ToChar());

		ogame->world->SearchVobByVisual(name, &ogame->world->globalVobTree);
		CMainFrame::mainframe->m_wndOutput.m_wndTabs.SetActiveTab(FINDTAB);

	}


	void CHomeSettings::OnStop()
	{
		ControllerEvents.CameraMoving = NULL;
		SetMoveMode(zMM_NONE);
	}


}

void GOTHIC_ENGINE::CHomeSettings::OnUpdateFindvobvis(CCmdUI* pCmdUI)
{
	UICheck
}
