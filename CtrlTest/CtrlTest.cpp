// CtrlTest.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include <iostream>  
#include "ICompTest.h"  
/**
*  @file CtrlTest.cpp
*  @author LiWang112358
*  @date 2012/3/17
*  @version 1.0
*  @brief COM客户端代码
*/


using namespace std;

int main()
{
	CoInitialize(NULL);     //初始化COM库，使用默认的内存分配器  
	IUnknown* pUnknown = NULL;
	GUID CLSID_CompTestClass;
	HRESULT hResult = CLSIDFromProgID(L"COMCTL.CompTest", &CLSID_CompTestClass);    //获取ProgID为COMCTL.CompTest组建的CLSID  
	if (S_OK != hResult) {
		printf("Can't find CLSID!\n");
		return -1;
	}
	else {
		LPOLESTR szCLSID;
		StringFromCLSID(CLSID_CompTestClass, &szCLSID);     //将其转化为字符串形式用来输出  
		wprintf(L"find CLSID \"%s\"\n", szCLSID);
		CoTaskMemFree(szCLSID);     //调用COM库的内存释放  
	}

	//用此CLSID创建一个COM对象并获取IUnknown接口  
	hResult = CoCreateInstance(CLSID_CompTestClass, NULL, CLSCTX_INPROC_SERVER, IID_IUnknown, (void **)&pUnknown);

	if (S_OK != hResult || NULL == pUnknown) {
		printf("Create Object Failed!\n");
		return -1;
	}

	ICompTest* pCompTest = NULL;
	hResult = pUnknown->QueryInterface(IID_ICompTest, (void**)&pCompTest);//通过此结构查询我们自己的ICompTest接口  

	cout << pCompTest->HelloWorld() << endl;//调用我们自己接口中的函数  
	string testHello=pCompTest->TestHello();
	cout << testHello << endl;
	pCompTest->Release();    //释放自己的接口  
	pUnknown->Release(); //释放IUnknown接口  
	CoUninitialize();       //COM库反初始化  
	return 0;
}