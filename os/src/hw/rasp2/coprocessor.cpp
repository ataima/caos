////////////////////////////////////////////////////////////////////////////////
//    Copyright (C) 2016  Angelo Coppi (angelogkcop at hotmail.com )
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

#include "hal.h"

#include "bcm2836.h"

#include "cpu.h"
#include "coprocessor.h"



void caArmCprs::DumpDFSR(){       
    s8 buff[512];
    caStringStream<s8> ss;
    ss.Init(buff,512);
    DumpDFSR(ss);
    Dbg::Put(ss.Str());
}

u32 caArmCprs::DumpDFSR(caStringStream<s8> & ss){       
    u32 res = 0;
    cprs_dfsr dfsr;
    dfsr.asReg=GetDFSR();
    ss << " --- DATA FAULT REGISTER ( SHORT FORMAT )---" << caEnd::endl;
    ss << caStringFormat::hex;
    ss<<"As Reg  = "<<dfsr.asReg << caEnd::endl;
    res=dfsr.asBit.most_fault_bit<<4 | dfsr.asBit.fault_status;
    ss<<"fault status = ";
    switch(res)
    {
        case 0:
            ss << "No fault status";
            break;
        case 1:
            ss << "Alignment fault";
            break;
        case 2:
            ss << "Debug event";
            break;
        case 3:
            ss << "Access flag fault, section";
            break;
        case 4:
            ss << "Instruction cache maintenance fault";
            break;
        case 5:
            ss << "Translation fault, section";
            break;
        case 6:
            ss << "Access flag fault, page";
            break;
        case 7:
            ss << "Translation fault, page";
            break;
        case 8:
            ss << "Synchronous external abort, non-translation";
            break;
        case 9:
            ss << "Domain fault, section";
            break;
        case 10:
            ss << "undefined 10";
            break;
        case 11:
            ss << "Domain fault, page";
            break;
        case 12:
            ss << "Synchronous external abort on translation table walk, 1st level";
            break;
        case 13:
            ss << "Permission fault, section";
            break;
        case 14:
            ss << "Synchronous external abort on translation table walk, 2nd level";
            break;
        case 15:
            ss << "Permission fault, page";
            break;
        case 16:
            ss << "Asynchronous external abort";
            break;
        case 17:
            ss << "undefined 17";
            break;
        case 18:
            ss << "undefined 18";
            break;
        case 19:
            ss << "undefined 19";
            break;
        case 20:
            ss << "undefined 20";
            break;
        case 21:
            ss << "undefined 21";
            break;
        case 22:
            ss << "undefined 22";
            break;
        case 23:
            ss << "undefined 23";
            break;
        case 24:
            ss << "undefined 24";
            break;
        case 25:
            ss << "undefined 25";
            break;
        case 26:
            ss << "undefined 26";
            break;
        case 27:
            ss << "undefined 27";
            break;
        case 28:
            ss << "undefined 28";
            break;
        case 29:
            ss << "undefined 29";
            break;
        case 30:
            ss << "undefined 30";
            break;
        case 31:
            ss << "undefined 31";
            break;
    }
    ss<< caEnd::endl;
    ss<<"domain = "<<dfsr.asBit.domain<< caEnd::endl;
    ss<<"raz    = "<<dfsr.asBit.raz<< caEnd::endl;
    ss<<"wnr    = "<<dfsr.asBit.wnr<< caEnd::endl;
    ss<<"ext    = "<<dfsr.asBit.ext<< caEnd::endl;
    ss<<"cache_m= "<<dfsr.asBit.cache_m<< caEnd::endl;
        ss.Str();
        res = ss.Size();
    return res;
}



u32 caArmCprs::DumpISR(caStringStream<s8> & /*ss*/){
    return 0;
}