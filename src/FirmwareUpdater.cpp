#include "FirmwareUpdater.h"
#include "IntelHexParser.h"
#include <stdint.h>
#include <fstream>
#include <time.h>
#include <string.h>

using namespace std;

FirmwareUpdater::FirmwareUpdater()
{

}

FirmwareUpdater::~FirmwareUpdater()
{

}

IRC_Status_t FirmwareUpdater::Open()
{
   if (IsOpened())
   {
      return IRC_SUCCESS;
   }

   return m_port.Open();
}

bool FirmwareUpdater::IsOpened()
{
	return m_port.IsOpened();
}

IRC_Status_t FirmwareUpdater::Close()
{
	return m_port.Close();
}

IRC_Status_t FirmwareUpdater::ProgramPROM(const fuDevideID_t id, const char *filename, const bool checkOnly, const unsigned int maxPacketSize)
{
   clock_t tStart;
   intelHexParser ihexParser;
   const intelHexParser::promArea_t *p_promArea;
   uint32_t byteCount;
   uint32_t totalByteCount;
   uint32_t promDataSize;
   uint32_t length;
   uint16_t crc16;
   uint32_t idxArea;

   FU_LOG("\n");
   FU_LOGDATETIME;
   FU_INF("%s %s FPGA PROM with %s...", checkOnly? "Verifying": "Programming", DEVICE_ID_STR(id), filename);

	try
	{
      tStart = clock();


      // Open USART port
      if (Open() != IRC_SUCCESS)
      {
         FU_ERR("Unable to open port.");
         throw 0;
      }
      
      // Validate maximum packet size
      if (maxPacketSize > USART_MAX_BUFFER_SIZE)
      {
         FU_ERR("Maximum packet size cannot be greater than %d.", USART_MAX_BUFFER_SIZE);
         throw 0;
      }

      // Check that device is present
      if (m_port.PromReadRequest(id, 0, 1, nullptr) != IRC_SUCCESS)
      {
         FU_ERR("Failed to reach device.");
         throw 0;
      }

		// Read PROM data from Intel Hex file
      if (ihexParser.parse(filename) != IRC_SUCCESS)
      {
         FU_ERR("Failed to parse %s.", filename);
         throw 0;
      }

      // Check PROM areas and compute total PROM data size and
      promDataSize = 0;
      idxArea = 0;
      p_promArea = ihexParser.getPromArea();
      while (p_promArea != nullptr)
      {
         if ((p_promArea->address + p_promArea->size) > FU_PROM_SIZE)
         {
            FU_ERR("Area %d is outside PROM boundaries (address: 0x%08X, size: %d).", idxArea, p_promArea->address, p_promArea->size);
            throw 0;
         }

         FU_INF("PROM area %d: %d byte(s) @ 0x%08X", idxArea, p_promArea->size, p_promArea->address);

         promDataSize += p_promArea->size;
         p_promArea = p_promArea->next;
         idxArea++;
      }
      FU_INF("Total PROM data size: %d byte(s)", promDataSize);

      if (!checkOnly)
      {
         // Erase PROM
         PRINTF("Erasing PROM sector(s)... 0%%");
         totalByteCount = 0;
         p_promArea = ihexParser.getPromArea();
         while (p_promArea != nullptr)
         {
            if (m_port.PromEraseRequest(id, p_promArea->address, p_promArea->size) != IRC_SUCCESS)
            {
               PRINTF("\n");
               FU_ERR("Failed to erase %d byte(s) in PROM @ 0x%08X.", p_promArea->size, p_promArea->address);
               throw 0;
            }

            totalByteCount += p_promArea->size;

            PRINTF("\rErasing PROM sector(s)... %d%%", (totalByteCount * 100) / promDataSize);

            p_promArea = p_promArea->next;
         }
         PRINTF("\n");

         // Write PROM data
         PRINTF("Writing PROM data... 0%%");
         totalByteCount = 0;
         p_promArea = ihexParser.getPromArea();
         while (p_promArea != nullptr)
         {
            byteCount = 0;
            while (byteCount < p_promArea->size)
            {
               length = MIN(p_promArea->size - byteCount, maxPacketSize - F1F2_MIN_PROM_PACKET_SIZE);
               if (m_port.PromWriteRequest(id, p_promArea->address + byteCount, length, &p_promArea->data[byteCount]) != IRC_SUCCESS)
               {
                  PRINTF("\n");
                  FU_ERR("Failed to write %d bytes in PROM @ 0x%08X.", length, p_promArea->address + byteCount);
                  throw 0;
               }

               byteCount += length;
               totalByteCount += length;

               PRINTF("\rWriting PROM data... %d%%", (totalByteCount * 100) / promDataSize);
            }

            p_promArea = p_promArea->next;
         }
         PRINTF("\n");
      }

      // Check PROM data
      PRINTF("Checking PROM data... 0%%");
      totalByteCount = 0;
      p_promArea = ihexParser.getPromArea();
      while (p_promArea != nullptr)
      {
         if (m_port.PromCheckRequest(id, p_promArea->address, p_promArea->size, &crc16) != IRC_SUCCESS)
         {
            PRINTF("\n");
            FU_ERR("Failed to check %d byte(s) in PROM @ 0x%08X.", p_promArea->size, p_promArea->address);
            throw 0;
         }

         if (crc16 != p_promArea->crc16)
         {
            PRINTF("\n");
            FU_ERR("CRC16 mismatch for %d byte(s) in PROM @ 0x%08X.", p_promArea->size, p_promArea->address);
            throw 0;
         }

         totalByteCount += p_promArea->size;

         PRINTF("\rChecking PROM data... %d%%", (totalByteCount * 100) / promDataSize);

         p_promArea = p_promArea->next;
      }
      PRINTF("\n");
		
      FU_INF("PROM has been successfully %s in %.3fs.", 
         checkOnly? "verified": "programmed",
         (double)(clock() - tStart)/CLOCKS_PER_SEC);
	}
	catch (...)
	{
		return IRC_FAILURE;
	}

	return IRC_SUCCESS;
}

