// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
	//#define EXPORT_RIBBON

#ifdef EXPORT_RIBBON
	AFX_API_EXPORT CMFCRibbonBar* __stdcall GetRibbonLink()
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		Common::cmd << "AFX_API_EXPORT CMFCRibbonBar* __stdcall GetRibbonLink()" << Common::endl;
		return &CMainFrame::mainframe->m_wndRibbonBar;
	};
#endif
}