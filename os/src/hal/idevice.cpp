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

#include "memaux.h"
#include "syslog.h"

IDevice::IDevice(void) {
    caMemAux<u8>::MemZero(reinterpret_cast<u8*> (this), sizeof (IDevice));
}

u32 caHalDeviceRules::isOpen(IDevice *device) {
    u32 res = 0;
    if (device)
        res = device->IGetOpenFlag(device);
    return res;
}

bool caHalDeviceRules::IsValidHandle(u32 handle, u32 mask) {
    bool res = false;
    if ((handle & ioCtrlRequest::maskIoCtrl) == mask &&
            (handle & ioCtrlRequest::maskHandle) > BASE_HANDLE)
        res = true;
    return res;
}

u32 caHalDeviceRules::Open(IDevice *dev, caIDeviceConfigure * setup,
        caDeviceHandle *port, u32 guid) {
    u32 res = deviceError::no_error;
    if (dev != nullptr && setup != nullptr && port != nullptr) {
        if (isOpen(dev) != 0) {
            res = deviceError::error_device_already_opened;
        } else {
            caMemAux<u32>::MemSet((u32 *) port, 0, sizeof (caDeviceHandle));
            res = dev->IOpen(dev, setup, port);
            if (res != deviceError::no_error) {
                caMemAux<u32>::MemSet((u32 *) port, 0, sizeof (caDeviceHandle));
            } else
                if (!IsValidHandle(port->handle, guid)) {
                res = deviceError::error_invalid_handle_port;
            }
        }
    } else
        if (dev == nullptr)
        res = deviceError::error_invalid_null_device;
    else
        if (setup == nullptr)
        res = deviceError::error_device_config_param;
    else
        if (port == nullptr)
        res = deviceError::error_invalid_null_port;
    return res;
}

u32 caHalDeviceRules::Close(IDevice *dev, caDeviceHandle *port, u32 guid) {
    u32 res = deviceError::no_error;
    if (dev != nullptr && port != nullptr) {
        if (isOpen(dev) == 0) {
            res = deviceError::error_device_not_opened;
        } else
            if (!IsValidHandle(port->handle, guid)) {
            res = deviceError::error_invalid_handle_port;
        } else {
            res = dev->IClose(dev, port);
            port->wrBuff = port->rdBuff = nullptr;
            port->wrSize = port->writed = port->wrError =
                    port->rdSize = port->readed = port->rdError = 0;
        }
    } else
        if (dev == nullptr)
        res = deviceError::error_invalid_null_device;
    else
        if (port == nullptr)
        res = deviceError::error_invalid_null_port;
    return res;
}

u32 caHalDeviceRules::Write(IDevice *dev, caDeviceHandle *port, u32 guid) {
    u32 res = deviceError::no_error;
    if (dev != nullptr && port != nullptr) {
        if (isOpen(dev) == 0) {
            res = deviceError::error_device_not_opened;
        } else
            if (!IsValidHandle(port->handle, guid)) {
            res = deviceError::error_invalid_handle_port;
        } else {
            if (port->wrBuff != nullptr) {
                // return no error... wrSize == 0 flush ? TODO ,can perform 
                //write action...
                res = dev->IWrite(dev, port);
            } else {
                res = deviceError::error_invalid_null_buffer_port;
            }
        }
    } else
        if (dev == nullptr)
        res = deviceError::error_invalid_null_device;
    else
        if (port == nullptr)
        res = deviceError::error_invalid_null_port;
    return res;
}

u32 caHalDeviceRules::Read(IDevice *dev, caDeviceHandle *port, u32 guid) {
    u32 res = deviceError::no_error;
    if (dev != nullptr && port != nullptr) {
        if (isOpen(dev) == 0) {
            res = deviceError::error_device_not_opened;
        } else
            if (!IsValidHandle(port->handle, guid)) {
            res = deviceError::error_invalid_handle_port;
        } else {
            if (port->rdBuff != nullptr) {
                // return no error... can perfom read action
                res = dev->IRead(dev, port);
            } else {
                res = deviceError::error_invalid_null_buffer_port;
            }
        }
    } else
        if (dev == nullptr)
        res = deviceError::error_invalid_null_device;
    else
        if (port == nullptr)
        res = deviceError::error_invalid_null_port;
    return res;

}

