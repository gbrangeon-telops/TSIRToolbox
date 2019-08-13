#include "UsartPortF1F2.h"
#include <PvDeviceFinderWnd.h>
#include <string>
#include <fstream>
#include <time.h>
#include "Utils.h"

using namespace std;

UsartPortF1F2::UsartPortF1F2() :
	UsartPort(), 
   m_maxRetry(UPF1F2_DEFAULT_MAX_RETRY)
{
	ResetCounters();
}

UsartPortF1F2::~UsartPortF1F2()
{

}

IRC_Status_t UsartPortF1F2::Open()
{
	if (!UsartPort::Open())
	{
		return IRC_FAILURE;
	}

	return IRC_SUCCESS;
}

IRC_Status_t UsartPortF1F2::Close()
{
	if (!UsartPort::Close())
	{
		return IRC_FAILURE;
	}

	return IRC_SUCCESS;
}

IRC_Status_t UsartPortF1F2::SendRecvF1F2(F1F2Command_t *request, F1F2Command_t *response, uint32_t timeoutPeriod)
{
	PvResult lResult;
	uint32_t txBufferLenght;
	unsigned int bytesRead;
	unsigned int totalBytesRead;
	IRC_Status_t status;
	uint32_t retryCount;
	unsigned long tTimeoutStart;
	bool timeout;

	txBufferLenght = F1F2_CommandBuilder(request, m_txBuffer, USART_MAX_BUFFER_SIZE);

	retryCount = 0;
	while (retryCount < m_maxRetry)
	{
		// Clean RX buffer
		bytesRead = 0;
		lResult = m_port.Read(m_rxBuffer, USART_MAX_BUFFER_SIZE, bytesRead, 0);
		if (bytesRead > 0)
		{
			UPF1F2_INF("sendrecvF1F2: %d bytes have been flushed.", bytesRead);
		}

		// Transmit request
		if (Write(m_txBuffer, txBufferLenght) == txBufferLenght)
		{
			// Listen for response
			UPF1F2_INF("sendrecvF1F2: Listening for response...");

			tTimeoutStart = clock();
			timeout = false;
			totalBytesRead = 0;
			do 
			{
				bytesRead = 0;
				lResult = m_port.Read(m_rxBuffer + totalBytesRead, USART_MAX_BUFFER_SIZE - totalBytesRead, bytesRead, 0);
				totalBytesRead += bytesRead;
				UPF1F2_INF("sendrecvF1F2: Result = %s, bytesRead = %d, totalBytesRead = %d.", lResult.GetCodeString().GetAscii(), bytesRead, totalBytesRead);

				if (bytesRead == 0)
				{
					// Check timeout
					timeout = ((clock() - tTimeoutStart) >= timeoutPeriod);
				}
				else
				{
					// Reset timeout
					tTimeoutStart = clock();
					timeout = false;
				}

				status = F1F2_CommandParser(m_rxBuffer, totalBytesRead, response, &m_crcErrorCount);
			} while ((status == IRC_NOT_DONE) && (!timeout));

			if (status == IRC_SUCCESS)
			{
				if (response->cmd != F1F2_CMD_NAK)
				{
					return IRC_SUCCESS;
				}
				else
				{
					UPF1F2_ERR("sendrecvF1F2: NAK received.");
				}
			}

			if (status == IRC_FAILURE)
			{
				UPF1F2_ERR("sendrecvF1F2: F1F2_CommandParser error.");
#ifdef USARTPORTF1F2_VERBOSE
				memdump(m_rxBuffer, totalBytesRead, 0, 16);
#endif
			}

			if (timeout)
			{
				UPF1F2_ERR("sendrecvF1F2: Timeout!");
				m_timeoutErrorCount++;
			}
		}
		else
		{
			UPF1F2_ERR("sendrecvF1F2: Write error.");
		}

		m_errorCount++;
		retryCount++;

		if (retryCount > m_maxRetryCount)
		{
			m_maxRetryCount = retryCount;
		}

		UPF1F2_INF("sendrecvF1F2: Retry %d.", retryCount);
	}

	return IRC_FAILURE;
}

IRC_Status_t UsartPortF1F2::SendRecvF1F2Ack(F1F2Command_t *request, uint32_t timeout)
{
	F1F2Command_t response;
	IRC_Status_t status;

	status = SendRecvF1F2(request, &response, timeout);

	if ((status != IRC_SUCCESS) || (response.cmd != F1F2_CMD_ACK) || (response.payload.ack.cmd != request->cmd))
	{
		return IRC_FAILURE;
	}

	return IRC_SUCCESS;
}

