/**
*  @file CompTestClass.cpp
*  @author LiWang112358
*  @date 2012/3/17
*  @version 1.0
*  @brief CompTestClass类的实现
*/
#include "stdafx.h"
#include "CompTestClass.h"


ULONG CompTestClass::m_objNum = 0;//组件中CompTestClass对象的个数,用于判断是否可以卸载本组建，如值为0则可以卸载  
CRITICAL_SECTION CompTestClass::m_cs;//为了多线程调用对m_objNum加的锁  

CompTestClass::CompTestClass()
{
	m_Ref = 0;
//	autoLock tlock(m_cs);

	m_objNum++;    //构造了一个对象  
}

CompTestClass::~CompTestClass()
{
	//autoLock tlock(m_cs);
	m_objNum--;    //释放了一个对象  
}

HRESULT _stdcall CompTestClass::QueryInterface(const IID &riid, void **ppvObject)
{
	if (IID_IUnknown == riid) {
		*ppvObject = (IUnknown*)this;
		((IUnknown*)(*ppvObject))->AddRef();
	}
	else if (IID_ICompTest == riid) {
		*ppvObject = (ICompTest*)this;
		((ICompTest*)(*ppvObject))->AddRef();
	}
	else {
		*ppvObject = NULL;
		return E_NOINTERFACE;
	}
	return S_OK;
}

ULONG _stdcall CompTestClass::AddRef()
{
	m_Ref++;
	return m_Ref;
}

ULONG _stdcall CompTestClass::Release()
{
	m_Ref--;
	if (0 == m_Ref) {
		delete this;
		return 0;
	}
	return m_Ref;
}

int _stdcall CompTestClass::HelloWorld()//ICompTest接口的实现，返回一个整数89  
{
	return 89;
}

std::string _stdcall CompTestClass::TestHello() {
	return std::string{ "TestHello" };
}

int CompTestClass::Init()
{
	m_objNum = 0;
	InitializeCriticalSection(&m_cs);
	return 0;
}

ULONG CompTestClass::ObjNum()
{
	return m_objNum;
}