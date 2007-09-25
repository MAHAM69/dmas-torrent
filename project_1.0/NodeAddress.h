#ifndef NODEADDRESS_H_
#define NODEADDRESS_H_

#include <string>
#include <iostream>
#include "StringConvert.h"
#include "NumberTools.h"
#include "commons.h"

using namespace std;

class NodeAddress : public StringConvert,
		    public cNumberTools
{
    public:
	int nodeAddress(const char* nodeName);
};

#endif /*NODEADDRESS_H*/
