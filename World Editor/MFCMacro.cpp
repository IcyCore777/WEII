// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
#define GetButton(id)			dynamic_cast<CMFCRibbonButton*>( GetMainFrame()->GetRibbonBar()->FindByID(id))
#define GetProgressBar(id)		dynamic_cast<CMFCRibbonProgressBar*>(  GetMainFrame()->GetRibbonBar()->FindByID(id))
#define GetMainFrame()			dynamic_cast<CMainFrame*>(AfxGetMainWnd ()) 
#define GetColorButton(id)		dynamic_cast<CMFCRibbonColorButton*>( GetMainFrame()->GetRibbonBar()->FindByID(id))
#define GetEdit(id)				dynamic_cast<CMFCRibbonEdit*>(  GetMainFrame()->GetRibbonBar()->FindByID(id))
#define GetEditInt(id)			uString(dynamic_cast<CMFCRibbonEdit*>(  GetMainFrame()->GetRibbonBar()->FindByID(id))->GetEditText()).ToInt32()
#define GetSlider(id)			dynamic_cast<CMFCRibbonSlider*>(  GetMainFrame()->GetRibbonBar()->FindByID(id))
#define GetCheck(id)			dynamic_cast<CMFCRibbonCheckBox*>(  GetMainFrame()->GetRibbonBar()->FindByID(id))
#define GetCombo(id)			dynamic_cast<CMFCRibbonComboBox*>(  GetMainFrame()->GetRibbonBar()->FindByID(id))
#define SetPaneText(id,text)	dynamic_cast<CMFCRibbonStatusBarPane*>(  GetMainFrame()->m_wndStatusBar.GetElement(id))->SetText(text); CMainFrame::mainframe->m_wndStatusBar.ForceRecalcLayout()
#define CheckBox(id)			dynamic_cast<CMFCRibbonCheckBox*>(CMainFrame::mainframe->GetRibbonBar()->FindByID(id))
#define GetComboText(id)		GetCombo(id)->GetItem(GetCombo(id)->GetCurSel())



#define FILEVIEW				CMainFrame::mainframe->m_wndVobList.m_wndVobList

	//#define GetSelectedVob()		CMainFrame::mainframe->m_wndProperties.vob

	//#define OutputBuild(str)		CMainFrame::mainframe->m_wndOutput.m_wndOutputBuild.SetTopIndex(CMainFrame::mainframe->m_wndOutput.m_wndOutputBuild.Insert(str))
	//#define ClearOutput()			CMainFrame::mainframe->m_wndOutput.m_wndOutputBuild.ResetContent();
	//#define ClearDebug()			CMainFrame::mainframe->m_wndOutput.m_wndOutputDebug.ResetContent();
	//#define ClearFind()				CMainFrame::mainframe->m_wndOutput.m_wndOutputFind.ResetContent();
	//#define OutputReport(str)		CMainFrame::mainframe->m_wndOutput.m_wndOutputSpy.SetTopIndex(CMainFrame::mainframe->m_wndOutput.m_wndOutputSpy.AddString(str,RGB(0,0,0)))
	//#define OutputDebug(str)		CMainFrame::mainframe->m_wndOutput.m_wndOutputDebug.SetTopIndex(CMainFrame::mainframe->m_wndOutput.m_wndOutputDebug.AddString(str,RGB(0,255,0)))
	//#define OutputError(str)		CMainFrame::mainframe->m_wndOutput.m_wndOutputDebug.SetTopIndex(CMainFrame::mainframe->m_wndOutput.m_wndOutputDebug.AddString(str,RGB(255,0,0)))
	//#define OutputWarning(str)		CMainFrame::mainframe->m_wndOutput.m_wndOutputDebug.SetTopIndex(CMainFrame::mainframe->m_wndOutput.m_wndOutputDebug.AddString(str,RGB(255,180,0)))
	//#define Camera					ogame->GetCameraVob()
	//#define SelectedVob				ControllerEvents.PickedVob
	//#define UICheckMesh				if(!isMergeMeshMode && !isWorldLoaded) {pCmdUI->Enable(FALSE); return;}
	//
	//#define WorldCheck				if(!isWorldLoaded) return;
	//#define UIWorldCheck			if(!isWorldLoaded) {pCmdUI->Enable(FALSE); return;}
	//#define OleVar					COleVariant
	//#define AddProp					new CMFCPropertyGridProperty
	//#define AddColor				new CMFCPropertyGridColorProperty
	//#define CheckSel 				if (ControllerEvents.SelectedVobs.GetNum() == 0) pCmdUI->Enable(FALSE);	else pCmdUI->Enable();

#define Log(str)				Common::cmd << str << Common::endl
}