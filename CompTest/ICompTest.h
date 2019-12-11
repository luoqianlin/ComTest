#pragma once
/**
*  @file ICompTest.h
*  @author LiWang112358
*  @date 2012/3/17
*  @version 1.0
*  @brief ICompTest接口的声明这个文件是服务端和客户端共有的
*/
#ifndef ICOMPTEST_H  
#define ICOMPTEST_H  

#include <unknwn.h>  

// {81A80687-6CC4-4996-8DD2-F058907FDCA8}  
static const GUID IID_ICompTest =
{ 0x81a80687, 0x6cc4, 0x4996,{ 0x8d, 0xd2, 0xf0, 0x58, 0x90, 0x7f, 0xdc, 0xa8 } };


class ICompTest :
	public IUnknown
{
public:
	virtual int _stdcall HelloWorld() = 0;
	virtual std::string _stdcall TestHello()=0;
};
#endif
