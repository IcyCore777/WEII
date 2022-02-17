// Supported with union (c) 2020 Union team
// Union HEADER file

namespace GOTHIC_ENGINE {

	
	enum mlMODE
	{
		PRESETS,
		MESHLIB
	};

	class CMeshListBox : public CListBox
	{
	public:
						CMeshListBox();
		virtual			~CMeshListBox();
	public:
		int   m_MovingIndex;
		int   m_MoveToIndex;
		BOOL  m_Captured;
		DWORD m_Interval;    //scroll speed when an item is dragged above or below the 
		mlMODE			mode;
	public:
		virtual void	DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
		virtual void	DeleteItem(LPDELETEITEMSTRUCT lpDeleteItemStruct);
		virtual int		CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct);
		virtual void	MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);

		void			LoadPreset(zSTRING name);
		void			AddIcon(CString iconName, CString preset);
		zCPresetView*	presets;
	public:
		
		int				num_columns;
		int				num_pix_space;
		int				num_pix_mat;
		void			UpdateList();
		CString			filterName;
		void			CreateList(CString filter, ELibMode type);
		HBITMAP*		selIcon;
		Common::Map<HBITMAP*, CString*>		iconList;
		Common::string	GetPreset();
		Common::string	GetSelectedMesh();
	public:
		int topLine;
		int selectedLine;
		DECLARE_MESSAGE_MAP()
		afx_msg void	OnLButtonDown(UINT nFlags, CPoint point);
		afx_msg void	OnSize(UINT nType, int cx, int cy);

		afx_msg void	OnLButtonDblClk(UINT nFlags, CPoint point);
		afx_msg void	OnSelchange();
	};
}