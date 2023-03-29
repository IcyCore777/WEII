// Supported with union (c) 2020 Union team
// Union HEADER file


using namespace GOTHIC_ENGINE;

	enum zEVobMotionMode {
		zMM_NONE,
		zMM_MOVE,
		zMM_ROTATE,
		zMM_SCALE
	};

	enum zEVobMotionHeading {
		zMH_WORLD,
		zMH_LOCALE,
		zMH_VIEW,
		zMH_TRACK
	};

	enum zEVobCollisionMode {
		zVC_DEFAULT,
		zVC_DISABLED,
		zVC_ENABLED
	};

	enum zECamMotionMode {
		zCM_DISABLED,
		zCM_ENABLED
	};

	enum zEPickMode {
		zPM_DISABLED,
		zPM_VOBS,
		zPM_POLYS
	};

	enum zEPickMethod {
		zPM_CURSOR,
		zPM_CROSSHAIR
	};

	struct zTWE_ControllerEvents {
		uArray<zCVob*>      SelectedVobs;
		zCVob*				PickedVob;
		zCPolygon*			PickedPolygon;
		zEPickMode			PickMode;
		zEPickMethod		PickMethod;
		zEVobMotionMode		MotionMode;
		zEVobMotionHeading	MotionHeading;
		zEVobCollisionMode	CollisionMode;
		BOOL				CameraMoving;
		zVEC2				CursorPosition;
		float				GizmoScale;
		HWND				mainframe;
		float				movingSpeed;
		float				rotatingSpeed;
		float				smoothFactor;

		void ClearSelection() {
			SelectedVobs.Clear();
			PickedVob = NULL;
		}
	};

	extern AFX_API_EXPORT zTWE_ControllerEvents ControllerEvents;






	//Moving interface
	void PickSingle();
	zVEC2 PickCursor(POINT);