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

void caDeviceHandle::Dump(caDeviceHandle & port, caStringStream<s8> & ss) {
    ss << caStringFormat::hex;
    ss << "HANDLE    =" << port.handle << caEnd::endl;
    ss << "STATUS    =" << port.status << caEnd::endl;
    ss << "ERROR     =" << port.error << caEnd::endl;
    ss << "TIME START=" << port.tStart << caEnd::endl;
    ss << "TIME STOP =" << port.tStop << caEnd::endl;
    ss << "TIME LAST =" << port.tLast << caEnd::endl;
    ss << "LAST CMD  =" << port.tLastCmd << caEnd::endl;
    ss << "WR BUFF   =" << (u32) port.wrBuff << caEnd::endl;
    ss << "WR SIZE   =" << port.wrSize << caEnd::endl;
    ss << "WRITED    =" << port.writed << caEnd::endl;
    ss << "RD BUFF   =" << (u32) port.rdBuff << caEnd::endl;
    ss << "RD SIZE   =" << port.rdSize << caEnd::endl;
    ss << "READED    =" << port.readed << caEnd::endl;
    ss.Str();
}