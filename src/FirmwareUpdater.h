#ifndef __FIRMWAREUPDATER_H__
#define __FIRMWAREUPDATER_H__

#include "UsartPortF1F2.h"
#include "IRC_status.h"
#include "Utils.h"

#define FU_LOG(fmt, ...)      {FILE *fidLog; fopen_s(&fidLog, "tsirfu.log", "a"); fprintf(fidLog, fmt, ##__VA_ARGS__); fclose(fidLog);}
#define FU_PRINTF(fmt, ...)   PRINTF(fmt, ##__VA_ARGS__); FU_LOG(fmt, ##__VA_ARGS__)
#define FU_ERR(fmt, ...)      FU_PRINTF("Error: " fmt "\n", ##__VA_ARGS__)
#define FU_INF(fmt, ...)      FU_PRINTF(fmt "\n", ##__VA_ARGS__)
#define FU_DBG(fmt, ...)      if (m_port.GetVerbose()) PRINTF("Debug: " fmt "\n", ##__VA_ARGS__)

#define FU_LOGDATETIME        {time_t tNow = time(0); struct tm tmNow; localtime_s(&tmNow, &tNow); FU_LOG("%d-%02d-%02d %d:%02d:%02d\n", \
                              tmNow.tm_year + 1900, tmNow.tm_mon + 1, tmNow.tm_mday, tmNow.tm_hour, tmNow.tm_min, tmNow.tm_sec);}
#define DEVICE_ID_STR(id)     ((id == FUDID_PROCESSING_FPGA)? "Processing" : \
                              ((id == FUDID_OUTPUT_FPGA)? "Output" : \
                              ((id == FUDID_STORAGE_FPGA)? "Storage" : "Unknown")))

#define FU_DEFAULT_READ_MAX_PACKET_SIZE	248
#define FU_DEFAULT_WRITE_MAX_PACKET_SIZE	USART_MAX_BUFFER_SIZE

#define FU_PROM_SECTOR_COUNT  256
#define FU_PROM_SECTOR_SIZE   65536
#define FU_PROM_SIZE          (FU_PROM_SECTOR_COUNT * FU_PROM_SECTOR_SIZE)

#define SetSectorErased(array, pos) BitSet(array[pos / 32], (pos%32))
#define GetSectorErased(array, pos) BitTst(array[pos / 32], (pos%32))



class FirmwareUpdater
{
public:
	FirmwareUpdater();
	~FirmwareUpdater();
	
	typedef enum fuDeviceIDEnum {
		FUDID_PROCESSING_FPGA = 1,
		FUDID_OUTPUT_FPGA = 2,
		FUDID_STORAGE_FPGA = 3,
	} fuDevideID_t;

	IRC_Status_t Open();
	bool IsOpened();
	IRC_Status_t Close();

	IRC_Status_t ProgramPROM(const fuDevideID_t id, const char *filename, const bool checkOnly, const unsigned int maxPacketSize = FU_DEFAULT_WRITE_MAX_PACKET_SIZE);
	IRC_Status_t ReadPROM(const fuDevideID_t id, const unsigned int address, const unsigned int byteCount, const char *filename, const unsigned int maxPacketSize = FU_DEFAULT_READ_MAX_PACKET_SIZE);
	IRC_Status_t CheckPROM(const fuDevideID_t id, const unsigned int address, const unsigned int byteCount);

	void SetVerbose(bool verbose);
	bool GetVerbose();

	void PrintReport();

private:
	UsartPortF1F2 m_port;
};

#endif // __FIRMWAREUPDATER_H__