IRC_Status_t UsartPortF1F2::FileCountRequest(uint32_t *p_fileCount)
{
	F1F2Command_t request;
	F1F2Command_t response;
	IRC_Status_t status;

	UPF1F2_INF("Reading file count...");

	if (!UsartPort::IsOpened())
	{
		UPF1F2_ERR("USART port is not opened.");
		return IRC_FAILURE;
	}

	F1F2_CommandClear(&request);
	request.cmd = F1F2_CMD_FILE_COUNT_REQ;

	status = SendRecvF1F2(&request, &response);
	if ((status != IRC_SUCCESS) || (response.cmd != F1F2_CMD_FILE_COUNT_RSP))
	{
		UPF1F2_ERR("File count request failed.");
		return IRC_FAILURE;
	}

	*p_fileCount = response.payload.fileCount.count;

	return IRC_SUCCESS;
}

IRC_Status_t UsartPortF1F2::FileInfoRequest(const uint32_t index, F1F2FileInfo_t *p_f1f2FileInfo)
{
	F1F2Command_t request;
	F1F2Command_t response;
	IRC_Status_t status;

	UPF1F2_INF("Reading file info...");

	if (!UsartPort::IsOpened())
	{
		UPF1F2_ERR("USART port is not opened.");
		return IRC_FAILURE;
	}

	F1F2_CommandClear(&request);
	request.cmd = F1F2_CMD_FILE_INFO_REQ;
	request.payload.fileIndex.index = index;

	status = SendRecvF1F2(&request, &response);
	if ((status != IRC_SUCCESS) || (response.cmd != F1F2_CMD_FILE_INFO_RSP))
	{
		UPF1F2_ERR("File info request failed.");
		return IRC_FAILURE;
	}

	*p_f1f2FileInfo = response.payload.fileInfo;

	return IRC_SUCCESS;
}

IRC_Status_t UsartPortF1F2::FileReadRequest(const uint32_t index, const uint32_t dataOffset, const uint16_t dataLength, uint8_t *p_data)
{
	F1F2Command_t request;
	F1F2Command_t response;
	IRC_Status_t status;

	UPF1F2_INF("Reading file data...");

	if (!UsartPort::IsOpened())
	{
		UPF1F2_ERR("USART port is not opened.");
		return IRC_FAILURE;
	}

	if (dataLength + F1F2_MIN_FILE_PACKET_SIZE > USART_MAX_BUFFER_SIZE)
	{
		UPF1F2_ERR("Cannot read more than %d bytes in one file read request.", USART_MAX_BUFFER_SIZE - F1F2_MIN_FILE_PACKET_SIZE);
		return IRC_FAILURE;
	}

	F1F2_CommandClear(&request);
	request.cmd = F1F2_CMD_FILE_READ_REQ;
	request.payload.fileIndex.index = index;
	request.payload.fileRW.offset = dataOffset;
	request.payload.fileRW.dataLength = dataLength;

	status = SendRecvF1F2(&request, &response);
	if ((status != IRC_SUCCESS) || (response.cmd != F1F2_CMD_FILE_READ_RSP))
	{
		UPF1F2_ERR("File read request failed.");
		return IRC_FAILURE;
	}

	if (response.payload.fileRW.dataLength != dataLength)
	{
		UPF1F2_ERR("File read data length mismatch.");
		return IRC_FAILURE;
	}

	memcpy(p_data, response.payload.fileRW.data, response.payload.fileRW.dataLength);

	return IRC_SUCCESS;
}

IRC_Status_t UsartPortF1F2::FileCreateRequest(const char *filename, uint32_t *p_index)
{
	F1F2Command_t request;
	F1F2Command_t response;
	IRC_Status_t status;

	UPF1F2_INF("Creating file...");

	if (!UsartPort::IsOpened())
	{
		UPF1F2_ERR("USART port is not opened.");
		return IRC_FAILURE;
	}

	F1F2_CommandClear(&request);
	request.cmd = F1F2_CMD_FILE_CREATE_REQ;
	strcpy_s(request.payload.fileName.name, F1F2_FILE_NAME_SIZE + 1, filename);

	status = SendRecvF1F2(&request, &response);
	if ((status != IRC_SUCCESS) || (response.cmd != F1F2_CMD_FILE_CREATE_RSP))
	{
		UPF1F2_ERR("File create request failed.");
		return IRC_FAILURE;
	}

	*p_index = response.payload.fileIndex.index;

	return IRC_SUCCESS;
}

