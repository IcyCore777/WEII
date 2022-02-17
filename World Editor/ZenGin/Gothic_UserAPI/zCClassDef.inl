// Supported with union (c) 2020 Union team

// User API for zCClassDef
// Add your methods here

zBOOL						IsScriptedClass() const { return !scriptClassName.IsEmpty(); };
zBOOL						IsAbstractClass() const { return (createNewInstance == 0); };
const zSTRING&				GetClassName_() const { return className; };
const zSTRING&				GetBaseClassName() const { return baseClassName; };
zCClassDef*					GetBaseClassDef() const { return baseClassDef; };
static int					GetNum() { return classDefList->GetNum(); };
static zCClassDef*			GetClassDefByIndex(int i) { return classDefList->GetSafe(i); };
zCObject*					SearchObject(const zSTRING& objectName) { return SearchHashTable(objectName); };
int							GetNumObjects() const { return objectList.GetNum(); };
zCObject*					GetObjectByIndex(int i) { return objectList[i]; };
int							GetIndexByObject(zCObject* object) { return objectList.Search(object); };