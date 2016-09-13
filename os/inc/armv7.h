#ifndef ARM_V7_HEADER
#define ARM_V7_HEADER

typedef union arm_v7_CPSR {

    struct tag_arm_v7_CPSR {
        _RW_ u32 M : 5;
        _RW_ u32 T : 1;
        _RW_ u32 F : 1;
        _RW_ u32 I : 1;
        _RW_ u32 A : 1;
        _RW_ u32 E : 1;
        _RW_ u32 IT : 6;
        _RW_ u32 GE : 4;
        _RW_ u32 RAZ : 4;
        _RW_ u32 J : 1;
        _RW_ u32 IT_L : 2;
        _RW_ u32 Q : 1;
        _RW_ u32 V : 1;
        _RW_ u32 C : 1;
        _RW_ u32 Z : 1;
        _RW_ u32 N : 1;
    } asBit;
    _RW_ u32 asReg;
} armv7CPSR, *ptrArmv7CPSR;



#endif 

