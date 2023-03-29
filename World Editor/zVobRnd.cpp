// Supported with union (c) 2020 Union team
// Union SOURCE file

using namespace GOTHIC_ENGINE;
	// Add your code here . . .

	zCVob* cur_vob = NULL;

	void ScaleVob(zCVob* pVob, float scale)
	{

		if (pVob && pVob->GetVisual())
		{

			zTBBox3D bbox = pVob->GetVisual()->GetBBox3D();
			float length = (bbox.maxs - bbox.mins).Length();
			//float vobSizeMult = SafeDiv(1.0f, length) * 80.0f;
			//print.PrintRed(A vobSizeMult);

			zMAT4& trafo = pVob->trafoObjToWorld;
			zVEC3 scaleVec = zVEC3(0.5f, 0.5f, 0.5f);

			//trafo.MakeIdentity();
			trafo.PreScale(scaleVec);
			//	trafo = Alg_Scaling3D(scaleVec) * trafo;
			//trafo.MakeOrthonormal();
			pVob->bbox3D.Scale(scaleVec);
		}
	}




	void AddVobToRender(zSTRING visual, bool isItem = false)
	{
		if (!ogame || !ogame->GetCamera() || !ogame->GetCamera()->connectedVob)
		{
			return;
		}

		zCVob* camVob = ogame->GetCamera()->connectedVob;

		if (cur_vob)
		{
			ogame->GetWorld()->RemoveVob(cur_vob);
			zRELEASE(cur_vob);
			//print.PrintRed("—ущ. воб удален!");
		}

		if (visual.Length() == 0)
		{
			cur_vob = NULL;
			return;
		}

		zCVob* pVob = NULL;

		if (isItem)
		{
			zSTRING itemName = zSTRING(visual).Upper();

			int index = parser->GetIndex(itemName);

			cur_vob = dynamic_cast<oCItem*>((zCVob*)ogame->GetGameWorld()->CreateVob(zVOB_TYPE_ITEM, index));
			pVob = cur_vob;
		}
		else
		{
			cur_vob = NULL;
			pVob = zNEW(zCVob);
			pVob->SetVisual(visual);
			pVob->SetShowVisual(TRUE);
			//pVob->SetVobName(ToStr "VobPreview_" + visual);

			pVob->SetCollDetStat(FALSE);
			pVob->SetCollDetDyn(FALSE);
		}


		if (pVob)
		{
			pVob->SetSleeping(FALSE);
			pVob->SetDrawBBox3D(FALSE);
			pVob->dontWriteIntoArchive = true;

			pVob->ignoredByTraceRay = TRUE;
			//theApp.nextInsertBlocked = true;
			ogame->GetWorld()->AddVob(pVob);

			pVob->CalcLightSampleAtOrigin();
		}
		else
		{
		//	print.PrintRed("Add to Render fail: " + A visual);
			return;
		}



		if (!pVob->GetVisual())
		{
			ogame->GetWorld()->RemoveVob(pVob);
			pVob->Release();
			cur_vob = NULL;
			//theApp.nextInsertBlocked = false;
			return;
		}

		zTBBox3D bbox = pVob->GetVisual()->GetBBox3D();
		float length = (bbox.maxs - bbox.mins).Length();
		float vobSizeMult = SafeDiv(1.0f, length) * 80.0f;

		zMAT4& trafo = pVob->GetNewTrafoObjToWorld();

		//print.PrintRed(zSTRING(vobSizeMult));

		if (!cur_vob)
		{
			trafo.MakeIdentity();
			trafo.PreScale(vobSizeMult);
		}




		//pVob->Release();

		if (visual.Search("SWORD", 1) != -1)
		{
			//pVob->RotateLocalX(90);
		}

		cur_vob = pVob;

		//theApp.nextInsertBlocked = false;

	}

	void RenderVobsLoop()
	{

		if (!ogame->GetCamera() || !ogame->GetWorld())
		{
			return;
		}

		zCVob* camVob = ogame->GetCamera()->connectedVob;



		if (cur_vob)
		{
			cur_vob->SetPositionWorld(camVob->GetPositionWorld()
				- camVob->GetAtVectorWorld().Cross(zVEC3(0, -1, 0) * 150)
				+ camVob->GetAtVectorWorld() * 200
				+ zVEC3(0, 25, 0)
			);


			float speedRot = 5 / 70 * ztimer->frameTimeFloat;

			if (cur_vob->GetVobName().Search(".TGA", 1) == -1)
			{
				cur_vob->RotateLocalY(speedRot);
				//currentVob->RotateLocalZ(0.04 * ztimer->frameTimeFloat);
			}
			else
			{
				cur_vob->SetHeadingWorld(camVob);
				cur_vob->RotateLocalY(180);
				cur_vob->ResetXZRotationsWorld();
			}

		}
	}
