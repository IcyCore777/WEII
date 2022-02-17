// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
	CProgressBar::~CProgressBar()
	{
		
	}

	void CProgressBar::Init(CMFCRibbonProgressBar* parent)
	{
		
		zCViewProgressBar::Init();
		progress = parent;
	}

	void CProgressBar::HandleChange()
	{
		
		progress->SetPos(percent);
		sysEvent();
	}
}