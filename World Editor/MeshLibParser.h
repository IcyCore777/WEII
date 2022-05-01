// Supported with union (c) 2020 Union team
// Union HEADER file

namespace GOTHIC_ENGINE {
	struct Group {
		uString			Name;
		uArray<uString> FileNames;
	};

	enum ELibMode
	{
		MSH = 2,
		ASC,
		MDS,
		MMS,
		ITEM
	};

	struct zTMeshRenderer {
		zCVob*			vob;
		zCCamera*		camera;
		zCWorld*		world;
		zCView*			view;
		zCParticleFX*	pfx;
		zTMeshRenderer();
		void SetVisual(zSTRING, int id);
		void Render();
		void SetPos(int x, int y);
	};
}