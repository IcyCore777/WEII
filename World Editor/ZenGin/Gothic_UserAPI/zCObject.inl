// Supported with union (c) 2020 Union team

// User API for zCObject
// Add your methods here

//zCObject* AddRef() { refCtr++; return this; };
zCClassDef*				GetClassDef()	const { return _GetClassDef(); };
const zSTRING&			GetClassName_()	const { return _GetClassDef()->GetClassName_(); };
static zCClassDef* GetClassDef(const zSTRING& className)   zCall(0x005A9FE0);