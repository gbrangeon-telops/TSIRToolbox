#include "UsartPort.h"

#include <PvDevice.h>
#include <PvDeviceFinderWnd.h>
#include <PvVersion.h>
#include <string>
#include <iostream>
#include <fstream>

#if (VERSION_MAJOR == 4)|| (VERSION_MAJOR == 5)
#include <PvDeviceGEV.h>
#include <PvDeviceInfoGEV.h>
#endif

using namespace std;

UsartPort::UsartPort():
#if (VERSION_MAJOR == 4)|| (VERSION_MAJOR == 5)
	m_device(nullptr),
	m_deviceAdapter(nullptr),
#endif
	m_verbose(false)
{

}

UsartPort::~UsartPort()
{
	// TODO Was not called for eBUS 3. Check if it's ok to call it anyway.
	Close();
}

bool UsartPort::Open(bool disableLoopback)
{
	if (!this->Connect())
	{
		return false;
	}

	// Get device parameters need to control streaming
	PvGenParameterArray *params;
#if (VERSION_MAJOR == 3)
	params = m_device.GetGenParameters();
#elif (VERSION_MAJOR == 4)|| (VERSION_MAJOR == 5)
	params = m_device->GetParameters();
#endif

	// Configure serial port - this is done directly on the device GenICam interface, not 
	// on the serial port object!
	params->SetEnumValue("BulkSelector", "Bulk0");
	params->SetEnumValue("BulkMode", "USRT");
	params->SetEnumValue("BulkSystemClockDivider", "By2");
	params->SetEnumValue("BulkNumOfStopBits", "One");
	params->SetEnumValue("BulkParity", "None");
	if (disableLoopback)
	{
		params->SetBooleanValue("BulkLoopback", false);
	}
	else
	{
		// Check IPEngine loopback state
		bool isLoopbackEnabled;
		params->GetBooleanValue("BulkLoopback", isLoopbackEnabled);
		if (isLoopbackEnabled)
		{
			PRINTF("Warning! IPEngine loopback is enabled.\n");
		}
	}

	// Open serial port
	PvResult result;
#if (VERSION_MAJOR == 3)
	result = m_port.Open(&m_device, PvIPEngineSerialBulk0);
#elif (VERSION_MAJOR == 4)|| (VERSION_MAJOR == 5)
	m_deviceAdapter = new PvDeviceAdapter(m_device);
	result = m_port.Open(m_deviceAdapter, PvDeviceSerialBulk0);
	if (!result.IsOK())
	{
		delete m_deviceAdapter;
		m_deviceAdapter = NULL;
	}
#endif

	if (!result.IsOK())
	{
		this->Disconnect();

		return false;
	}

	return true;
}

bool UsartPort::Connect()
{
	if (!this->IsConnected())
	{
		// Try to automatically connect to the GEV Device
		ifstream infile("GEVDeviceInfo.txt");
		if (infile.is_open())
		{
			string macAddressIn;
			getline(infile, macAddressIn);
			infile.close();

			this->ConnectDeviceGEV(macAddressIn.c_str());
		}
	}

	if (this->IsConnected())
	{
		return true;
	}
	else
	{
		// Select device
		const PvDeviceInfo *deviceInfo = nullptr;
		PvDeviceFinderWnd deviceFinderWnd;
		if(!deviceFinderWnd.ShowModal().IsOK())
		{
			return false;
		}

		deviceInfo = deviceFinderWnd.GetSelected();
		if (deviceInfo == nullptr)
		{
			return false;
		}

		// Connect to the selected Device
		if (!this->ConnectDevice(deviceInfo))
		{
			return false;
		}

		string macAddressOut;
#if (VERSION_MAJOR == 3)
		macAddressOut = string(deviceInfo->GetMACAddress().GetAscii());
#elif (VERSION_MAJOR == 4) || (VERSION_MAJOR == 5)
		if (m_device->GetType() == PvDeviceTypeGEV)
		{
			macAddressOut = string(static_cast<const PvDeviceInfoGEV *>(deviceInfo)->GetMACAddress().GetAscii());
#endif
			ofstream outfile("GEVDeviceInfo.txt");
			if (outfile.is_open())
			{
				outfile << macAddressOut << endl;
				outfile.close();
			}
#if (VERSION_MAJOR == 4) || (VERSION_MAJOR == 5)
		}
#endif

		return true;
	}
}

bool UsartPort::ConnectDeviceGEV(const PvString &macAddress)
{
	PvResult result;

#if (VERSION_MAJOR == 3)
	result = m_device.Connect(macAddress);
#elif (VERSION_MAJOR == 4)|| (VERSION_MAJOR == 5)
	PvDeviceGEV *gevDevice = new PvDeviceGEV;

	result = gevDevice->Connect(macAddress);
	if (result.IsOK())
	{
		m_device = static_cast<PvDevice *>(gevDevice);
	}
	else
	{
		delete gevDevice;
	}
#endif

	return result.IsOK();
}

