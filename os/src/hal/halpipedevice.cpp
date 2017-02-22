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
#include "memory.h"
#include "halpipedevice.h"
#include "memaux.h"

caHalPipeDevice::caHalPipeDevice(hal_llc_mem_io *iface, u32 mask) {
    isOpen = 0;
    link = iface;
    handle_guid = BASE_HANDLE;
    mask_guid = (mask & ioCtrlRequest::maskIoCtrl);
}

caPipeDeviceDescriptor * caHalPipeDevice::FindDescriptor(caPipeDeviceConfigure* setup) {
    caPipeDeviceDescriptor *desc = nullptr;
    u32 i;
    for (i = 0; i < MAX_PIPE_BLOCK; i++) {
        if (caStrAux::StrICmp(descriptors[i].name, setup->name) == 0) {
            desc = &descriptors[i];
            break;
        }
    }
    return desc;
}

caPipeDeviceDescriptor * caHalPipeDevice::GetDescriptor(u32 handle) {
    u32 index = (handle & (MAX_PIPE_MASK_ID_BLOCK)); //retrieve descriptors index
    return &descriptors[index];
}

u32 caHalPipeDevice::Open(caIDeviceConfigure * set_up, caDeviceHandle *port) {
    u32 res = deviceError::no_error;
    u32 i;
    LOG(caLog, device) << " in : isOpen = " << isOpen << caEnd::endl;
    caPipeDeviceConfigure *setup = static_cast<caPipeDeviceConfigure *> (set_up);
    if (setup != nullptr) {
        if (setup->name != nullptr) {
            if (caStrAux::StrLen(setup->name) == 0) {
                LOG(caLog, error) << " deviceError::error_pipe_invalid_name " << caEnd::endl;
                res = deviceError::error_pipe_invalid_name;
            } else {
                caPipeDeviceDescriptor * desc = FindDescriptor(setup);
                if (desc == nullptr && setup->host_guest == caPipeDeviceConfigure::host) {
                    //check if available empty descriptor
                    for (i = 0; i < MAX_PIPE_BLOCK; i++) {
                        if (descriptors[i].name[0] == '\0')
                            break;
                    }
                    if (i < MAX_PIPE_BLOCK) {
                        //create new
                        desc = &descriptors[i];
                        desc->index = i;
                        //copy name block
                        caStrAux::StrCpy(desc->name, setup->name);
                        desc->size = (setup->size / sizeof (u32)) + 1;
                        u32 *buff = (u32 *) caMemory::Allocate(desc->size * sizeof (u32));
                        if (buff == nullptr) {
                            LOG(caLog, error) << " deviceError::error_pipe_no_memory " << caEnd::endl;
                            res = deviceError::error_pipe_no_memory;
                            caMemAux<u32>::MemSet((u32*) desc, 0,
                                    sizeof (caPipeDeviceDescriptor));
                        } else {
                            desc->queue.Init(buff, desc->size);
                            desc->action = caPipeDeviceDescriptor::actionMem::opened;
                        }
                    } else {
                        LOG(caLog, error) << " deviceError::error_pipe_queue_full " << caEnd::endl;
                        res = deviceError::error_pipe_queue_full;
                    }
                } else
                    if (desc == nullptr && setup->host_guest == caPipeDeviceConfigure::guest) {
                    LOG(caLog, error) << " deviceError::error_pipe_not_exist " << caEnd::endl;
                    res = deviceError::error_pipe_not_exist;
                }
                if (desc != nullptr && res == deviceError::no_error) {
                    isOpen++;
                    port->handle = caHalDeviceRules::addHandle(desc->index, mask_guid);
                    port->status = caDeviceHandle::statusHandle::Open;
                    port->tStart = hal_llc_time_1.hll_tick();
                    port->tLast = port->tStart;
                    port->tStop = 0;
                    port->tLastCmd = caDeviceAction::caActionOpen;
                    if (setup->host_guest == caPipeDeviceConfigure::host) {
                        desc->host = port->handle;
                        LOG(caLog, device) << " Pipe " << setup->name << " Open as Host" << caEnd::endl;
                    } else {
                        desc->guest = port->handle;
                        LOG(caLog, device) << " Pipe " << setup->name << " Open as device" << caEnd::endl;
                    }
                }
            }

        } else {
            LOG(caLog, error) << " deviceError::error_pipe_invalid_name " << caEnd::endl;
            res = deviceError::error_pipe_invalid_name;
        }
    }
    //
    LOG(caLog, device) << " handle = " << port->handle << caEnd::endl;
    LOG(caLog, device) << " out : res = " << res << caEnd::endl;
    return res;
}

