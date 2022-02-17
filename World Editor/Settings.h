// Supported with union (c) 2020 Union team
// Union HEADER file

namespace GOTHIC_ENGINE {
	class CSettings : public CCmdTarget
	{
	private:
		CMFCRibbonSlider* logLevel;
		CMFCRibbonSlider* farClip;
		CMFCRibbonComboBox* videoResulution;
	public:
		CSettings();
		void				InitData();
		afx_msg void		OnLoglevel();
		afx_msg void		OnFarclip();
		afx_msg void		OnVideo();
		afx_msg void		OnWire();
		afx_msg void		OnMatwire();
		afx_msg void		OnMat();
		afx_msg void		OnFlat();
		afx_msg void		OnUpdateFarclip(CCmdUI* pCmdUI);
		afx_msg void		OnUpdateLoglevel(CCmdUI* pCmdUI);
		afx_msg void		OnUpdateWire(CCmdUI* pCmdUI);
		afx_msg void		OnUpdateMatwire(CCmdUI* pCmdUI);
		afx_msg void		OnUpdateMat(CCmdUI* pCmdUI);
		afx_msg void		OnUpdateFlat(CCmdUI* pCmdUI);
		afx_msg void		OnFog();
		afx_msg void		OnUpdateFog(CCmdUI* pCmdUI);
		afx_msg void		OnIgnorepfx();
		DECLARE_MESSAGE_MAP()
	};
}