#ifndef CORTEXV7_H
#define CORTEXV7_H
////////////////////////////////////////////////////////////////////////////////
//    Copyright (C) 2011  Angelo Coppi (angelogkcop at hotmail.com )
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

/* here struct of description for return result of mrc p15 x xx cx cx xx*/


typedef union main_id_register_u {

    struct tag_main_id_register {
        _R_ u32 revision : 4;
        _R_ u32 part_number : 12;
        _R_ u32 arch : 4;
        _R_ u32 variant : 4;
        _R_ u32 manft : 8;

    } asBit;
    _R_ u32 asReg;
} MainIdRegister;

typedef union isar3_u {

    struct tag_isar3 {
        _R_ u32 staurate_inst : 4;
        _R_ u32 smid_inst : 4;
        _R_ u32 svc_inst : 4;
        _R_ u32 sync_prim_inst : 4;
        _R_ u32 tab_branch_inst : 4;
        _R_ u32 trumb_copy_inst : 4;
        _R_ u32 true_nop_inst : 4;
        _R_ u32 trumb_ee_inst : 4;
    } asBit;
    _R_ u32 asReg;
} Isar3;

typedef union un_sys_control {

    struct tag_sys_control {
        _RW_ u32 M_e : 1;
        _RW_ u32 A_e : 1;
        _RW_ u32 C_e : 1;
        _RW_ u32 reserved_0 : 4;
        _RW_ u32 reserved_1 : 3;
        _RW_ u32 SW_e : 1;
        _RW_ u32 Z_e : 1;
        _RW_ u32 I_e : 1;
        _RW_ u32 V_e : 1;
        _RW_ u32 reserved_2 : 5;
        _RW_ u32 WXN : 1;
        _RW_ u32 UWXN : 1;
        _RW_ u32 reserved_7 : 3;
        _RW_ u32 EE : 1;
        _RW_ u32 reserved_10 : 1;
        _RW_ u32 TRE : 1;
        _RW_ u32 AFE : 1;
        _RW_ u32 TE : 1;
        _RW_ u32 reserved_11 : 1;
    } asBit;
    _R_ u32 asReg;
} SysControlRegister;

typedef union un_aux_sys_control {

    struct tag_aux_sys_control {
        _RW_ u32 reserved_0 : 6;
        _RW_ u32 SMP : 1;
        _RW_ u32 reserved_1 : 3;
        _RW_ u32 DODMBS : 1;
        _RW_ u32 L2RADIS : 1;
        _RW_ u32 L1RADIS : 1;
        _RW_ u32 L1PCTL : 2;
        _RW_ u32 DDWM : 2;
        _RW_ u32 reserved_2 : 12;
        _RW_ u32 DDI : 1;
        _RW_ u32 reserved_3 : 3;
    } asBit;
    _RW_ u32 asReg;
} AuxSysControlRegister;

typedef union un_cache_level {

    struct tag_cache_level {
        _R_ u32 ctype1 : 3;
        _R_ u32 ctype2 : 3;
        _R_ u32 reserved : 15;
        _R_ u32 louis : 3;
        _R_ u32 loc : 3;
        _R_ u32 louu : 3;
    } asBit;
    _R_ u32 asReg;
} CacheLevIDStatus;

typedef union un_cache_size {

    struct tag_cache_size {
        _R_ u32 ind : 1;
        _R_ u32 level : 3;
    } asBit;
    _R_ u32 asReg;
} CacheSizeStatus;

typedef union un_TTBR0_format {

    struct tag_TTBR0 {
        _RW_ u32 IRGN1 : 1;
        _RW_ u32 Sbit : 1;
        _RW_ u32 IMP : 1;
        _RW_ u32 RGN : 2;
        _RW_ u32 NOS : 1;
        _RW_ u32 IRGN0 : 1;
        _RW_ u32 reserverd : 8;
        _RW_ u32 TTBADDR : 16;
    } asBit;
    _RW_ u32 asReg;
} TTBR0_Info;

typedef union un_TTB_Section_1MB {

    struct tag_TTB_section_1MB {
        _RW_ u32 PXN : 1;
        _RW_ u32 SECTION : 1; // ALWAYS 1 FOR SECTION
        _RW_ u32 B : 1;
        _RW_ u32 C : 1;
        _RW_ u32 XN : 1;
        _RW_ u32 DOMAIN : 4;
        _RW_ u32 reserved : 1;
        _RW_ u32 AP01 : 2;
        _RW_ u32 TEX : 3;
        _RW_ u32 AP2 : 1;
        _RW_ u32 S : 1;
        _RW_ u32 nG : 1;
        _RW_ u32 always0 : 1;
        _RW_ u32 nS : 1;
        _RW_ u32 BASE : 12;
    } asBit;
    _RW_ u32 asReg;
} TTBSection1MB;
#endif /* CORTEXV7_H */