IRC_Status_t UsartPortF1F2::FileWriteRequest(const uint32_t index, const uint32_t dataOffset, const uint16_t dataLength, uint8_t *p_data)
{
	F1F2Command_t request;

	UPF1F2_INF("Writing file data...");

	if (!UsartPort::IsOpened())
	{
		UPF1F2_ERR("USART port is not opened.");
		return IRC_FAILURE;
	}

	if (dataLength + F1F2_MIN_FILE_PACKET_SIZE > USART_MAX_BUFFER_SIZE)
	{
		UPF1F2_ERR("Cannot write more than %d bytes in one file write request.", USART_MAX_BUFFER_SIZE - F1F2_MIN_FILE_PACKET_SIZE);
		return IRC_FAILURE;
	}

	F1F2_CommandClear(&request);
	request.cmd = F1F2_CMD_FILE_WRITE;
	request.payload.fileRW.index = index;
	request.payload.fileRW.offset = dataOffset;
	request.payload.fileRW.dataLength = dataLength;
	memcpy(request.payload.fileRW.data, p_data, dataLength);

	if (SendRecvF1F2Ack(&request) != IRC_SUCCESS)
	{
		UPF1F2_ERR("File write request failed.");
		return IRC_FAILURE;
	}

	return IRC_SUCCESS;
}

IRC_Status_t UsartPortF1F2::FileCheckRequest(const uint32_t index, uint16_t *p_crc16)
{
	F1F2Command_t request;
	F1F2Command_t response;
	IRC_Status_t status;

	UPF1F2_INF("Checking file...");

	if (!UsartPort::IsOpened())
	{
		UPF1F2_ERR("USART port is not opened.");
		return IRC_FAILURE;
	}

   F1F2_CommandClear(&request);
   request.cmd = F1F2_CMD_FILE_CHECK_REQ;
	request.payload.fileIndex.index = index;

	status = SendRecvF1F2(&request, &response, 60000);
	if ((status != IRC_SUCCESS) || (response.cmd != F1F2_CMD_FILE_CHECK_RSP))
	{
		UPF1F2_ERR("File check request failed.");
		return IRC_FAILURE;
	}

	*p_crc16 = response.payload.fileCheck.crc16;

	return IRC_SUCCESS;
}

IRC_Status_t UsartPortF1F2::FileCloseRequest(const uint32_t index)
{
	F1F2Command_t request;

	UPF1F2_INF("Closing file...");

	if (!UsartPort::IsOpened())
	{
		UPF1F2_ERR("USART port is not opened.");
		return IRC_FAILURE;
	}

	F1F2_CommandClear(&request);
	request.cmd = F1F2_CMD_FILE_CLOSE;
	request.payload.fileIndex.index = index;

	if (SendRecvF1F2Ack(&request) != IRC_SUCCESS)
	{
		UPF1F2_ERR("File close request failed.");
		return IRC_FAILURE;
	}

	return IRC_SUCCESS;
}

IRC_Status_t UsartPortF1F2::FileDeleteRequest(const uint32_t index)
{
	F1F2Command_t request;

	UPF1F2_INF("Deleting file...");

	if (!UsartPort::IsOpened())
	{
		UPF1F2_ERR("USART port is not opened.");
		return IRC_FAILURE;
	}

	F1F2_CommandClear(&request);
	request.cmd = F1F2_CMD_FILE_DELETE;
	request.payload.fileIndex.index = index;

	if (SendRecvF1F2Ack(&request) != IRC_SUCCESS)
	{
		UPF1F2_ERR("File delete request failed.");
		return IRC_FAILURE;
	}

	return IRC_SUCCESS;
}

IRC_Status_t UsartPortF1F2::FileFormatRequest()
{
	F1F2Command_t request;

	UPF1F2_INF("Formating file system...");

	if (!UsartPort::IsOpened())
	{
		UPF1F2_ERR("USART port is not opened.");
		return IRC_FAILURE;
	}

	F1F2_CommandClear(&request);
	request.cmd = F1F2_CMD_FILE_FORMAT;

	if (SendRecvF1F2Ack(&request, 30000) != IRC_SUCCESS)
	{
		UPF1F2_ERR("File system format request failed.");
		return IRC_FAILURE;
	}

	return IRC_SUCCESS;
}

