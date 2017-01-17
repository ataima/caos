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


#include "console.h"
#include "memaux.h"
#include "stream.h"
#include "heaparray.h"
#include "scheduler.h"
#include "memory.h"
#include "caos.h"

s8 caConsole::buffio[2048];
pairCmd caConsole::commands[MAX_CONSOLE_COMMAND];

u32 caConsole::Init(void) {
    commands[0].cmd = "ps";
    commands[0].size = 2;
    commands[0].func = caConsole::Command_PS;
    commands[0].help = "list all system tasks";

    commands[1].cmd = "mem";
    commands[1].size = 3;
    commands[1].func = caConsole::Command_MEM;
    commands[1].help = "more info about RAM memory: \r\n"
            "\tlist : list all mem block from memory manager\r\n"
            "\tdump <address> : dump memory at address as integer\r\n"
            "\tlist <address> : dump memory at address as char\r\n";

    commands[2].cmd = "help";
    commands[2].size = 4;
    commands[2].func = caConsole::Command_HELP;
    commands[2].help = "more info about commands";

    commands[3].cmd = "info";
    commands[3].size = 4;
    commands[3].func = caConsole::Command_INFO;
    commands[3].help = "more info about system";

    commands[4].cmd = "lshw";
    commands[4].size = 4;
    commands[4].func = caConsole::Command_HARDWARE;
    commands[4].help = "list hardware device registers\r\n"
            "\tavailable devices are:\r\n"
            "\t com1 : miniuart serial port \r\n"
            "\t systimer : arm internal system timer";

    commands[5].cmd = "read";
    commands[5].size = 4;
    commands[5].func = caConsole::Command_READ_DEVICE;
    commands[5].help = "read from device : ex read systimer \r\n"
            "\tavailable devices are:\r\n"
            "\t com1 : miniuart serial port \r\n"
            "\t systimer : arm internal system timer";

    commands[6].cmd = "write";
    commands[6].size = 5;
    commands[6].func = caConsole::Command_WRITE_DEVICE;
    commands[6].help = "write to device : ex write com1 hello world \r\n"
            "\tavailable devices are:\r\n"
            "\t com1 : miniuart serial port \r\n"
            "\t systimer : arm internal system timer";

    commands[7].cmd = "quit";
    commands[7].size = 4;
    commands[7].func = caConsole::Command_QUIT;
    commands[7].help = "quitting current console io \r\n"
            "and reload bootloader at address 0x10800";

    return 0;
}

static const char * deviceMsgError[] = {
    "error_unknow_device_name",
    "error_unknow_device_ioctrl",
    "error_open_device",
    "error_close_device",
    "error_write_device",
    "error_read_device",
    "error_Ioctrl_device",
    "error_generic_fail_device",
    "error_device_config_param",
    "error_port_out_config",
    "error_pipe_no_memory",
    "error_pipe_invalid_name",
    "error_pipe_not_exist",
    "error_pipe_no_free_resize",
    "error_pipe_queue_full",
    "error_pipe_port_not_host",
    "error_pipe_port_not_guest",
    "error_pipe_invalid_descriptor",
    "error_invalid_null_port",
    "error_invalid_handle_port",
    "error_device_not_opened",
    "error_read_less_data",
    "error_write_less_data",
    "error_configure_serial_port",
    "error_configure_irq_serial_port",
    "error_stop_serial_port",
    "error_tx_queue_full",
    "error_rx_queue_empty",
    "error_invalid_null_destination",
    "error_device_is_busy",
    "error_signal_already_set",
};

u32 caConsole::SyntaxError(caDeviceHandle &port, caTokenizeSStream <u8> & iss) {
    caStringStream<s8> ss;
    ss.Init(buffio, sizeof (buffio));
    ss << "ciao! : " << (const char *) iss.Str();
    iss.Dump(ss);
    ss << " .. see help..." << caEnd::endl;
    caOS::Write(port, ss);
    return deviceError::error_generic_fail_device;
}

