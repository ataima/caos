#include "config.h"
#include "caos_c_types.h"
#include "test.h"

#if TEST_TOKENIZE_SSTREAM    && TEST 
#include "bcm2836.h"
#include "idevice.h"
#include "stream.h"
#include "auxmain.h"
#include "gpio.h"
#include "miniuart.h"
#include "interrupt.h"
#include "memaux.h"

s8 buffout[1024];
s8 buff8[1024];

static void test_01(u32 & success, u32 &failed) {
    caTokenizeSStream<s8> iss;
    caStringStream<s8> ss;
    ss.Init(buffout, 1024);
    iss.Init(buff8, 1024, 0);
    if (iss.Size() == 0) {
        success++;
    } else {
        failed++;
        ss.Clear();
        ss << "TEST TOKENIZER : FAILED TEST 01:01 " << caEnd::endl;
        Dbg::Put((const char *) ss.Str());
    }
    if (iss.Capacity() == 1023) {
        success++;
    } else {
        failed++;
        ss.Clear();
        ss << "TEST TOKENIZER : FAILED TEST 01:02 " << caEnd::endl;
        Dbg::Put((const char *) ss.Str());
    }
    if (iss.Empty() == true) {
        success++;
    } else {
        failed++;
        ss.Clear();
        ss << "TEST TOKENIZER : FAILED TEST 01:03 " << caEnd::endl;
        Dbg::Put((const char *) ss.Str());
    }

    if (iss.Str() == buff8) {
        success++;
    } else {
        failed++;
        ss.Clear();
        ss << "TEST TOKENIZER : FAILED TEST 01:04 " << caEnd::endl;
        Dbg::Put((const char *) ss.Str());
    }
}

