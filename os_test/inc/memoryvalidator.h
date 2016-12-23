/*-------------------------------------------
|		ANGELO COPPI CASE REFACTOR		|
|       VERS.1.0	2005....			|
|	    angelogkcop@hotmail.com			|
|		coppi.angelo@virgilio.it		|
----------------------------------------*/

#ifndef __MEMORYVALIDATOR_H__
#define __MEMORYVALIDATOR_H__
//#define  EXT_DUMP_ALLOCA
#define _SAFE_MEMORY_LEAK


#ifdef _SAFE_MEMORY_LEAK
#define MAX_STR_REC    256

class _mn_Ident_Mem {
public:

    ~_mn_Ident_Mem() {
    }
    void Set(void * _ptr, size_t _size, const char *_file, int _line, int rel_alloc);
    void dump(int nm, FILE * file);

    inline size_t getSize(void) {
        return size;
    }

    inline void * getPtr(void) {
        return ptrMem;
    }

    inline char * getFile(void) {
        return file;
    }

    inline int getLine(void) {
        return line;
    }
private:
    size_t size;
    char file[MAX_STR_REC];
    int line;
    void * ptrMem;
    int numAlloc;
};

class MemoryValidator {
private:
    static std::map<void *, _mn_Ident_Mem *> _mn_alloc;
    static bool _mn_record;
    static int _mnAlloc;
    static int _mnAllocMax;
    static FILE *_mn_file;
    static void clean();
public:
    static void dumpMemory(int mode = 1);
    static void startRecordMem(const char *filename);
    static void startRecordMem(FILE *filename);
    static void EndRecordMem(void);
    static void exitProg(int value);

    static int getLeakNum(void) {
        return _mnAlloc;
    }

    static int getLeakMaxNum(void) {
        return _mnAllocMax;
    }
    static void *Malloc(size_t len) throw ();
    static void Free(void *ptr) throw ();
    static char _mn_file_Rq [MAX_STR_REC * 2];
    static char _mn_lastfile_Rq[MAX_STR_REC * 2];
    static int _mn_line_Rq;
    static int _mn_lastline_Rq;
    static int _mn_lastalloc_Rq;
    // ONLY FOR TEST

    inline FILE* getFile(void) {
        return _mn_file;
    }

    inline std::map<void *, _mn_Ident_Mem *> *getMap(void) {
        return &_mn_alloc;
    }

    static void enableRecordMem(void) {
        _mn_record = false;
    }

    static void disableRecordMem(void) {
        _mn_record = true;
    }
};
#define START_RECORD_MEM(a)  MemoryValidator::startRecordMem(a)
#define END_RECORD_MEM()  MemoryValidator::EndRecordMem()
#define DUMP_MMEM()    MemoryValidator::dumpMemory()
#define EXIT(a)     MemoryValidator::exitProg(a)
#define GET_LEAK_NUM()          MemoryValidator::getLeakNum()
#define GET_MAX_ALLOC_NUM()     MemoryValidator::getLeakMaxNum()
#else
#define START_RECORD_MEM(a)
#define END_RECORD_MEM()
#define DUMP_MMEM()
#define EXIT(a)     exit(a)
#define GET_LEAK_NUM()
#define GET_MAX_ALLOC_NUM()
#endif

///////////////////////////////////////////////////////////////////
#ifdef _SAFE_MEMORY_LEAK
extern char _mn_file_Rq [MAX_STR_REC * 2];
extern int _mn_line_Rq;
#endif




#ifdef _SAFE_MEMORY_LEAK
#define _safeNEW(a,b)  \
        {\
	strncpy(MemoryValidator::_mn_file_Rq,__FILE__,MAX_STR_REC);\
	strcat(MemoryValidator::_mn_file_Rq," :: _safeNEW(");\
	strcat(MemoryValidator::_mn_file_Rq,#a);\
	strcat(MemoryValidator::_mn_file_Rq,",");\
	strcat(MemoryValidator::_mn_file_Rq,#b);\
	strcat(MemoryValidator::_mn_file_Rq,")");\
	MemoryValidator::_mn_line_Rq=__LINE__;\
	a=new b;\
	}
#else
#define _safeNEW(a,b)  a=new b;
#endif

#ifdef _SAFE_MEMORY_LEAK
#define _safe_STD_NEW(a,b,c)  \
        {\
	strncpy(MemoryValidator::_mn_file_Rq,__FILE__,MAX_STR_REC);\
	strcat(MemoryValidator::_mn_file_Rq," :: _safe_STD_NEW(");\
	strcat(MemoryValidator::_mn_file_Rq,#a);\
	strcat(MemoryValidator::_mn_file_Rq,",");\
	strcat(MemoryValidator::_mn_file_Rq,#b);\
	strcat(MemoryValidator::_mn_file_Rq,")");\
        strcat(MemoryValidator::_mn_file_Rq," - ");\
        strcat(MemoryValidator::_mn_file_Rq,b::gc->getName());\
        strcat(MemoryValidator::_mn_file_Rq," - ");\
	MemoryValidator::_mn_line_Rq=__LINE__;\
	a=reinterpret_cast<b*>(MemoryValidator::Malloc(c));\
	}
#else
#define _safe_STD_NEW(a,b,c)  \
        a=reinterpret_cast<b*>(malloc(sizeof(c)));
#endif



// PERCH� NON USO SEMPRE _safeDELETE invece di delete ?
// RISPOSTA:
// devo usare delete perche' cosi posso implemntare classi con GC
// senza verificare tutti i _safeDELETE .
#ifdef _SAFE_MEMORY_LEAK
#define _safeDELETE(a)  MemoryValidator::Free(a);
#else
#define _safeDELETE(a)  free(a);
#endif
///////////////////////////////////////////////////////////////////
/*
extern void * operator new (size_t len)throw (std::bad_alloc);

extern void * operator new[] (size_t len)throw (std::bad_alloc);

extern void operator delete[] (void *ptr)throw ();

extern void operator delete ( void *ptr)throw ();
 */



#endif