void caConsole::Dump(u8 * buff, u32 readed, caStringStream<s8> &ss) {
    u32 i, u, *ptr = (u32*) buff;
    ss << "READ DEVICE RETURN : " << readed << " bytes" << caEnd::endl;
    ss << caStringFormat::hex;
    for (i = 0; i < readed;) {
        if (!ss.Good())break;
        ss << (u32) ptr << " - ";
        for (u = 0; u < 4 && i < readed; u++, i++) {
            if (!ss.Good())break;
            ss << *ptr++ << "  ";
        }
        ss << caEnd::endl;
    }
    ss << caEnd::endl;
    ss.Str();
}

void caConsole::Error(caDeviceHandle &port, caTokenizeSStream <u8> & iss, u32 errno) {
    caStringStream<s8> ss;
    ss.Init(buffio, sizeof (buffio));
    u32 idx = errno - 1000;
    ss << "Command ' " << (const char *) iss.Str() <<
            " ' return error n." << errno
            << " ( " << deviceMsgError[idx] << " ) " << caEnd::endl;
    caOS::Write(port, ss);
}

///////////////////////////////////////////// COMMANDS

deviceError caConsole::Command_PS(caDeviceHandle &port,
        caTokenizeSStream <u8> & /*iss*/) {
    u32 res = 0;
    caStringStream<s8> ss;
    ss.Init(buffio, sizeof (buffio));
    ss << " --- TASK ---\r\n";
    res = caScheduler::Dump(ss);
    if (res) {
        ss.Forward(res);
        ss << caEnd::endl;
        res = caOS::Write(port, ss);
        return (deviceError) (res);
    }
    return deviceError::error_generic_fail_device;
}

deviceError caConsole::Command_HARDWARE(caDeviceHandle &port,
        caTokenizeSStream <u8> & iss) {
    u32 res = deviceError::no_error;
    caStringStream<s8> ss;
    ss.Init(buffio, sizeof (buffio));
    TokenString<u8> opt;
    iss>>opt;
    if (opt.size > 0) {
        if (caStrAux::StrNCmp((char *) opt.ptr, "com1", 4) == 0) {
            caComDeviceCtrl req;
            req.command = caComDeviceCtrl::IoCtrlDirect::comListHardware;
            req.ss = &ss;
            res = caOS::IoCtrl(port, req);
            if (res == deviceError::no_error) {
                res = caOS::Write(port, ss);
            }
        }
#if SYS_TIMER_DEVICE        
        else
            if (caStrAux::StrNCmp((char *) opt.ptr, "systimer", 8) == 0) {
            caIDeviceConfigure in;
            caDeviceHandle portTimer;
            res = caOS::Open("SYSTIMER", in, portTimer);
            if (res == deviceError::no_error) {
                caSysTimerDeviceCtrl req;
                req.command = caSysTimerDeviceCtrl::IoCtrlDirect::sysTimerListHardware;
                req.ss = &ss;
                res = caOS::IoCtrl(portTimer, req);
                if (res == deviceError::no_error) {
                    res = caOS::Write(port, ss);
                }
                res = caOS::Close(portTimer);
            }
        }
#endif        
    } else {
        SyntaxError(port, iss);
    }
    return (deviceError) res;
}

