#ifndef CA_KDEBUG_HEADER
#define CA_KDEBUG_HEADER

// DIRECT TO MINIUART LOW LEVEL DEBUG
namespace Dbg {

    typedef enum _format_ {
        hex, dec, bin
    } kformat;


    extern void Put(const char *s, void *p, u32 v, kformat f = kformat::hex, u32 cr = TRUE);
    extern void Put(const char *msg, u32 val, kformat format = hex, u32 cr = TRUE);
    extern void Put(const char *msg);
}

extern "C" {
    void msgWelcome(void);
    void msgByeBye(void);
    void msgMMU(void);
    void msgCACHE(void);
    void msgSTART(void);
}

#if TRACE 


#define TIN()       u32 trace;\
                    Dbg::Put("\r\nIN : ");\
                    Dbg::Put(__func__);\
                    Dbg::Put("\r\n");\
                    trace=caSysTimer::ReadFreeCounter();\

#define TOUT()      trace=caSysTimer::ReadFreeCounter()-trace;\
                    Dbg::Put("\r\nOUT : ");\
                    Dbg::Put(__func__);\
                    Dbg::Put(" : uSec =",trace,Dbg::kformat::dec);

#else
#define TIN()
#define TOUT()
#endif


#endif 

