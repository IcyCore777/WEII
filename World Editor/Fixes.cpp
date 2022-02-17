// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
	HOOK Hook_CGameManager_InitScreen_Open					PATCH(&CGameManager::InitScreen_Open, &CGameManager::InitScreen_Open_Union);
#ifndef __G1A
	HOOK Hook_zCVob_Unarchive								AS_IF(&zCVob::Unarchive, &zCVob::Unarchive_Union, FALSE);
#endif
	HOOK Hook_zCVob_Archive									PATCH(&zCVob::Archive, &zCVob::Archive_Union);
	HOOK Hook_zCVobSound_Archive							PATCH(&zCVobSound::Archive, &zCVobSound::Archive_Union);
	HOOK Hook_zCVobSound_Unarchive							PATCH(&zCVobSound::Unarchive, &zCVobSound::Unarchive_Union);
	HOOK Hook_zCVobSound_ProcessZoneList					PATCH(&zCVobSound::ProcessZoneList, &zCVobSound::ProcessZoneList_Union);

	HOOK Hook_zCParticleFX_zCParticleFX						PATCH(&zCParticleFX::zCParticleFX, &zCParticleFX::zCParticleFX_Union);
	HOOK Hook_oCVisualFX_InitParser							PATCH(&oCVisualFX::InitParser, &oCVisualFX::InitParser_Union);
	HOOK Hook_CGameManager_HandleEvent						PATCH(&CGameManager::HandleEvent, &CGameManager::HandleEvent_Union);
	HOOK Hook_zCModel_TraceRay								PATCH(&zCModel::TraceRay, &zCModel::TraceRay_Union);
	HOOK Hook_zCAICamera_CheckUnderWaterFX					PATCH(&zCAICamera::CheckUnderWaterFX, &zCAICamera::CheckUnderWaterFX_Union);

	HOOK Hook_oCWorld_SaveWorld								PATCH(&oCWorld::SaveWorld, &oCWorld::SaveWorld_Union);
	HOOK Hook_zCTriggerWorldStart_PostLoad					PATCH(&zCTriggerWorldStart::PostLoad, &zCTriggerWorldStart::PostLoad_Union);
	HOOK Hook_zCPFXControler_PostLoad						PATCH(&zCPFXControler::PostLoad, &zCPFXControler::PostLoad_Union);
	HOOK Hook_zCWorld_zCWorld								PATCH(&zCWorld::zCWorld, &zCWorld::zCWorld_Union);
	HOOK Hook_zCRnd_D3D_XD3D_InitPerDX						PATCH(&zCRnd_D3D::XD3D_InitPerDX, &zCRnd_D3D::XD3D_InitPerDX_Union);
	HOOK Hook_zCCSCamera_Unarchive							PATCH(&zCCSCamera::Unarchive, &zCCSCamera::Unarchive_Union);



	HOOK Hook_zCScanDir_ScanX PATCH(&zCScanDir::ScanX, &zCScanDir::ScanX_Union);

	void zCScanDir::ScanX_Union()
	{
		
		sysEvent();
		return THISCALL(Hook_zCScanDir_ScanX)();
	}


	void CGameManager::InitScreen_Open_Union()
	{
		
		return;
	}


	void zCCSCamera::Unarchive_Union(zCArchiver& arch)
	{
		
		THISCALL(Hook_zCCSCamera_Unarchive)(arch);
		if (!arch.InProperties())
			SetSleeping(FALSE);
	}


#ifdef __G2A
	void zCCamTrj_KeyFrame::EndMovement()
	{
		
		zCVob::EndMovement(TRUE);
		if (cscam) cscam->Refresh();
	}
#endif


	int CGameManager::HandleEvent_Union(int key)
	{
		
		if (key == 0x01)
			return FALSE;
		return THISCALL(Hook_CGameManager_HandleEvent)(key);
	}


	zCParticleFX* zCParticleFX::zCParticleFX_Union()
	{
		
		THISCALL(Hook_zCParticleFX_zCParticleFX)();
#ifdef __G2A
		dontKillPFXWhenDone = TRUE;
#endif
		return this;
	}


#ifdef __G2A
	void zCVob::Archive_Union(zCArchiver& arc)
	{
		
		if (zDYNAMIC_CAST<zCParticleFX>(GetVisual()))
			zDYNAMIC_CAST<zCParticleFX>(GetVisual())->m_bVisualNeverDies = TRUE;

		THISCALL(Hook_zCVob_Archive)(arc);
	}
