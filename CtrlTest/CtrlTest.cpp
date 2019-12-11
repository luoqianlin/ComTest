// CtrlTest.cpp : �������̨Ӧ�ó������ڵ㡣
//
#include "stdafx.h"
#include <iostream>  
#include "ICompTest.h"  
/**
*  @file CtrlTest.cpp
*  @author LiWang112358
*  @date 2012/3/17
*  @version 1.0
*  @brief COM�ͻ��˴���
*/


using namespace std;

int main()
{
	CoInitialize(NULL);     //��ʼ��COM�⣬ʹ��Ĭ�ϵ��ڴ������  
	IUnknown* pUnknown = NULL;
	GUID CLSID_CompTestClass;
	HRESULT hResult = CLSIDFromProgID(L"COMCTL.CompTest", &CLSID_CompTestClass);    //��ȡProgIDΪCOMCTL.CompTest�齨��CLSID  
	if (S_OK != hResult) {
		printf("Can't find CLSID!\n");
		return -1;
	}
	else {
		LPOLESTR szCLSID;
		StringFromCLSID(CLSID_CompTestClass, &szCLSID);     //����ת��Ϊ�ַ�����ʽ�������  
		wprintf(L"find CLSID \"%s\"\n", szCLSID);
		CoTaskMemFree(szCLSID);     //����COM����ڴ��ͷ�  
	}

	//�ô�CLSID����һ��COM���󲢻�ȡIUnknown�ӿ�  
	hResult = CoCreateInstance(CLSID_CompTestClass, NULL, CLSCTX_INPROC_SERVER, IID_IUnknown, (void **)&pUnknown);

	if (S_OK != hResult || NULL == pUnknown) {
		printf("Create Object Failed!\n");
		return -1;
	}

	ICompTest* pCompTest = NULL;
	hResult = pUnknown->QueryInterface(IID_ICompTest, (void**)&pCompTest);//ͨ���˽ṹ��ѯ�����Լ���ICompTest�ӿ�  

	cout << pCompTest->HelloWorld() << endl;//���������Լ��ӿ��еĺ���  
	string testHello=pCompTest->TestHello();
	cout << testHello << endl;
	pCompTest->Release();    //�ͷ��Լ��Ľӿ�  
	pUnknown->Release(); //�ͷ�IUnknown�ӿ�  
	CoUninitialize();       //COM�ⷴ��ʼ��  
	return 0;
}