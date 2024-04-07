// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the LVOPC_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// LVOPC_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef LVOPC_EXPORTS
#define LVOPC_API __declspec(dllexport)
#else
#define LVOPC_API __declspec(dllimport)
#endif

LVOPC_API int fnGetServers(char* address, char* list);
LVOPC_API int fnConnect(int server_id);
LVOPC_API int fnStatus(char* LV_status);
LVOPC_API int fnWaitRunning();
LVOPC_API int fnBrowseServer(char* LV_items);
LVOPC_API int fnCreateDemoGroup();

LVOPC_API int fnReadItemI32(int idx);
LVOPC_API float fnReadItemSGL(int idx);
LVOPC_API double fnReadItemDBL(int idx);
LVOPC_API int fnWriteItemI32(int idx, int val);
LVOPC_API int fnWriteItemSGL(int idx, float val);
LVOPC_API int fnWriteItemDBL(int idx, double val);
