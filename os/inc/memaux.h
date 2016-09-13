#ifndef MEMAUX_HEADER
#define MEMAUX_HEADER

class caMemAux {
public:
    static void* MemCpy(u32 *dest, u32 *src, u32 size);
    static void* MemSet(u32 *dest, u32 pat, u32 size);
    static void* MemZero(u32 *dest, u32 size);
    static void* MemMove(u32 *dest, u32 *src, u32 size);
    static void* MemMove(u16 *dest, u16 *src, u32 size);
    static void* MemMove(u8 *dest, u8 *src, u32 size);
    static u32 StrICmp(const char *s1, const char *s2);
    static u32 StrCmp(const char *s1, const char *s2);
    static u32 StrNCmp(char *s1, const char *s2, u32 max);
    static s8* StrCpy(char *s1, const char *s2);
    static s8* StrNCpy(char *s1, const char *s2, u32 max);
    static u32 StrLen(const char *s1);
};




#endif 