bool UsartPort::ConnectDevice(const PvDeviceInfo *deviceInfo)
{
	PvResult result;

#if (VERSION_MAJOR == 3)
	result = m_device.Connect(deviceInfo);
#elif (VERSION_MAJOR == 4)|| (VERSION_MAJOR == 5)
	m_device = PvDevice::CreateAndConnect(deviceInfo, &result);
#endif

	return result.IsOK();
}

bool UsartPort::IsConnected()
{
#if (VERSION_MAJOR == 3)
	return m_device.IsConnected();
#elif (VERSION_MAJOR == 4)|| (VERSION_MAJOR == 5)
	return ((m_device != nullptr) && (m_device->IsConnected()));
#endif
}

bool UsartPort::IsOpened()
{
	return (IsConnected() && m_port.IsOpened());
}

bool UsartPort::Close()
{
	bool retval = true;

	// Close serial port
	if (m_port.IsOpened())
	{
		retval = m_port.Close().IsOK();
	}

	return (retval && this->Disconnect());
}

bool UsartPort::Disconnect()
{
	bool retval = true;

#if (VERSION_MAJOR == 3)
	if (m_device.IsConnected())
	{
		retval = m_device.Disconnect().IsOK();
	}
#elif (VERSION_MAJOR == 4)|| (VERSION_MAJOR == 5)
	if (m_deviceAdapter != nullptr)
	{
		delete m_deviceAdapter;
		m_deviceAdapter = nullptr;
	}

	if (m_device != nullptr)
	{
		PvDevice::Free(m_device);
		m_device = nullptr;
	}
#endif

	return retval;
}

int UsartPort::Write(const unsigned char *buffer, unsigned int size)
{
	unsigned int bytesWritten = 0;

#ifdef USARTPORT_SHOW_RX_TX_BYTES
	printf("usartPort::Write: [ TX > ");
	for (uint32_t i = 0; i < size; i++)
	{
		printf("%02X ", buffer[i]);
	}
	printf("]\n");
#endif

	PvResult lResult = m_port.Write(buffer, size, bytesWritten);
	if (!lResult.IsOK())
	{
		UP_ERR("Write: %s", lResult.GetCodeString().GetAscii());
		return 0;
	}

	return bytesWritten;
}

int UsartPort::Read(unsigned char *buffer, unsigned int size, unsigned int timeout)
{
	PvResult lResult;
	unsigned int bytesRead = 0;
	unsigned int totalBytesRead = 0;

	UP_INF("Read: Entering Read() function.");

	// Waiting for first response byte
	lResult = m_port.Read(buffer, size, bytesRead, timeout);
	UP_INF("Read: Result = %s, bytesRead = %d.", lResult.GetCodeString().GetAscii(), bytesRead);

	if (lResult.IsOK())
	{
		totalBytesRead += bytesRead;
		UP_INF("totalBytesRead = %d.", totalBytesRead);

		// Receive remaining response bytes
		while (totalBytesRead < size)
		{
			bytesRead = 0;
			lResult = m_port.Read(buffer + totalBytesRead, size - totalBytesRead, bytesRead, USART_EOP_TIMEOUT);
			UP_INF("Read: Result = %s, bytesRead = %d.", lResult.GetCodeString().GetAscii(), bytesRead);

			if (!lResult.IsOK())
			{
				break;
			}

			totalBytesRead += bytesRead;
			UP_INF("totalBytesRead = %d.", totalBytesRead);
		}
	}

	if (!lResult.IsOK())
	{
		if (lResult.GetCode() == PvResult::Code::TIMEOUT)
		{
			UP_INF("Read: Timeout after %d byte(s) have been received (Total byte(s) received = %d).",
				bytesRead, totalBytesRead);
		}
		else
		{
			UP_ERR("Read: %s", lResult.GetCodeString().GetAscii());
		}
	}

#ifdef USARTPORT_SHOW_RX_TX_BYTES
	PRINTF("usartPort::Read: [ RX < ");
	for (uint32_t i = 0; i < totalBytesRead; i++)
	{
		PRINTF("%02X ", buffer[i]);
	}
	PRINTF("]\n");
#endif

	UP_INF("Read: Exiting Read() function (totalBytesRead = %d).", totalBytesRead);

	return totalBytesRead;
}

void UsartPort::SetVerbose(bool verbose)
{
	m_verbose = verbose;
}

bool UsartPort::GetVerbose()
{
   return m_verbose;
}
