/**
*  @file CompTestClass.cpp
*  @author LiWang112358
*  @date 2012/3/17
*  @version 1.0
*  @brief CompTestClass���ʵ��
*/
#include "stdafx.h"
#include "CompTestClass.h"


ULONG CompTestClass::m_objNum = 0;//�����CompTestClass����ĸ���,�����ж��Ƿ����ж�ر��齨����ֵΪ0�����ж��  
CRITICAL_SECTION CompTestClass::m_cs;//Ϊ�˶��̵߳��ö�m_objNum�ӵ���  

CompTestClass::CompTestClass()
{
	m_Ref = 0;
//	autoLock tlock(m_cs);

	m_objNum++;    //������һ������  
}

CompTestClass::~CompTestClass()
{
	//autoLock tlock(m_cs);
	m_objNum--;    //�ͷ���һ������  
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

int _stdcall CompTestClass::HelloWorld()//ICompTest�ӿڵ�ʵ�֣�����һ������89  
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