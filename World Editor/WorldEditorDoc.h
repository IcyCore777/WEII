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

	//-----------------------------
	//
	//+++++++++++++++++++++++++++++
	inline zMAT4 CalcMatrixFromPoly(zCPolygon* poly)
	{

		zMAT4 mat = Alg_Identity3D();
		if (!poly) return mat;

		zCVertex* v1 = 0;
		zCVertex* v2 = 0;
		zREAL		bestDist = 0;

		for (int i = 0; i < poly->polyNumVert; i++) {
			zCVertex* _v1 = poly->vertex[i];
			zCVertex* _v2 = poly->vertex[(i + 1) % poly->polyNumVert];
			if ((_v1->position - _v2->position).Length() > bestDist) {
				v1 = _v1;
				v2 = _v2;
				bestDist = (_v1->position - _v2->position).Length();
			};
		};

		zVEC3	right, up, at;
		up = poly->GetNormal();
		right = (v2->position - v1->position);
		right.Normalize();
		at = up ^ right;
		mat.SetAtVector(at);
		mat.SetUpVector(up);
		mat.SetRightVector(right);
		mat.SetTranslation(v1->position);

		return mat;
	};

	inline zMAT4 CalcMatrixFromVob(zCVob* vob)
	{

		zMAT4 mat = Alg_Identity3D();
		if (!vob) return mat;
		mat = vob->trafoObjToWorld;

		return mat;
	};

	inline zBOOL CalcMatrixDifference(zMAT4& mat1, zMAT4& mat2, zMAT4& res)
	{

		res = Alg_Identity3D();

		// Testen, ob Matrizen =FCberhaupt verschieden sind
		zBOOL diff = FALSE;
		if (!mat1.GetAtVector().IsEqualEps(mat2.GetAtVector()))	diff = TRUE;
		if (!mat1.GetUpVector().IsEqualEps(mat2.GetUpVector()))	diff = TRUE;
		if (!mat1.GetRightVector().IsEqualEps(mat2.GetRightVector()))	diff = TRUE;
		zREAL trans = (mat2.GetTranslation() - mat1.GetTranslation()).Length();
		if (trans > 0.1F)	diff = TRUE;		// 1mm

		if (!diff) {
			return FALSE;
		};
		res = mat2 * mat1.Inverse();
		return TRUE;
	};

}