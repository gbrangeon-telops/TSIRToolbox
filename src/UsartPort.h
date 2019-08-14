#ifndef __USARTPORT_H__
#define __USARTPORT_H__

#include "Utils.h"
#include <PvDevice.h>
#include <PvVersion.h>

#if (VERSION_MAJOR == 3)
#include <PvSerialPortIPEngine.h>
#elif (VERSION_MAJOR == 4)|| (VERSION_MAJOR == 5)
#include <PvDeviceAdapter.h>
#include <PvDeviceSerialPort.h>
#endif

// #define USARTPORT_SHOW_RX_TX_BYTES

#define UP_PRINTF(fmt, ...)    if (m_verbose) PRINTF("UP: " fmt, ##__VA_ARGS__)
#define UP_ERR(fmt, ...)       UP_PRINTF("Error: " fmt "\n", ##__VA_ARGS__)
#define UP_INF(fmt, ...)       UP_PRINTF("Info: " fmt "\n", ##__VA_ARGS__)

#define USART_MAX_BUFFER_SIZE			256
#define USART_EOP_TIMEOUT				100

class UsartPort
{
	friend class UsartPortF1F2;

public:
	UsartPort();
	~UsartPort();

	bool Open(bool disableLoopback = true);
	bool IsOpened();
	bool Close();

	int Write(const unsigned char *buffer, unsigned int size);
	int Read(unsigned char *buffer, unsigned int size, unsigned int timeout = 0);

	void SetVerbose(bool verbose);
	bool GetVerbose();
	PvGenParameterArray *GetGenParams();

private:
	bool Connect();
	bool ConnectDeviceGEV(const PvString &macAddress);
	bool ConnectDevice(const PvDeviceInfo *lDeviceInfo);
	bool IsConnected();
	bool Disconnect();

#if (VERSION_MAJOR == 3)
	PvDevice m_device;
	PvSerialPortIPEngine m_port;
#elif (VERSION_MAJOR == 4)|| (VERSION_MAJOR == 5)
	PvDevice *m_device;
	PvDeviceAdapter *m_deviceAdapter;
	PvDeviceSerialPort m_port;
#endif

	bool m_verbose;
};

#endif // __USARTPORT_H__