IRC_Status_t FirmwareUpdater::ReadPROM(const fuDevideID_t id, const unsigned int address, const unsigned int byteCount, const char *filename, const unsigned int maxPacketSize)
{
   char *promData = nullptr;
   std::ofstream file;
   clock_t tStart;
   uint32_t dataOffset;
   uint16_t length;

   FU_LOG("\n");
   FU_LOGDATETIME;
   FU_INF("Reading %d byte(s) from %s FPGA PROM @ 0x%08X to %s...", byteCount, DEVICE_ID_STR(id), address, filename);

   try
   {
      tStart = clock();

      // Open USART port
      if (Open() != IRC_SUCCESS)
      {
         FU_ERR("Unable to open port.");
         throw 0;
      }

      // Validate maximum packet size
      if (maxPacketSize > USART_MAX_BUFFER_SIZE)
      {
         FU_ERR("Maximum packet size cannot be greater than %d.", USART_MAX_BUFFER_SIZE);
         throw 0;
      }

      // Check that device is present
      if (m_port.PromReadRequest(id, 0, 1, nullptr) != IRC_SUCCESS)
      {
         FU_ERR("Failed to reach device.");
         throw 0;
      }

      // Read PROM data
      promData = new char[byteCount];
      dataOffset = 0;

      PRINTF("Reading PROM data... 0%%");
      while (dataOffset < byteCount)
      {
         length = MIN(byteCount - dataOffset, maxPacketSize - F1F2_MIN_PROM_PACKET_SIZE);
         if (m_port.PromReadRequest(id, address + dataOffset, length, (uint8_t *)&promData[dataOffset]) != IRC_SUCCESS)
         {
            PRINTF("\n");
            FU_ERR("Failed to read %d bytes from PROM @ 0x%08X.", length, address + dataOffset);
            throw 0;;
         }

         dataOffset += length;

         PRINTF("\rReading PROM data... %d%%", (dataOffset * 100) / byteCount);
      }
      PRINTF("\n");

      // Open destination file
      file.open(filename, std::ios::out | std::ios::binary | std::ios::trunc);
      if (!file.is_open())
      {
         FU_ERR("Unable to open %s.", filename);
         throw 0;
      }

      // Write data to destination file
      file.write(promData, byteCount);
      file.close();

      FU_INF("PROM has been successfully read in %.3fs.", 
         (double)(clock() - tStart)/CLOCKS_PER_SEC);

      delete[] promData;
   }
   catch (...)
   {
      if (promData != nullptr)
      {
         delete[] promData;
      }

      return IRC_FAILURE;
   }

   return IRC_SUCCESS;
}

IRC_Status_t FirmwareUpdater::CheckPROM(const fuDevideID_t id, const unsigned int address, const unsigned int byteCount)
{
   clock_t tStart;
   uint16_t crc16;

   FU_LOG("\n");
   FU_LOGDATETIME;
   FU_INF("Checking %d byte(s) in %s FPGA PROM @ 0x%08X...", byteCount, DEVICE_ID_STR(id), address);

   try
   {
      tStart = clock();

      // Open USART port
      if (Open() != IRC_SUCCESS)
      {
         FU_ERR("Unable to open port.");
         throw 0;
      }

      // Check that device is present
      if (m_port.PromReadRequest(id, 0, 1, nullptr) != IRC_SUCCESS)
      {
         FU_ERR("Failed to reach device.");
         throw 0;
      }

      FU_INF("Checking %d byte(s) in PROM @ 0x%08X...", byteCount, address);
      if (m_port.PromCheckRequest(id, address, byteCount, &crc16) != IRC_SUCCESS)
      {
         FU_ERR("Failed to check %d byte(s) in PROM @ 0x%08X.", byteCount, address);
         throw 0;
      }
      FU_INF("CRC-16 is 0x%04X.", crc16);
      FU_INF("PROM has been successfully checked in %.3fs.", 
         (double)(clock() - tStart)/CLOCKS_PER_SEC);
   }
   catch (...)
   {
      return IRC_FAILURE;
   }

   return IRC_SUCCESS;
}

void FirmwareUpdater::SetVerbose(bool verbose)
{
   m_port.SetVerbose(verbose);
}

bool FirmwareUpdater::GetVerbose()
{
   return m_port.GetVerbose();
}

void FirmwareUpdater::PrintReport()
{
   FU_DBG("Error count: %d\n", m_port.GetErrorCount());
   FU_DBG("Timeout error count: %d\n", m_port.GetTimeoutErrorCount());
   FU_DBG("CRC-16 error count: %d\n", m_port.GetCRCErrorCount());
   FU_DBG("Max retry count: %d / %d\n", m_port.GetMaxRetryCount(), m_port.GetMaxRetry());
}
