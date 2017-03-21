#ifndef CONSOLE_H
#define CONSOLE_H
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



typedef deviceError(*consoleFunc)(caDeviceHandle &port, caTokenizeSStream <u8> & iss);

typedef struct tag_pair_str_func {
    const char * cmd;
    s_t size;
    const char * help;
    consoleFunc func;
} pairCmd;



#define MAX_CONSOLE_COMMAND   8
#define CONS_LINE_LENGHT 1024 

class caConsole {
private:
    static pairCmd commands[MAX_CONSOLE_COMMAND];
    static s8 buffio[2048];
private:
    static void Dump(u8 * buff, u32 size, caStringStream<s8> &ss);
    static void Error(caDeviceHandle &port, caTokenizeSStream <u8> & iss, u32 errno);
    static u32 SyntaxError(caDeviceHandle &port, caTokenizeSStream <u8> & iss);
    static deviceError Command_PS(caDeviceHandle &port, caTokenizeSStream <u8> & iss);
    static deviceError Command_MEM(caDeviceHandle &port, caTokenizeSStream <u8> & iss);
    static deviceError Command_HELP(caDeviceHandle &port, caTokenizeSStream <u8> & iss);
    static deviceError Command_INFO(caDeviceHandle &port, caTokenizeSStream <u8> & iss);
    static deviceError Command_HARDWARE(caDeviceHandle &port, caTokenizeSStream <u8> & iss);
    static deviceError Command_READ_DEVICE(caDeviceHandle &port, caTokenizeSStream <u8> & iss);
    static deviceError Command_WRITE_DEVICE(caDeviceHandle &port, caTokenizeSStream <u8> & iss);
    static deviceError Command_QUIT(caDeviceHandle &port, caTokenizeSStream <u8> & iss);
    static deviceError Execute(caTokenizeSStream <u8> & in, caDeviceHandle &out);
public:
    static u32 Init(void);  // from caos::Init
    static u32 consoleTask(u32 thIdx, u32 p1, u32 p2 );  //from main
};


#endif /* CONSOLE_H */

