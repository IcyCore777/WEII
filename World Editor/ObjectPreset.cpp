// Supported with union (c) 2020 Union team
// Union SOURCE file
#include <assert.h>
namespace GOTHIC_ENGINE {
	CONST int DELETED_PRESET = -1;


	const zSTRING PRESET_SUBDIR = "objpresets\\";

	zCLASS_UNION_DEFINITION(zCObjPreset, zCObject, 0, 1)

		zCObjPreset::~zCObjPreset()
	{
		
		obj = NULL;
	}


	void zCObjPreset::Archive(zCArchiver& arc)
	{
		
		zCObject::Archive(arc);
		arc.WriteString("presetName", presetName);
		arc.WriteObject("preset", obj);
	}


	void zCObjPreset::Unarchive(zCArchiver& arc)
	{
		
		zCObject::Unarchive(arc);
		presetName = arc.ReadString("presetName");
		obj = arc.ReadObject("preset",FALSE);
	}


	zCObjPresetLib::~zCObjPresetLib()
	{
		
		Dispose();

		for (int z = 0; z < viewPointer.GetNumInList(); z++)
			delete viewPointer[z];
	}


	void zCObjPresetLib::Dispose()
	{
		
		for (int z = 0; z < GetNum(); z++)
		{
			if (parray[z]->obj) zRELEASE(parray[z]->obj);
			zRELEASE(parray[z]);
		}
		this->DeleteList();
	}
	int zCObjPresetLib::Load()
	{
		
#ifdef __G1
		CreateDirectory(Union.GetGameDirectory() + "_work\\tools\\data\\objpresets", 0);
#endif
#ifdef __G1A
		CreateDirectory(Union.GetGameDirectory() + "_work\\tools\\data\\objpresets", 0);
#endif
		int result = 0;
		DeleteList();
		OutputBuild("PresetLib: Loading ObjectPresets ...");

		zoptions->ChangeDir(DIR_TOOLS_DATA);
		zFILE_FILE subdir(PRESET_SUBDIR);
		subdir.ChangeDir(false);


		zFILE_FILE presetFile;
		bool go = presetFile.FindFirst(zSTRING("*.opf"));
		while (go)
		{
			result = Load(presetFile.GetFile());
			go = presetFile.FindNext();
		}


		return result;
	}


	int zCObjPresetLib::Load(zSTRING& filename)
	{
		
		zoptions->ChangeDir(DIR_TOOLS_DATA);
		zFILE_FILE subdir(PRESET_SUBDIR);
		subdir.ChangeDir(false);

		zCObjPreset* preset;
		zCArchiver* arch = zarcFactory->CreateArchiverRead(filename);
		if (!arch) return 1;

		OutputBuild("PresetLib: Read ObjectPresets \"" + filename + "\"");
		while (!arch->EndOfArchive())
		{
			preset = dynamic_cast<zCObjPreset*>(arch->ReadObject(FALSE));
			if (preset)
			{
				if (!GetObjectByName(preset->presetName))
				{
					Insert(preset);
					OutputBuild("PresetLib: Inserted preset " + preset->presetName);
				}
				else
				{
					OutputBuild("PresetLib: Preset already exists: " + preset->presetName);
					zRELEASE(preset);
				};
			}
		}

		arch->Close();
		zRELEASE(arch);
		return 0;
	}


	int zCObjPresetLib::Save()
	{
		
		OutputBuild("PresetLib: Saving presets ..");

		if (!IsDirty())
		{
			OutputBuild("PresetLib: ObjectPresets did not change. ");
			return 0;
		}
		zoptions->ChangeDir(DIR_TOOLS_DATA);
		zFILE_FILE subdir(PRESET_SUBDIR);
		subdir.ChangeDir(false);

		zSTRING filename;
		int num_presets_in_view = 0;
		int num_presets_in_lib = viewPointer.GetNumInList();
		for (int vc = 0; vc < num_presets_in_lib; vc++)
		{
			num_presets_in_view = viewPointer[vc]->GetNumInList();
			if (num_presets_in_view > 0)
			{
				filename = viewPointer[vc]->GetViewName() + zSTRING(".opf");

				OutputBuild("PresetLib: Write ObjectPresets \"" + viewPointer[vc]->GetViewName() + ".OPF\"");

				zCArchiver* arch = zarcFactory->CreateArchiverWrite(filename, zARC_MODE_ASCII);
				if (arch)
				{
					for (int z = 0; z < num_presets_in_view; z++)
					{
						int lib_index = viewPointer[vc]->GetSafe(z);
						if (lib_index != DELETED_PRESET
							&& parray[lib_index]->obj
							&& !parray[lib_index]->presetName.IsEmpty())
						{
							arch->WriteObject(parray[lib_index]);
						}
					}
					arch->Close();
					zRELEASE(arch);
				}
			}
		}

		SetDirty(FALSE);
		return 0;
	}


	zCObject* zCObjPresetLib::GetObjectByName(zSTRING& pName)
	{
		
		int z = 0;
		while (z < GetNumInList() && parray[z]->presetName != pName) z++;
		if (z < GetNumInList()) return parray[z]->obj;
		else return 0;
	}


