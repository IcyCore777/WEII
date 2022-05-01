// Supported with union (c) 2020 Union team

// User API for zCVob
// Add your methods here


//Methods
void				Archive_Union(zCArchiver& arc);
void				Unarchive_Union(zCArchiver&);
void				SetVobPresetName_Union(zSTRING const&);
void				ThisVobAddedToWorld_Union(zCWorld*);
void				ThisVobRemovedFromWorld_Union(zCWorld*);


static	zCClassDef* GetStaticClassDef(void) { return classDef; };
//Inline
void				SetShowVisual(zBOOL show) { showVisual = show; };
zBOOL				GetShowVisual() const { return showVisual; };
const zTBBox3D&		GetBBox3DWorld() const { return bbox3D; };
zTVisualCamAlign	GetVisualCamAlign() const { return visualCamAlign; };
void				SetVisualCamAlign(const zTVisualCamAlign a) { visualCamAlign = a; };
void				SetVisualAlphaEnabled(const zBOOL b) { visualAlphaEnabled = b; };
zBOOL				GetVisualAlphaEnabled() const { return visualAlphaEnabled; };
void				SetVisualAlpha(const zREAL a) { visualAlpha = a; };
zREAL				GetVisualAlpha() const { return visualAlpha; };
void				RestoreAlpha() { visualAlpha = 1; visualAlphaEnabled = FALSE; };
static zBOOL		GetIgnoreVisuals() { return s_ignoreVisuals; };
static void			SetIgnoreVisuals(zBOOL b) { s_ignoreVisuals = b; };
zTVobSleepingMode	GetSleepingMode() const { return zTVobSleepingMode(sleepingMode); };
inline zDWORD		GetVobID() const { return zDWORD(1); };
zBOOL				GetSleeping() const { return (sleepingMode == zVOB_SLEEPING); };
zMAT4&				GetTrafoLocal() { if (!trafo) CreateTrafoLocal(); return *trafo; }  
const zMAT4&		GetTrafoLocal() const { if (!trafo) CreateTrafoLocal(); return *trafo; }
zBOOL				GetPhysicsEnabled() const { return physicsEnabled; };
zBOOL				GetStaticVob() const { return staticVob; };
void				SetIgnoredByTraceRay(const zBOOL i) { ignoredByTraceRay = i; };
zBOOL				GetIgnoredByTraceRay() const { return ignoredByTraceRay; };
const zSTRING&		GetVobName() const { return GetObjectName(); };
zBOOL				GetCollDetStat() const { return collDetectionStatic; };
void				GetPositionWorld(zVEC3& pos) const { trafoObjToWorld.GetTranslation(pos); };
zBOOL				GetCollDetDyn() const { return collDetectionDynamic; };
zCEventManager*		GetEventManager(const zBOOL dontCreateIfNotPresent = FALSE) { return GetEM(dontCreateIfNotPresent); };
void				RotateLocal(const float x, const float y, const float z, const float angle) { RotateLocal(zVEC3(x, y, z), angle); };
void				RotateWorld(const float x, const float y, const float z, const float angle) { RotateWorld(zVEC3(x, y, z), angle); };
void				SetPositionLocal(const float x, const float y, const float z) { SetPositionLocal(zVEC3(x, y, z)); };
zCPolygon*			GetGroundPoly() const { return groundPoly; };
void				SetPositionWorld(const float x, const float y, const float z) { SetPositionWorld(zVEC3(x, y, z)); };
zCClassDef*			GetVisualClass() const { return (visual ? visual->GetClassDef() : 0); };
void				MoveLocal(const zVEC3& dir) { MoveLocal(dir.n[VX], dir.n[VY], dir.n[VZ]); };
void				MoveWorld(const zVEC3& dir) { MoveWorld(dir.n[VX], dir.n[VY], dir.n[VZ]); };
zBOOL				GetDrawBBox3D() const { return drawBBox3D; };
int					GetZBias() const { return m_zBias; };
void				SetZBias(const int a_iZBias) { m_zBias = a_iZBias; };

// visual animation
void				SetVisualAniMode(const zTAnimationMode& aniMode) { m_AniMode = aniMode; };
zTAnimationMode		GetVisualAniMode()	const { return m_AniMode; };
void				SetVisualAniModeStrength(zREAL aniModeStrength) { m_aniModeStrength = aniModeStrength; };
zREAL				GetVisualAniModeStrength()  const { return m_aniModeStrength; };
void				SetCastDynShadow(const zTDynShadowType b) { castDynShadow = b; };
zTDynShadowType		GetCastDynShadow() const { return zTDynShadowType(castDynShadow); };
zBOOL				GetInMovement() const { return (isInMovementMode != zVOB_MOVE_MODE_NOTINBLOCK); };

void RenderPFX(zCWorld* i_wld, zCViewBase* viewport, zREAL addon = 0.0f);