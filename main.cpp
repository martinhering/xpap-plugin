// Custom Commands
//
// This example program illustrates creating a custom command.  In this case clicking a generic trigger on
// your aircraft panel writes to the DataRef controlling the pilots head position either continuously or one
// knotch at a time. We could use the existing position command: sim/view/move_right, however for illustrative
// purposes, this example uses a custom command:  BSUB/ViewPoint/MoveRight
//
// For this example to work it is necessary to create a generic trigger on the panel of your aircraft keyed
// to the command: BSUB/ViewPoint/MoveRight.
//
// Content added by BlueSideUpBob.
//

#define XPLM210

#include "IPAddress.h"
#include "B777MCP.h"


static XPAP::B777MCP* b777mcp = NULL;

float XPAPDataLoopCallback(float elapsedMe, float elapsedSim, int counter, void * refcon);


PLUGIN_API int XPluginStart(
					 char *        outName,
					 char *        outSig,
					 char *        outDesc)
{
	// Plugin Info
	strcpy(outName, "X-Plane Arduino Project");
	strcpy(outSig, "org.xpap");
	strcpy(outDesc, "This plugin exchanges data between X-Plane and an Arduino Home Cockpit Controller.");

	
    b777mcp = new XPAP::B777MCP(XPAP::IPAddress("192.168.1.160"));

	return 1;
}

PLUGIN_API void XPluginStop(void)
{
    
}


PLUGIN_API void XPluginDisable(void)
{
    XPLMDebugString("xpap: disable\n");
    XPLMUnregisterFlightLoopCallback(XPAPDataLoopCallback, 0);
}

PLUGIN_API int XPluginEnable(void)
{
    XPLMDebugString("xpap: enable\n");
    
    XPLMRegisterFlightLoopCallback(XPAPDataLoopCallback, 1.0, NULL);
	return 1;
}

PLUGIN_API void XPluginReceiveMessage(XPLMPluginID    inFromWho, long inMessage, void* inParam)
{

}

float XPAPDataLoopCallback(float elapsedMe, float elapsedSim, int counter, void * refcon)
{
    b777mcp->loop();
    
	return 0.1f;
}