// LineChange.cpp : main source file for LineChange.exe
//

#include "stdafx.h"

#include "resource.h"

#include "maindlg.h"

CAppModule _Module;


int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
	HRESULT hRes = ::CoInitialize(NULL);
// If you are running on NT 4.0 or higher you can use the following call instead to 
// make the EXE free threaded. This means that calls come in on a random RPC thread.
//	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	ATLASSERT(SUCCEEDED(hRes));
	HMODULE hRichEdit;
	hRichEdit = LoadLibrary(_T("RICHED20.DLL"));
	if (!hRichEdit) {
		OutputDebugString(_T("*** load RICHED20 error!"));
		return(-1);
	}

	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProc(NULL, 0, 0, 0L);

	AtlInitCommonControls(ICC_COOL_CLASSES | ICC_BAR_CLASSES);	// add flags to support other controls

	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

	int nRet = 0;
	// BLOCK: Run application
	{
		CMainDlg dlgMain;
		nRet = dlgMain.DoModal();
	}

	_Module.Term();
	
	FreeLibrary(hRichEdit);
	::CoUninitialize();

	return nRet;
}