static void test_02(u32 & success, u32 &failed) {
    caTokenizeSStream<s8> iss;
    caStringStream<s8> ss;
    caStringStream<s8> tt;
    s8 buft[1024];
    ss.Init(buffout, 1024);
    tt.Init(buft, 1024);
    iss.Init(tt);
    if (iss.Size() == 0) {
        success++;
    } else {
        failed++;
        ss.Clear();
        ss << "TEST TOKENIZER : FAILED TEST 02:01 " << caEnd::endl;
        Dbg::Put((const char *) ss.Str());
    }
    if (iss.Capacity() == 1023) {
        success++;
    } else {
        failed++;
        ss.Clear();
        ss << "TEST TOKENIZER : FAILED TEST 02:02 " << caEnd::endl;
        Dbg::Put((const char *) ss.Str());
    }
    if (iss.Empty() == true) {
        success++;
    } else {
        failed++;
        ss.Clear();
        ss << "TEST TOKENIZER : FAILED TEST 02:03 " << caEnd::endl;
        Dbg::Put((const char *) ss.Str());
    }

    if (iss.Str() == buft) {
        success++;
    } else {
        failed++;
        ss.Clear();
        ss << "TEST TOKENIZER : FAILED TEST 02:04 " << caEnd::endl;
        Dbg::Put((const char *) ss.Str());
    }
    tt << "pippo pluto paperino";
    iss.Forward(tt.Size());
    if (iss.Size() == tt.Size()) {
        success++;
    } else {
        failed++;
        ss.Clear();
        ss << "TEST TOKENIZER : FAILED TEST 02:04 " << caEnd::endl;
        Dbg::Put((const char *) ss.Str());
    }
    TokenString<s8> res;
    iss >> res;
    u32 i = caMemAux::StrNCmp(res.ptr, "pippo", 5);
    if (i == 0) {
        success++;
    } else {
        failed++;
        ss.Clear();
        ss << "TEST TOKENIZER : FAILED TEST 02:05 " << caEnd::endl;
        Dbg::Put((const char *) ss.Str());
    }
    if (res.size == 5) {
        success++;
    } else {
        failed++;
        ss.Clear();
        ss << "TEST TOKENIZER : FAILED TEST 02:06 " << caEnd::endl;
        Dbg::Put((const char *) ss.Str());
    }

    iss >> res;
    i = caMemAux::StrNCmp(res.ptr, "pluto", 5);
    if (i == 0) {
        success++;
    } else {
        failed++;
        ss.Clear();
        ss << "TEST TOKENIZER : FAILED TEST 02:07 " << caEnd::endl;
        Dbg::Put((const char *) ss.Str());
    }
    if (res.size == 5) {
        success++;
    } else {
        failed++;
        ss.Clear();
        ss << "TEST TOKENIZER : FAILED TEST 02:08 " << caEnd::endl;
        Dbg::Put((const char *) ss.Str());
    }

    iss >> res;
    i = caMemAux::StrNCmp(res.ptr, "paperino", 8);
    if (i == 0) {
        success++;
    } else {
        failed++;
        ss.Clear();
        ss << "TEST TOKENIZER : FAILED TEST 02:09 " << caEnd::endl;
        Dbg::Put((const char *) ss.Str());
    }
    if (res.size == 8) {
        success++;
    } else {
        failed++;
        ss.Clear();
        ss << "TEST TOKENIZER : FAILED TEST 02:10 " << caEnd::endl;
        Dbg::Put((const char *) ss.Str());
    }

    iss >> res;
    if (res.ptr == NULL) {
        success++;
    } else {
        failed++;
        ss.Clear();
        ss << "TEST TOKENIZER : FAILED TEST 02:11 " << caEnd::endl;
        Dbg::Put((const char *) ss.Str());
    }
    if (res.size == 0) {
        success++;
    } else {
        failed++;
        ss.Clear();
        ss << "TEST TOKENIZER : FAILED TEST 02:12 " << caEnd::endl;
        Dbg::Put((const char *) ss.Str());
    }
    tt.Clear();
    iss.Clear();
    tt << "12345678 0x12345678 0b101001001";
    iss.Init(tt);
    iss >> i;
    if (i == 12345678) {
        success++;
    } else {
        failed++;
        ss.Clear();
        ss << "TEST TOKENIZER : FAILED TEST 02:13 " << caEnd::endl;
        Dbg::Put((const char *) ss.Str());
    }
    iss >> i;
    if (i == 0x12345678) {
        success++;
    } else {
        failed++;
        ss.Clear();
        ss << "TEST TOKENIZER : FAILED TEST 02:14 " << caEnd::endl;
        Dbg::Put((const char *) ss.Str());
    }
    iss >> i;
    if (i == 0x149) {
        success++;
    } else {
        failed++;
        ss.Clear();
        ss << "TEST TOKENIZER : FAILED TEST 02:15 " << caEnd::endl;
        Dbg::Put((const char *) ss.Str());
    }

    iss.Init(tt);
    u32 a, b, c;
    iss >> a >> b >> c;
    if (a == 12345678) {
        success++;
    } else {
        failed++;
        ss.Clear();
        ss << "TEST TOKENIZER : FAILED TEST 02:16 " << caEnd::endl;
        Dbg::Put((const char *) ss.Str());
    }
    if (b == 0x12345678) {
        success++;
    } else {
        failed++;
        ss.Clear();
        ss << "TEST TOKENIZER : FAILED TEST 02:17 " << caEnd::endl;
        Dbg::Put((const char *) ss.Str());
    }
    if (c == 0x149) {
        success++;
    } else {
        failed++;
        ss.Clear();
        ss << "TEST TOKENIZER : FAILED TEST 02:18 " << caEnd::endl;
        Dbg::Put((const char *) ss.Str());
    }
    tt.Clear();
    iss.Init(tt);
    iss >> a >> b >> c;
    if (a == 0 && b == 0 && c == 0) {
        success++;
    } else {
        failed++;
        ss.Clear();
        ss << "TEST TOKENIZER : FAILED TEST 02:19 " << caEnd::endl;
        Dbg::Put((const char *) ss.Str());
    }
    if (iss.Good() == false) {
        success++;
    } else {
        failed++;
        ss.Clear();
        ss << "TEST TOKENIZER : FAILED TEST 02:20 " << caEnd::endl;
        Dbg::Put((const char *) ss.Str());
    }

}

void test_Tokenize_sstream(u32 &success, u32 & failed) {
    test_01(success, failed);
    test_02(success, failed);
}
#endif