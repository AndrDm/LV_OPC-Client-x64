// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the LVOPC_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// LVOPC_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef LVOPC_EXPORTS
#define LVOPC_API __declspec(dllexport)
#else
//#define LVOPC_API __declspec(dllimport)
#endif

// This class is exported from the dll
class LVOPC_API CLVOPC {
public:
	CLVOPC(void);
	// TODO: add your methods here.
};

extern LVOPC_API int nLVOPC;

LVOPC_API int fnLVOPC(char* buf);
LVOPC_API int fnConnect(int server_id);
