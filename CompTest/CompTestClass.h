#pragma once
/**
*  @file CompTestClass.h
*  @author LiWang112358
*  @date 2012/3/17
*  @version 1.0
*  @brief CompTestClass����������������Ҫʵ��ICompTest�ӿ�
*/

#include "ICompTest.h"  

// {9CA9DBE8-C0B1-42c9-B6C7-856BE5756855}  
static const GUID CLSID_CompTestClass =
{ 0x9ca9dbe8, 0xc0b1, 0x42c9,{ 0xb6, 0xc7, 0x85, 0x6b, 0xe5, 0x75, 0x68, 0x55 } };

class CompTestClass: public ICompTest
{
public:

	CompTestClass();
	~CompTestClass();

	//Ҫʵ��IUnknown�ӿ�  
	virtual HRESULT _stdcall QueryInterface(const IID& riid, void** ppvObject);
	virtual ULONG _stdcall AddRef();
	virtual ULONG _stdcall Release();

	//Ҫʵ��ICompTest�ӿ�  
	virtual int _stdcall HelloWorld();
	virtual std::string _stdcall TestHello();
	static int Init();
	ULONG ObjNum();
protected:
	static ULONG CompTestClass::m_objNum;//�����CompTestClass����ĸ���,�����ж��Ƿ����ж�ر��齨����ֵΪ0�����ж��  
	static CRITICAL_SECTION CompTestClass::m_cs;
	ULONG m_Ref;
};
