// Supported with union (c) 2020 Union team

// User API for oCWorld
// Add your methods here

int oCWorld::SaveWorld_Union(zSTRING const& filename, zCWorld::zTWorldSaveMode saveMode, int writeBinary, int _saveLevelMesh);


void                SetProgressBar(zCViewProgressBar* progressBar) { this->progressBar = progressBar; };    // progressbar that is updated during world saves/loads
void                CollectVobsInBBox3D(zCArray<zCVob*>& resVobList, const zTBBox3D& inbbox3D) { bspTree.bspRoot->CollectVobsInBBox3D(resVobList, inbbox3D); };
zBOOL               IsCompiled() const { return compiled; };
zBOOL               IsCompiledEditorMode() const { return compiledEditorMode; };
zTWld_RenderMode    GetWorldRenderMode() const { return worldRenderMode; };
zCWayNet*           GetWayNet() const { return wayNet; };
static zBOOL        S_GetEnvMappingEnabled() { return s_bEnvMappingEnabled; };
static void         S_SetEnvMappingEnabled(const zBOOL a_bEn) { s_bEnvMappingEnabled = a_bEn; };
static zBOOL        GetFadeOutFarVertices() { return s_bFadeOutFarVerts; };
void                SetWorldRenderMode(const zTWld_RenderMode& rmode) { worldRenderMode = rmode; bspTree.worldRenderMode = rmode; };


void MoveVobSubtreeTo1(zCVob* vobSubtree, zCVob* destParent)
{
    if (!destParent) return;
    if (!destParent->globalVobTreeNode) return;
    MoveVobSubtreeTo(vobSubtree, destParent->globalVobTreeNode);
};

zCTree<zCVob>* AddVobAsChild1(zCVob* childVob, zCVob* parentVob) {
    if ((!parentVob) || (!parentVob->globalVobTreeNode)) return 0;
    return AddVobAsChild(childVob, parentVob->globalVobTreeNode);
};