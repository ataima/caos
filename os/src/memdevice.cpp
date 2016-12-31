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
#include "caos_c_types.h"

#if MEM_PIPE_DEVICE    

#include "interrupt.h"
#include "systimer.h"
#include "memory.h"
#include "memaux.h"
#include "circularbuffer.h"

caMemDeviceDescriptor caMemDevice::descriptors[MAX_SHARED_MEM_BLOCK];
u32 caMemDevice::guid = ioCtrlRequest::MemPipe + BASE_HANDLE;
u32 caMemDevice::isOpen = 0;
u32 caMemDevice::n_desc = 0;

bool caMemDevice::IsValidHandle(u32 handle) {
    bool res = false;
    if ((handle & ioCtrlRequest::maskIoCtrl) == ioCtrlRequest::MemPipe &&
            (handle & ioCtrlRequest::maskHandle) > BASE_HANDLE)
        res = true;
    return res;
}

caMemDeviceDescriptor * caMemDevice::FindDescriptor(caMemDeviceConfigure* setup) {
    caMemDeviceDescriptor *desc = NULL;
    u32 i;
    for (i = 0; i < MAX_SHARED_MEM_BLOCK; i++) {
        if (caStrAux::StrICmp(descriptors[i].name, setup->name) == 0) {
            desc = &descriptors[i];
            break;
        }
    }
    return desc;
}

caMemDeviceDescriptor * caMemDevice::GetDescriptor(u32 handle) {
    u32 index = (handle & (MAX_SHARED_MASK_ID)); //retrieve descriptors index
    return &descriptors[index];
}
////////////////////////////  DEVICE CTRL 

u32 caMemDevice::Open(caIDeviceConfigure * in,
        caDeviceHandle *port) {
    u32 i;
    u32 res = deviceError::no_error;
    //TIN();
    if (in != NULL && port != NULL) {
        caMemDeviceConfigure *setup = static_cast<caMemDeviceConfigure *> (in);
        // already exist or is a new mem block
        if (caStrAux::StrLen(setup->name) == 0) {
            res = deviceError::error_pipe_invalid_name;
        } else {
            caMemDeviceDescriptor * desc = FindDescriptor(setup);
            if (desc == NULL && setup->host_guest == caMemDeviceConfigure::host) {
                //check if available empty descriptor
                for (i = 0; i < n_desc; i++) {
                    if (descriptors[i].name[0] == '\0')
                        break;
                }
                //create new
                desc = &descriptors[i];
                desc->index = i;
                //copy name block
                caStrAux::StrCpy(desc->name, setup->name);
                desc->size = (setup->size / sizeof (u32)) + 1;
                u32 *buff = (u32 *) caMemory::Allocate(desc->size * sizeof (u32));
                if (buff == NULL) {
                    res = deviceError::error_pipe_no_memory;
                    caMemAux<u32>::MemSet((u32*) desc, 0,
                            sizeof (caMemDeviceDescriptor));
                } else {
                    desc->queue.Init(buff, desc->size);
                    desc->action = caMemDeviceDescriptor::actionMem::opened;
                    if (i == n_desc)
                        n_desc++;
                }
            } else
                if (desc == NULL && setup->host_guest == caMemDeviceConfigure::guest) {
                res = deviceError::error_pipe_not_exist;
            }
            if (desc != NULL) {
                guid += MAX_SHARED_MEM_BLOCK;
                port->handle = guid + desc->index;
                port->status = caDeviceHandle::statusHandle::Open;
                port->tStart = caSysTimer::GetCount();
                port->tLast = port->tStart;
                port->tStop = 0;
                port->tLastCmd = caDeviceAction::caActionOpen;
                if (setup->host_guest == caMemDeviceConfigure::host)
                    desc->host = port->handle;
                else
                    desc->guest = port->handle;
                isOpen++;
            }
        }
    } else
        if (in == NULL)
        res = deviceError::error_device_config_param;
    else
        if (port == NULL)
        res = deviceError::error_invalid_null_port;
    //TOUT();
    return res;
}

u32 caMemDevice::Close(caDeviceHandle *port) {
    u32 res = deviceError::no_error;
    u32 size = 0;
    //TIN();
    if (port == NULL) {
        res = deviceError::error_invalid_null_port;
    } else
        if (isOpen == 0) {
        res = deviceError::error_device_not_opened;
    } else
        if (!IsValidHandle(port->handle)) {
        res = deviceError::error_invalid_handle_port;
    } else {
        //OK 
        caMemDeviceDescriptor * desc = GetDescriptor(port->handle);
        if (desc != NULL) {
            if (desc->guest == port->handle) {
                desc->guest = 0;
                isOpen--;
                port->status = caDeviceHandle::statusHandle::Close;
                port->tStop = caSysTimer::GetCount();
                port->tLast = port->tStop;
                port->tLastCmd = caDeviceAction::caActionClose;
                port->wrError = port->rdError = 0;
            } else
                if (desc->host == port->handle) {
                caMemory::Free(desc->queue.GetBase(), &size);
                caMemAux<u32>::MemSet((u32*) desc, 0, sizeof (caMemDeviceDescriptor));
                isOpen--;
                port->status = caDeviceHandle::statusHandle::Close;
                port->tStop = caSysTimer::GetCount();
                port->tLast = port->tStop;
                port->tLastCmd = caDeviceAction::caActionClose;
                port->wrError = port->rdError = 0;
            } else {
                res = deviceError::error_invalid_handle_port;
            }
        } else {
            res = deviceError::error_pipe_invalid_descriptor;
        }
    }
    //TOUT();
    return res;
}

