#ifndef CONSOLE_H
#define CONSOLE_H



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

