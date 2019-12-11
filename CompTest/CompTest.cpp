// CompTest.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"

/**
*  @file CompTest.cpp
*  @author LiWang112358
*  @date 2012/3/17
*  @version 1.0
*  @brief 四个重要的引出函数的实现，
*  @      分别是DllRegisterServer，用于注册本dll,由regsvr32.exe调用
*  @            DllUnregisterServer, 用于反注册本dll，由regsvr32.exe -u 调用
*  @            DllCanUnloadNow，用于判断是否可以卸载本组建, 由CoFreeUnusedLibraries函数调用
*  @            DllGetClassObject，用于创建类厂并返回所需接口，由CoGetClassObject函数调用
*/
#include <iostream>  
#include "factory.h"  
#include "CompTestClass.h"  

using namespace std;

HMODULE g_hModule;  //dll进程实例句柄  
ULONG g_num;        //组件中CompTestClass对象的个数,用于判断是否可以卸载本组建，如值为0则可以卸载  

int myReg(LPCWSTR lpPath)   //将本组件的信息写入注册表,包括CLSID、所在路径lpPath、ProgID  
{
	HKEY thk, tclsidk;

	//打开键HKEY_CLASSES_ROOT\CLSID,创建新键为CompTestClass的CLSID，  
	//在该键下创建键InprocServer32，并将本组件(dll)所在路径lpPath写为该键的默认值  
	if (ERROR_SUCCESS == RegOpenKey(HKEY_CLASSES_ROOT, L"CLSID", &thk)) {
		if (ERROR_SUCCESS == RegCreateKey(thk, L"{9CA9DBE8-C0B1-42c9-B6C7-856BE5756855}", &tclsidk)) {
			HKEY tinps32k, tprogidk;
			if (ERROR_SUCCESS == RegCreateKey(tclsidk, L"InprocServer32", &tinps32k)) {
				if (ERROR_SUCCESS == RegSetValue(tinps32k, NULL, REG_SZ, lpPath, wcslen(lpPath) * 2)) {
				}
				RegCloseKey(tinps32k);
			}
			RegCloseKey(tclsidk);
		}
		RegCloseKey(thk);
	}
	//在键HKEY_CLASSES_ROOT下创建新键为COMCTL.CompTest，  
	//在该键下创建子键，并将CompTestClass的CLSID写为该键的默认值  
	if (ERROR_SUCCESS == RegCreateKey(HKEY_CLASSES_ROOT, L"COMCTL.CompTest", &thk)) {
		if (ERROR_SUCCESS == RegCreateKey(thk, L"CLSID", &tclsidk)) {
			if (ERROR_SUCCESS == RegSetValue(tclsidk,
				NULL,
				REG_SZ,
				L"{9CA9DBE8-C0B1-42c9-B6C7-856BE5756855}",
				wcslen(L"{9CA9DBE8-C0B1-42c9-B6C7-856BE5756855}") * 2)) {
			}
		}
	}
	//这样的话一个客户端程序如果想要使用本组件，首先可以以COMCTL.CompTest为参数调用CLSIDFromProgID函数  
	//来获取CompTestClass的CLSID，再以这个CLSID为参数调用CoCreateInstance创建COM对象  
	return 0;
}

extern "C" HRESULT _stdcall DllRegisterServer()
{
	WCHAR szModule[1024];
	DWORD dwResult = GetModuleFileName(g_hModule, szModule, 1024); //获取本组件(dll)所在路径  
	if (0 == dwResult) {
		return -1;
	}
	MessageBox(NULL, szModule, L"", MB_OK);
	myReg(szModule);//将路径等信息写入注册表  
	return 0;
}

int myDelKey(HKEY hk, LPCWSTR lp)
{
	if (ERROR_SUCCESS == RegDeleteKey(hk, lp)) {
	}
	return 0;
}

int myDel() //删除注册时写入注册表的信息  
{
	HKEY thk;
	if (ERROR_SUCCESS == RegOpenKey(HKEY_CLASSES_ROOT, L"CLSID", &thk)) {
		myDelKey(thk, L"{9CA9DBE8-C0B1-42c9-B6C7-856BE5756855}\\InprocServer32");
		myDelKey(thk, L"{9CA9DBE8-C0B1-42c9-B6C7-856BE5756855}");

		RegCloseKey(thk);
	}
	if (ERROR_SUCCESS == RegOpenKey(HKEY_CLASSES_ROOT, L"COMCTL.CompTest", &thk)) {
		myDelKey(thk, L"CLSID");
	}
	myDelKey(HKEY_CLASSES_ROOT, L"COMCTL.CompTest");
	return 0;
}

extern "C" HRESULT _stdcall DllUnregisterServer()
{
	myDel();//删除注册时写入注册表的信息  

	return 0;
}

extern "C" HRESULT _stdcall DllCanUnloadNow()//用于判断是否可以卸载本组建, 由CoFreeUnusedLibraries函数调用  
{
	if (0 == g_num) {//如果对象个数为0，则可以卸载  
		return S_OK;
	}
	else {
		return S_FALSE;
	}
}

extern "C" HRESULT _stdcall DllGetClassObject(__in REFCLSID rclsid, __in REFIID riid, LPVOID FAR* ppv)//用于创建类厂并返回所需接口，由CoGetClassObject函数调用  
{
	if (CLSID_CompTestClass == rclsid) {
		CompTestFactory* pFactory = new CompTestFactory();//创建类厂对象  
		if (NULL == pFactory) {
			return E_OUTOFMEMORY;
		}
		HRESULT result = pFactory->QueryInterface(riid, ppv);//获取所需接口  
		return result;
	}
	else {
		return CLASS_E_CLASSNOTAVAILABLE;
	}
}


BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	g_hModule = hModule;//获取进程实例句柄，用于获取本组件(dll)路径  
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}