/**
 * @file IntelHex.cpp
 * Intel HEX file format parser module implementation.
 *
 * This file implements the module used to parse Intel HEX file format.
 *
 * $Rev: 19221 $
 * $Author: dalain $
 * $Date: 2016-09-22 14:04:24 -0400 (jeu., 22 sept. 2016) $
 * $Id: IntelHexParser.cpp 19221 2016-09-22 18:04:24Z dalain $
 * $URL$
 *
 * (c) Copyright 2014 Telops Inc.
 */

#include "IntelHexParser.h"
#include <fstream>
#include "CRC.h"

intelHexParser::intelHexParser():
m_ihexRecord(nullptr),
m_promArea(nullptr)
{

}

intelHexParser::~intelHexParser()
{
   deleteIntelHexRecord(m_ihexRecord);
   deletePROMArea(m_promArea);
}

/**
 * Parse Intel HEX PROM file.
 *
 * @param filename is the Intel Hex PROM file to parse.
 *
 * @return IRC_SUCCESS if Intel HEX PROM file was successfully parsed.
 * @return IRC_FAILURE if failed to parse Intel HEX PROM file.
 */
IRC_Status_t intelHexParser::parse(const char *filename)
{
   uint32_t fileSize;
   uint32_t fileDataCount;
   char *fileData = nullptr;
   char *p_fileData;
   std::ifstream file;
   char lbuffer[IHEX_MAX_LINE_SIZE];
   uint32_t lbuflen;
   uint32_t charRead;
   uint32_t ihexLineIndex;
   uint8_t terminatorCount;
   ihexRecord_t *p_ihexRecord;
   promArea_t *p_promArea;
   uint32_t promBaseAddr;
   bool eofFound;
   uint32_t dataLength;

   try
   {
      // Open source file
      file.open(filename, std::ios::in | std::ios::binary | std::ios::ate);
      if (!file.is_open())
      {
         IHEX_ERR("Unable to open %s.", filename);
         throw 0;
      }

      // Read source file data
      fileSize = (uint32_t) file.tellg();
      fileData = new char[fileSize];
      file.seekg(0, std::ios::beg);
      file.read((char *) fileData, fileSize);
      file.close();

      deleteIntelHexRecord(m_ihexRecord);
      deletePROMArea(m_promArea);
      p_fileData = fileData;
      fileDataCount = fileSize;
      charRead = 0;
      ihexLineIndex = 0;
      promBaseAddr = 0x00000000;
      eofFound = false;

      while (fileDataCount > 0)
      {
         lbuflen = 0;
         terminatorCount = 0;
         ihexLineIndex++;

         if (getNextLine(p_fileData, fileDataCount, lbuffer, &lbuflen, &charRead, &terminatorCount) != IRC_SUCCESS)
         {
            IHEX_ERR("Failed to get next IHEX line (%d).\n", ihexLineIndex);
            throw 0;
         }

         p_fileData += charRead;
         fileDataCount -= charRead;

         lbuflen -= terminatorCount;
         lbuffer[lbuflen] = '\0';

         if (m_ihexRecord == nullptr)
         {
            // First record
            m_ihexRecord = parseLine(lbuffer, lbuflen);
            p_ihexRecord = m_ihexRecord;
         }
         else            
         {
            p_ihexRecord->next = parseLine(lbuffer, lbuflen);
            p_ihexRecord = p_ihexRecord->next;
         }

         if (p_ihexRecord == nullptr)
         {
            IHEX_ERR("Failed to parse IHEX line %d: %s.\n", ihexLineIndex, lbuffer);
            throw 0;
         }

         switch (p_ihexRecord->recordType)
         {
         case IHEXRT_00:   /** Data */
            if (m_promArea == nullptr)
            {
               // First PROM area
               m_promArea = new promArea_t;
               p_promArea = m_promArea;
               p_promArea->address = promBaseAddr + p_ihexRecord->address;
               p_promArea->size = p_ihexRecord->byteCount;
               p_promArea->data = nullptr;
               p_promArea->next = nullptr;
            }
            else
            {
               if ((promBaseAddr + p_ihexRecord->address) == (p_promArea->address + p_promArea->size))
               {
                  // Contiguous record, merge with actual area
                  p_promArea->size += p_ihexRecord->byteCount;
               }
               else
               {
                  // Not a contiguous record, create another area
                  p_promArea->next = new promArea_t;
                  p_promArea = p_promArea->next;
                  p_promArea->address = promBaseAddr + p_ihexRecord->address;
                  p_promArea->size = p_ihexRecord->byteCount;
                  p_promArea->data = nullptr;
                  p_promArea->next = nullptr;
               }
            }
            break;

         case IHEXRT_01:   /** End Of File */
            if (fileDataCount != 0)
            {
               IHEX_ERR("End of file record found before the end of file data.");
               throw 0;
            }

            eofFound = true;
            break;

         case IHEXRT_02:   /** Extended Segment Address */
         case IHEXRT_03:   /** Start Segment Address */
            IHEX_ERR("Segment address record types (02 and 03) are not supported by I32HEX format.");
            throw 0;
            break;

         case IHEXRT_04:   /** Extended Linear Address */
            promBaseAddr = (((uint32_t)p_ihexRecord->data[0]) << 24) | (((uint32_t)p_ihexRecord->data[1]) << 16);
            break;

         case IHEXRT_05:   /** Start Linear Address */
            IHEX_ERR("Start linear address record type (05) is not supported.");
            throw 0;
            break;
         }
      }

      if (!eofFound)
      {
         IHEX_ERR("End of file not found.\n");
         throw 0;
      }

      // Fill areas data with Intel Hex records data and compute area CRC-16 value
      p_promArea = m_promArea;
      p_ihexRecord = m_ihexRecord;
      while (p_promArea != nullptr)
      {
         dataLength = 0;
         p_promArea->data = new uint8_t[p_promArea->size];
         while ((p_ihexRecord != nullptr) && (dataLength < p_promArea->size))
         {
            if (p_ihexRecord->recordType == IHEXRT_00)
            {
               if (p_ihexRecord->byteCount > (p_promArea->size - dataLength))
               {
                  IHEX_ERR("Intel Hex records and PROM area data size mismatch.");
                  throw 0;
               }

               memcpy(&p_promArea->data[dataLength], p_ihexRecord->data, p_ihexRecord->byteCount);
               dataLength += p_ihexRecord->byteCount;
            }

            p_ihexRecord = p_ihexRecord->next;
         }
         p_promArea->crc16 = CRC16(0xFFFF, p_promArea->data, p_promArea->size);
         p_promArea = p_promArea->next;
      }
   }
   catch (...)
   {
      return IRC_FAILURE;
   }

   return IRC_SUCCESS;
}

