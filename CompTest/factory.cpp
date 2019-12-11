#pragma once
/**
*  @file factory.cpp
*  @author LiWang112358
*  @date 2012/3/17
*  @version 1.0
*  @brief CompTestClass对象的类厂的实现
*/
#include "stdafx.h"
#include "factory.h"  
#include "CompTestClass.h"  


CompTestFactory::CompTestFactory()
{
	m_Ref = 0;
}

CompTestFactory::~CompTestFactory()
{

}

HRESULT _stdcall CompTestFactory::QueryInterface(const IID &riid, void **ppvObject)
{
	if (IID_IUnknown == riid) {
		*ppvObject = (IUnknown*)this;
		((IUnknown*)(*ppvObject))->AddRef();
	}
	else if (IID_IClassFactory == riid) {
		*ppvObject = (IClassFactory*)this;
		((IClassFactory*)(*ppvObject))->AddRef();
	}
	else {
		*ppvObject = NULL;
		return E_NOINTERFACE;
	}
	return S_OK;
}

ULONG _stdcall CompTestFactory::AddRef()
{
	m_Ref++;
	return m_Ref;
}

ULONG _stdcall CompTestFactory::Release()
{
	m_Ref--;
	if (0 == m_Ref) {
		delete this;
		return 0;
	}
	return m_Ref;
}

HRESULT _stdcall CompTestFactory::CreateInstance(IUnknown *pUnkOuter, const IID &riid, void **ppvObject)//最重要的函数，这个函数创建CompTestClass对象，并返回所需接口  
{
	if (NULL != pUnkOuter) {
		return CLASS_E_NOAGGREGATION;
	}
	HRESULT hr = E_OUTOFMEMORY;
	CompTestClass::Init();
	CompTestClass* pObj = new CompTestClass();
	if (NULL == pObj) {
		return hr;
	}

	hr = pObj->QueryInterface(riid, ppvObject);
	if (S_OK != hr) {
		delete pObj;
	}
	return hr;
}

HRESULT _stdcall CompTestFactory::LockServer(BOOL fLock)
{
	return NOERROR;
}