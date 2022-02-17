// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
#ifdef __G2A
	HOOK Hook_zERROR_Report AS(0x0044C8D0, &zERROR::Report_Union);
#endif
#ifdef __G1
	HOOK Hook_zERROR_Report AS(0x00448250, &zERROR::Report_Union);
#endif
#ifdef __G1A
	HOOK Hook_zERROR_Report AS(0x0044E2B0, &zERROR::Report_Union);
#endif
	HOOK Hook_zERROR_Init PATCH(&zERROR::Init, &zERROR::Init_Union);
	HOOK Hook_zERROR_Separator PATCH(&zERROR::Separator, &zERROR::Separator_Union);


#define zERR_MESSAGE(level, flag, text)		((level)<=zerr->GetFilterLevel()) ? zerr->Report(zERR_TYPE_INFO,  0, text, level, flag, __LINE__,  __FILE__, 0 ) : zERR_NONE
#define zERR_WARNING(text)					zerr->Report(zERR_TYPE_WARN,  0, text, 0, 0, __LINE__,  __FILE__, 0 )

#define zERR_REPORT(text)		            zerr->Report(zERR_TYPE_INFO,  0, text, 3, 0, __LINE__,  __FILE__, 0 )

#define zERR_FAULT(text)					zerr->Report(zERR_TYPE_FAULT, 0, text, 0, 0, __LINE__,  __FILE__, 0 )
#define zERR_FATAL(text)					zerr->Report(zERR_TYPE_FATAL, 0, text, -1, 0, __LINE__,  __FILE__, 0 )
}