/**
 * Get PROM areas list.
 *
 * @return PROM areas list.
 */
const intelHexParser::promArea_t *intelHexParser::getPromArea()
{
   return m_promArea;
}

/**
 * Get next Intel HEX record from a char buffer.
 *
 * @param buffer is the char buffer.
 * @param buflen is the char buffer length.
 * @param lbuffer is the line char buffer (updated in this function).
 * @param p_lbuflen is a pointer to the line char buffer length (updated in this function).
 * @param p_charRead is a pointer to the number of char that have been read from buffer.
 * @param p_termCount is a pointer to the number of terminator that have been read from buffer (updated in this function).
 *
 * @return IRC_SUCCESS if a complete line has been copied into line char buffer.
 * @return IRC_NOT_DONE if a chars have been copied into line char buffer but no terminator have been met.
 * @return IRC_FAILURE if maximum number of char have been copied into line char buffer but no terminator have been met.
 */
IRC_Status_t intelHexParser::getNextLine(const char *buffer, uint32_t buflen, char *lbuffer, uint32_t *p_lbuflen, uint32_t *p_charRead, uint8_t *p_termCount)
{
   *p_charRead = 0;

   while (*p_charRead < buflen)
   {
      if (((*p_termCount) > 0) && ((buffer[(*p_charRead)] != (char)0x0D) && (buffer[(*p_charRead)] != (char)0x0A)))
      {
         // No other terminator char, this is the end of the line
         return IRC_SUCCESS;
      }

      // Read next char
      lbuffer[(*p_lbuflen)++] = buffer[(*p_charRead)++];

      if ((lbuffer[(*p_lbuflen) - 1] == (char)0x0D) || (lbuffer[(*p_lbuflen) - 1] == (char)0x0A))
      {
         (*p_termCount)++;

         if ((*p_termCount) == IHEX_MAX_TERMINATOR_SIZE)
         {
            // Max number of terminator reached
            return IRC_SUCCESS;
         }
      }

      if ((*p_lbuflen) == IHEX_MAX_LINE_SIZE)
      {
         IHEX_ERR("Maximum line size has been reached.");
         return IRC_FAILURE;
      }
   }

   return IRC_NOT_DONE;
}

/**
 * Parse Intel HEX record from a line char buffer.
 *
 * @param lbuffer is the line char buffer without line terminator(s).
 * @param lbuflen is the line char buffer length without line terminator(s).
 *
 * @return an Intel HEX record pointer when buffer has been successfully parsed.
 * @return Null pointer otherwise.
 */