#endif // __G2A


	int zCAICamera::CheckUnderWaterFX_Union()
	{
		
		return FALSE;
	}

	/*void zCVob::SetVobPresetName(const zSTRING& presetName)
	{

		if (!vobPresetName)
			vobPresetName = zNEW(zSTRING(presetName));
		else	(*vobPresetName) = presetName;

	};*/


	int oCWorld::SaveWorld_Union(zSTRING const& filename, zCWorld::zTWorldSaveMode saveMode, int writeBinary, int _saveLevelMesh)
	{
		
		if (GetWayNet())
			if (GetWayNet()) GetWayNet()->UpdateVobDependencies();
		GetWayNet()->ClearVobDependencies();
		int result = THISCALL(Hook_oCWorld_SaveWorld)(filename, saveMode, writeBinary, _saveLevelMesh);
		if (GetWayNet()) GetWayNet()->CreateVobDependencies(this);

		return result;
	}


	int zCModel::TraceRay_Union(zVEC3 const& rayOrigin, zVEC3 const& ray, int traceFlags, zTTraceRayReport& report)
	{
		
		if (meshSoftSkinList.GetNum() > 0)
		{
			report.Clear();
			report.foundHit = TRUE;
			return TRUE;
		};
		return THISCALL(Hook_zCModel_TraceRay)(rayOrigin, ray, traceFlags, report);
	}


#ifndef __G1A
	void zCVob::Unarchive_Union(zCArchiver& arc)
	{
		
		zCObject::Unarchive(arc);
		ResetOnTimer();

		{
			int	pack = 0;
			arc.ReadInt("pack", pack);
			if (pack == 0)			UnarchiveVerbose(arc);
			else					UnarchivePacked(arc, pack);
		};

		if (arc.InSaveGame())
		{
			SetSleepingMode(zTVobSleepingMode(arc.ReadByte("sleepMode")));
			zREAL nextOnTimerRel = nextOnTimer;
			arc.ReadFloat("nextOnTimer", nextOnTimerRel);
			nextOnTimer = (nextOnTimerRel >= zREAL(+FLT_MAX)) ? nextOnTimerRel : (ztimer->totalTimeFloat + nextOnTimerRel);
			if (GetPhysicsEnabled())
				GetRigidBody()->Unarchive(arc);
		};

		if (!arc.InProperties())
		{
			RepairIllegalFloats(trafoObjToWorld);
			if (!trafoObjToWorld.IsUpper3x3Orthonormal())
				trafoObjToWorld.MakeOrthonormal();
			CorrectTrafo();
		};


		UpdateVisualDependencies(TRUE);


#ifdef __G2A
		const zREAL VOB_IGNORE_MIN_EXTEND = 10;
		zREAL minExtend = GetBBox3DWorld().GetMinExtent();
		if (minExtend > VOB_IGNORE_MIN_EXTEND) m_zBias = 0;
#endif
	}
#endif // !__G1A


	void zCTriggerWorldStart::PostLoad_Union()
	{
		
	}


	void oCVisualFX::InitParser_Union(void)
	{
		
		if (oCVisualFX::fxParser != NULL)
			return;
		Hook_oCVisualFX_InitParser();
	}


	void zCPFXControler::PostLoad_Union()
	{
		
		if (pfxStartOn)	OnTrigger(0, 0);
	}


	zCWorld* zCWorld::zCWorld_Union()
	{
		
		THISCALL(Hook_zCWorld_zCWorld)();
		addZonesToWorld = TRUE;
#ifdef __G2A
		SetWaveAnisEnabled(zoptions->ReadBool("SPACER", "zSpacerWaterAniEnabled", FALSE));
#endif
		return this;
	}


	void zCVobSound::Archive_Union(zCArchiver& arc)
	{
		
		THISCALL(Hook_zCVobSound_Archive)(arc);
		if (arc.InProperties())
		{
			StopSound();
			StartSound(TRUE);
		};
	}


	void zCVobSound::Unarchive_Union(zCArchiver& arc)
	{
		
		THISCALL(Hook_zCVobSound_Unarchive)(arc);
		if (arc.InProperties())
		{
			StopSound();
			StartSound(TRUE);
		};
	}

	int zCRnd_D3D::XD3D_InitPerDX_Union(long flags, int x, int y, int bpp, int id)
	{
		
		return THISCALL(Hook_zCRnd_D3D_XD3D_InitPerDX)(0, x, y, bpp, id);
	}


	void zCVobSound::ProcessZoneList_Union(class zCArraySort<zCZone*> const& zoneList, zCArraySort<zCZone*> const& zoneDeactivateList, zCWorld* homeWorld)
	{
		
		for (int i = 0; i < zoneList.GetNum(); i++)
		{
			zCVobSound* vobSound = ((zCVobSound*)zoneList[i]);

			if (vobSound->soundAllowedToRun)
			{
				if (vobSound->soundAutoStart)
				{
					vobSound->soundAutoStart = FALSE;
					vobSound->StartSound(TRUE);
				};
				vobSound->DoSoundUpdate(1.0f);
			}
			else {
				if (vobSound->soundIsRunning)
					vobSound->StopSound();
			};
		};
	}
}