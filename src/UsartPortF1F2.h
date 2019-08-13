#ifndef __USARTPORTF1F2_H__
#define __USARTPORTF1F2_H__

#include "usartPort.h"
#include "Protocol_F1F2.h"
#include "IRC_status.h"
#include "utils.h"

#define UPF1F2_PRINTF(fmt, ...)    if (m_verbose) PRINTF("UPF1F2: " fmt, ##__VA_ARGS__)
#define UPF1F2_ERR(fmt, ...)       UPF1F2_PRINTF("Error: " fmt "\n", ##__VA_ARGS__)
#define UPF1F2_INF(fmt, ...)       UPF1F2_PRINTF("Info: " fmt "\n", ##__VA_ARGS__)

#define UPF1F2_DEFAULT_TIMEOUT		2000
#define UPF1F2_DEFAULT_MAX_RETRY	   3

class UsartPortF1F2: public UsartPort
{
public:
	UsartPortF1F2();
	~UsartPortF1F2();

	IRC_Status_t Open();
	IRC_Status_t Close();

	IRC_Status_t SendRecvF1F2(F1F2Command_t *request, F1F2Command_t *response, uint32_t timeout = UPF1F2_DEFAULT_TIMEOUT);
	IRC_Status_t SendRecvF1F2Ack(F1F2Command_t *request, uint32_t timeout = UPF1F2_DEFAULT_TIMEOUT);
	IRC_Status_t FileCountRequest(uint32_t *p_fileCount);
	IRC_Status_t FileInfoRequest(const uint32_t index, F1F2FileInfo_t *p_f1f2FileInfo);
	IRC_Status_t FileReadRequest(const uint32_t index, const uint32_t dataOffset, const uint16_t dataLength, uint8_t *p_data);
	IRC_Status_t FileCreateRequest(const char *filename, uint32_t *p_index);
	IRC_Status_t FileWriteRequest(const uint32_t index, const uint32_t dataOffset, const uint16_t dataLength, uint8_t *p_data);
	IRC_Status_t FileCheckRequest(const uint32_t index, uint16_t *p_crc16);
	IRC_Status_t FileCloseRequest(const uint32_t index);
	IRC_Status_t FileDeleteRequest(const uint32_t index);
	IRC_Status_t FileFormatRequest();
	IRC_Status_t FileUsedSpaceRequest(uint64_t *p_fileUsedSpace);
	IRC_Status_t FileFreeSpaceRequest(uint64_t *p_fileFreeSpace);
	IRC_Status_t FileTotalSpaceRequest(uint64_t *p_fileTotalSpace);
   IRC_Status_t PromEraseRequest(const uint8_t device, const uint32_t dataOffset, const uint32_t dataLength);
   IRC_Status_t PromReadRequest(const uint8_t device, const uint32_t dataOffset, const uint16_t dataLength, uint8_t *p_data);
   IRC_Status_t PromWriteRequest(const uint8_t device, const uint32_t dataOffset, const uint16_t dataLength, uint8_t *p_data);
   IRC_Status_t PromCheckRequest(const uint8_t device, const uint32_t dataOffset, const uint32_t dataLength, uint16_t *p_crc16);
	IRC_Status_t PingRequest();
   uint32_t GetMaxRetry();
   void SetMaxRetry(uint32_t maxRetry);
	uint32_t GetErrorCount();
	uint32_t GetTimeoutErrorCount();
	uint32_t GetCRCErrorCount();
	uint32_t GetMaxRetryCount();
	void ResetCounters();

private:
	uint8_t m_rxBuffer[USART_MAX_BUFFER_SIZE];
	uint8_t m_txBuffer[USART_MAX_BUFFER_SIZE];
   uint32_t m_maxRetry;
	uint32_t m_errorCount;
	uint32_t m_timeoutErrorCount;
	uint32_t m_crcErrorCount;
	uint32_t m_maxRetryCount;
};

#endif // __USARTPORTF1F2_H__