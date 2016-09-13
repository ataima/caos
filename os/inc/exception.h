#ifndef EXCEPTION_H
#define EXCEPTION_H

class caException {
public:
    static u32 IoctlReq(ioCtrlFunction request, u32 *p1, u32 *p2);
};




#endif /* EXCEPTION_H */