u32 caHalDeviceRules::IoCtrl(IDevice *dev, caDeviceHandle *port,
        caIDeviceCtrl *inp, u32 guid) {
    u32 res = deviceError::no_error;
    if (dev != nullptr && inp != nullptr && port != nullptr && guid != 0) {
        //ok perform ioctl command
        switch (inp->command) {
            case caIDeviceCtrl::IoCtrlDirect::ctrl_LogCreate:
            {
                caSysLog *caLog = dev->IGetDeviceLog(dev);
                if (caLog != nullptr) {
                    if (!caLog->IsValid()) {
                        if (caLog->Init(inp->params[0],
                                (deviceloglevels) inp->params[1]) != true)
                            res = deviceError::error_cannot_create_log;
                    } else {
                        res = deviceError::error_log_already_set;
                    }
                } else {
                    res = deviceError::error_log_null;
                }
            }
                break;
            case caIDeviceCtrl::IoCtrlDirect::ctrl_LogDestroy:
            {
                caSysLog *caLog = dev->IGetDeviceLog(dev);
                if (caLog != nullptr) {
                    if (caLog->IsValid()) {
                        if (caLog->Destroy() != true)
                            res = deviceError::error_cannot_destroy_log;
                    } else {
                        res = deviceError::error_log_not_set;
                    }
                } else {
                    res = deviceError::error_log_null;
                }
            }
                break;
            case caIDeviceCtrl::IoCtrlDirect::ctrl_LogStart:
            {
                caSysLog *caLog = dev->IGetDeviceLog(dev);
                if (caLog != nullptr) {
                    if (!caLog->IsEnabled()) {
                        caLog->Enable();
                        PLOG(caLog, device) << dev->ItoString() << " START "
                                << caEnd::endl;
                    } else {
                        res = deviceError::error_log_already_set;
                    }
                } else {
                    res = deviceError::error_log_null;
                }
            }
                break;
            case caIDeviceCtrl::IoCtrlDirect::ctrl_LogStop:
            {
                caSysLog *caLog = dev->IGetDeviceLog(dev);
                if (caLog != nullptr) {
                    if (caLog->IsEnabled()) {
                        PLOG(caLog, device) << dev->ItoString() << " STOP "
                                << caEnd::endl;
                        caLog->Disable();
                    } else {
                        res = deviceError::error_log_not_set;
                    }
                } else {
                    res = deviceError::error_log_null;
                }
            }
                break;
            case caIDeviceCtrl::IoCtrlDirect::ctrl_LogGet:
            {
                caSysLog *caLog = dev->IGetDeviceLog(dev);
                if (caLog != nullptr) {
                    if (caLog->IsValid()) {
                        if (inp->ss != nullptr) {
                            caLog->Stream((deviceloglevels) inp->params[0]).
                                    Str(*inp->ss);
                        } else
                            res = deviceError::error_invalid_null_destination;
                    } else {
                        res = deviceError::error_log_not_set;
                    }
                } else {
                    res = deviceError::error_log_null;
                }
            }
                break;
            default:
                if (isOpen(dev) == 0) {
                    res = deviceError::error_device_not_opened;
                } else
                    if (!IsValidHandle(port->handle, guid)) {
                    res = deviceError::error_invalid_handle_port;
                } else {
                    res = dev->IIoCtrl(dev, port, inp);

                }
                break;
        }
    } else
        if (dev == nullptr)
        res = deviceError::error_invalid_null_device;
    else
        if (port == nullptr)
        res = deviceError::error_invalid_null_port;
    else
        if (inp == nullptr)
        res = deviceError::error_device_config_param;
    return res;
}