u32 caMemDevice::Write(caDeviceHandle *port) {
    u32 res = deviceError::no_error;
    //TIN();
    if (port == NULL) {
        res = deviceError::error_invalid_null_port;
    } else
        if (isOpen == 0) {
        res = deviceError::error_device_not_opened;
    } else
        if (!IsValidHandle(port->handle)) {
        res = deviceError::error_invalid_handle_port;
    } else {
        if (port->wrBuff != NULL && port->wrSize > 0) {
            caMemDeviceDescriptor *desc = GetDescriptor(port->handle);
            if (desc != NULL) {
                if (desc->host == port->handle) {
                    u32 wSize;
                    u32 pSize = (port->wrSize / sizeof (u32)) + 1;
                    u32 maxWr = desc->queue.Available();
                    if (maxWr > 0) {
                        if (maxWr > pSize)
                            wSize = pSize;
                        else
                            wSize = maxWr;
                        if (wSize > 0) {
                            if (!desc->queue.Push((u32*) port->wrBuff, wSize, pSize))
                                res = deviceError::error_write_less_data;
                            pSize *= sizeof (u32);
                            port->writed += pSize;
                            port->wrBuff += pSize;
                            port->wrSize -= pSize;
                            port->tLast = caSysTimer::GetCount();
                            port->tLastCmd = caDeviceAction::caActionWrite;
                            //port->error = 0;
                            desc->action = caMemDeviceDescriptor::actionMem::writed;
                        }
                    } else {
                        res = deviceError::error_pipe_queue_full;
                    }
                } else {
                    res = deviceError::error_pipe_port_not_host;
                }
            } else {
                res = deviceError::error_pipe_invalid_descriptor;
            }

        } else {
            res = deviceError::error_invalid_handle_port;
        }
    }
    //TOUT();
    return res;
}

u32 caMemDevice::Read(caDeviceHandle *port) {
    u32 res = deviceError::no_error;
    //TIN();
    if (port == NULL) {
        res = deviceError::error_invalid_null_port;
    } else
        if (isOpen == 0) {
        res = deviceError::error_device_not_opened;
    } else
        if (!IsValidHandle(port->handle)) {
        res = deviceError::error_invalid_handle_port;
    } else {
        if (port->rdBuff != NULL && port->rdSize > 0) {
            u32 rSize, pSize;
            caMemDeviceDescriptor *desc = GetDescriptor(port->handle);
            if (desc != NULL) {
                if (desc->guest == port->handle) {
                    pSize = (port->rdSize / sizeof (u32)) + 1;
                    if (desc->queue.Size() < pSize)
                        rSize = desc->queue.Size();
                    else
                        rSize = pSize;
                    if (rSize > 0) {
                        if (!desc->queue.Pop((u32*) port->rdBuff, rSize, pSize))
                            res = deviceError::error_read_less_data;
                        pSize *= sizeof (u32);
                        port->readed += pSize;
                        port->rdBuff += pSize;
                        port->rdSize -= pSize;
                        port->tLast = caSysTimer::GetCount();
                        port->tLastCmd = caDeviceAction::caActionRead;
                        //port->error = 0;
                        desc->action = caMemDeviceDescriptor::actionMem::readed;
                    }
                } else {
                    res = deviceError::error_pipe_port_not_guest;
                }
            } else {
                res = deviceError::error_pipe_invalid_descriptor;
            }
        } else {
            res = deviceError::error_invalid_handle_port;
        }
    }
    //TOUT();
    return res;
}

u32 caMemDevice::Resize(caDeviceHandle *port, u32 size) {
    u32 res = deviceError::no_error;
    //TIN();
    if (port == NULL) {
        res = deviceError::error_invalid_null_port;
    } else
        if (isOpen == 0) {
        res = deviceError::error_device_not_opened;
    } else
        if (!IsValidHandle(port->handle)) {
        res = deviceError::error_invalid_handle_port;
    } else {
        u32 old_size;
        caMemDeviceDescriptor * desc = GetDescriptor(port->handle);
        if (desc != NULL) {

            if (caMemory::Free(desc->queue.GetBase(), &old_size)) {
                desc->size = (size / sizeof (u32)) + 1;
                u32 *buff = (u32 *) caMemory::Allocate(desc->size * sizeof (u32));
                if (buff == NULL) {
                    res = deviceError::error_pipe_no_memory;
                    caMemAux<u32>::MemSet((u32*) desc, 0, sizeof (caMemDeviceDescriptor));
                } else {
                    desc->queue.Init(buff, desc->size);
                }
            } else {
                res = deviceError::error_pipe_no_free_resize;
            }
        } else {
            res = deviceError::error_pipe_invalid_descriptor;
        }
    }
    return res;
}