intelHexParser::ihexRecord_t *intelHexParser::parseLine(const char *lbuffer, uint32_t lbuflen)
{
   uint32_t charIdx = 0;
   ihexRecord_t *ihexRecord = nullptr;

   try
   {
      // Read and validate start code
      if (lbuflen < IHEX_START_CODE_SIZE)
      {
         IHEX_ERR("Not enough chars to start code.");
         throw 0;
      }

      if (lbuffer[charIdx] != IHEX_START_CODE)
      {
         IHEX_ERR("Intel HEX must start with "":"".");
         throw 0;
      }
      charIdx += IHEX_START_CODE_SIZE;

      // Read data count
      if (lbuflen < (charIdx + IHEX_BYTE_COUNT_SIZE))
      {
         IHEX_ERR("Not enough chars to read data count.");
         throw 0;
      }

      ihexRecord = new ihexRecord_t;
      ihexRecord->next = nullptr;

      ihexRecord->byteCount = Hex2Byte(&lbuffer[charIdx]);
      charIdx += IHEX_BYTE_COUNT_SIZE;

      // Read address
      if (lbuflen < (charIdx + IHEX_ADDRESS_SIZE))
      {
         IHEX_ERR("Not enough chars to read address.");
         throw 0;
      }

      ihexRecord->address = ((uint16_t)Hex2Byte(&lbuffer[charIdx]) << 8) +
         (uint16_t)Hex2Byte(&lbuffer[charIdx + 2]);
      charIdx += IHEX_ADDRESS_SIZE;

      // Read record type
      if (lbuflen < (charIdx + IHEX_REC_TYPE_SIZE))
      {
         IHEX_ERR("Not enough chars to read record type.");
         throw 0;
      }

      ihexRecord->recordType = (ihexRecordType_t) Hex2Byte(&lbuffer[charIdx]);
      charIdx += IHEX_REC_TYPE_SIZE;

      // Read data
      if (lbuflen < (charIdx + (2 * ihexRecord->byteCount)))
      {
         IHEX_ERR("Not enough chars to read data.");
         throw 0;
      }

      Hex2Bytes(&lbuffer[charIdx], (2 * ihexRecord->byteCount), ihexRecord->data, IHEX_MAX_DATA_COUNT);
      charIdx += (2 * ihexRecord->byteCount);

      // Read checksum
      if (lbuflen < (charIdx + IHEX_CHECKSUM_SIZE))
      {
         IHEX_ERR("Not enough chars to read checksum.");
         throw 0;
      }

      ihexRecord->chksum = Hex2Byte(&lbuffer[charIdx]);
      charIdx += IHEX_CHECKSUM_SIZE;

      // Validate checksum
      if (ihexRecord->chksum != computeChecksum(&lbuffer[IHEX_START_CODE_SIZE],
         IHEX_BYTE_COUNT_SIZE + IHEX_ADDRESS_SIZE + IHEX_REC_TYPE_SIZE + (2 * ihexRecord->byteCount)))
      {
         IHEX_ERR("Invalid checksum.");
         throw 0;
      }
   }
   catch (...)
   {
      deleteIntelHexRecord(ihexRecord);
   }

   return ihexRecord;
}

/**
 * Compute Intel HEX checksum.
 *
 * @param buffer is the hex digits char buffer.
 * @param buflen is the hex digits char buffer length.
 *
 * @return the Intel HEX checksum.
 */
uint8_t intelHexParser::computeChecksum(const char *buffer, uint32_t buflen)
{
   return ~Checksum8Hex(buffer, buflen) + 1;
}

/**
 * Delete Intel Hex records list.
 *
 * @param ihexRecord is a pointer to the Intel Hex records list to delete.
 */
void intelHexParser::deleteIntelHexRecord(ihexRecord_t *ihexRecord)
{
   ihexRecord_t *ihexRecordToDelete;

   while (ihexRecord)
   {
      ihexRecordToDelete = ihexRecord;
      ihexRecord = ihexRecord->next;
      delete ihexRecordToDelete;
   }
}

/**
 * Delete PROM areas list.
 *
 * @param promArea is a pointer to the PROM areas list to delete.
 */
void intelHexParser::deletePROMArea(promArea_t *promArea)
{
   promArea_t *promAreaToDelete;

   while (promArea)
   {
      promAreaToDelete = promArea;
      promArea = promArea->next;

      if (promAreaToDelete->data != nullptr)
      {
         delete [] promAreaToDelete->data;
      }
      delete promAreaToDelete;
   }
}
