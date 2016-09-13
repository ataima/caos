#include "config.h"
#include "bcm2836.h"
#include "idevice.h"
#include "stream.h"
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
