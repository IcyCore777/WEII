// Supported with union (c) 2020 Union team
// Union HEADER file

namespace GOTHIC_ENGINE {
	class CProgressBar : public zCViewProgressBar
	{
		CMFCRibbonProgressBar* progress;
	public:
		CProgressBar() : zCViewProgressBar(0, 0, 0, 0, VIEW_ITEM) {  };
		virtual ~CProgressBar();
		virtual void Init(CMFCRibbonProgressBar*);
		virtual void HandleChange();
	};
}