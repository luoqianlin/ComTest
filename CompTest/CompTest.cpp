// CompTest.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"

/**
*  @file CompTest.cpp
*  @author LiWang112358
*  @date 2012/3/17
*  @version 1.0
*  @brief �ĸ���Ҫ������������ʵ�֣�
*  @      �ֱ���DllRegisterServer������ע�᱾dll,��regsvr32.exe����
*  @            DllUnregisterServer, ���ڷ�ע�᱾dll����regsvr32.exe -u ����
*  @            DllCanUnloadNow�������ж��Ƿ����ж�ر��齨, ��CoFreeUnusedLibraries��������
*  @            DllGetClassObject�����ڴ����೧����������ӿڣ���CoGetClassObject��������
*/
#include <iostream>  
#include "factory.h"  
#include "CompTestClass.h"  

using namespace std;

HMODULE g_hModule;  //dll����ʵ�����  
ULONG g_num;        //�����CompTestClass����ĸ���,�����ж��Ƿ����ж�ر��齨����ֵΪ0�����ж��  

int myReg(LPCWSTR lpPath)   //�����������Ϣд��ע���,����CLSID������·��lpPath��ProgID  
{
	HKEY thk, tclsidk;

	//�򿪼�HKEY_CLASSES_ROOT\CLSID,�����¼�ΪCompTestClass��CLSID��  
	//�ڸü��´�����InprocServer32�����������(dll)����·��lpPathдΪ�ü���Ĭ��ֵ  
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
	//�ڼ�HKEY_CLASSES_ROOT�´����¼�ΪCOMCTL.CompTest��  
	//�ڸü��´����Ӽ�������CompTestClass��CLSIDдΪ�ü���Ĭ��ֵ  
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
	//�����Ļ�һ���ͻ��˳��������Ҫʹ�ñ���������ȿ�����COMCTL.CompTestΪ��������CLSIDFromProgID����  
	//����ȡCompTestClass��CLSID���������CLSIDΪ��������CoCreateInstance����COM����  
	return 0;
}

extern "C" HRESULT _stdcall DllRegisterServer()
{
	WCHAR szModule[1024];
	DWORD dwResult = GetModuleFileName(g_hModule, szModule, 1024); //��ȡ�����(dll)����·��  
	if (0 == dwResult) {
		return -1;
	}
	MessageBox(NULL, szModule, L"", MB_OK);
	myReg(szModule);//��·������Ϣд��ע���  
	return 0;
}

int myDelKey(HKEY hk, LPCWSTR lp)
{
	if (ERROR_SUCCESS == RegDeleteKey(hk, lp)) {
	}
	return 0;
}

int myDel() //ɾ��ע��ʱд��ע������Ϣ  
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
	myDel();//ɾ��ע��ʱд��ע������Ϣ  

	return 0;
}

extern "C" HRESULT _stdcall DllCanUnloadNow()//�����ж��Ƿ����ж�ر��齨, ��CoFreeUnusedLibraries��������  
{
	if (0 == g_num) {//����������Ϊ0�������ж��  
		return S_OK;
	}
	else {
		return S_FALSE;
	}
}

extern "C" HRESULT _stdcall DllGetClassObject(__in REFCLSID rclsid, __in REFIID riid, LPVOID FAR* ppv)//���ڴ����೧����������ӿڣ���CoGetClassObject��������  
{
	if (CLSID_CompTestClass == rclsid) {
		CompTestFactory* pFactory = new CompTestFactory();//�����೧����  
		if (NULL == pFactory) {
			return E_OUTOFMEMORY;
		}
		HRESULT result = pFactory->QueryInterface(riid, ppv);//��ȡ����ӿ�  
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
	g_hModule = hModule;//��ȡ����ʵ����������ڻ�ȡ�����(dll)·��  
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