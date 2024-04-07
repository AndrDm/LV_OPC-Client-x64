// LV_OPC.cpp : Defines the exported functions for the DLL.
//

#include "framework.h"
#include "LV_OPC.h"

#include <stdio.h>
#include <sys\timeb.h>

#define OPCDACLIENT_LIBRARY

#include "OPCClient.h"

#include "OPCGroup.h"
#include "OPCHost.h"
#include "OPCItem.h"
#include "OPCServer.h"
#include "opcda.h"

COPCHost *host;
std::vector<CLSID> localClassIdList;
std::vector<std::wstring> localServerList;

// This is an example of an exported variable
LVOPC_API int nLVOPC=0;

// This is an example of an exported function.
LVOPC_API int fnLVOPC(void)
{
    return 0;
}

LVOPC_API int fnGetServers(char* buf)
{
    
    COPCClient::init();

    char c_string[100] = {0};
    strcpy_s(c_string, sizeof(c_string), "localhost");

    host = COPCClient::makeHost(COPCHost::S2WS(c_string));

    // list servers

    host->getListOfDAServers(IID_CATID_OPCDAServer20, localServerList, localClassIdList);
    
    //printf("\nserver ID list:\n");
    int server_id = -1;
    for (unsigned i = 0; i < localServerList.size(); ++i){
        //printf("%d: '%ws'\n", i, localServerList[i].c_str());
        sprintf(buf, "%d: '%ws'\n", i, localServerList[i].c_str());
        if (localServerList[i] == L"Matrikon.OPC.Simulation.1")
        {
            server_id = i;
        }
    } // for

    return localServerList.size();
    }


LVOPC_API int fnConnect(int server_id)
{
    std::wstring serverName = localServerList[server_id];
    COPCServer *opcServer = host->connectDAServer(serverName);
    return 0;
}

// This is the constructor of a class that has been exported.
CLVOPC::CLVOPC()
{
    return;
}
