#ifndef CONSOLE_H
#define CONSOLE_H
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



typedef deviceError(*consoleFunc)(caDevicePort &port, caTokenizeSStream <u8> & iss);

typedef struct tag_pair_str_func {
    const char * cmd;
    size_t size;
    const char * help;
    consoleFunc func;
} pairCmd;



#define MAX_CONSOLE_COMMAND   8

class caConsole {
private:
    static pairCmd commands[MAX_CONSOLE_COMMAND];
    static s8 buffio[2048];
private:
    static void Dump(u8 * buff, u32 size, caStringStream<s8> &ss);
    static void Error(caDevicePort &port, caTokenizeSStream <u8> & iss, u32 errno);
    static u32 SyntaxError(caDevicePort &port, caTokenizeSStream <u8> & iss);
    static deviceError Command_PS(caDevicePort &port, caTokenizeSStream <u8> & iss);
    static deviceError Command_MEM(caDevicePort &port, caTokenizeSStream <u8> & iss);
    static deviceError Command_HELP(caDevicePort &port, caTokenizeSStream <u8> & iss);
    static deviceError Command_INFO(caDevicePort &port, caTokenizeSStream <u8> & iss);
    static deviceError Command_HARDWARE(caDevicePort &port, caTokenizeSStream <u8> & iss);
    static deviceError Command_READ_DEVICE(caDevicePort &port, caTokenizeSStream <u8> & iss);
    static deviceError Command_WRITE_DEVICE(caDevicePort &port, caTokenizeSStream <u8> & iss);
    static deviceError Command_QUIT(caDevicePort &port, caTokenizeSStream <u8> & iss);
public:
    static u32 Init(void);
    static deviceError Execute(caTokenizeSStream <u8> & in, caDevicePort &out);
};


#endif /* CONSOLE_H */

