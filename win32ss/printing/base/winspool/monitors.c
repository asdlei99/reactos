/*
 * PROJECT:     ReactOS Spooler API
 * LICENSE:     GPL-2.0+ (https://spdx.org/licenses/GPL-2.0+)
 * PURPOSE:     Functions related to Print Monitors
 * COPYRIGHT:   Copyright 2015-2017 Colin Finck (colin@reactos.org)
 */

#include "precomp.h"

static void
_MarshallUpMonitorInfo(PBYTE pMonitorInfo, DWORD Level)
{
    PMONITOR_INFO_2W pMonitorInfo2 = (PMONITOR_INFO_2W)pMonitorInfo;        // MONITOR_INFO_1W is a subset of MONITOR_INFO_2W

    // Replace relative offset addresses in the output by absolute pointers.
    pMonitorInfo2->pName = (PWSTR)((ULONG_PTR)pMonitorInfo2->pName + (ULONG_PTR)pMonitorInfo2);

    if (Level == 2)
    {
        pMonitorInfo2->pDLLName = (PWSTR)((ULONG_PTR)pMonitorInfo2->pDLLName + (ULONG_PTR)pMonitorInfo2);
        pMonitorInfo2->pEnvironment = (PWSTR)((ULONG_PTR)pMonitorInfo2->pEnvironment + (ULONG_PTR)pMonitorInfo2);
    }
}

BOOL WINAPI
AddMonitorA(PSTR pName, DWORD Level, PBYTE pMonitors)
{
    TRACE("AddMonitorA(%s, %lu, %p)\n", pName, Level, pMonitors);
    UNIMPLEMENTED;
    return FALSE;
}

BOOL WINAPI
AddMonitorW(PWSTR pName, DWORD Level, PBYTE pMonitors)
{
    TRACE("AddMonitorW(%S, %lu, %p)\n", pName, Level, pMonitors);
    UNIMPLEMENTED;
    return FALSE;
}

BOOL WINAPI
DeleteMonitorA(PSTR pName, PSTR pEnvironment, PSTR pMonitorName)
{
    TRACE("DeleteMonitorA(%s, %s, %s)\n", pName, pEnvironment, pMonitorName);
    UNIMPLEMENTED;
    return FALSE;
}

BOOL WINAPI
DeleteMonitorW(PWSTR pName, PWSTR pEnvironment, PWSTR pMonitorName)
{
    TRACE("DeleteMonitorW(%S, %S, %S)\n", pName, pEnvironment, pMonitorName);
    UNIMPLEMENTED;
    return FALSE;
}

BOOL WINAPI
EnumMonitorsA(PSTR pName, DWORD Level, PBYTE pMonitors, DWORD cbBuf, PDWORD pcbNeeded, PDWORD pcReturned)
{
    TRACE("EnumMonitorsA(%s, %lu, %p, %lu, %p, %p)\n", pName, Level, pMonitors, cbBuf, pcbNeeded, pcReturned);
    UNIMPLEMENTED;
    return FALSE;
}

BOOL WINAPI
EnumMonitorsW(PWSTR pName, DWORD Level, PBYTE pMonitors, DWORD cbBuf, PDWORD pcbNeeded, PDWORD pcReturned)
{
    DWORD dwErrorCode;
    DWORD i;
    PBYTE p = pMonitors;

    TRACE("EnumMonitorsW(%S, %lu, %p, %lu, %p, %p)\n", pName, Level, pMonitors, cbBuf, pcbNeeded, pcReturned);

    // Do the RPC call
    RpcTryExcept
    {
        dwErrorCode = _RpcEnumMonitors(pName, Level, pMonitors, cbBuf, pcbNeeded, pcReturned);
    }
    RpcExcept(EXCEPTION_EXECUTE_HANDLER)
    {
        dwErrorCode = RpcExceptionCode();
        ERR("_RpcEnumPorts failed with exception code %lu!\n", dwErrorCode);
    }
    RpcEndExcept;

    if (dwErrorCode == ERROR_SUCCESS)
    {
        // Replace relative offset addresses in the output by absolute pointers.
        for (i = 0; i < *pcReturned; i++)
        {
            _MarshallUpMonitorInfo(p, Level);

            if (Level == 1)
                p += sizeof(MONITOR_INFO_1W);
            else if (Level == 2)
                p += sizeof(MONITOR_INFO_2W);
        }
    }

    SetLastError(dwErrorCode);
    return (dwErrorCode == ERROR_SUCCESS);
}
