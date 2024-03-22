// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include <iostream>

class cv {
public:
	class Mat {

	};
};
namespace YoudaoOCR
{
	
	_declspec(dllexport) void __cdecl release_model(void);
	_declspec(dllexport) char* __cdecl ocr_recognize_line(class cv::Mat const&, bool);
	_declspec(dllexport) void __cdecl release_text(char**);
	_declspec(dllexport) bool __cdecl recog_model_init(char const*, int, int, bool);
	_declspec(dllexport) bool __cdecl switch_to_language(int);
};


void __cdecl YoudaoOCR::release_model(void) {
	MessageBoxA(0, "1", 0, 0);
}
char* __cdecl YoudaoOCR::ocr_recognize_line(class cv::Mat const&, bool) {
	MessageBoxA(0, "2", 0, 0);
	return NULL;
}
void __cdecl YoudaoOCR::release_text(char**) {
	MessageBoxA(0, "3", 0, 0);
}
bool __cdecl YoudaoOCR::recog_model_init(char const*, int, int, bool) {
	MessageBoxA(0, "4", 0, 0);
	return FALSE;
}
bool __cdecl YoudaoOCR::switch_to_language(int) {
	MessageBoxA(0, "5", 0, 0);
	return FALSE;
}

extern "C" _declspec(dllexport) int _stdcall XL_InitBugHandler(DWORD, DWORD, DWORD, DWORD, DWORD){
	MessageBoxA(0, "1", 0, 0);
	return 0;
}
extern "C" _declspec(dllexport) int _stdcall XL_SetReportShowMode(DWORD){
	MessageBoxA(0, "2", 0, 0);
	return 0;
}
extern "C" _declspec(dllexport) int _stdcall XL_SetBugReportRootDir(DWORD){
	MessageBoxA(0, "3", 0, 0);
	return 0;
}
extern "C" _declspec(dllexport) int _stdcall XL_SetPeerID(DWORD){
	MessageBoxA(0, "4", 0, 0);
	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{MessageBoxA(0, "DllMain", 0, 0);
		
		
		break;
	}
	case DLL_THREAD_ATTACH:
	{
		break;
	}
	case DLL_THREAD_DETACH:
	{
	
		break;
	}
	case DLL_PROCESS_DETACH:
	{
		break;
	}
	break;
	}
	return TRUE;
}
