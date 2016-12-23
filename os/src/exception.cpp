////////////////////////////////////////////////////////////////////////////////
//    Copyright (C) 2016  Angelo Coppi (angelogkcop at hotmail.com )
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
////////////////////////////////////////////////////////////////////////////////
// Author : Angelo Coppi (coppi dot angelo at virgilio dot it )
// History:        
////////////////////////////////////////////////////////////////////////////////

#include "hal.h"

#include "bcm2836.h"

#include "sysleds.h"
#include "interrupt.h"
#include "miniuart.h"
#include "exception.h"

u32 caException::IoctlReq(ioCtrlFunction request, u32 *p1, u32 *p2) {
    u32 res = FALSE;
    *p1 -= 4;
    switch (request) {
        case ioCtrlFunction::caExceptionBadAlloc:
            Dbg::Put("\r\n\r\nBad Alloc Exception at address = ", *p1);
            break;
        case ioCtrlFunction::caExceptionLengthError:
            Dbg::Put("\r\n\r\nLenght Error Exception at address = ", *p1);
            break;
        case ioCtrlFunction::caExceptionOutOfRange:
            Dbg::Put("\r\n\r\nOut Of Range Exception at address = ", *p1);
            Dbg::Put((const char *) p2);
            break;
        case ioCtrlFunction::caExceptionInvalidObject:
            Dbg::Put("\r\n\r\nInvalid Object Exception at address = ", *p1);
            Dbg::Put((const char *) p2);
            break;
        case ioCtrlFunction::caExceptionObjectFull:
            Dbg::Put("\r\n\r\nObject Full Exception at address = ", *p1);
            Dbg::Put((const char *) p2);
            break;
        case ioCtrlFunction::caExceptionObjectEmpty:
            Dbg::Put("\r\n\r\nObject Empty Exception at address = ", *p1);
            Dbg::Put((const char *) p2);
            break;
        default:
            break;
    }
    return res;

}
