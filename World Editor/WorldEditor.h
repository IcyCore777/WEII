// Supported with union (c) 2020 Union team
// Union HEADER file
#include "resource.h"


namespace GOTHIC_ENGINE {

	class CEditorApp : public CWinAppEx
	{
	public:
		CEditorApp() ;
		bool	bInitialised;
		bool	bReady;
		bool	bDrawing;
	public:
		int Process();
		virtual BOOL InitInstance();
		virtual int ExitInstance();

		UINT  m_nAppLook;
		BOOL  m_bHiColorIcons;

		virtual void PreLoadState();
		virtual void LoadCustomState();
		virtual void SaveCustomState();
		void EnableDrawing(bool enable = true);
		afx_msg void OnAppAbout();
		DECLARE_MESSAGE_MAP()
		
		virtual BOOL OnIdle(LONG lCount);
	};

	extern CEditorApp theApp;

}