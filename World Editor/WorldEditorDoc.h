// Supported with union (c) 2020 Union team
// Union HEADER file

namespace GOTHIC_ENGINE {

	enum zSAVE_TYPE { SAVE_ZEN, SAVE_ZEN_UC, SAVE_ZEN_BIN, SAVE_3DS };
	enum zLOAD_TYPE { LOAD_ZEN, LOAD_ZEN_UC, LOAD_3DS };


	class CEditorDoc : public CDocument
	{
	protected:
		CEditorDoc() ;
		DECLARE_DYNCREATE(CEditorDoc)
	public:
		struct TFlagMem
		{
			zBOOL	dynCol;
			zBOOL	statCol;
			zCVob* vob;

			TFlagMem() : vob(0), dynCol(FALSE), statCol(FALSE) {};
			TFlagMem(zCVob* _vob) { Set(_vob); };
			void Set(zCVob* _vob)
			{
				vob = _vob;
				dynCol = vob->GetCollDetDyn();
				statCol = vob->GetCollDetStat();
				vob->SetCollDet(FALSE);
			};
			void Put()
			{
				vob->SetCollDetDyn(dynCol);
				vob->SetCollDetStat(statCol);
				vob = 0;
			};
		};



		virtual BOOL OnNewDocument();
		virtual void Serialize(CArchive& ar);

		bool InsertVob(zCVob* vob, zVEC3* pos, zVEC3* dir, zCVob* parent);

		void RemoveVob(zCVob* vob);

		void VobTreeCorrX(zCTree<zCVob>* node, zCArray<TFlagMem>& flagMem);

		void VobTreeCorrection(CString& matName);

		void HandleWorldAfterLoad();

		zCVob* GetParentVob(zCVob* candidate = NULL);

		void InitCamera(zVEC3 pos);
		BOOL isWorldLoaded() {
			if (bWorldLoaded) return TRUE; else return FALSE;
		};
	public:
		virtual ~CEditorDoc();
	public:
		BOOL				bRemoveAsk;
		BOOL				bChilds;
		static CEditorDoc*  doc;
		BOOL				bWorldLoaded;
	

		// Созданные функции схемы сообщений
	protected:
		DECLARE_MESSAGE_MAP()
	public:
		virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
		afx_msg void OnParsemsh();
		virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
		afx_msg void OnUpdateFileNew(CCmdUI* pCmdUI);
		afx_msg void OnUpdateFileSaveAs(CCmdUI* pCmdUI);
		afx_msg void OnFileNew();
		afx_msg void OnTest();
	};

}