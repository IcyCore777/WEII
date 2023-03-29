// Supported with union (c) 2020 Union team
// Union HEADER file

using namespace GOTHIC_ENGINE;
	enum zEVobMotionMode;

	class  CHomeSettings : public CCmdTarget
	{
		CMFCRibbonSlider*		sliderMov;
		CMFCRibbonSlider*		sliderRot;
		CMFCRibbonSlider*		sliderSmooth;
		CMFCRibbonSlider*		sliderFps;
		CMFCRibbonComboBox*		gizmoHeading;
		CMFCRibbonEdit*			hour;
		CMFCRibbonEdit*			minute;
	public:
		zCBuffer*				copyBuffer;
		Common::Array<zCVob*>	cuttedVobs;
		uint32					objNum;
		bool					holdTime;
		bool					isCut;
		zEVobMotionMode			modeMove;
	public:
						CHomeSettings();
		void			InitData();
		void			ClearCut();
		void			SetMoveMode(zEVobMotionMode newmode);
		bool			UpdateTime(int& hour, int& min);
	public:
		afx_msg void	OnHoldtime();
		afx_msg void	OnUpdateTime(CCmdUI* pCmdUI);
		afx_msg void	OnUpdateHoldtime(CCmdUI* pCmdUI);
		afx_msg void	OnCopy();
		afx_msg void	OnCut();
		afx_msg void	OnUpdateCut(CCmdUI* pCmdUI);
		afx_msg void	OnPaste();
		afx_msg void	OnDelete();
		afx_msg void	OnUpdatePaste(CCmdUI* pCmdUI);
		afx_msg void	OnHour() {};
		afx_msg void	OnMinute() {};
		afx_msg void	OnMove();
		afx_msg void	OnRotate();
		afx_msg void	OnUpdateMove(CCmdUI* pCmdUI);
		afx_msg void	OnUpdateRotate(CCmdUI* pCmdUI);
		afx_msg void	OnMovspd();
		afx_msg void	OnRotspd();
		afx_msg void	OnSmooth();
		afx_msg void	OnGizmoHeading();
		afx_msg void	OnUpdateGizmoHeading(CCmdUI* pCmdUI);
		afx_msg void	OnUpdateSmooth(CCmdUI* pCmdUI);
		afx_msg void	OnUpdateRotspd(CCmdUI* pCmdUI);
		afx_msg void	OnUpdateMovspd(CCmdUI* pCmdUI);
		afx_msg void	OnFreecam();
		afx_msg void	OnUpdateFreecam(CCmdUI* pCmdUI);
		afx_msg void	OnWinrestore();
		afx_msg void	OnInsert();
		afx_msg void	OnUpdateCopy(CCmdUI* pCmdUI);
		afx_msg void	OnUpdateDelete(CCmdUI* pCmdUI);
		afx_msg void	OnUpdateInsert(CCmdUI* pCmdUI);
		afx_msg void	OnFps();
		afx_msg void	OnFindvobvis();
		afx_msg void	OnStop();
		DECLARE_MESSAGE_MAP()
		afx_msg void OnUpdateFindvobvis(CCmdUI* pCmdUI);
	};

