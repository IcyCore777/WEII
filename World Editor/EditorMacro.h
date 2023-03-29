// Supported with union (c) 2020 Union team
// Union HEADER file

using namespace GOTHIC_ENGINE;
#define UICheck					if(!CEditorDoc::doc->bWorldLoaded) {pCmdUI->Enable(FALSE); return;}
#define CheckSel 				if (ControllerEvents.SelectedVobs.GetNum() == 0) pCmdUI->Enable(FALSE);	else pCmdUI->Enable();
#define _camera					ogame->camVob


#define GetSelectedVob()		CMainFrame::mainframe->m_wndProperties.vob



#define BUILDTAB	0
#define DEBUGTAB	1
#define FINDTAB		2
#define SPYTAB		3
#define TV_IT		1
#define TV_ITSEL	0
