// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
    void InsertVobToVirtualVobTree(zCVob* vob)
    {
        if (VirtualVobTree_Portals::GetInstance().FindParentVobTreeForVob(vob))
            VirtualVobTree_Portals::GetInstance().InsertAsChild(vob);
        else if (VirtualVobTree_Globals::GetInstance().FindParentVobTreeForVob(vob))
            VirtualVobTree_Globals::GetInstance().InsertAsChild(vob);
        else if (vob->GetSectorNameVobIsIn())
            VirtualVobTree_Portals::GetInstance().InsertAsChild(vob);
        else
            VirtualVobTree_Globals::GetInstance().InsertAsChild(vob);
    }


    HOOK Hook_zCVob_ThisVobAddedToWorld PATCH(&zCVob::ThisVobAddedToWorld, &zCVob::ThisVobAddedToWorld_Union);

    void zCVob::ThisVobAddedToWorld_Union(zCWorld* worl)
    {

        THISCALL(Hook_zCVob_ThisVobAddedToWorld)(worl);
        if (!CEditorDoc::doc->bWorldLoaded)
            return;
        CMainFrame::mainframe->m_wndVobList.RedrawWindow();
        InsertVobToVirtualVobTree(this);
    }



    void RemoveVobToVirtualVobTree(zCVob* vob)
    {
        if (VirtualVobTree_Portals::GetInstance().IsIn(vob))
            VirtualVobTree_Portals::GetInstance().Remove(vob);
        else if (VirtualVobTree_Globals::GetInstance().IsIn(vob))
            VirtualVobTree_Globals::GetInstance().Remove(vob);
    }



    HOOK Hook_zCVob_ThisVobRemovedFromWorld PATCH(&zCVob::ThisVobRemovedFromWorld, &zCVob::ThisVobRemovedFromWorld_Union);

    void zCVob::ThisVobRemovedFromWorld_Union(zCWorld* worl)
    {

        if (CEditorDoc::doc->bWorldLoaded)
            RemoveVobToVirtualVobTree(this); //<<<<< 

        THISCALL(Hook_zCVob_ThisVobRemovedFromWorld)(worl);
    }
}