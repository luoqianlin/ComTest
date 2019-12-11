#pragma once
/**
*  @file factory.h
*  @author LiWang112358
*  @date 2012/3/17
*  @version 1.0
*  @brief CompTestClass������೧������
*/
#ifndef FACTORY_H  
#define FACTORY_H  

#include <unknwn.h>  


class CompTestFactory :
	public IClassFactory
{
public:
	CompTestFactory();
	~CompTestFactory();

	//Ҫʵ��IUnknown�ӿ�  
	virtual HRESULT _stdcall QueryInterface(const IID& riid, void** ppvObject);
	virtual ULONG _stdcall AddRef();
	virtual ULONG _stdcall Release();

	//Ҫʵ��IClassFactory�ӿ�  
	virtual HRESULT _stdcall CreateInstance(IUnknown *pUnkOuter, const IID& riid, void **ppvObject);
	virtual HRESULT _stdcall LockServer(BOOL fLock);

protected:
	ULONG m_Ref;
};

#endif