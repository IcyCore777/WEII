// Supported with union (c) 2020 Union team
// Union HEADER file

using namespace GOTHIC_ENGINE;
	class zCPropEle;
#define AddProp					new CMFCPropertyGridProperty
#define AddColor				new CMFCPropertyGridColorProperty
	class CPropList : public uArray<zCPropEle*>
	{
	public:
		CPropList();
		virtual ~CPropList();

		zCPropEle* SearchElementByString(CString item);
	};


	////////////////////////////////////////////////////////////
	enum TPropEditType {
		PETunknown,
		PETbool,
		PETenum,
		PETint,
		PETfloat,
		PETstring,
		PETvec3,
		PETcolor,
		PETraw,
		PETclass,
		PETgroup,
		PETchunkend
	};

	class zCPropEle : public zSTRING
	{
	public:

		zCPropEle(const char* c);
		zCPropEle();

	public:

		static CString PickValue(CString text);
		static CString PickName(CString text);

		TPropEditType	GetPropType();
		CString			GetName();
		CString			GetDescription();
		int				GetIconID();


		int				GetIntVal();  // für bool integer float und enum
		float			GetFloatVal();  // für float
		CString			GetStrVal();	 // für string, raw
		zVEC3			GetVecVal();  // für vec3
		CString			GetEnumVal();  // für enum
		void GetEnumList(CComboBox& list);
		CMFCPropertyGridProperty* GetEnumList();
		COLORREF		GetColor();

		void			ToggleBoolVal();
		void			ToggleEnumVal();
		_variant_t		GetValue();


		void SetByInt(int v);
		void SetByFloat(float v);
		void SetByStr(CString v);
		void SetByVec(zVEC3 v);
		void SetByEnum(CString v);
		void SetByColor(COLORREF col);

		void SetChecked(bool ch = true) { checked = ch; };
		bool IsChecked() { return checked; };

	private:
		TPropEditType	ParseDataType(const zSTRING&);

		bool			checked;
	};