IRC_Status_t UsartPortF1F2::FileUsedSpaceRequest(uint64_t *p_fileUsedSpace)
{
	F1F2Command_t request;
	F1F2Command_t response;
	IRC_Status_t status;

	UPF1F2_INF("Reading filesystem used space...");

	if (!UsartPort::IsOpened())
	{
		UPF1F2_ERR("USART port is not opened.");
		return IRC_FAILURE;
	}

	F1F2_CommandClear(&request);
	request.cmd = F1F2_CMD_FILE_USED_SPACE_REQ;

	status = SendRecvF1F2(&request, &response);
	if ((status != IRC_SUCCESS) || (response.cmd != F1F2_CMD_FILE_USED_SPACE_RSP))
	{
		UPF1F2_ERR("Filesystem used space request failed.");
		return IRC_FAILURE;
	}

	*p_fileUsedSpace = response.payload.fileSpace.space;

	return IRC_SUCCESS;
}

IRC_Status_t UsartPortF1F2::FileFreeSpaceRequest(uint64_t *p_fileFreeSpace)
{
	F1F2Command_t request;
	F1F2Command_t response;
	IRC_Status_t status;

	UPF1F2_INF("Reading filesystem free space...");

	if (!UsartPort::IsOpened())
	{
		UPF1F2_ERR("USART port is not opened.");
		return IRC_FAILURE;
	}

	F1F2_CommandClear(&request);
	request.cmd = F1F2_CMD_FILE_FREE_SPACE_REQ;

	status = SendRecvF1F2(&request, &response);
	if ((status != IRC_SUCCESS) || (response.cmd != F1F2_CMD_FILE_FREE_SPACE_RSP))
	{
		UPF1F2_ERR("Filesystem free space request failed.");
		return IRC_FAILURE;
	}

	*p_fileFreeSpace = response.payload.fileSpace.space;

	return IRC_SUCCESS;
}

IRC_Status_t UsartPortF1F2::FileTotalSpaceRequest(uint64_t *p_fileTotalSpace)
{
	F1F2Command_t request;
	F1F2Command_t response;
	IRC_Status_t status;

	UPF1F2_INF("Reading filesystem total space...");

	if (!UsartPort::IsOpened())
	{
		UPF1F2_ERR("USART port is not opened.");
		return IRC_FAILURE;
	}

	F1F2_CommandClear(&request);
	request.cmd = F1F2_CMD_FILE_TOTAL_SPACE_REQ;

	status = SendRecvF1F2(&request, &response);
	if ((status != IRC_SUCCESS) || (response.cmd != F1F2_CMD_FILE_TOTAL_SPACE_RSP))
	{
		UPF1F2_ERR("Filesystem total space request failed.");
		return IRC_FAILURE;
	}

	*p_fileTotalSpace = response.payload.fileSpace.space;

	return IRC_SUCCESS;
}

IRC_Status_t UsartPortF1F2::PromEraseRequest(const uint8_t device, const uint32_t dataOffset, const uint32_t dataLength)
{
   F1F2Command_t request;

   UPF1F2_INF("Erasing PROM...");

   if (!UsartPort::IsOpened())
   {
      UPF1F2_ERR("USART port is not opened.");
      return IRC_FAILURE;
   }

   F1F2_CommandClear(&request);
   request.cmd = F1F2_CMD_PROM_ERASE;
   request.payload.promErase.device = device;
   request.payload.promErase.offset = dataOffset;
   request.payload.promErase.dataLength = dataLength;

   if (SendRecvF1F2Ack(&request, 300000) != IRC_SUCCESS)
   {
      UPF1F2_ERR("PROM erase request failed.");
      return IRC_FAILURE;
   }

   return IRC_SUCCESS;
}

IRC_Status_t UsartPortF1F2::PromReadRequest(const uint8_t device, const uint32_t dataOffset, const uint16_t dataLength, uint8_t *p_data)
{
   F1F2Command_t request;
   F1F2Command_t response;
   IRC_Status_t status;

   UPF1F2_INF("Reading PROM data...");

   if (!UsartPort::IsOpened())
   {
      UPF1F2_ERR("USART port is not opened.");
      return IRC_FAILURE;
   }

   if (dataLength + F1F2_MIN_PROM_PACKET_SIZE > USART_MAX_BUFFER_SIZE)
   {
      UPF1F2_ERR("Cannot read more than %d bytes in one PROM read request.", USART_MAX_BUFFER_SIZE - F1F2_MIN_PROM_PACKET_SIZE);
      return IRC_FAILURE;
   }

   F1F2_CommandClear(&request);
   request.cmd = F1F2_CMD_PROM_READ_REQ;
   request.payload.promRW.device = device;
   request.payload.promRW.offset = dataOffset;
   request.payload.promRW.dataLength = dataLength;

   status = SendRecvF1F2(&request, &response);
   if ((status != IRC_SUCCESS) || (response.cmd != F1F2_CMD_PROM_READ_RSP))
   {
      UPF1F2_ERR("PROM read request failed.");
      return IRC_FAILURE;
   }

   if (response.payload.promRW.dataLength != dataLength)
   {
      UPF1F2_ERR("PROM read data length mismatch.");
      return IRC_FAILURE;
   }

   if (p_data != nullptr)
   {
      memcpy(p_data, response.payload.promRW.data, response.payload.promRW.dataLength);
   }

   return IRC_SUCCESS;
}

