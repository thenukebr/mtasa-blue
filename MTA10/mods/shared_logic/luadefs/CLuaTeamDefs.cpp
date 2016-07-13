/*****************************************************************************
*
*  PROJECT:     Multi Theft Auto
*  LICENSE:     See LICENSE in the top level directory
*  FILE:        mods/shared_logic/luadefs/CLuaTeamDefs.cpp
*  PURPOSE:     Lua team definitions class
*
*  Multi Theft Auto is available from http://www.multitheftauto.com/
*
*****************************************************************************/

#include "StdInc.h"
using std::list;

void CLuaTeamDefs::LoadFunctions ( void )
{
    CLuaCFunctions::AddFunction ( "getTeamFromName", GetTeamFromName );
    CLuaCFunctions::AddFunction ( "getTeamName", GetTeamName );
    CLuaCFunctions::AddFunction ( "getTeamColor", GetTeamColor );
    CLuaCFunctions::AddFunction ( "getTeamFriendlyFire",GetTeamFriendlyFire );
    CLuaCFunctions::AddFunction ( "getPlayersInTeam", GetPlayersInTeam );
    CLuaCFunctions::AddFunction ( "countPlayersInTeam", CountPlayersInTeam );
}


void CLuaTeamDefs::AddClass ( lua_State* luaVM )
{
    lua_newclass ( luaVM );

    lua_classfunction ( luaVM, "create", "getTeamFromName" );
    lua_classfunction ( luaVM, "getFromName", "getTeamFromName" );
    lua_classfunction ( luaVM, "countPlayers", "countPlayersInTeam" );
    lua_classfunction ( luaVM, "getFriendlyFire", "getTeamFriendlyFire" );
    lua_classfunction ( luaVM, "getName", "getTeamName" );
    lua_classfunction ( luaVM, "getColor", "getTeamColor" );

    lua_classvariable ( luaVM, "playerCount", NULL, "countPlayersInTeam" );
    lua_classvariable ( luaVM, "friendlyFire", NULL, "getTeamFriendlyFire" );
    lua_classvariable ( luaVM, "name", NULL, "getTeamName" );
    //lua_classvariable ( luaVM, "color", NULL, "getTeamColor" );

    lua_registerclass ( luaVM, "Team", "Element" );
}

int CLuaTeamDefs::GetTeamFromName ( lua_State* luaVM )
{
    SString strName = "";
    CScriptArgReader argStream ( luaVM );
    argStream.ReadString ( strName );

    if ( !argStream.HasErrors () )
    {
        CClientTeam* pTeam = m_pTeamManager->GetTeam ( strName );
        if ( pTeam )
        {
            lua_pushelement ( luaVM, pTeam );
            return 1;
        }
    }
    else
        m_pScriptDebugging->LogCustom ( luaVM, argStream.GetFullErrorMessage () );

    lua_pushboolean ( luaVM, false );
    return 1;
}


int CLuaTeamDefs::GetTeamName ( lua_State* luaVM )
{
    CClientTeam* pTeam = NULL;
    CScriptArgReader argStream ( luaVM );
    argStream.ReadUserData ( pTeam );

    if ( !argStream.HasErrors () )
    {
        if ( pTeam )
        {
            const char* szName = pTeam->GetTeamName ();
            if ( szName )
            {
                lua_pushstring ( luaVM, szName );
                return 1;
            }
        }
        else
            m_pScriptDebugging->LogBadPointer ( luaVM, "team", 1 );
    }
    else
        m_pScriptDebugging->LogCustom ( luaVM, argStream.GetFullErrorMessage () );

    lua_pushboolean ( luaVM, false );
    return 1;
}


int CLuaTeamDefs::GetTeamColor ( lua_State* luaVM )
{
    CClientTeam* pTeam = NULL;
    CScriptArgReader argStream ( luaVM );
    argStream.ReadUserData ( pTeam );

    if ( !argStream.HasErrors () )
    {
        if ( pTeam )
        {
            unsigned char ucRed, ucGreen, ucBlue;
            pTeam->GetColor ( ucRed, ucGreen, ucBlue );

            lua_pushnumber ( luaVM, ucRed );
            lua_pushnumber ( luaVM, ucGreen );
            lua_pushnumber ( luaVM, ucBlue );
            return 3;
        }
        else
            m_pScriptDebugging->LogBadPointer ( luaVM, "team", 1 );
    }
    else
        m_pScriptDebugging->LogCustom ( luaVM, argStream.GetFullErrorMessage () );

    lua_pushboolean ( luaVM, false );
    return 1;
}


int CLuaTeamDefs::GetTeamFriendlyFire ( lua_State* luaVM )
{
    CClientTeam* pTeam = NULL;
    CScriptArgReader argStream ( luaVM );
    argStream.ReadUserData ( pTeam );

    if ( !argStream.HasErrors () )
    {
        if ( pTeam )
        {
            bool bFriendlyFire = pTeam->GetFriendlyFire ();
            lua_pushboolean ( luaVM, bFriendlyFire );
            return 1;
        }
        else
            m_pScriptDebugging->LogBadPointer ( luaVM, "team", 1 );
    }
    else
        m_pScriptDebugging->LogCustom ( luaVM, argStream.GetFullErrorMessage () );

    lua_pushboolean ( luaVM, false );
    return 1;
}


int CLuaTeamDefs::GetPlayersInTeam ( lua_State* luaVM )
{
    CClientTeam* pTeam = NULL;
    CScriptArgReader argStream ( luaVM );
    argStream.ReadUserData ( pTeam );

    if ( !argStream.HasErrors () )
    {
        if ( pTeam )
        {
            lua_newtable ( luaVM );

            unsigned int uiIndex = 0;

            list < CClientPlayer* > ::const_iterator iter = pTeam->IterBegin ();
            for ( ; iter != pTeam->IterEnd (); iter++ )
            {
                CClientPlayer* pPlayer = *iter;
                if ( !pPlayer->IsBeingDeleted () )
                {
                    lua_pushnumber ( luaVM, ++uiIndex );
                    lua_pushelement ( luaVM, pPlayer );
                    lua_settable ( luaVM, -3 );
                }
            }

            return 1;
        }
        else
            m_pScriptDebugging->LogBadPointer ( luaVM, "team", 1 );
    }
    else
        m_pScriptDebugging->LogCustom ( luaVM, argStream.GetFullErrorMessage () );

    lua_pushboolean ( luaVM, false );
    return 1;
}


int CLuaTeamDefs::CountPlayersInTeam ( lua_State* luaVM )
{
    CClientTeam* pTeam = NULL;
    CScriptArgReader argStream ( luaVM );
    argStream.ReadUserData ( pTeam );

    if ( !argStream.HasErrors () )
    {
        if ( pTeam )
        {
            unsigned int uiCount = pTeam->CountPlayers ();
            lua_pushnumber ( luaVM, uiCount );
            return 1;
        }
        else
            m_pScriptDebugging->LogBadPointer ( luaVM, "team", 1 );
    }
    else
        m_pScriptDebugging->LogCustom ( luaVM, argStream.GetFullErrorMessage () );

    lua_pushboolean ( luaVM, false );
    return 1;
}
