#ifndef ARM_V7_HEADER
#define ARM_V7_HEADER
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