IRC_Status_t UsartPortF1F2::PromWriteRequest(const uint8_t device, const uint32_t dataOffset, const uint16_t dataLength, uint8_t *p_data)
{
   F1F2Command_t request;

   UPF1F2_INF("Writing PROM data...");

   if (!UsartPort::IsOpened())
   {
      UPF1F2_ERR("USART port is not opened.");
      return IRC_FAILURE;
   }

   if (dataLength + F1F2_MIN_PROM_PACKET_SIZE > USART_MAX_BUFFER_SIZE)
   {
      UPF1F2_ERR("Cannot write more than %d bytes in one PROM write request.", USART_MAX_BUFFER_SIZE - F1F2_MIN_PROM_PACKET_SIZE);
      return IRC_FAILURE;
   }

   F1F2_CommandClear(&request);
   request.cmd = F1F2_CMD_PROM_WRITE;
   request.payload.promRW.device = device;
   request.payload.promRW.offset = dataOffset;
   request.payload.promRW.dataLength = dataLength;
   memcpy(request.payload.promRW.data, p_data, dataLength);

   if (SendRecvF1F2Ack(&request) != IRC_SUCCESS)
   {
      UPF1F2_ERR("PROM write request failed.");
      return IRC_FAILURE;
   }

   return IRC_SUCCESS;
}

IRC_Status_t UsartPortF1F2::PromCheckRequest(const uint8_t device, const uint32_t dataOffset, const uint32_t dataLength, uint16_t *p_crc16)
{
   F1F2Command_t request;
   F1F2Command_t response;
   IRC_Status_t status;

   UPF1F2_INF("Checking PROM...");

   if (!UsartPort::IsOpened())
   {
      UPF1F2_ERR("USART port is not opened.");
      return IRC_FAILURE;
   }

   F1F2_CommandClear(&request);
   request.cmd = F1F2_CMD_PROM_CHECK_REQ;
   request.payload.promCheck.device = device;
   request.payload.promCheck.offset = dataOffset;
   request.payload.promCheck.dataLength = dataLength;

   status = SendRecvF1F2(&request, &response, 300000);
   if ((status != IRC_SUCCESS) || (response.cmd != F1F2_CMD_PROM_CHECK_RSP))
   {
      UPF1F2_ERR("PROM check request failed.");
      return IRC_FAILURE;
   }

   *p_crc16 = response.payload.promCheck.crc16;

   return IRC_SUCCESS;
}

IRC_Status_t UsartPortF1F2::PingRequest()
{
	F1F2Command_t request;

	UPF1F2_INF("Sending ping request...");

	if (!UsartPort::IsOpened())
	{
		UPF1F2_ERR("USART port is not opened.");
		return IRC_FAILURE;
	}

	F1F2_CommandClear(&request);
	request.cmd = F1F2_CMD_PING;

	if (SendRecvF1F2Ack(&request) != IRC_SUCCESS)
	{
		UPF1F2_ERR("Ping request failed.");
		return IRC_FAILURE;
	}

	return IRC_SUCCESS;
}

uint32_t UsartPortF1F2::GetMaxRetry()
{
   return m_maxRetry;
}

void UsartPortF1F2::SetMaxRetry(uint32_t maxRetry)
{
   m_maxRetry = maxRetry;
}

uint32_t UsartPortF1F2::GetErrorCount()
{
	return m_errorCount;
}

uint32_t UsartPortF1F2::GetTimeoutErrorCount()
{
	return m_timeoutErrorCount;
}

uint32_t UsartPortF1F2::GetCRCErrorCount()
{
	return m_crcErrorCount;
}

uint32_t UsartPortF1F2::GetMaxRetryCount()
{
	return m_maxRetryCount;
}

void UsartPortF1F2::ResetCounters()
{
	m_errorCount = 0;
	m_timeoutErrorCount = 0;
	m_crcErrorCount = 0;
	m_maxRetryCount = 0;
}
