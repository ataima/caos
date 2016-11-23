

#include <stdio.h>
#include <ctype.h>
#include <stddef.h>
#include <string.h>
#include <map>
#include <stdint.h>
#include "memoryvalidator.h"



#ifdef _SAFE_MEMORY_LEAK
//#define EXT_DUMP_ALLOCA
//variabili globali di appoggio
/*
char _mn_file_Rq [MAX_STR_REC*2];
int  _mn_line_Rq ;
static bool _mn_record = true;
static FILE *_mn_file;
static int _mnAlloc=0;
*/

char  MemoryValidator::_mn_file_Rq [MAX_STR_REC*2];
char  MemoryValidator::_mn_lastfile_Rq [MAX_STR_REC*2];
int   MemoryValidator::_mn_lastalloc_Rq;
int   MemoryValidator::_mn_line_Rq =0;
int   MemoryValidator::_mn_lastline_Rq =0;
bool  MemoryValidator::_mn_record = true;
FILE *MemoryValidator::_mn_file=NULL;
int   MemoryValidator::_mnAlloc=0;
int   MemoryValidator::_mnAllocMax=0;
std::map<void *, _mn_Ident_Mem *> MemoryValidator::_mn_alloc;
// char *cs_stream::buff=new char[0x3fff];

void _mn_Ident_Mem::Set(void * _ptr,size_t _size,const char *_file,int _line,int rel_alloc)
{
    ptrMem =_ptr;
    size=_size;
    strncpy(file,_file,MAX_STR_REC);
    line=_line;
    if (rel_alloc==0)
        numAlloc=MemoryValidator::getLeakNum();
    else
        numAlloc=rel_alloc;
}
//
void _mn_Ident_Mem::dump(int nm,FILE *out_file)
{
#ifdef WIN32
#pragma warning( disable : 4311)
#endif
    fprintf(out_file,"%04d) %05d 0x%08x%5ld  %s,%d",nm,numAlloc,reinterpret_cast<long>(ptrMem),size,file,line);
#ifdef WIN32
#pragma warning( default : 4311)
#endif
    // if (strlen(file)==0)
    if (file=='\0')
    {
        fprintf(out_file,"=> ");
        size_t i;
        size_t p=60;
        if (size<p)
            p=size;
        unsigned char  v[60];
        for (i=0; i<p; i++)
        {
            v[i] = *(unsigned char *)( (int64_t)((int64_t)(ptrMem)+i));
        }
        for (i=0; i<p; i++)
        {
            if (isprint(v[i])!=0)
                fprintf(out_file,"%c",v[i]);
            else
                fprintf(out_file,".");
        }
        fprintf(out_file," - ");
        for (i=0; i<p; i++)
        {
            fprintf(out_file,"%02X ",v[i]);
        }
    }
    fprintf(out_file,"\n");
    fflush(out_file);
    /*
    int i;
    size_t p=0;
    i=0;
    while(p<size)
    {
    int j;
    unsigned char  v[34];
    for(i=0;i<34 && p<size;i++,p++)
    {
    			v[i] = *(unsigned char *)(ptrMem+p);
    			}
    			j=i;
    			for(i=0;i<j;i++)
    			{
    			fprintf(_mn_file,"%02X.",v[i]);
    			}
    			fprintf(_mn_file," - ");
    			for(i=0;i<j;i++)
    			{
    			if(isprint(v[i]) )
    			fprintf(_mn_file,"%c",v[i]);
    			else
    			fprintf(_mn_file,".");

    			  }
    			  fprintf(_mn_file,"\n");
    			  }
    */
}

////////////////////////////////////////////////////


void MemoryValidator::dumpMemory(int mode)
{
    bool oldFlg;
    int nm=1;

    fprintf(_mn_file,"--------\n");
    fprintf(_mn_file,"MEMORY VALIDATOR :: WARNING MEMORY LEAKS!\n");
    fflush(_mn_file);
    fprintf(_mn_file,"ALLOCATED BLOCKS = %d\n",_mn_alloc.size());
    fflush(_mn_file);
    if (mode!=0)fprintf(_mn_file,"INDX  NUM      PTR       SIZE    FILE                                 CODE               LINE\n");
    fflush(_mn_file);
    oldFlg=_mn_record;
    _mn_record=true;
    size_t leaksize=0;
    //
    std::map<void *,_mn_Ident_Mem *>::iterator it;
    //
    for (it=_mn_alloc.begin(); it!=_mn_alloc.end(); it++)
    {
        _mn_Ident_Mem *tmp = (*it).second;
        if (mode!=0)tmp->dump(nm++,_mn_file);
        fflush(_mn_file);
        leaksize+=tmp->getSize();
    }
    //
    if (leaksize!=0)
        fprintf(_mn_file,"SIZE ALLOCATED = %ld\n",leaksize);
    fflush(_mn_file);
    fprintf(_mn_file,"--------\n");
    _mn_record=oldFlg;
}
void MemoryValidator::startRecordMem(const char *filename)
{
    _mn_record=false;
    _mn_file = fopen(filename,"w");
}

