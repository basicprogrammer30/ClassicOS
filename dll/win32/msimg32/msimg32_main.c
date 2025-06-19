/*
 * Copyright 2002 Uwe Bonnes
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

#include <stdarg.h>

#include "windef.h"
#include "winbase.h"
#include "wingdi.h"
#include "winerror.h"
#include "wine/debug.h"

WINE_DEFAULT_DEBUG_CHANNEL(msimg32);

/******************************************************************************
 *           vSetDdrawflag   (MSIMG32.@)
 */
void WINAPI vSetDdrawflag(void)
{
    static unsigned int vDrawflag=1;
    FIXME("stub: vSetDrawFlag %u\n", vDrawflag);
    SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
}

/******************************************************************************
 *           AlphaBlend   (MSIMG32.@)
 */
BOOL WINAPI AlphaBlend(HDC hdcDest, int xDest, int yDest, int cxDest, int cyDest,
                      HDC hdcSrc, int xSrc, int ySrc, int cxSrc, int cySrc,
                      BLENDFUNCTION blendFunction)
{
    FIXME("stub\n");
    SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
    return FALSE;
}

/******************************************************************************
 *           DllInitialize   (MSIMG32.@)
 */
BOOL WINAPI DllInitialize(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            DisableThreadLibraryCalls(hinstDLL);
            break;
    }
    return TRUE;
}

/******************************************************************************
 *           GradientFill   (MSIMG32.@)
 */
BOOL WINAPI GradientFill(HDC hdc, PTRIVERTEX pVertex, ULONG nVertex,
                        PVOID pMesh, ULONG nMesh, ULONG ulMode)
{
    FIXME("stub\n");
    SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
    return FALSE;
}

/******************************************************************************
 *           TransparentBlt   (MSIMG32.@)
 */
BOOL WINAPI TransparentBlt(_In_ HDC hdcDest, _In_ int xDest, _In_ int yDest, _In_ int cxDest, _In_ int cyDest,
                          _In_ HDC hdcSrc, _In_ int xSrc, _In_ int ySrc, _In_ int cxSrc, _In_ int cySrc,
                          _In_ UINT colorTransparent)
{
    FIXME("stub\n");
    SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
    return FALSE;
}
