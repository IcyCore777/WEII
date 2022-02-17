// Supported with union (c) 2020 Union team
// Union SOURCE file
extern void* operator new (size_t sz);
extern void  operator delete (void* mem);
void* operator new[](size_t sz);
extern void operator delete[](void* mem);
namespace GOTHIC_ENGINE {



	void* Hooked_operator_new_MFC(size_t sz) {
		return shi_malloc(sz);
	}

	void Hooked_operator_delete_MFC(void* mem) {
		shi_free(mem);
	}



	void _Patch_operator_new_MFC() {
		void* (*proc)(size_t sz) = operator new[];
		static CInvoke<void* (*)(size_t sz)> hook(proc, &Hooked_operator_new_MFC);
	}


	void _Patch_operator_delete_MFC() {
		void (*proc)(void*) = operator delete[];
		static CInvoke<void* (*)(size_t sz)> hook(proc, &Hooked_operator_delete_MFC);
	}

	void Patch_operator_new_MFC() {
		void* (*proc)(size_t sz) = operator new;
		static CInvoke<void* (*)(size_t sz)> hook(proc, &Hooked_operator_new_MFC);
	}


	void Patch_operator_delete_MFC() {
		void (*proc)(void*) = operator delete;
		static CInvoke<void* (*)(size_t sz)> hook(proc, &Hooked_operator_delete_MFC);
	}

}