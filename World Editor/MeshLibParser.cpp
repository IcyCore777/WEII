// Supported with union (c) 2020 Union team
// Union SOURCE file

using namespace GOTHIC_ENGINE;
	struct zTMeshRenderer;

	uArray<uString>		meshLib;
	uArray<Group*>		Groups3ds;
	uArray<Group*>		GroupsMDS;
	uArray<Group*>		GroupsMMS;
	uArray<Group*>		GroupsITEMS;
	uArray<Group*>		GroupsASC;
	uArray<zSTRING>		items;
	uArray<Group*>		GroupsPfx;
	zTMeshRenderer* meshRenderer = 0;
	uString				_3dsPath = "_work\\Data\\Textures\\meshThumbs\\%z.JPG";
	uString				_pfxPath = "_work\\Data\\Textures\\pfx\\%z.JPG";
	uString				_itemPath = "_work\\Data\\Textures\\itemThumbs\\%z.JPG";


	uArray<Group*> GetGroupType(int type)
	{
		if (type == ITEM)   return GroupsITEMS;
		if (type == MSH)	return Groups3ds;
		if (type == MMS)	return GroupsMMS;
		if (type == ASC)	return GroupsASC;
		if (type == MDS)	return GroupsMDS;
	}

	bool IsModelFile(const char* fileName_) {
		uString fileName = fileName_;
		uString shortName = fileName.GetWord("\\", -1);
		if (shortName.Search("IT") != -1)				return false;
		if (shortName.Search("HUM") != -1)				return false;
		if (shortName.Search("ORC") != -1)				return false;
		if (shortName.Search("PUMA") != -1)				return false;
		if (shortName.Search("SWAMP") != -1)			return false;
		if (shortName.Search("SCAVEN") != -1)			return false;
		if (shortName.Search("GOBB") != -1)				return false;
		if (shortName.Search("GOLE") != -1)				return false;
		if (shortName.Search("DEMON") != -1)			return false;
		if (shortName.Search("FLY") != -1)				return false;
		if (shortName.Search("DRAGON") != -1)			return false;
		if (shortName.Search("GIANT") != -1)			return false;
		if (shortName.Search("LURK") != -1)				return false;
		if (shortName.Search("ARMOR") != -1)			return false;
		if (shortName.Search("BODY") != -1)				return false;
		if (shortName.Search("DRA") != -1)				return false;
		if (shortName.Search("CRAW") != -1)				return false;
		if (shortName.Search("SHADOW") != -1)			return false;
		if (shortName.Search("TRO") != -1)				return false;
		if (shortName.Search("WAR") != -1)				return false;
		if (shortName.Search("STONEGUARDIAN") != -1)	return false;
		if (shortName.Search("ALLIG") != -1)			return false;
		if (shortName.Search("SNAP") != -1)				return false;
		if (shortName.Search("ZOMBIE") != -1)			return false;
		if (shortName.Search("KEILER") != -1)			return false;
		if (shortName.Search("RAZOR") != -1)			return false;
		if (shortName.Search("HARPIE") != -1)			return false;
		if (shortName.Search("SHEEP") != -1)			return false;
		if (shortName.Search("MEATBUG") != -1)			return false;
		if (shortName.Search("WOLF") != -1)				return false;
		if (shortName.Search("MOLERAT") != -1)			return false;
		if (shortName.Search("IRRLICHT") != -1)			return false;

#ifdef __G1 
		if (fileName.EndWith(".MMB") || fileName.EndWith(".MRM") || fileName.EndWith(".MDL") || fileName.EndWith(".MDS")) return true;
#else
		if (fileName.EndWith(".MMB") || fileName.EndWith(".MRM") || fileName.EndWith(".MDL") || fileName.EndWith(".MSB")) return true;
#endif

		return false;
	}

	bool IsTEXFile(const char* fileName_) {
		
		uString fileName = fileName_;
		if (fileName.EndWith("3DS.JPG")) return true;

		return false;
	}

	bool IsASCFile(const char* fileName_) {
		
		uString fileName = fileName_;
		if (fileName.EndWith("ASC.JPG")) return true;

		return false;
	}

	bool IsMDSFile(const char* fileName_) {
		
		uString fileName = fileName_;
		if (fileName.EndWith("MDS.JPG")) return true;

		return false;
	}

	bool IsMMSFile(const char* fileName_) {
		
		uString fileName = fileName_;
		if (fileName.EndWith("MMS.JPG")) return true;

		return false;
	}

	bool IsPfxFile(const char* fileName_) {
		
		uString fileName = fileName_;
		if (fileName.EndWith("PFX.JPG")) return true;

		return false;
	}

	void  GetTEXListPhysical(uArray<uString>& modelList, int type) {
		
		char** fileList = Null;
		uint numInList = Vdfs32::vdf_filelist_physical(fileList);

		for (size_t i = 0; i < numInList; i++) {
			if (type == MSH)
				if (IsTEXFile(fileList[i]))
					modelList += fileList[i];
			if (type == ASC)
				if (IsASCFile(fileList[i]))
					modelList += fileList[i];
			if (type == MDS)
				if (IsMDSFile(fileList[i]))
					modelList += fileList[i];
			if (type == MMS)
				if (IsMMSFile(fileList[i]))
					modelList += fileList[i];
			delete fileList[i];
		}

		delete fileList;
	}

	void  GetModelListPhysical(uArray<uString>& modelList) {
		
		char** fileList = Null;
		uint numInList = Vdfs32::vdf_filelist_physical(fileList);

		for (size_t i = 0; i < numInList; i++) {
			if (IsModelFile(fileList[i]))
				modelList |= fileList[i];
			delete fileList[i];
		}

		delete fileList;
	}

	void GetModelListVirtual(uArray<uString>& modelList) {
		
		char** fileList = Null;
		uint numInList = Vdfs32::vdf_filelist_virtual(fileList);

		for (size_t i = 0; i < numInList; i++) {
			if (IsModelFile(fileList[i]))
				modelList |= fileList[i];
			delete fileList[i];
		}

		delete fileList;
	}

	uArray<uString> GetModelList() {
		
		uArray<uString> modelList;
		GetModelListPhysical(modelList);
		GetModelListVirtual(modelList);
		return modelList;
	}

	void AddFileNameToGroup(const uString& fileName, int type) {
		
		uString shortName = fileName.GetWord("\\", -1);

		uString groupName = shortName.GetPattern("", "_", 2);

		for (uint i = 0; i < GetGroupType(type).GetNum(); i++) {
			Group* gp = GetGroupType(type)[i];
			if (gp->Name == groupName) {
				gp->FileNames.Insert(shortName);
				return;
			}
		}

		Group* gp = new Group();
		GetGroupType(type).Insert(gp);


		gp->Name = groupName;
		gp->FileNames.Insert(shortName);
	}

	Group* GetTrashGroup(int type) {
		
		for (uint i = 0; i < GetGroupType(type).GetNum(); i++)
			if (GetGroupType(type)[i]->Name.IsEmpty())
				return GetGroupType(type)[i];
		Group* gp = new Group();

		GetGroupType(type).Insert(gp);
		return gp;
	}

	void CreateTrashGroupContent(int type) {
		
		Group* trash = GetTrashGroup(type);
		trash->Name = "TRASH";
		for (uint i = 0; i < GetGroupType(type).GetNum(); i++) {
			if (!GetGroupType(type)[i]->Name.IsEmpty() && GetGroupType(type)[i]->FileNames.GetNum() <= 2) {
				trash->FileNames.MergeArray(GetGroupType(type)[i]->FileNames);
				GetGroupType(type).RemoveAt(i--);
			}
		}
	}

	void SaveTumbsByMask(int type) {
		
		uArray<uString> fileNames;

		GetTEXListPhysical(fileNames, type);
		for (uint i = 0; i < fileNames.GetNum(); i++)
			AddFileNameToGroup(fileNames[i], type);
		CreateTrashGroupContent(type);
	}

	void SaveThumbPic(zSTRING name, int size, int id)
	{
		
		zCTextureConvert* texCon = zrenderer->CreateTextureConvert();
		zrenderer->Vid_GetFrontBufferCopy(*texCon);
		zCTextureInfo texInfo = texCon->GetTextureInfo();

		texInfo.texFormat = zRND_TEX_FORMAT_RGBA_8888;
		texInfo.numMipMap = 1;
		texInfo.sizeX = size;
		texInfo.sizeY = size;
		texInfo.refSizeX = size;
		texInfo.refSizeY = size;
		texCon->SetDetailTextureMode(FALSE);
		texCon->ConvertTextureFormat(texInfo);

		int nPitch = texInfo.sizeX * 4;
		int nImageSize = nPitch * texInfo.sizeY;
		void* pb = (void*) ::malloc(nImageSize);
		texCon->CopyTextureDataTo(0, pb, texInfo.sizeX * 4);

		HBITMAP hBitmap = 0;

		hBitmap = CreateBitmap(texInfo.sizeX, texInfo.sizeY, 1, 32, pb);
		::free(pb);
		CImage* thumb = new CImage;

		thumb->Attach(hBitmap);

		if (id == ITEM)
			thumb->Save(uString::Combine(_itemPath.ToChar(), name).ToChar(), Gdiplus::ImageFormatJPEG);
		thumb->Save(uString::Combine(id ? _3dsPath.ToChar() : _pfxPath.ToChar(), name).ToChar(), Gdiplus::ImageFormatJPEG);

		delete thumb; thumb = 0;
		hBitmap = 0;
		Vdfs32::vdf_fexists(uString(zoptions->GetDirString(DIR_TEX) + "itemthumbs\\" + name + ".JPG").ToChar(), VDF_PHYSICAL);
		Vdfs32::vdf_fexists(uString(zoptions->GetDirString(DIR_TEX) + "meshThumbs\\" + name + ".JPG").ToChar(), VDF_PHYSICAL);
	}

	void RenderItemPlaceCamera(zCVob* vob, zCCamera* cam, float zoom)
	{
		
		cam->SetFOV(45);
		zVEC3 tempL;
		zVEC3 tempW;
		zTBBox3D box3dl = vob->GetBBox3DLocal();
		zTBBox3D box3dw = vob->GetBBox3DWorld();
		tempL.n[0] = (box3dl.maxs[0] - box3dl.mins[0]) * 0.5f;
		tempL.n[1] = (box3dl.maxs[1] - box3dl.mins[1]) * 0.5f;
		tempL.n[2] = (box3dl.maxs[2] - box3dl.mins[2]) * 0.5f;

		tempW.n[0] = (box3dw.maxs[0] - box3dw.mins[0]) * 0.5f;
		tempW.n[1] = (box3dw.maxs[1] - box3dw.mins[1]) * 0.5f;
		tempW.n[2] = (box3dw.maxs[2] - box3dw.mins[2]) * 0.5f;

		float tempRad = tempL.Length();

		float beta = cam->GetFOV() * 0.5f;
		float alpha = 90 - beta;
		float dist = (tempRad * sinf(alpha)) / sinf(beta);

		dist *= zoom;
		cam->connectedVob->SetPositionWorld(box3dw.mins[0] + tempW.n[0], box3dw.mins[1] + tempW.n[1], -fabs(dist));
	};

	zTMeshRenderer::zTMeshRenderer() {
		
		view = new zCView(0, 0, 8192, 8192);
		world = zfactory->CreateWorld();
		world->GetBspTree()->bspTreeMode = zBSP_MODE_INDOOR;
		world->GetActiveSkyControler()->SetFillBackground(False);

		pfx = 0;
		camera = new zCCamera();
		camera->targetView = view;
		camera->farClipZ = 9999.0f;
		camera->vobFarClipZ = 9999.0f;
		camera->connectedVob = new zCVob();
		world->AddVob(camera->connectedVob);
		camera->connectedVob->Release();
	}
	void  zTMeshRenderer::SetPos(int x, int y)
	{
		view->SetPos(x, y);
	}

	void zTMeshRenderer::SetVisual(zSTRING fileName, int id)
	{
		
		vob = new zCVob();
		world->AddVob(vob);

		if (id == 32)
		{
			pfx = new zCParticleFX();
			_ASSERT(pfx);
			pfx->SetAndStartEmitter(fileName,FALSE);
			vob->SetVisual(pfx);

			zRELEASE(pfx)
				return;
		}

		if (id == MSH || id == ASC || id == MDS || id == MMS)
		{
			vob->SetVisual(fileName);
		}
	}

	void zTMeshRenderer::Render() {
		
		RenderItemPlaceCamera(vob, camera, 1.3);
		vob->SetPositionWorld(0, 0, 0);
		vob->ResetRotationsLocal();
		vob->RotateWorld(1, 0, 0, -45);
		zCCamera* activeCam = zCCamera::activeCam;
		zCCamera::activeCam = camera;

		zCEventManager::disableEventManagers = True;
		zrenderer->SetAlphaBlendFunc(zRND_ALPHA_FUNC_BLEND);
		zrenderer->Vid_Clear(zCOLOR(100, 100, 100, 0), 3);
		world->Render(*zCCamera::activeCam);

		zCEventManager::disableEventManagers = False;
		zCCamera::activeCam = activeCam;
		world->RemoveVob(vob);
	}

	CProgressBar* progressBar = 0;
	void ParseMeshThumbs()
	{
		
		::CreateDirectory("_work\\Data\\Textures\\MeshThumbs\\", 0);
		ClearOutput();

		CMainFrame::mainframe->ShowProgressBar("Parsing vob library... ", TRUE);

		progressBar = new CProgressBar();
		progressBar->Init(CMainFrame::mainframe->m_progressBar);


		uArray<uString> arr = GetModelList();

		if (!meshRenderer)
			meshRenderer = new zTMeshRenderer;

		for (size_t i = 0; i < arr.GetNum(); i++)
		{
			uString name = arr[i].GetWord("\\", -1);

			zFILE* file = zfactory->CreateZFile(zoptions->GetDirString(DIR_COMPILED_MESHES) + name.ToChar());
			if (file->Exists())
			{
				if (file->Open(false) == zERR_NONE)
				{
					if (file->Size() <= 223)
					{

						OutputError(zSTRING(name) + " <- Corrupted mesh! Skipping...");
						file->Close();
						continue;
					}
				}
			}
			delete file; file = 0;
#ifndef __G1
			name.Regex_Replace("\\.MSB$", ".MDS"); //if MDS file
#endif // !__G1
			name.Regex_Replace("\\.MDL$", ".ASC"); //if ASC file
			name.Regex_Replace("\\.MRM$", ".3DS"); //if 3ds file
			name.Regex_Replace("\\.MMB$", ".MMS"); //if MMS file

			int percent = (int)(100.0 / (double)arr.GetNum() * (double)i);

			if (progressBar) progressBar->SetPercent(percent, "");
			OutputBuild("Rendering - " + zSTRING(name));
			meshRenderer->SetVisual(name.ToChar(), MSH);
			if (meshRenderer) meshRenderer->Render();
			SaveThumbPic(name.ToChar(), 256, 1);
			zrenderer->Vid_Blit(1, 0, 0);
			CEditorView::view->RedrawWindow(0, 0, RDW_INVALIDATE);
		}
		OutputBuild("Done!");
		CMainFrame::mainframe->ShowProgressBar("Done", FALSE);
		CEditorView::view->RedrawWindow(0, 0, RDW_INVALIDATE);
	}


	void SaveTumbsItems(uArray<uString> fileNames) {
		
		for (uint i = 0; i < fileNames.GetNum(); i++)
			AddFileNameToGroup(fileNames[i], ITEM);
		CreateTrashGroupContent(ITEM);
	}


	void BuildItemList()
	{
		
		uArray<uString> fileNames;
		zSTRING scriptClassName = "C_Item";

		int pos, typ, anz, c, classindex;

		classindex = parser->GetIndex(scriptClassName);
		if (classindex != -1)
		{
			parser->GetSymbolInfo(classindex, typ, anz);
			pos = parser->GetInstance(classindex, 0);
			while (pos >= 0)
			{
				zSTRING s = parser->GetSymbolInfo(pos, typ, c);
				items.Insert(s);
				pos = parser->GetInstance(classindex, pos + 1);
				fileNames += zSTRING(s + ".jpg").ToChar();
			}
		}
		SaveTumbsItems(fileNames);
	}


	void RenderItems()
	{
		
		BuildItemList();
#ifdef __G2A
		oCItem::s_bItemEffectEnabled = FALSE;
#endif //  __G2A

		oCItem* vob;
		zCWorld* world = new zCWorld;
		zCView* dtp_item = 0;


		CMainFrame::mainframe->ShowProgressBar("Parsing Item library... ", TRUE);
		progressBar = new CProgressBar();
		progressBar->Init(CMainFrame::mainframe->m_progressBar);
		::CreateDirectory("_work\\Data\\Textures\\ItemThumbs\\", 0);
		for (size_t i = 0; i < items.GetNum(); i++)
		{

			vob = (oCItem*)zCObject::CreateNewInstance("oCItem");
			if (!vob) continue;

			vob->InitByScript(parser->GetIndex(items[i]), 1);
			int percent = (int)(100.0 / (double)items.GetNum() * (double)i);

			if (progressBar) progressBar->SetPercent(percent, "");
			OutputBuild("Rendering - " + zSTRING(items[i]));
			if (!dtp_item)
				dtp_item = new zCView(0, 0, 8191, 8191, VIEW_VIEWPORT);
			zrenderer->Vid_Clear(zCOLOR(100, 100, 100, 255), 3);


			vob->oCItem::RenderItem(world, dtp_item, 0.0);

			SaveThumbPic(items[i].ToChar(), 256, ITEM);
			zrenderer->Vid_Blit(1, 0, 0);
		}
		CMainFrame::mainframe->ShowProgressBar("Done", FALSE);
	}