u32 caHalPipeDevice::Close(caDeviceHandle *port) {
    u32 res = deviceError::no_error;
    LOG(caLog, device) << " in : isOpen = " << isOpen << caEnd::endl;
    caPipeDeviceDescriptor * desc = GetDescriptor(port->handle);
    if (desc != nullptr) {
        if (desc->guest == port->handle) {
            desc->guest = 0;
        } else
            if (desc->host == port->handle) {
            caMemory::Free(desc->queue.GetBase(), nullptr);
            caMemAux<u32>::MemSet((u32*) desc, 0, sizeof (caPipeDeviceDescriptor));
        }
        isOpen--;
        port->status = caDeviceHandle::statusHandle::Close;
        port->tStop = hal_llc_time_1.hll_tick();
        port->tLast = port->tStop;
        port->tLastCmd = caDeviceAction::caActionClose;
        port->handle = mask_guid | BASE_HANDLE;
        port->wrError = port->rdError = 0;
        LOG(caLog, device) << "Close port : " << isOpen << caEnd::endl;
    } else {
        LOG(caLog, error) << " deviceError::error_invalid_handle_port " << caEnd::endl;
        res = deviceError::error_invalid_handle_port;
    }
    LOG(caLog, device) << " out : res = " << res << caEnd::endl;
    return res;
}

u32 caHalPipeDevice::Write(caDeviceHandle *port) {
    u32 res = deviceError::no_error;
    LOG(caLog, device) << " in : isOpen = " << isOpen << caEnd::endl;
    if (port->wrSize != 0) {
        caPipeDeviceDescriptor *desc = GetDescriptor(port->handle);
        if (desc != nullptr) {
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
                        port->tLast = hal_llc_time_1.hll_tick();
                        port->tLastCmd = caDeviceAction::caActionWrite;
                        port->wrError = port->rdError = 0;
                        desc->action = caPipeDeviceDescriptor::actionMem::writed;
                        LOG(caLog, device) << "Writed port : " << pSize << caEnd::endl;
                    }
                } else {
                    LOG(caLog, error) << " deviceError::error_pipe_queue_full" << caEnd::endl;
                    res = deviceError::error_pipe_queue_full;
                }
            } else {
                LOG(caLog, error) << " deviceError::error_pipe_port_not_host" << caEnd::endl;
                res = deviceError::error_pipe_port_not_host;
            }
        } else {
            LOG(caLog, error) << " deviceError::error_pipe_invalid_descriptor" << caEnd::endl;
            res = deviceError::error_pipe_invalid_descriptor;
        }
    } else {
        res = deviceError::error_invalid_handle_port_wr_size;
        LOG(caLog, error) << " deviceError::error_invalid_handle_port_wr_size:"
                << port->wrSize << caEnd::endl;
    }
    LOG(caLog, device) << " out : res = " << res << caEnd::endl;
    return res;
}

u32 caHalPipeDevice::Read(caDeviceHandle *port) {
    u32 res = deviceError::no_error;
    LOG(caLog, device) << " in : isOpen = " << isOpen << caEnd::endl;
    if (port->rdSize != 0) {
        u32 rSize, pSize;
        caPipeDeviceDescriptor *desc = GetDescriptor(port->handle);
        if (desc != nullptr) {
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
                    port->tLast = hal_llc_time_1.hll_tick();
                    port->tLastCmd = caDeviceAction::caActionRead;
                    port->wrError = port->rdError = 0;
                    desc->action = caPipeDeviceDescriptor::actionMem::readed;
                    LOG(caLog, device) << "Readed port : " << pSize << caEnd::endl;
                }
            } else {
                LOG(caLog, error) << " deviceError::error_pipe_port_not_guest" << caEnd::endl;
                res = deviceError::error_pipe_port_not_guest;
            }
        } else {
            LOG(caLog, error) << " deviceError::error_pipe_invalid_descriptor" << caEnd::endl;
            res = deviceError::error_pipe_invalid_descriptor;
        }
    } else {
        LOG(caLog, error) << " deviceError::error_invalid_handle_port_rd_size:"
                << port->rdSize << caEnd::endl;
        res = deviceError::error_invalid_handle_port_rd_size;
    }
    LOG(caLog, device) << " out : res = " << res << caEnd::endl;
    return res;
}