deviceError caConsole::Command_READ_DEVICE(caDeviceHandle &port,
        caTokenizeSStream <u8> & iss) {
    u32 res = deviceError::no_error;
    caStringStream<s8> ss;
    ss.Init(buffio, sizeof (buffio));
    TokenString<u8> opt;
    u8 rdbuff[128];
    iss>>opt;
    if (opt.size > 0) {
        if (caStrAux::StrNCmp((char *) opt.ptr, "com1", 4) == 0) {
            port.rdBuff = rdbuff;
            port.rdSize = 64;
            port.readed = 0;
            res = caOS::Read(port);
            if (res == deviceError::no_error) {
                if (port.readed > 0) {
                    Dump(rdbuff, port.readed, ss);
                } else {
                    ss << " No Data from " << opt.ptr << caEnd::endl;
                }
                res = caOS::Write(port, ss);
            }
        }
#if SYS_TIMER_DEVICE                
        else
            if (caStrAux::StrNCmp((char *) opt.ptr, "systimer", 8) == 0) {
            caIDeviceConfigure in;
            caDeviceHandle portTimer;
            res = caOS::Open("SYSTIMER", in, portTimer);
            if (res == deviceError::no_error) {
                portTimer.rdBuff = rdbuff;
                portTimer.rdSize = 64;
                portTimer.readed = 0;
                res = caOS::Read(portTimer);
                if (res == deviceError::no_error) {
                    if (portTimer.readed > 0) {
                        Dump(rdbuff, portTimer.readed, ss);
                    } else {
                        ss << " No Data from " << opt.ptr << caEnd::endl;
                    }
                    caOS::Write(port, ss);
                }
                res = caOS::Close(portTimer);
            }
        }
#endif        
    } else {
        SyntaxError(port, iss);
    }
    return (deviceError) res;
}

deviceError caConsole::Command_WRITE_DEVICE(caDeviceHandle &port,
        caTokenizeSStream <u8> & iss) {
    u32 res = deviceError::no_error;
    caStringStream<s8> ss;
    ss.Init(buffio, sizeof (buffio));
    TokenString<u8> opt;
    iss>>opt;
    if (opt.size > 0) {
        if (caStrAux::StrNCmp((char *) opt.ptr, "com1", 4) == 0) {
            if (iss.Good()) {
                port.wrBuff = iss.Position();
                port.wrSize = iss.Remain();
                port.writed = 0;
                res = caOS::Write(port);
            } else {
                res = deviceError::error_read_less_data;
            }
        }
#if SYS_TIMER_DEVICE             
        else
            if (caStrAux::StrNCmp((char *) opt.ptr, "systimer", 8) == 0) {
            caIDeviceConfigure in;
            caDeviceHandle portTimer;
            res = caOS::Open("SYSTIMER", in, portTimer);
            if (res == deviceError::no_error) {
                u32 day, min, hour, sec;
                iss >> day;
                iss >> hour;
                iss >> min;
                iss >> sec;
                u32 data[4];
                data[0] = day;
                data[1] = hour;
                data[2] = min;
                data[3] = sec;
                portTimer.wrBuff = (u8*) data;
                portTimer.wrSize = sizeof (data);
                portTimer.writed = 0;
                res = caOS::Write(portTimer);
                res = caOS::Close(portTimer);
            }
        }
#endif        
    } else {
        SyntaxError(port, iss);
    }
    return (deviceError) res;
}

deviceError caConsole::Command_MEM(caDeviceHandle &port,
        caTokenizeSStream <u8> & iss) {
    u32 res = deviceError::no_error;
    caStringStream<s8> ss;
    ss.Init(buffio, sizeof (buffio));
    TokenString<u8> opt;
    iss>>opt;
    if (opt.size == 0 || caStrAux::StrNCmp((char *) opt.ptr, "list", 4) == 0) {
        //MemoryList(buffio, sizeof (buffio), &res);
        res = caMemory::List(buffio, sizeof (buffio));
        if (res) {
            ss.Forward(res);
            res = caOS::Write(port, ss);
        }
    } else
        if (caStrAux::StrNCmp((char *) opt.ptr, "dump", 4) == 0) {
        dumpAddrReq req;
        req.buffo = buffio;
        req.size = sizeof (buffio);
        if (iss.Good()) {
            iss >> req.addr;
            //MemoryDump(&req,&res);
            res = caMemory::Dump(&req);
            if (res) {
                ss.Forward(res);
                res = caOS::Write(port, ss);
            }
        } else
            res = SyntaxError(port, iss);
    } else
        if (caStrAux::StrNCmp((char *) opt.ptr, "ascii", 5) == 0) {
        dumpAddrReq req;
        req.buffo = buffio;
        req.size = sizeof (buffio);
        if (iss.Good()) {
            iss >> req.addr;
            //MemoryAsciiDump(&req, &res);
            res = caMemory::Ascii(&req);
            if (res) {
                ss.Forward(res);
                res = caOS::Write(port, ss);
            }
        } else
            res = SyntaxError(port, iss);
    } else
        res = SyntaxError(port, iss);
    return (deviceError) res;
}

