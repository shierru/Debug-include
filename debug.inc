
/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * The original code is copyright (c) 2023, shierru (Nikita).
*/

#if defined _INC_debug
	#endinput
#endif
#define _INC_debug

/**
 * <library name="debug" summary="Include for debug">
 *   <license>
 *     (c) Copyright 2023, shierru (Nikita).
 *   </license>
 *   <summary pawndoc="true">
 *     This library uses the enhanced <em>pawndoc.xsl</em> from
 *     <a href="https://github.com/pawn-lang/pawndoc">pawn-lang/pawndoc</a>.
 *     This XSL has features such as library and markdown support, and will not
 *     render this message when used.
 *   </summary>
 * </library>
 */

/// <p/>

#pragma tabsize 4

/// <p/>
#tryinclude <open.mp>

#if !defined _INC_open_mp 
    #error Can not found open.mp includes
#endif

#tryinclude "YSI_Coding\y_va"
#tryinclude <YSI_Coding\y_va>
#tryinclude <YSI-Includes\YSI_Coding\y_va>

#if !defined _INC_y_va
    #error Download `github.com/pawn-lang/YSI-Includes/archive/refs/heads/5.x.zip` and extract it to `include\`.
#endif

/**
 * <library>debug</library>
 */
enum DEBUG_TYPES
{
    // Unknown type
    DEBUG_TYPE_UNKNOWN = -1,

    // Type simple output to console/chat
    DEBUG_TYPE_INFO,

    // Type simple output to console/chat
    DEBUG_TYPE_WARNING,

    // If the output is directed to a specific player, this player will be disconnected from the server
    DEBUG_TYPE_ERROR,

    // If the output is directed to a specific player or console, the server will shut down after the message is sent
    DEBUG_TYPE_ERROR_FATAL
};
static stock DEBUG_TYPES:_@DEBUG_TYPES() { return DEBUG_TYPES; }

#define UNKNOWN                       (DEBUG_TYPES:-1)
#define INFO                          (DEBUG_TYPES:0)
#define WARNING                       (DEBUG_TYPES:1)
#define ERROR                         (DEBUG_TYPES:2)
#define ERROR_FATAL                   (DEBUG_TYPES:3)

/// <p/>

#if defined DEBUG_ON
    #define _@DEBUG_STATE(%0)         state _@DEBUG:%0
#else
    #define _@DEBUG_STATE(%0)         state _@DEBUG:None
#endif

#define _@DEBUG_CONSOLE: _@DEBUG_CONSOLE0:_@DEBUG_CONSOLE1:
#define _@DEBUG_CONSOLE0:_@DEBUG_CONSOLE1:%0(%2) _@DEBUG_STATE(Console);_DEBUG@PRINT(%0,_,_,%2)
#define _@DEBUG_CONSOLE1:%0(%2,%3) _@DEBUG_STATE(Console);_DEBUG@PRINT(%0,_,_,%2,%3)

#define _@DEBUG_PLAYER: _@DEBUG_PLAYER0:_@DEBUG_PLAYER1:
#define _@DEBUG_PLAYER0:_@DEBUG_PLAYER1:%0(%1,%2,%3) _@DEBUG_STATE(Player);_DEBUG@PRINT(%0,%1,%2,%3)
#define _@DEBUG_PLAYER1:%0(%1,%2,%3,%4) _@DEBUG_STATE(Player);_DEBUG@PRINT(%0,%1,%2,%3,%4)

// Interface
#define C:%0(  _@DEBUG_CONSOLE:%0(
#define P:%0(  _@DEBUG_PLAYER:%0(

// Static function
static stock strcatPrintType(DEBUG_TYPES:type, dest[], len = sizeof (dest))
{
    switch(type) 
    {
        case DEBUG_TYPE_INFO:
            return strcat(dest, "[Debug] [Info]: ", len);

        case DEBUG_TYPE_WARNING:
            return strcat(dest, "[Debug] [Warning]: ", len);

        case DEBUG_TYPE_ERROR:
            return strcat(dest, "[Debug] [Error]: ", len);

        case DEBUG_TYPE_ERROR_FATAL:
            return strcat(dest, "[Debug] [Fatal Error]: ", len);

        default: { }
    }
    return 0;
}

// Internal functions
stock _DEBUG@PRINT(DEBUG_TYPES:type, playerid = INVALID_PLAYER_ID, const colour = -1, const message[], OPEN_MP_TAGS:...) <_@DEBUG:Console>
{
    #pragma unused playerid
    #pragma unused colour

    if(type == DEBUG_TYPE_UNKNOWN)
        return 0;

    // Before using for correctness of further message output we zeroize the array
    YSI_UNSAFE_HUGE_STRING[0] = '\0';

    strcatPrintType(type, YSI_UNSAFE_HUGE_STRING);
    strcat(YSI_UNSAFE_HUGE_STRING, message, YSI_UNSAFE_HUGE_LENGTH);

    new ret = 0;

    if(YSI_CheckNumargs__(4))
        ret = print(YSI_UNSAFE_HUGE_STRING);
    else
        ret = printf(YSI_UNSAFE_HUGE_STRING, ___(4));

    // If the error is fatal, then after sending the message we disconnect the server
    // But if it is different, just output to the console
    return (type == DEBUG_TYPE_ERROR_FATAL ? SendRconCommand("exit") : ret);
}

stock _DEBUG@PRINT(DEBUG_TYPES:type, playerid = INVALID_PLAYER_ID, const colour = -1, const message[], OPEN_MP_TAGS:...)  <_@DEBUG:Player>
{
    if(playerid == INVALID_PLAYER_ID || !IsPlayerConnected(playerid))
        return 0;

    if(type == DEBUG_TYPE_UNKNOWN)
        return 0;

    // Before using for correctness of further message output we zeroize the array
    YSI_UNSAFE_HUGE_STRING[0] = '\0';

    strcatPrintType(type, YSI_UNSAFE_HUGE_STRING);
    strcat(YSI_UNSAFE_HUGE_STRING, message, YSI_UNSAFE_HUGE_LENGTH);

    new ret = 0;

    if(YSI_CheckNumargs__(4))
        ret = SendClientMessage(playerid, colour, YSI_UNSAFE_HUGE_STRING);
    else
        ret = SendClientMessage(playerid, colour, YSI_UNSAFE_HUGE_STRING, ___(4));

    // If it's a simple error, just disconnect the player from the server
    // But if the error is fatal, then after sending the message we disconnect the server
    if(type == DEBUG_TYPE_ERROR)
        ret = Kick(playerid);
    else if(type == DEBUG_TYPE_ERROR_FATAL)
        ret = SendRconCommand("exit");

    return ret;
}

stock _DEBUG@PRINT(DEBUG_TYPES:type, playerid = INVALID_PLAYER_ID, const colour = -1, const message[], OPEN_MP_TAGS:...) <_@DEBUG:None>
{
    #pragma unused type
    #pragma unused playerid
    #pragma unused colour
    #pragma unused message
    return 0;
}