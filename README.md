# Debug - include for open.mp

This include adds the ability to defer to console/chat.

## Example
```pawn
#define DEBUG_ON
#include <debug>

public OnGameModeInit()
{
    C:INFO("INFO IS %s", "PRINT");
    C:WARNING("WARNING IS %s", "PRINT");
    C:ERROR("ERROR IS %s", "PRINT");
    C:ERROR_FATAL("ERROR FATAL IS %s. AND STOP SERVER", "PRINT");
}

public OnPlayerConnect(playerid)
{
    P:INFO(playerid, 0xFFFFFFFF, "PLAYER: INFO IS %s", "PRINT");
    P:WARNING(playerid, 0xFFFFFFFF, "PLAYER: WARNING IS %s", "PRINT");
    P:ERROR(playerid, 0xFFFFFFFF, "PLAYER: ERROR IS %s", "PRINT");
    P:ERROR_FATAL(playerid, 0xFFFFFFFF, "PLAYER: ERROR FATAL IS %s. AND STOP SERVER", "PRINT");
}
```

### Why? There are already analogs.
This repository and include are intended for internal use and were not made for use in a public environment.
