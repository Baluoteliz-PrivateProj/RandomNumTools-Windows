#pragma once

#include "Singleton.h"

class CProjData
{
public:
	CProjData();
	~CProjData();

private:

};

class CProjDataInstance :public CSingleton<CProjDataInstance>
{
public:
	friend CSingleton;
	~CProjDataInstance();

private:
	CProjDataInstance();
	static CSingleton<CProjDataInstance>::CGrabo graboInstance;
};
