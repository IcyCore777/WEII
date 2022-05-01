// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
	const int TEX_PREVIEW_SIZE = 64;

	CMeshListBox::CMeshListBox()
	{
		mode = MESHLIB;
		num_columns = 0;
		num_pix_space = 6;
		num_pix_mat = TEX_PREVIEW_SIZE + num_pix_space;
		selectedLine = -1;
		topLine = -1;
		presets = NULL;
		selIcon = NULL;
	}

	CMeshListBox::~CMeshListBox()
	{
		
	}


	BEGIN_MESSAGE_MAP(CMeshListBox, CListBox)
		ON_WM_LBUTTONDOWN()
		ON_WM_SIZE()
		ON_CONTROL_REFLECT(LBN_SELCHANGE, OnSelchange)
		ON_WM_LBUTTONDBLCLK()
	END_MESSAGE_MAP()


	void CMeshListBox::DrawItem(LPDRAWITEMSTRUCT lpDIS)
	{
		
		RECT rc;
		GetClientRect(&rc);


		CDC* pDC = CDC::FromHandle(lpDIS->hDC);
		rc = lpDIS->rcItem;

		if (lpDIS->itemID < 0) return;
		int line = GetItemData(lpDIS->itemID);
		if (line < 0) return;

		int x = rc.left;
		int y = rc.top;


		HBITMAP* icon = NULL;
		CString matname;


		for (int column = 1; column <= num_columns; column++)
		{
			icon = 0;

			uint listIndex = (line - 1) * num_columns + (column - 1);
			if (listIndex >= 0 && listIndex < iconList.GetNum())
				icon = iconList.GetArray().operator[](listIndex).GetKey();

			if (icon)
			{

				int width = TEX_PREVIEW_SIZE;
				int height = TEX_PREVIEW_SIZE;


				rc.left = x + (width + num_pix_space) * (column - 1);
				rc.right = x + (width + num_pix_space) * (column - 1) + num_pix_mat;

				if (icon == selIcon)
				{
					pDC->FillSolidRect(&rc, RGB(0, 180, 180));
					pDC->SetBkColor(RGB(0, 180, 180));
					pDC->SetTextColor(RGB(255, 255, 255));
				}
				else
				{
					pDC->FillSolidRect(&rc, RGB(255, 255, 255));
					pDC->SetBkColor(RGB(255, 255, 255));
					pDC->SetTextColor(RGB(0, 0, 0));
				}


				CFont oFont; oFont.CreateFont(12, 0, 0, 0, 400, false, false,
					0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
					CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
					DEFAULT_PITCH | FF_SWISS, "Arial");

				pDC->ExtTextOut(rc.left + 2, y + height, ETO_CLIPPED, &rc, *iconList.GetArray().operator[](listIndex).GetValue(), NULL);


				CBitmap bitmap;
				bitmap.Attach(CopyImage(icon, 0, 0, 0, 0));

				CDC dcMemory;
				dcMemory.CreateCompatibleDC(pDC);
				dcMemory.SelectObject(bitmap);
				pDC->BitBlt(x + (width + num_pix_space) * (column - 1) + 2, y + 2, 64, 64, &dcMemory, 0, 0, SRCCOPY);
			}
		}
	}


	void CMeshListBox::DeleteItem(LPDELETEITEMSTRUCT lpD)
	{
		
		CListBox::DeleteItem(lpD);
	}


	int CMeshListBox::CompareItem(LPCOMPAREITEMSTRUCT lpC)
	{
		
		return 0;
	}


	void CMeshListBox::MeasureItem(LPMEASUREITEMSTRUCT lpM)
	{
		
		lpM->itemWidth = num_pix_mat * num_columns;
		lpM->itemHeight = 80;
	}


	void CMeshListBox::LoadPreset(zSTRING name)
	{
		
		iconList.Clear();
		presets = CMainFrame::mainframe->presetLib.GetNewView(name);

		zCObjPreset* presetObj;
		zCVob* object = NULL;
		CString presetname;
		int count = presets->GetNumInList();
		for (int i = 0; i < count; i++)
		{
			presetObj = (*presets)[i];
			if (presetObj)
			{
				object = (zCVob*)presetObj->obj;
				presetname = CString(presetObj->presetName.ToChar());
				if (object->visual)
					AddIcon(uString(((object->GetVisual()) ? object->GetVisual()->GetVisualName() : "") + ".jpg").ToChar(), presetname);
				else
					AddIcon("INVALID_SOURCE_FILE.3DS.JPG", presetname);

			}
		};
		UpdateList();
	}


	void CMeshListBox::AddIcon(CString iconName, CString preset)
	{
		
		CImage* cimg = new CImage;
		cimg->Load(uString("_work\\Data\\Textures\\meshThumbs\\" + iconName));
		HBITMAP* bmp = (HBITMAP*)CopyImage(cimg->Detach(), IMAGE_BITMAP, 64, 64, 0);

		iconList.Insert(bmp, new CString(preset));
	}

	void CMeshListBox::CreateList(CString filter, ELibMode type)
	{
		
		iconList.Clear();
		if (type == MSH)
			for (size_t i = 0; i < Groups3ds.GetNum(); i++)
			{
				if (Groups3ds[i]->Name == filter.GetBuffer(0))
				{
					for (size_t r = 0; r < Groups3ds[i]->FileNames.GetNum(); r++)
					{
						if (Groups3ds[i]->FileNames[r].IsEmpty()) continue;

						CImage* cimg = new CImage;

						cimg->Load(uString("_work\\Data\\Textures\\meshThumbs\\" + Groups3ds[i]->FileNames[r]));
						HBITMAP* bmp = (HBITMAP*)CopyImage(cimg->Detach(), IMAGE_BITMAP, 64, 64, 0);
						iconList.Insert(bmp, new CString(Groups3ds[i]->FileNames[r].ToChar()));
					}
				}
			}
		if (type == ASC)
			for (size_t i = 0; i < GroupsASC.GetNum(); i++)
			{
				if (GroupsASC[i]->Name == filter.GetBuffer(0))
				{
					for (size_t r = 0; r < GroupsASC[i]->FileNames.GetNum(); r++)
					{
						if (GroupsASC[i]->FileNames[r].IsEmpty()) continue;

						CImage* cimg = new CImage;

						cimg->Load(uString("_work\\Data\\Textures\\meshThumbs\\" + GroupsASC[i]->FileNames[r]));
						HBITMAP* bmp = (HBITMAP*)CopyImage(cimg->Detach(), IMAGE_BITMAP, 64, 64, 0);
						iconList.Insert(bmp, new CString(GroupsASC[i]->FileNames[r].ToChar()));
					}
				}
			}
		if (type == MDS)
			for (size_t i = 0; i < GroupsMDS.GetNum(); i++)
			{
				if (GroupsMDS[i]->Name == filter.GetBuffer(0))
				{
					for (size_t r = 0; r < GroupsMDS[i]->FileNames.GetNum(); r++)
					{
						if (GroupsMDS[i]->FileNames[r].IsEmpty()) continue;

						CImage* cimg = new CImage;

						cimg->Load(uString("_work\\Data\\Textures\\meshThumbs\\" + GroupsMDS[i]->FileNames[r]));
						HBITMAP* bmp = (HBITMAP*)CopyImage(cimg->Detach(), IMAGE_BITMAP, 64, 64, 0);
						iconList.Insert(bmp, new CString(GroupsMDS[i]->FileNames[r].ToChar()));
					}
				}
			}
		if (type == MMS)
			for (size_t i = 0; i < GroupsMMS.GetNum(); i++)
			{
				if (GroupsMMS[i]->Name == filter.GetBuffer(0))
				{
					for (size_t r = 0; r < GroupsMMS[i]->FileNames.GetNum(); r++)
					{
						if (GroupsMMS[i]->FileNames[r].IsEmpty()) continue;

						CImage* cimg = new CImage;

						cimg->Load(uString("_work\\Data\\Textures\\meshThumbs\\" + GroupsMMS[i]->FileNames[r]));
						HBITMAP* bmp = (HBITMAP*)CopyImage(cimg->Detach(), IMAGE_BITMAP, 64, 64, 0);
						iconList.Insert(bmp, new CString(GroupsMMS[i]->FileNames[r].ToChar()));
					}
				}
			}
		if (type == ITEM)
			for (size_t i = 0; i < GroupsITEMS.GetNum(); i++)
			{
				if (GroupsITEMS[i]->Name == filter.GetBuffer(0))
				{
					for (size_t r = 0; r < GroupsITEMS[i]->FileNames.GetNum(); r++)
					{
						if (GroupsITEMS[i]->FileNames[r].IsEmpty()) continue;

						CImage* cimg = new CImage;

						cimg->Load(uString("_work\\Data\\Textures\\meshThumbs\\" + GroupsITEMS[i]->FileNames[r]));
						HBITMAP* bmp = (HBITMAP*)CopyImage(cimg->Detach(), IMAGE_BITMAP, 64, 64, 0);
						iconList.Insert(bmp, new CString(GroupsITEMS[i]->FileNames[r].ToChar()));
					}
				}
			}
		//if (type == PFX)
		//	for (size_t i = 0; i < GroupsPfx.GetNum(); i++)
		//	{
		//		if (GroupsPfx[i]->Name == filter.GetBuffer(0))
		//		{
		//			for (size_t r = 0; r < GroupsPfx[i]->FileNames.GetNum(); r++)
		//			{
		//				CImage* cimg = new CImage;

		//				cimg->Load(uString("_work\\Data\\Textures\\pfx\\" + GroupsPfx[i]->FileNames[r]));
		//				HBITMAP* bmp = (HBITMAP*)CopyImage(cimg->Detach(), IMAGE_BITMAP, 64, 64, 0);
		//				iconList.Insert(bmp, GroupsPfx[i]->FileNames[r]);
		//			}
		//		}
		//	}
		UpdateList();
	}


	void CMeshListBox::OnLButtonDown(UINT nFlags, CPoint point)
	{
		
		BOOL out;
		int line = ItemFromPoint(point, out);
		if (out || line < 0) return;
		line++;

		int matIndex = (num_pix_mat != 0) ? (line - 1) * num_columns + int(point.x / num_pix_mat) : -1;

		if (matIndex >= 0)
		{

			selIcon = iconList.GetArray().operator[](matIndex).GetKey();

			CListBox::OnLButtonDown(nFlags, point);
		}
	}

	void CMeshListBox::OnSize(UINT nType, int cx, int cy)
	{
		
		int width = cx - 5;
		int old_num_columns = num_columns;
		num_columns = int(width / TEX_PREVIEW_SIZE);
		if (num_columns < 1) num_columns = 1;
		num_pix_space = (num_columns == 1 ? 0 : int((width - (num_columns * TEX_PREVIEW_SIZE)) / (num_columns - 1)));
		num_pix_mat = num_pix_space + TEX_PREVIEW_SIZE;

		CListBox::OnSize(nType, cx, cy);
		if (num_columns != old_num_columns)
			UpdateList();
		RedrawWindow();
	}

	uString CMeshListBox::GetPreset()
	{
		
		if (selIcon && !iconList[selIcon].IsNull())
			return uString(*iconList[selIcon].GetValue());
		else
			return uString("");
	}

	uString CMeshListBox::GetSelectedMesh()
	{
		
		if (selIcon && !iconList[selIcon].IsNull())
		{
			if (CMainFrame::mainframe->m_wndMeshLib.mode == MSH)
				iconList[selIcon].GetValue()->Replace(".3DS.JPG", ".3DS");
			if (CMainFrame::mainframe->m_wndMeshLib.mode == ASC)
				iconList[selIcon].GetValue()->Replace(".ASC.JPG", ".ASC");
			if (CMainFrame::mainframe->m_wndMeshLib.mode == MDS)
				iconList[selIcon].GetValue()->Replace(".MDS.JPG", ".MDS");
			if (CMainFrame::mainframe->m_wndMeshLib.mode == MMS)
				iconList[selIcon].GetValue()->Replace(".MMS.JPG", ".MMS");

			return uString(*iconList[selIcon].GetValue());
		}
		else
			return uString("INVALID_SOURCE_FILE.3DS");
	}


	void CMeshListBox::UpdateList()
	{
		
		int num_lines = int(iconList.GetNum() / num_columns);
		if (iconList.GetNum() % num_columns != 0) num_lines++;

		ResetContent();
		int index;
		for (int i = 1; i <= num_lines; i++)
		{
			index = InsertString(-1, "Line " + CString(zSTRING(i).ToChar()));
			if ((index != LB_ERR) && (index != LB_ERRSPACE)) { SetItemData(index, i); }
		}

		if (topLine >= 0)
			SetTopIndex(topLine);
		RedrawWindow();
	}


	void CMeshListBox::OnSelchange()
	{
		
		selectedLine = GetCurSel();
		topLine = GetTopIndex();
		RedrawWindow();
	}

	void CMeshListBox::OnLButtonDblClk(UINT nFlags, CPoint point)
	{
		//WorldCheck
		CEditorView::view->OnObjInsert();
		CListBox::OnLButtonDblClk(nFlags, point);
	}

}