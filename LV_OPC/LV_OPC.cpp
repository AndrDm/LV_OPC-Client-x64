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
COPCServer* opcServer;
COPCGroup* demoGroup;
std::vector<std::wstring> opcItemNames;
std::vector<COPCItem*> itemsCreated;
std::vector<HRESULT> errors;
int NI_server_id = -1;
bool Connected = FALSE;

LVOPC_API int fnGetServers(char* address, char* list)
{
    char c_string[100] = {0};
    char buf[100] = { 0 };
    if (strlen(address)) strcpy_s(c_string, strlen(address), address);
    else strcpy_s(c_string, sizeof(c_string), "localhost");
    COPCClient::init();
    host = COPCClient::makeHost(COPCHost::S2WS(c_string));

    // list servers
    localServerList.clear();
    localClassIdList.clear();
    host->getListOfDAServers(IID_CATID_OPCDAServer20, localServerList, localClassIdList);
    
    //printf("\nserver ID list:\n");
    for (unsigned i = 0; i < localServerList.size(); ++i){
        //printf("%d: '%ws'\n", i, localServerList[i].c_str());
        sprintf(buf, "%d: '%ws';", i, localServerList[i].c_str());
        OutputDebugStringA(buf);
        strcat(list, buf);
        if (localServerList[i] == L"National Instruments.Variable Engine.1") NI_server_id = i;
    }
    return (int)localServerList.size();
}


LVOPC_API int fnConnect(int server_id)
{
    std::wstring serverName = localServerList[server_id];
    opcServer = host->connectDAServer(serverName);
    if (opcServer) Connected = TRUE;
    if (opcServer) return 0; else return -1;
}

LVOPC_API void fnDisconnect(int server_id)
{
    Connected = FALSE;
    host->~COPCHost();
}


LVOPC_API int fnStatus(char* LV_status)
{
    // check server status
    ServerStatus status = { 0 };
    const char* serverStateMsg[] = { "unknown", "running", "failed", "co-config", "suspended", "test", "comm-fault" };

    opcServer->getStatus(status);
    //printf("server state is %s, vendor is '%ws'\r", serverStateMsg[status.dwServerState], status.vendorInfo.c_str());
    sprintf(LV_status, "server state is %s, vendor is '%ws'\r", serverStateMsg[status.dwServerState], status.vendorInfo.c_str());
    return status.dwServerState; //1 is running
} 

LVOPC_API int fnWaitRunning()
{
    ServerStatus status = { 0 };
    do { //we are optimists
        Sleep(100);
        opcServer->getStatus(status);
    } while (status.dwServerState != OPC_STATUS_RUNNING);
    return 0;
}

LVOPC_API int fnBrowseServer(char* list)
{
    char buf[100] = { 0 };
    // browse server
    opcItemNames.clear();
    opcServer->getItemNames(opcItemNames);
    for (unsigned i = 0; i < opcItemNames.size(); ++i)
    {
        //printf("%3d: '%ws'\n", i + 1, opcItemNames[i].c_str());
        sprintf(buf, "%3d: '%ws';", i, opcItemNames[i].c_str());
        strcat(list, buf);
    }
    return static_cast<int>(opcItemNames.size());
}

LVOPC_API int fnCreateDemoGroup()
{
    // make demo group
    unsigned long refreshRate;
    demoGroup = opcServer->makeGroup(L"DemoGroup", true, 1000, refreshRate, 0.0);
    if (!demoGroup) return -1;
    //add all items from browser   
    if (demoGroup->addItems(opcItemNames, itemsCreated, errors, true) != 0) return -2;
    else return 0;
}

LVOPC_API int fnReadItemI32(int idx)
{
    OPCItemData data;
    itemsCreated[idx]->readSync(data, OPC_DS_DEVICE);
    return data.vDataValue.iVal;
}

LVOPC_API float fnReadItemSGL(int idx)
{
    OPCItemData data;
    itemsCreated[idx]->readSync(data, OPC_DS_DEVICE);
    return data.vDataValue.fltVal;
}

LVOPC_API double fnReadItemDBL(int idx)
{
    OPCItemData data;
    itemsCreated[idx]->readSync(data, OPC_DS_DEVICE);
    return data.vDataValue.dblVal;
}

LVOPC_API int fnWriteItemI32(int idx, int val)
{
    VARIANT var;
    var.vt = VT_I4;
    var.iVal = val;
    itemsCreated[idx]->writeSync(var);
    return 0;
}
//should go to walk with my dog...

LVOPC_API int fnWriteItemSGL(int idx, float val)
{
    VARIANT var;
    var.vt = VT_R4;
    var.fltVal = val;
    itemsCreated[idx]->writeSync(var);
    return 0;
}

LVOPC_API int fnWriteItemDBL(int idx, double val)
{
    VARIANT var;
    var.vt = VT_R8;
    var.dblVal = val;
    itemsCreated[idx]->writeSync(var);
    return 0;
}