u32 caMemDevice::Reset(caDeviceHandle *port) {
    u32 res = deviceError::no_error;
    //TIN();
    if (port == NULL) {
        res = deviceError::error_invalid_null_port;
    } else
        if (isOpen == 0) {
        res = deviceError::error_device_not_opened;
    } else
        if (!IsValidHandle(port->handle)) {
        res = deviceError::error_invalid_handle_port;
    } else {
        caMemDeviceDescriptor * desc = GetDescriptor(port->handle);
        if (desc != NULL) {
            desc->queue.Clean();
        } else {
            res = deviceError::error_pipe_invalid_descriptor;
        }
    }
    return res;
}

u32 caMemDevice::Dump(caDeviceHandle *port, caStringStream<s8> *ss) {
    u32 res = deviceError::no_error;
    //TIN();
    if (port == NULL) {
        res = deviceError::error_invalid_null_port;
    } else
        if (isOpen == 0) {
        res = deviceError::error_device_not_opened;
    } else
        if (!IsValidHandle(port->handle)) {
        res = deviceError::error_invalid_handle_port;
    } else {
        caMemDeviceDescriptor * desc = GetDescriptor(port->handle);
        if (desc != NULL) {
            ss->Clear();
            *ss << "Name     = " << (const char *) (desc->name) << caEnd::endl;
            *ss << "Size     = " << caStringFormat::dec << desc->size << caEnd::endl;
            *ss << "Index    = " << caStringFormat::dec << (u32) desc->index << caEnd::endl;
            *ss << "Host     = " << caStringFormat::hex << (u32) desc->host << caEnd::endl;
            *ss << "Guest    = " << caStringFormat::hex << (u32) desc->guest << caEnd::endl;
            *ss << "Queue S  = " << caStringFormat::hex << (u32) desc->queue.Size() << caEnd::endl;
            *ss << "Queue A  = " << caStringFormat::hex << (u32) desc->queue.Available();
        } else {
            res = deviceError::error_pipe_invalid_descriptor;
        }
    }
    return res;
}

u32 caMemDevice::IoCtrl(caDeviceHandle *port, caIDeviceCtrl *inp) {
    u32 res = deviceError::no_error;
    //TIN();
    if (port == NULL) {
        res = deviceError::error_invalid_null_port;
    } else
        if (inp == NULL) {
        res = deviceError::error_device_config_param;
    } else
        if (isOpen == 0) {
        res = deviceError::error_device_not_opened;
    } else
        if (!IsValidHandle(port->handle)) {
        res = deviceError::error_invalid_handle_port;
    } else {
        caMemDeviceCtrl *in = static_cast<caMemDeviceCtrl *> (inp);
        switch (in->command) {
            case caMemDeviceCtrl::IoCtrlDirect::Resize:
            {
                caMemResizeDeviceCtrl *ns = static_cast<caMemResizeDeviceCtrl *> (in);
                if (ns != NULL)
                    res = Resize(port, ns->new_size);
            }
                break;

            case caMemDeviceCtrl::IoCtrlDirect::Dump:
            {
                caMemDumpDeviceCtrl *dump = static_cast<caMemDumpDeviceCtrl *> (in);
                if (dump != NULL)
                    res = Dump(port, dump->ss);
            }
                break;
            case caMemDeviceCtrl::IoCtrlDirect::Reset:
                res = Reset(port);
                break;
        }
    }
    port->tLastCmd = caDeviceAction::caActionIoCtrl;
    //TOUT();
    return res;
}

u32 caMemDevice::IoctlReq(ioCtrlFunction request, u32 *p1, u32 *p2) {
    u32 res = deviceError::error_unknow_device_ioctrl;
    //TIN();
    switch (request) {
        case ioCtrlFunction::caOpenDevice:
            res = Open((caMemDeviceConfigure *) p1, (caDeviceHandle *) p2);
            break;
        case ioCtrlFunction::caCloseDevice:
            res = Close((caDeviceHandle *) p1);
            break;
        case ioCtrlFunction::caWriteDevice:
            res = Write((caDeviceHandle *) p1);
            break;
        case ioCtrlFunction::caReadDevice:
            res = Read((caDeviceHandle *) p1);
            break;
        case ioCtrlFunction::caIoCtrlDevice:
            res = IoCtrl((caDeviceHandle *) p1, (caMemDeviceCtrl *) p2);
            break;
        default:
            break;
    }
    //TOUT();
    return res;
}
#endif