deviceError caConsole::Command_INFO(caDeviceHandle &port,
        caTokenizeSStream <u8> & /*iss*/) {
    caStringStream<s8> ss;
    ss.Init(buffio, sizeof (buffio));
    ss << "About c.a.O.S :" << caEnd::endl;
    ss << "\t c. ->  Coppi" << caEnd::endl;
    ss << "\t a. ->  Angelo" << caEnd::endl;
    ss << "\t O. ->  Operating" << caEnd::endl;
    ss << "\t S. ->  System" << caEnd::endl;
    ss << "\t Contacts : coppi.angelo@virgilio.it, angelogkcop@hotmail.com" << caEnd::endl;
    return caOS::Write(port, ss);
}

deviceError caConsole::Command_HELP(caDeviceHandle &port,
        caTokenizeSStream <u8> & iss) {
    u32 u, i = 0;
    caStringStream<s8> ss;
    ss.Init(buffio, sizeof (buffio));
    TokenString<u8> opt;
    iss>>opt;
    if (opt.size > 0) {
        for (i = 0; i < MAX_CONSOLE_COMMAND; i++) {
            if (opt.size == commands[i].size && caStrAux::StrNCmp((char *) opt.ptr, commands[i].cmd, commands[i].size) == 0) {
                ss << commands[i].help;
                break;
            }
        }
        if (i == MAX_CONSOLE_COMMAND) {
            SyntaxError(port, iss);
        }
    } else {
        caStringFiller t(' ', 20);
        ss << "Available Commands :" << caEnd::endl;
        for (i = 0; i < MAX_CONSOLE_COMMAND;) {
            for (u = 0; u < 4 && i < MAX_CONSOLE_COMMAND; u++, i++) {
                const char *str = commands[i].cmd;
                ss << i;
                ss.Fix(t);
                ss << ") - " << str << t;
            }
            ss << caEnd::endl;
        }
    }
    return caOS::Write(port, ss);
}

deviceError caConsole::Command_QUIT(caDeviceHandle &port, caTokenizeSStream <u8> & /*iss*/) {
    caStringStream<s8> ss;
    ss.Init(buffio, sizeof (buffio));
    ss << " Shutdown system, and reload bootloader. bye bye" << caEnd::endl;
    caOS::Write(port, ss);
    hal_llc_reset_req.hll_reset();
    return deviceError::no_error;
}

deviceError caConsole::Execute(caTokenizeSStream <u8> & iss, caDeviceHandle &port) {
    u32 i;
    deviceError res = deviceError::no_error;
    caStringStream<s8> ss;
    ss.Init(buffio, sizeof (buffio));
    ss << "[" << hal_llc_time_1.hll_day() << ":" << hal_llc_time_1.hll_hour();
    ss << ":" << hal_llc_time_1.hll_min() << ":" << hal_llc_time_1.hll_sec();
    ss << ":" << hal_llc_time_1.hll_ms() << "] : c.a.O.S >" << caEnd::endl;
    res = caOS::Write(port, ss);
    TokenString<u8> tmp;
    iss>>tmp;
    if (iss.Good()) {
        for (i = 0; i < MAX_CONSOLE_COMMAND; i++) {
            if (tmp.size == commands[i].size && caStrAux::StrNCmp((char *) tmp.ptr, commands[i].cmd, commands[i].size) == 0) {
                res = commands[i].func(port, iss);
                break;
            }
        }
        if (res != deviceError::no_error) {
            Error(port, iss, res);
        }
        if (i == MAX_CONSOLE_COMMAND) {
            SyntaxError(port, iss);
        }
    }
    return res;
}

