// Supported with union (c) 2020 Union team
// Union HEADER file

namespace GOTHIC_ENGINE {
#ifdef __G2A
	void __cdecl sysEvent()
	{
		zCall(0x005053E0);
	}
#endif
#ifdef __G1A
	void __cdecl sysEvent()
	{
		zCall(0x00509530);
	}
#endif
#ifdef __G1
	void __cdecl sysEvent()
	{
		zCall(0x004F6AC0);
	}
#endif
}