	int zCObjPresetLib::GetIndexByName(const zSTRING& name)
	{
		
		int z = 0;
		int num_in_list = GetNumInList();
		while (
			z < num_in_list
			&&
			(parray[z]->presetName != name)
			)
		{
			z++;
		}

		if (z >= num_in_list) return -2;
		return z;
	}


	zCPresetView* zCObjPresetLib::GetNewView(zSTRING& className)
	{
		
		int sc = 0;
		while (sc < viewPointer.GetNumInList() &&
			viewPointer[sc]->GetViewName() != className) sc++;

		zCPresetView* newview;
		if (sc == viewPointer.GetNumInList())
			newview = new zCPresetView(this, className);
		else
			newview = viewPointer[sc];

		newview->EmptyList();
		for (int z = 0; z < GetNumInList(); z++)
		{
			if (parray[z]->obj)
				if (parray[z]->obj->GetClassName_() == className)
					newview->Insert(z);
		}

		if (sc == viewPointer.GetNumInList())
			viewPointer.Insert(newview);

		return newview;
	}


	zBOOL zCObjPresetLib::AddPreset(zCObject* obj, zSTRING& name)
	{
		
		if (name.IsEmpty()) return FALSE;

		int z = 0;
		while (z < GetNumInList()
			&&
			(
				parray[z]->presetName != name
				||
				parray[z]->GetClassName_() != obj->GetClassName_()
				)
			)
		{
			z++;
		}
		if (z < GetNumInList()) return FALSE;

		SetDirty();

		zCObjPreset* preset = new zCObjPreset(0, name);
		if(obj)
			preset->obj = obj->CreateCopy();
		else
		{
			OutputDebug("presetlib:: PIIDOR");
			return FALSE;
		}
		Insert(preset);

		return TRUE;
	}


	zBOOL zCObjPresetLib::RemovePreset(int lib_index)
	{
		
		for (int z = 0; z < viewPointer.GetNumInList(); z++)
			viewPointer[z]->RemoveFromViewByIndex(lib_index);

		zRELEASE(parray[lib_index]->obj);
		parray[lib_index]->obj = NULL;
		parray[lib_index]->presetName.Clear();

		SetDirty();

		return TRUE;
	}


	void zCPresetView::AddToView(zSTRING& pName)
	{
		
		int z = 0;
		while (z < lib->GetNumInList() && (*lib)[z]->presetName != pName) z++;

		if (z < lib->GetNumInList())
			Insert(z);
	}


	void zCPresetView::RemoveFromView(int view_index)
	{
		
		parray[view_index] = DELETED_PRESET;
	}


	void zCPresetView::RemoveFromViewByIndex(int lib_index)
	{
		
		int view_index = Search(lib_index);
		if (view_index >= 0) parray[view_index] = DELETED_PRESET;
	};


	void zCPresetView::AddPreset(zCObject* obj, zSTRING& name)
	{
		
		bool result = bool(lib->AddPreset(obj, name) == TRUE);
		if (result)
		{
			int index = lib->GetIndexByName(name);
			if (index >= 0) Insert(index);
		}
	}


	void zCPresetView::ChangePreset(zCObject* obj, int view_index)
	{
		
		assert(numAlloc > 0);
		assert((view_index >= 0) && (view_index < numInArray));

		if (parray[view_index] == DELETED_PRESET) return;
		(*lib)[parray[view_index]]->obj = obj;
	}


	zSTRING zCPresetView::GetViewName()const
	{
		
		return viewName;
	}


	void zCPresetView::RemovePreset(int view_index)
	{
		
		if (parray[view_index] == DELETED_PRESET) return;
		lib->RemovePreset(parray[view_index]);
	}


	void zCPresetView::RemovePresetByName(zSTRING name)
	{
		
		int z = GetViewIndexByName(name);
		if (z >= 0 && z < GetNumInList() && (parray[z] != DELETED_PRESET))
			RemovePreset(z);
	}


	zCObjPreset* zCPresetView::GetPresetByName(zSTRING name)
	{
		
		int z = GetViewIndexByName(name);
		if (z >= 0 && z < GetNumInList() && (parray[z] != DELETED_PRESET))
			return (*lib)[parray[z]];
		return 0;
	}


	zCObjPreset* zCPresetView::operator [] (int view_index) const
	{
		
		assert(numInArray > 0);
		assert(numAlloc > 0);
		assert((view_index >= 0) && (view_index < numInArray));
		if (parray[view_index] == DELETED_PRESET) return 0;
		return (*lib)[parray[view_index]];
	};


	int zCPresetView::GetViewIndexByName(zSTRING name)
	{
		
		int z = 0;
		int num_in_list = GetNumInList();
		while
			(
				z < num_in_list
				&&
				(
					(parray[z] == DELETED_PRESET)
					||
					((*lib)[parray[z]]->presetName != name)
					)
				)
		{
			z++;
		}

		if (z >= num_in_list) return -2;
		return z;
	}
}