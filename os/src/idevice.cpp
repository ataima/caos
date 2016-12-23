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

u32 caHalDeviceRules::isOpen(IDevice *device) {
    u32 res = 0;
    if (device)
        res = device->GetOpenFlag();
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
    if (dev != NULL && setup != NULL && port != NULL) {
        caMemAux::MemSet((u32*) port, 0, sizeof (caDeviceHandle) / sizeof (u32));
        res = dev->Open(setup, port);
        if (!IsValidHandle(port->handle, guid)) {
            res = deviceError::error_invalid_handle_port;
        }
    } else
        if (dev == NULL)
        res = deviceError::error_invalid_null_device;
    else
        if (setup == NULL)
        res = deviceError::error_device_config_param;
    else
        if (port == NULL)
        res = deviceError::error_invalid_null_port;
    return res;
}

u32 caHalDeviceRules::Close(IDevice *dev, caDeviceHandle *port, u32 guid) {
    u32 res = deviceError::no_error;
    if (dev != NULL && port != NULL) {
        if (isOpen(dev) == 0) {
            res = deviceError::error_device_not_opened;
        } else
            if (!IsValidHandle(port->handle, guid)) {
            res = deviceError::error_invalid_handle_port;
        } else {
            res = dev->Close(port);
        }
    } else
        if (dev == NULL)
        res = deviceError::error_invalid_null_device;
    else
        if (port == NULL)
        res = deviceError::error_invalid_null_port;
    return res;
}

u32 caHalDeviceRules::Write(IDevice *dev, caDeviceHandle *port, u32 guid) {
    u32 res = deviceError::no_error;
    if (dev != NULL && port != NULL) {
        if (isOpen(dev) == 0) {
            res = deviceError::error_device_not_opened;
        } else
            if (!IsValidHandle(port->handle, guid)) {
            res = deviceError::error_invalid_handle_port;
        } else {
            if (port->wrBuff != NULL) {
                // return no error... can perform write action...
                res = dev->Write(port);
            } else {
                res = deviceError::error_invalid_null_buffer_port;
            }
        }
    } else
        if (dev == NULL)
        res = deviceError::error_invalid_null_device;
    else
        if (port == NULL)
        res = deviceError::error_invalid_null_port;
    return res;
}

u32 caHalDeviceRules::Read(IDevice *dev, caDeviceHandle *port, u32 guid) {
    u32 res = deviceError::no_error;
    if (dev != NULL && port != NULL) {
        if (isOpen(dev) == 0) {
            res = deviceError::error_device_not_opened;
        } else
            if (!IsValidHandle(port->handle, guid)) {
            res = deviceError::error_invalid_handle_port;
        } else {
            if (port->rdBuff != NULL) {
                // return no error... can perfom read action
                res = dev->Read(port);
            } else {
                res = deviceError::error_invalid_null_buffer_port;
            }
        }
    } else
        if (dev == NULL)
        res = deviceError::error_invalid_null_device;
    else
        if (port == NULL)
        res = deviceError::error_invalid_null_port;
    return res;

}

u32 caHalDeviceRules::IoCtrl(IDevice *dev, caDeviceHandle *port,
        caIDeviceCtrl *inp, u32 guid) {
    u32 res = deviceError::no_error;
    if (dev != NULL && inp != NULL && port != NULL && guid != NULL) {
        if (isOpen(dev) == 0) {
            res = deviceError::error_device_not_opened;
        } else
            if (!IsValidHandle(port->handle, guid)) {
            res = deviceError::error_invalid_handle_port;
        } else {
            //ok perform ioctl command
            res = dev->IoCtrl(port, inp);
        }
    } else
        if (dev == NULL)
        res = deviceError::error_invalid_null_device;
    else
        if (port == NULL)
        res = deviceError::error_invalid_null_port;
    else
        if (inp == NULL)
        res = deviceError::error_device_config_param;
    return res;
}

u32 caHalDeviceRules::Flush(IDevice *dev, caDeviceHandle *port, u32 guid) {
    u32 res = deviceError::no_error;
    if (dev != NULL && port != NULL && guid != NULL) {
        if (!IsValidHandle(port->handle, guid)) {
            res = deviceError::error_invalid_handle_port;
        } else
            if (isOpen(dev) == 0) {
            res = deviceError::error_device_not_opened;
        } else {
            res = dev->Flush(port);
        }
    } else
        if (dev == NULL)
        res = deviceError::error_invalid_null_device;
    else
        if (port == NULL)
        res = deviceError::error_invalid_null_port;
    return res;
}