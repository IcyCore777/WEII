// Supported with union (c) 2020 Union team
// Union SOURCE file


using namespace GOTHIC_ENGINE;

	//Pick 3D object
	void PickSingle()
	{
		
		if (!ogame->world) return;
		POINT  cur;
		GetCursorPos(&cur);

		RECT rect;
		GetWindowRect(CEditorView::view->m_hWnd, &rect);

		cur.x -= rect.left;
		cur.y -= rect.top;


		CEditorView::view->GetWindowRect(&rect);

		float rw = rect.right - rect.left;
		float rh = rect.bottom - rect.top;

		float ax = (float)cur.x / rw * (float)zrenderer->vid_xdim;
		float ay = (float)cur.y / rh * (float)zrenderer->vid_ydim;

		ogame->world->PickScene(*ogame->camera, ax, ay,-1);
	}

	//Pick virtual cursor pos
	zVEC2 PickCursor(POINT  pos) {
		
		if (!ogame->world) return zVEC2();
		RECT rect;
		GetWindowRect(CEditorView::view->m_hWnd, &rect);
		pos.x -= rect.left;
		pos.y -= rect.top;

		float rw = rect.right - rect.left;
		float rh = rect.bottom - rect.top;

		float ax = (double)pos.x / (double)rw * (double)zrenderer->vid_xdim;
		float ay = (double)pos.y / (double)rh * (double)zrenderer->vid_ydim;

		return zVEC2(zPixelX(ax), zPixelY(ay));
	}
	
	AFX_API_EXPORT zTWE_ControllerEvents ControllerEvents;
