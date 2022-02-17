// Supported with union (c) 2020 Union team
// Union HEADER file

namespace GOTHIC_ENGINE {
	class zCObjPreset : public zCObject
	{
	public:
		zCLASS_UNION_DECLARATION(zCObjPreset)

		zCObject* obj;
		zSTRING presetName;

		zCObjPreset() : obj(0), presetName("") {};
		zCObjPreset(zCObject* _obj, zSTRING& name) : obj(_obj), presetName(name) {};
		virtual ~zCObjPreset();

		void	Archive(zCArchiver& arc);
		void	Unarchive(zCArchiver& arc);
	};

	class zCPresetView;

	class zCObjPresetLib : public zCArray<zCObjPreset*>
	{
	public:
		zCArray<zCPresetView*> viewPointer;
		zBOOL isDirty;

	public:
		zCObjPresetLib() : zCArray<zCObjPreset*>(), isDirty(FALSE), viewPointer() {};
		virtual ~zCObjPresetLib();
		void Dispose();

		// load/save
		int Load();
		int Load(zSTRING& filename);
		int Save();

		void SetDirty(zBOOL dirt = TRUE) { isDirty = dirt; };
		zBOOL IsDirty() { return isDirty; };

		// find first perset with that name
		zCObject* GetObjectByName(zSTRING& pName);
		int			GetIndexByName(const zSTRING& name);

		// Create new view from class (do not delete!)
		zCPresetView* GetNewView(zSTRING& className);

		zBOOL AddPreset(zCObject* obj, zSTRING& name);
		zBOOL RemovePreset(int lib_index);
	};

	class zCPresetView : public zCArray<int>
	{
	private:
		zSTRING viewName;

	protected:
		zCObjPresetLib* lib;

	public:
		zCPresetView(zCObjPresetLib* from) : zCArray<int>(), lib(from), viewName() {};
		zCPresetView(zCObjPresetLib* from, zSTRING cName)
			:zCArray<int>(), lib(from), viewName(cName) {};

		void AddToView(zSTRING& pName);
		void RemoveFromView(int view_index);
		void RemoveFromViewByIndex(int lib_index);

		int GetViewIndexByName(zSTRING name);

		void AddPreset(zCObject* obj, zSTRING& name);
		void ChangePreset(zCObject* obj, int view_index);

		void RemovePreset(int view_index);
		void RemovePresetByName(zSTRING name);

		zCObjPreset* GetPresetByName(zSTRING name);

		void SetDirty(zBOOL dirt = TRUE) { lib->SetDirty(dirt); };

		zCObjPreset* operator [] (int view_index) const;

		zSTRING GetViewName()const;
	};






	

}