u32 caHalDeviceRules::Flush(IDevice *dev, caDeviceHandle *port, u32 guid) {
    u32 res = deviceError::no_error;
    if (dev != nullptr && port != nullptr && guid != 0) {
        if (!IsValidHandle(port->handle, guid)) {
            res = deviceError::error_invalid_handle_port;
        } else
            if (isOpen(dev) == 0) {
            res = deviceError::error_device_not_opened;
        } else {
            res = dev->IFlush(dev, port);
        }
    } else
        if (dev == nullptr)
        res = deviceError::error_invalid_null_device;
    else
        if (port == nullptr)
        res = deviceError::error_invalid_null_port;
    return res;
}

u32 caHalDeviceRules::IrqService1(void *obj, u8 * buff, s_t size, s_t & iosize) {
    u32 res = deviceError::error_invalid_null_device;
    if (obj != nullptr) {
        IDevice *dev = reinterpret_cast<IDevice *> (obj);
        if (dev != nullptr) {
            res = dev->IIrqService1(dev, buff, size, iosize);
        }
    } else {
        res = deviceError::error_invalid_null_device;
    }
    return res;
}

u32 caHalDeviceRules::IrqService2(void *obj, u8 * buff, s_t size, s_t & iosize) {
    u32 res = deviceError::error_invalid_null_device;
    if (obj != nullptr) {
        IDevice *dev = reinterpret_cast<IDevice *> (obj);
        if (dev != nullptr) {
            res = dev->IIrqService2(dev, buff, size, iosize);
        }
    } else {
        res = deviceError::error_invalid_null_device;
    }
    return res;
}

u32 caHalDeviceRules::IrqService3(void *obj, u8 * buff, s_t size, s_t & iosize) {
    u32 res = deviceError::error_invalid_null_device;
    if (obj != nullptr) {
        IDevice *dev = reinterpret_cast<IDevice *> (obj);
        if (dev != nullptr) {
            res = dev->IIrqService3(dev, buff, size, iosize);
        }
    } else {
        res = deviceError::error_invalid_null_device;
    }
    return res;
}

u32 caHalDeviceRules::IrqService4(void *obj, u8 * buff, s_t size, s_t & iosize) {
    u32 res = deviceError::error_invalid_null_device;
    if (obj != nullptr) {
        IDevice *dev = reinterpret_cast<IDevice *> (obj);
        if (dev != nullptr) {
            res = dev->IIrqService4(dev, buff, size, iosize);
        }
    } else {
        res = deviceError::error_invalid_null_device;
    }
    return res;
}

u32 caHalDeviceRules::IrqService5(void *obj, u8 * buff, s_t size, s_t & iosize) {
    u32 res = deviceError::error_invalid_null_device;
    if (obj != nullptr) {
        IDevice *dev = reinterpret_cast<IDevice *> (obj);
        if (dev != nullptr) {
            res = dev->IIrqService5(dev, buff, size, iosize);
        }
    } else {
        res = deviceError::error_invalid_null_device;
    }
    return res;
}

u32 caHalDeviceRules::IrqService6(void *obj, u8 * buff, s_t size, s_t & iosize) {
    u32 res = deviceError::error_invalid_null_device;
    if (obj != nullptr) {
        IDevice *dev = reinterpret_cast<IDevice *> (obj);
        if (dev != nullptr) {
            res = dev->IIrqService6(dev, buff, size, iosize);
        }
    } else {
        res = deviceError::error_invalid_null_device;
    }
    return res;
}

u32 caHalDeviceRules::IrqService7(void *obj, u8 * buff, s_t size, s_t & iosize) {
    u32 res = deviceError::error_invalid_null_device;
    if (obj != nullptr) {
        IDevice *dev = reinterpret_cast<IDevice *> (obj);
        if (dev != nullptr) {
            res = dev->IIrqService7(dev, buff, size, iosize);
        }
    } else {
        res = deviceError::error_invalid_null_device;
    }
    return res;
}

u32 caHalDeviceRules::IrqService8(void *obj, u8 * buff, s_t size, s_t & iosize) {
    u32 res = deviceError::error_invalid_null_device;
    if (obj != nullptr) {
        IDevice *dev = reinterpret_cast<IDevice *> (obj);
        if (dev != nullptr) {
            res = dev->IIrqService8(dev, buff, size, iosize);
        }
    } else {
        res = deviceError::error_invalid_null_device;
    }
    return res;
}