void MemoryValidator::startRecordMem(FILE *file)
{
    _mn_record=false;
    _mn_file = file;
}

void MemoryValidator::EndRecordMem(void)
{
    _mn_record=true;
    if (_mn_alloc.size()!=0)
    {
        dumpMemory(1);
        clean();
        fflush(_mn_file);
    }
    fflush(_mn_file);
    if (_mn_file!=stderr)
        fclose(_mn_file);
}

void  MemoryValidator::exitProg(int value)
{
    _mn_record=true;
    if (_mn_alloc.size()!=0)
    {
        std::map<void *,_mn_Ident_Mem *>::iterator it;
        for (it=_mn_alloc.begin(); it!=_mn_alloc.end(); it++)
        {
            free((*it).second);
        }
        //
    }
    fprintf(_mn_file,"EXIT FROM PROGRAM\n");
    fflush(_mn_file);
    if (_mn_file!=stderr)
        fclose(_mn_file);
    exit(value);
}




void * MemoryValidator::Malloc(size_t len) throw ()
{
    //alloca memoria pulita
    void* ptrMem = ::malloc(len);
    if (_mn_record==false)
    {
        bool noFile=false;
        _mn_record=true;
        _mn_Ident_Mem *newBlock =new _mn_Ident_Mem;
        if (_mn_file_Rq[0]==0 )
        {
            strcpy(_mn_file_Rq,"LAST ALLOC > ");
            strcat(_mn_file_Rq,_mn_lastfile_Rq);
            _mn_line_Rq=_mn_lastline_Rq;
            _mn_lastalloc_Rq++;
            noFile=true;
        }
        newBlock->Set(ptrMem,len,_mn_file_Rq,_mn_line_Rq,_mn_lastalloc_Rq);
        if (noFile==false )
        {
            strcpy(_mn_lastfile_Rq,_mn_file_Rq);
            _mn_lastline_Rq=_mn_line_Rq;
            _mn_lastalloc_Rq=0;
        }

        _mn_line_Rq=0;
        _mn_alloc[ptrMem]=newBlock;
        _mn_file_Rq[0]=0;
        _mnAlloc++;
        if (_mnAlloc>_mnAllocMax)
            _mnAllocMax=_mnAlloc;
#ifdef EXT_DUMP_ALLOCA
        fprintf(_mn_file,"Memory Validator :: NEW!\n");
        fflush(_mn_file);
        newBlock->dump(_mnAlloc,_mn_file);
#endif
        _mn_record=false;
    }
    if (ptrMem==NULL)
    {
        fprintf(_mn_file,"WARNING MEMORY FAILURE!\n");
        fflush(_mn_file);
    }
    return ptrMem;
}

void MemoryValidator::Free(void *ptr ) throw ()
{
    if (ptr==NULL)
    {
//	fprintf(_mn_file,"WARNING FREE NULL POINTER!\n");
//	fflush(_mn_file);
        return;
    }
    if (_mn_record==false && _mn_alloc.size()!=0)
    {
        _mn_record=true;
        std::map<void *,_mn_Ident_Mem *>::iterator it= _mn_alloc.find(ptr);
        if (it!=_mn_alloc.end())
        {
#ifdef EXT_DUMP_ALLOCA
            fprintf(_mn_file,"Memory Validator :: DELETE!\n");
            fflush(_mn_file);
            _mn_Ident_Mem * Block=it->second;
            Block->dump(_mnAlloc,_mn_file);
#endif
            delete it->second;
            _mn_alloc.erase(it);
            _mnAlloc--;
            free(ptr);
        }
        // se ho allocato con _mn_record=true finisce qui
        else
        {
            fprintf(_mn_file,"Memory Validator :: PTR =%08lX non esiste nella lista dei puntatori attivi:\r\n  Allocazione statica oppure  doppia chiamata a delete\n",ptr);
            fflush(_mn_file);
        }
        _mn_record=false;
    }
    else
        free(ptr);
}


void MemoryValidator::clean(void)
{
    std::map<void *,_mn_Ident_Mem *>::iterator it;
    for (it=_mn_alloc.begin(); it!=_mn_alloc.end(); it++)
    {
        free(it->first);
        free(it->second);
    }
    _mn_alloc.clear();
    _mnAlloc=0;
    _mnAllocMax=0;
}
////////////////////////////////////////////////////

void * operator new (size_t len) 
{
    return MemoryValidator::Malloc(len);
}

void * operator new[] (size_t  len)
{
    return MemoryValidator::Malloc(len);
}
void operator delete[] ( void *ptr )throw ()
{
    MemoryValidator::Free(ptr);
}

void operator delete ( void *ptr )throw ()
{
    MemoryValidator::Free(ptr);
}




#endif
