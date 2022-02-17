// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {






	BEGIN_MESSAGE_MAP(CSettings, CCmdTarget)
		ON_COMMAND(ID_LOGLEVEL, &CSettings::OnLoglevel)
		ON_COMMAND(ID_FARCLIP, &CSettings::OnFarclip)
		ON_COMMAND(ID_VIDEO, &CSettings::OnVideo)
		ON_COMMAND(ID_WIRE, &CSettings::OnWire)
		ON_COMMAND(ID_MATWIRE, &CSettings::OnMatwire)
		ON_COMMAND(ID_MAT, &CSettings::OnMat)
		ON_COMMAND(ID_FLAT, &CSettings::OnFlat)
		ON_UPDATE_COMMAND_UI(ID_FARCLIP, &CSettings::OnUpdateFarclip)
		ON_UPDATE_COMMAND_UI(ID_LOGLEVEL, &CSettings::OnUpdateLoglevel)
		ON_UPDATE_COMMAND_UI(ID_WIRE, &CSettings::OnUpdateWire)
		ON_UPDATE_COMMAND_UI(ID_MATWIRE, &CSettings::OnUpdateMatwire)
		ON_UPDATE_COMMAND_UI(ID_MAT, &CSettings::OnUpdateMat)
		ON_UPDATE_COMMAND_UI(ID_FLAT, &CSettings::OnUpdateFlat)
		ON_COMMAND(ID_FOG, &CSettings::OnFog)
		ON_UPDATE_COMMAND_UI(ID_FOG, &CSettings::OnUpdateFog)
		//ON_COMMAND(ID_IGNOREPFX, &CSettings::OnIgnorepfx)
	END_MESSAGE_MAP()

	CSettings::CSettings()
	{

	}

	void CSettings::InitData()
	{
		GetCombo(ID_VIDEO)->RemoveAllItems();
		zTRnd_VidModeInfo info;
		zBOOL result;
		int currentModeNr = -1;
		int count = zrenderer->Vid_GetNumModes();
		for (int nr = 0; nr < count; nr++) {
			result = zrenderer->Vid_GetModeInfo(info, nr);
			if (result) {
				CString s;
				s.Format("%i:%i (%i bit)", info.xdim, info.ydim, info.bpp);

				if (info.xdim == viewx && info.ydim == viewy) {
					currentModeNr = nr;
					s = s + " *";
				};
				GetCombo(ID_VIDEO)->AddItem(s);

			}
		}
		GetCombo(ID_VIDEO)->SelectItem(currentModeNr);
	}


	void CSettings::OnLoglevel()
	{
		zerr->SetFilterLevel(GetSlider(ID_LOGLEVEL)->GetPos());
		theApp.WriteProfileInt(strLog, strSpy, GetSlider(ID_LOGLEVEL)->GetPos());
	}


	void CSettings::OnFarclip()
	{
		float farclipz = (float)GetSlider(ID_FARCLIP)->GetPos();

		ogame->world->SetVobFarClipZScalability(farclipz);
		ogame->world->GetActiveSkyControler()->SetFarZ(farclipz * 10000);
		ogame->world->GetActiveSkyControler()->SetFarZScalability(farclipz * 10000);
	}


	void CSettings::OnVideo()
	{
		int modeNr = GetCombo(ID_VIDEO)->GetCurSel();
		zTRnd_VidModeInfo info;
		zrenderer->Vid_GetModeInfo(info, modeNr);
		viewx = info.xdim;
		viewy = info.ydim;
		CMainFrame::mainframe->ApplyVideoSettings();
		InitData();

	}


	void CSettings::OnWire()
	{
		if (zrenderer->GetPolyDrawMode() != zRND_DRAW_WIRE)
			zrenderer->SetPolyDrawMode(zRND_DRAW_WIRE);
		else
			zrenderer->SetPolyDrawMode(zRND_DRAW_MATERIAL);
		theApp.WriteProfileInt(strRnd, strMode, (int)zrenderer->GetPolyDrawMode());
	}


	void CSettings::OnMatwire()
	{
		if (zrenderer->GetPolyDrawMode() != zRND_DRAW_MATERIAL_WIRE)
			zrenderer->SetPolyDrawMode(zRND_DRAW_MATERIAL_WIRE);
		else
			zrenderer->SetPolyDrawMode(zRND_DRAW_MATERIAL);
		theApp.WriteProfileInt(strRnd, strMode, (int)zrenderer->GetPolyDrawMode());
	}


	void CSettings::OnMat()
	{
		zrenderer->SetPolyDrawMode(zRND_DRAW_MATERIAL);
		theApp.WriteProfileInt(strRnd, strMode, (int)zrenderer->GetPolyDrawMode());
	}


	void CSettings::OnFlat()
	{
		zrenderer->SetPolyDrawMode(zRND_DRAW_FLAT);
		theApp.WriteProfileInt(strRnd, strMode, (int)zrenderer->GetPolyDrawMode());
	}


	void CSettings::OnUpdateFarclip(CCmdUI* pCmdUI)
	{
		pCmdUI->Enable(CEditorDoc::doc->bWorldLoaded);
	}

	void CSettings::OnUpdateLoglevel(CCmdUI* pCmdUI)
	{

	}


	void CSettings::OnUpdateWire(CCmdUI* pCmdUI)
	{
		if (CEditorDoc::doc->bWorldLoaded)
			pCmdUI->SetCheck(zrenderer->GetPolyDrawMode() == zRND_DRAW_WIRE);
		else
			pCmdUI->Enable(FALSE);
	}


	void CSettings::OnUpdateMatwire(CCmdUI* pCmdUI)
	{
		if (CEditorDoc::doc->bWorldLoaded)
			pCmdUI->SetCheck(zrenderer->GetPolyDrawMode() == zRND_DRAW_MATERIAL_WIRE);
		else
			pCmdUI->Enable(FALSE);
	}


	void CSettings::OnUpdateMat(CCmdUI* pCmdUI)
	{
		if (CEditorDoc::doc->bWorldLoaded)
			pCmdUI->SetCheck(zrenderer->GetPolyDrawMode() == zRND_DRAW_MATERIAL);
		else
			pCmdUI->Enable(FALSE);
	}


	void CSettings::OnUpdateFlat(CCmdUI* pCmdUI)
	{
		if (CEditorDoc::doc->bWorldLoaded)
			pCmdUI->SetCheck(zrenderer->GetPolyDrawMode() == zRND_DRAW_FLAT);
		else
			pCmdUI->Enable(FALSE);
	}


	void CSettings::OnFog()
	{
		zrenderer->SetFog(!zrenderer->GetFog());
		theApp.WriteProfileInt(strRnd, strFog, zrenderer->GetFog());
	}


	void CSettings::OnUpdateFog(CCmdUI* pCmdUI)
	{
		if (CEditorDoc::doc->bWorldLoaded)
			pCmdUI->SetCheck(zrenderer->GetFog());
		else
			pCmdUI->Enable(FALSE);
	}


	void CSettings::OnIgnorepfx()
	{
		// TODO: добавьте свой код обработчика команд
	}

}