u32 caHalPipeDevice::Flush(caDeviceHandle *port) {
    u32 res = deviceError::no_error;
    port->tLast = link->hll_tick();
    port->wrError = port->rdError = 0;
    port->tLastCmd = caDeviceAction::caActionFlush;
    return res;
}

u32 caHalPipeDevice::Resize(caDeviceHandle *port, u32 size) {
    u32 res = deviceError::no_error;
    u32 old_size;
    caPipeDeviceDescriptor * desc = GetDescriptor(port->handle);
    if (desc != nullptr) {
        if (caMemory::Free(desc->queue.GetBase(), &old_size)) {
            desc->size = (size / sizeof (u32)) + 1;
            u32 *buff = (u32 *) caMemory::Allocate(desc->size * sizeof (u32));
            if (buff == nullptr) {
                res = deviceError::error_pipe_no_memory;
                caMemAux<u32>::MemSet((u32*) desc, 0, sizeof (caPipeDeviceDescriptor));
            } else {
                desc->queue.Init(buff, desc->size);
            }
        } else {
            res = deviceError::error_pipe_no_free_resize;
        }
    } else {
        res = deviceError::error_pipe_invalid_descriptor;
    }
    return res;
}

u32 caHalPipeDevice::Dump(caDeviceHandle *port, caStringStream<s8> *ss) {
    u32 res = deviceError::no_error;
    caPipeDeviceDescriptor * desc = GetDescriptor(port->handle);
    if (desc != nullptr) {
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
    return res;
}

u32 caHalPipeDevice::IoCtrl(caDeviceHandle *port,
        caIDeviceCtrl *inp) {
    u32 res = deviceError::no_error;
    port->tLast = link->hll_tick();
    LOG(caLog, device) << " in : isOpen = " << isOpen << caEnd::endl;
    caPipeDeviceCtrl *in = static_cast<caPipeDeviceCtrl *> (inp);
    switch (in->command) {
        default:
            LOG(caLog, error) << " deviceError::error_ioctrl_command_error"
                    << caEnd::endl;
            res = deviceError::error_ioctrl_command_error;
            break;

        case caPipeDeviceCtrl::IoPipeCtrlDirect::Resize:
            res = Resize(port, inp->params[0]);
            break;

        case caPipeDeviceCtrl::IoPipeCtrlDirect::Dump:
            res = Dump(port, inp->ss);
            break;

        case caPipeDeviceCtrl::IoPipeCtrlDirect::Reset:
        {
            caPipeDeviceDescriptor * desc = GetDescriptor(port->handle);
            if (desc != nullptr) {
                desc->queue.Clean();
            } else {
                res = deviceError::error_pipe_invalid_descriptor;
            }
        }
            break;
    }
    port->tLastCmd = caDeviceAction::caActionIoCtrl;
    LOG(caLog, device) << " out : res = " << res << caEnd::endl;
    return res;
}

u32 caHalPipeDevice::IrqService1(u8 *, s_t size, s_t &) {
    LOG(caLog, irq_1) << " IrqService1 S=" << size << caEnd::endl;

    return 1;
}

u32 caHalPipeDevice::IrqService2(u8 *, s_t size, s_t &) {
    LOG(caLog, irq_2) << " IrqService2 S=" << size << caEnd::endl;

    return 1;
}

u32 caHalPipeDevice::IrqService3(u8 *, s_t, s_t &) {
    LOG(caLog, irq_3) << " IrqService3 " << caEnd::endl;

    return 1;
}

u32 caHalPipeDevice::IrqService4(u8 *, s_t, s_t &) {
    LOG(caLog, irq_4) << " IrqService4 " << caEnd::endl;

    return 1;
}

u32 caHalPipeDevice::IrqService5(u8 *, s_t, s_t &) {
    LOG(caLog, irq_5) << " IrqService5 " << caEnd::endl;

    return 1;
}

u32 caHalPipeDevice::IrqService6(u8 *, s_t, s_t &) {
    LOG(caLog, irq_6) << " IrqService6 " << caEnd::endl;

    return 1;
}

u32 caHalPipeDevice::IrqService7(u8 *, s_t, s_t &) {
    LOG(caLog, irq_7) << " IrqService7 " << caEnd::endl;

    return 1;
}

u32 caHalPipeDevice::IrqService8(u8 *, s_t, s_t &) {
    LOG(caLog, irq_8) << " IrqService8 " << caEnd::endl;
    return 1;
}



