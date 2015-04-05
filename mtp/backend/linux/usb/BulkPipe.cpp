/*
 * Android File Transfer for Linux: MTP client for android devices
 * Copyright (C) 2015  Vladimir Menshakov

 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
#include <usb/BulkPipe.h>
#include <usb/DeviceDescriptor.h>

namespace mtp { namespace usb
{
	BulkPipe::BulkPipe(DevicePtr device, ConfigurationPtr conf, InterfacePtr interface, EndpointPtr in, EndpointPtr out, EndpointPtr interrupt):
		_device(device), _conf(conf), _interface(interface), _in(in), _out(out), _interrupt(interrupt)
	{
		int currentConfigurationIndex = _device->GetConfiguration();
		if (conf->GetIndex() != currentConfigurationIndex)
			_device->SetConfiguration(conf->GetIndex());

		_claimToken = _device->ClaimInterface(interface->GetIndex());
		//USB_CALL(libusb_claim_interface(_device->GetHandle(), interface->GetIndex()));
		//libusb_clear_halt(device->GetHandle(), in->GetAddress());
		//libusb_clear_halt(device->GetHandle(), out->GetAddress());
		//libusb_clear_halt(device->GetHandle(), interrupt->GetAddress());
	}

	BulkPipe::~BulkPipe()
	{
		//libusb_release_interface(_device->GetHandle(), _interface->GetIndex());
	}

	ByteArray BulkPipe::ReadInterrupt()
	{
		ByteArray data;//_interrupt->GetMaxPacketSize());
		return data;
	}

	ByteArray BulkPipe::Read(int timeout)
	{
		return _device->ReadBulk(_in, timeout);
	}

	void BulkPipe::Write(const ByteArray &data, int timeout)
	{
		_device->WriteBulk(_out, data, timeout);
	}

	BulkPipePtr BulkPipe::Create(usb::DevicePtr device, ConfigurationPtr conf, usb::InterfacePtr interface)
	{
		int epn = interface->GetEndpointsCount();

		usb::EndpointPtr out, in, interrupt;
		//fprintf(stderr, "endpoints: %d\n", epn);
		for(int i = 0; i < epn; ++i)
		{
			usb::EndpointPtr ep = interface->GetEndpoint(i);
			//fprintf(stderr, "endpoint: %d: %02x\n", i, ep->GetAddress());
			//check for bulk here
			if (ep->GetDirection() == usb::EndpointDirection::Out)
			{
				if (ep->GetType() == usb::EndpointType::Bulk)
				{
					//fprintf(stderr, "OUT\n");
					out = ep;
				}
			}
			else
			{
				if (ep->GetType() == usb::EndpointType::Bulk)
				{
					//fprintf(stderr, "IN\n");
					in = ep;
				}
				else
				{
					//fprintf(stderr, "INTERRUPT\n");
					interrupt = ep;
				}
			}
		}
		if (!in || !out || !interrupt)
			throw std::runtime_error("invalid endpoint");

		return std::make_shared<BulkPipe>(device, conf, interface, in, out, interrupt);
	}

}}