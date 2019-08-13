/**
 * @file Protocol_F1F2.c
 * F1F2 protocol module implementation.
 *
 * This file implements the F1F2v2 protocol module.
 *
 * $Rev: 19221 $
 * $Author: dalain $
 * $Date: 2016-09-22 14:04:24 -0400 (jeu., 22 sept. 2016) $
 * $Id: Protocol_F1F2.cpp 19221 2016-09-22 18:04:24Z dalain $
 * $URL: http://einstein/svn/firmware/FIR-00251-Common/branchs/2015-07-06%20-%20Comm%20Inter-FPGA/Software/Protocol_F1F2.c $
 *
 * (c) Copyright 2014 Telops Inc.
 */

#include "Protocol_F1F2.h"
#include "CRC.h"
#include "Utils.h"
#include <string.h> // memcpy

IRC_Status_t F1F2_PayloadParser(uint8_t *p_payload, uint16_t pdc, F1F2Command_t *f1f2Cmd);
uint32_t F1F2_FrameBuilder(uint8_t *buffer, uint16_t buflen, uint8_t cmd, F1F2PayloadData_t *payloadData, uint16_t payloadDataCount);

/**
 * Clear F1F2 file info structure.
 *
 * @param f1f2FileInfo is the pointer to the F1F2 file info structure to clear.
 */
void F1F2_FileInfoClear(F1F2FileInfo_t *f1f2FileInfo)
{
   memset(f1f2FileInfo, 0, sizeof(F1F2FileInfo_t));
}

/**
 * Clear F1F2 command structure.
 *
 * @param f1f2Cmd is the pointer to the F1F2 command structure to clear.
 */
void F1F2_CommandClear(F1F2Command_t *f1f2Cmd)
{
   memset(f1f2Cmd, 0, sizeof(F1F2Command_t));
}

/**
 * F1F2 protocol command parser function.
 *
 * @param buffer is a pointer to the command raw data that was received.
 * @param buflen is the length of the command raw data that was received.
 * @param f1f2Cmd is the pointer to the F1F2 command structure to fill using received data.
 * @param crcErrorCount is the pointer to the CRC error counter.
 *
 * @return IRC_SUCCESS if successfully parsed
 * @return IRC_FAILURE if failed to parse.
 * @return IRC_NOT_DONE if not enough data in buffer.
 */
IRC_Status_t F1F2_CommandParser(uint8_t *buffer, uint16_t buflen, F1F2Command_t *f1f2Cmd, uint32_t *crcErrorCount)
{
   uint16_t pdc;        // Payload Data Count
   uint16_t crc16;      // CRC-16 Value

   // Reset F1F2 command data
   F1F2_CommandClear(f1f2Cmd);
   f1f2Cmd->cmd = F1F2_CMD_ERROR;

   // Validate that there is enough bytes
   if (buflen < F1F2_MIN_PACKET_SIZE)
   {
      return IRC_NOT_DONE;
   }

   // Validate SOP
   if (buffer[F1F2_OFFSET_SOP] != F1F2_SOP)
   {
      F1F2_ERR("Invalid SOP.");
      return IRC_FAILURE;
   }

   // Read F1F2 command code
   f1f2Cmd->cmd = buffer[F1F2_OFFSET_CMD];

   // Read F1F2 command payload data count
   memcpy(&pdc, &buffer[F1F2_OFFSET_PDC], sizeof(pdc));

   // Check that all frame bytes have been received
   if (buflen < (F1F2_MIN_PACKET_SIZE + pdc))
   {
      return IRC_NOT_DONE;
   }

   // Validate packet length
   if (buflen > MIN(F1F2_MIN_PACKET_SIZE + pdc, F1F2_MAX_PACKET_SIZE))
   {
      F1F2_ERR("Invalid packet length.");
      return IRC_FAILURE;
   }

   // Validate EOP
   if (buffer[buflen - 1] != F1F2_EOP)
   {
      F1F2_ERR("Invalid EOP.");
      return IRC_FAILURE;
   }
   
   // Validate CRC-16
   memcpy(&crc16, &buffer[F1F2_OFFSET_PAYLOAD_DATA + pdc], sizeof(crc16));
   if (crc16 != CRC16(0xFFFF, buffer, F1F2_OFFSET_PAYLOAD_DATA + pdc))
   {
      F1F2_ERR("Invalid CRC-16.");
		if (crcErrorCount != nullptr)
		{
			(*crcErrorCount)++;
		}
      return IRC_FAILURE;
   }

   // Parse command payload
   return F1F2_PayloadParser(&buffer[F1F2_OFFSET_PAYLOAD_DATA], pdc, f1f2Cmd);
}

/**
 * F1F2 protocol command payload parser function.
 *
 * @param p_payload is a pointer to the command payload data.
 * @param pdc is the payload data count in bytes.
 * @param f1f2Cmd is the pointer to the F1F2 command structure to fill using payload data.
 *
 * @return IRC_SUCCESS if successfully parsed
 * @return IRC_FAILURE if failed to parse.
 */
IRC_Status_t F1F2_PayloadParser(uint8_t *p_payload, uint16_t pdc, F1F2Command_t *f1f2Cmd)
{
   switch (f1f2Cmd->cmd)
   {
      case F1F2_CMD_ACK:
      case F1F2_CMD_NAK:
         if (pdc != F1F2_CMD_CODE_SIZE)
         {
            F1F2_ERR("Invalid Payload Data Count (cmd = %d).", f1f2Cmd->cmd);
            return IRC_FAILURE;
         }
         f1f2Cmd->payload.ack.cmd = p_payload[F1F2_PD_OFFSET_CMD];
         break;

      case F1F2_CMD_REG_READ_REQ:
         if (pdc != F1F2_REG_ADDR_SIZE)
         {
            F1F2_ERR("Invalid Payload Data Count (cmd = %d).", f1f2Cmd->cmd);
            return IRC_FAILURE;
         }
         memcpy(&f1f2Cmd->payload.regRW.address, &p_payload[F1F2_PD_OFFSET_REG_ADDR], F1F2_REG_ADDR_SIZE);
         break;

      case F1F2_CMD_REG_READ_RSP:
      case F1F2_CMD_REG_WRITE:
         if (pdc <= F1F2_REG_ADDR_SIZE)
         {
            F1F2_ERR("Invalid Payload Data Count (cmd = %d).", f1f2Cmd->cmd);
            return IRC_FAILURE;
         }
         memcpy(&f1f2Cmd->payload.regRW.address, &p_payload[F1F2_PD_OFFSET_REG_ADDR], F1F2_REG_ADDR_SIZE);
         f1f2Cmd->payload.regRW.dataLength = pdc - F1F2_REG_ADDR_SIZE;
         memcpy(f1f2Cmd->payload.regRW.data, &p_payload[F1F2_PD_OFFSET_REG_VALUE], f1f2Cmd->payload.regRW.dataLength);
         f1f2Cmd->payload.regRW.padLength = 0;
         break;

      case F1F2_CMD_FILE_COUNT_REQ:
         if (pdc != 0)
         {
            F1F2_ERR("Invalid Payload Data Count (cmd = %d).", f1f2Cmd->cmd);
            return IRC_FAILURE;
         }
         break;

      case F1F2_CMD_FILE_COUNT_RSP:
         if (pdc != F1F2_FILE_COUNT_SIZE)
         {
            F1F2_ERR("Invalid Payload Data Count (cmd = %d).", f1f2Cmd->cmd);
            return IRC_FAILURE;
         }
         memcpy(&f1f2Cmd->payload.fileCount.count, &p_payload[F1F2_PD_OFFSET_FILE_COUNT], F1F2_FILE_COUNT_SIZE);
         break;

      case F1F2_CMD_FILE_INFO_REQ:
      case F1F2_CMD_FILE_CREATE_RSP:
      case F1F2_CMD_FILE_CLOSE:
      case F1F2_CMD_FILE_CHECK_REQ:
      case F1F2_CMD_FILE_DELETE:
         if (pdc != F1F2_FILE_INDEX_SIZE)
         {
            F1F2_ERR("Invalid Payload Data Count (cmd = %d).", f1f2Cmd->cmd);
            return IRC_FAILURE;
         }
         memcpy(&f1f2Cmd->payload.fileIndex.index, &p_payload[F1F2_PD_OFFSET_FILE_INDEX], F1F2_FILE_INDEX_SIZE);
         break;

      case F1F2_CMD_FILE_INFO_RSP:
         if (pdc != F1F2_FILE_INFO_SIZE)
         {
            F1F2_ERR("Invalid Payload Data Count (cmd = %d).", f1f2Cmd->cmd);
            return IRC_FAILURE;
         }
         memcpy(&f1f2Cmd->payload.fileInfo.index, &p_payload[F1F2_PD_OFFSET_FILE_INDEX], F1F2_FILE_INDEX_SIZE);
         memcpy(&f1f2Cmd->payload.fileInfo.name, &p_payload[F1F2_PD_OFFSET_FILE_INF_NAME], F1F2_FILE_NAME_SIZE);
         f1f2Cmd->payload.fileInfo.name[F1F2_FILE_NAME_SIZE] = '\0';
         memcpy(&f1f2Cmd->payload.fileInfo.size, &p_payload[F1F2_PD_OFFSET_FILE_INF_SIZE], F1F2_FILE_SIZE_SIZE);
         memcpy(&f1f2Cmd->payload.fileInfo.attributes, &p_payload[F1F2_PD_OFFSET_FILE_INF_ATTR], F1F2_FILE_ATTR_SIZE);
         memcpy(&f1f2Cmd->payload.fileInfo.id, &p_payload[F1F2_PD_OFFSET_FILE_INF_ID], F1F2_FILE_ID_SIZE);
         memcpy(&f1f2Cmd->payload.fileInfo.type, &p_payload[F1F2_PD_OFFSET_FILE_INF_TYPE], F1F2_FILE_TYPE_SIZE);
         f1f2Cmd->payload.fileInfo.type[F1F2_FILE_TYPE_SIZE] = '\0';
         break;

      case F1F2_CMD_FILE_CREATE_REQ:
         if (pdc != F1F2_FILE_NAME_SIZE)
         {
            F1F2_ERR("Invalid Payload Data Count (cmd = %d).", f1f2Cmd->cmd);
            return IRC_FAILURE;
         }
         memcpy(f1f2Cmd->payload.fileName.name, &p_payload[F1F2_PD_OFFSET_FILE_NAME], F1F2_FILE_NAME_SIZE);
         f1f2Cmd->payload.fileName.name[F1F2_FILE_NAME_SIZE] = '\0';
         break;

      case F1F2_CMD_FILE_READ_REQ:
         if (pdc != (F1F2_FILE_INDEX_SIZE + F1F2_FILE_OFFSET_SIZE + F1F2_FILE_LENGTH_SIZE))
         {
            F1F2_ERR("Invalid Payload Data Count (cmd = %d).", f1f2Cmd->cmd);
            return IRC_FAILURE;
         }
         memcpy(&f1f2Cmd->payload.fileRW.index, &p_payload[F1F2_PD_OFFSET_FILE_INDEX], F1F2_FILE_INDEX_SIZE);
         memcpy(&f1f2Cmd->payload.fileRW.offset, &p_payload[F1F2_PD_OFFSET_FILE_OFFSET], F1F2_FILE_OFFSET_SIZE);
         memcpy(&f1f2Cmd->payload.fileRW.dataLength, &p_payload[F1F2_PD_OFFSET_FILE_LENGTH], F1F2_FILE_LENGTH_SIZE);
         break;

      case F1F2_CMD_FILE_READ_RSP:
      case F1F2_CMD_FILE_WRITE:
         if (pdc <= (F1F2_FILE_INDEX_SIZE + F1F2_FILE_OFFSET_SIZE + F1F2_FILE_LENGTH_SIZE))
         {
            F1F2_ERR("Invalid Payload Data Count (cmd = %d).", f1f2Cmd->cmd);
            return IRC_FAILURE;
         }
         memcpy(&f1f2Cmd->payload.fileRW.index, &p_payload[F1F2_PD_OFFSET_FILE_INDEX], F1F2_FILE_INDEX_SIZE);
         memcpy(&f1f2Cmd->payload.fileRW.offset, &p_payload[F1F2_PD_OFFSET_FILE_OFFSET], F1F2_FILE_OFFSET_SIZE);
         memcpy(&f1f2Cmd->payload.fileRW.dataLength, &p_payload[F1F2_PD_OFFSET_FILE_LENGTH], F1F2_FILE_LENGTH_SIZE);
         if (pdc != (F1F2_FILE_INDEX_SIZE + F1F2_FILE_OFFSET_SIZE + F1F2_FILE_LENGTH_SIZE + f1f2Cmd->payload.fileRW.dataLength))
         {
            F1F2_ERR("Invalid Payload Data Count (cmd = %d).", f1f2Cmd->cmd);
            return IRC_FAILURE;
         }
         memcpy(f1f2Cmd->payload.fileRW.data, &p_payload[F1F2_PD_OFFSET_FILE_DATA], f1f2Cmd->payload.fileRW.dataLength);
         break;

      case F1F2_CMD_FILE_CHECK_RSP:
         if (pdc < (F1F2_FILE_INDEX_SIZE + F1F2_FILE_CRC16_SIZE))
         {
            F1F2_ERR("Invalid Payload Data Count (cmd = %d).", f1f2Cmd->cmd);
            return IRC_FAILURE;
         }
         memcpy(&f1f2Cmd->payload.fileCheck.index, &p_payload[F1F2_PD_OFFSET_FILE_INDEX], F1F2_FILE_INDEX_SIZE);
         memcpy(&f1f2Cmd->payload.fileCheck.crc16, &p_payload[F1F2_PD_OFFSET_FILE_CRC16], F1F2_FILE_CRC16_SIZE);
         break;

      case F1F2_CMD_PROM_ERASE:
         if (pdc != (F1F2_PROM_DEVICE_SIZE + F1F2_PROM_OFFSET_SIZE + F1F2_PROM_LENGTH_SIZE))
         {
            F1F2_ERR("Invalid Payload Data Count (cmd = %d).", f1f2Cmd->cmd);
            return IRC_FAILURE;
         }
         f1f2Cmd->payload.promErase.device = p_payload[F1F2_PD_OFFSET_PROM_DEVICE];
         memcpy(&f1f2Cmd->payload.promErase.offset, &p_payload[F1F2_PD_OFFSET_PROM_OFFSET], F1F2_PROM_OFFSET_SIZE);
         memcpy(&f1f2Cmd->payload.promErase.dataLength, &p_payload[F1F2_PD_OFFSET_PROM_LENGTH], F1F2_PROM_LENGTH_SIZE);
         break;

      case F1F2_CMD_PROM_READ_REQ:
         if (pdc != (F1F2_PROM_DEVICE_SIZE + F1F2_PROM_OFFSET_SIZE + F1F2_PROM_RW_LENGTH_SIZE))
         {
            F1F2_ERR("Invalid Payload Data Count (cmd = %d).", f1f2Cmd->cmd);
            return IRC_FAILURE;
         }
         f1f2Cmd->payload.promRW.device = p_payload[F1F2_PD_OFFSET_PROM_DEVICE];
         memcpy(&f1f2Cmd->payload.promRW.offset, &p_payload[F1F2_PD_OFFSET_PROM_OFFSET], F1F2_PROM_OFFSET_SIZE);
         memcpy(&f1f2Cmd->payload.promRW.dataLength, &p_payload[F1F2_PD_OFFSET_PROM_RW_LENGTH], F1F2_PROM_RW_LENGTH_SIZE);
         break;

      case F1F2_CMD_PROM_READ_RSP:
      case F1F2_CMD_PROM_WRITE:
         if (pdc <= (F1F2_PROM_DEVICE_SIZE + F1F2_PROM_OFFSET_SIZE + F1F2_PROM_RW_LENGTH_SIZE))
         {
            F1F2_ERR("Invalid Payload Data Count (cmd = %d).", f1f2Cmd->cmd);
            return IRC_FAILURE;
         }
         f1f2Cmd->payload.promRW.device = p_payload[F1F2_PD_OFFSET_PROM_DEVICE];
         memcpy(&f1f2Cmd->payload.promRW.offset, &p_payload[F1F2_PD_OFFSET_PROM_OFFSET], F1F2_PROM_OFFSET_SIZE);
         memcpy(&f1f2Cmd->payload.promRW.dataLength, &p_payload[F1F2_PD_OFFSET_PROM_RW_LENGTH], F1F2_PROM_RW_LENGTH_SIZE);
         if (pdc != (F1F2_PROM_DEVICE_SIZE + F1F2_PROM_OFFSET_SIZE + F1F2_PROM_RW_LENGTH_SIZE + f1f2Cmd->payload.promRW.dataLength))
         {
            F1F2_ERR("Invalid Payload Data Count (cmd = %d).", f1f2Cmd->cmd);
            return IRC_FAILURE;
         }
         memcpy(f1f2Cmd->payload.promRW.data, &p_payload[F1F2_PD_OFFSET_PROM_DATA], f1f2Cmd->payload.promRW.dataLength);
         break;

      case F1F2_CMD_PROM_CHECK_REQ:
         if (pdc != (F1F2_PROM_DEVICE_SIZE + F1F2_PROM_OFFSET_SIZE + F1F2_PROM_LENGTH_SIZE))
         {
            F1F2_ERR("Invalid Payload Data Count (cmd = %d).", f1f2Cmd->cmd);
            return IRC_FAILURE;
         }
         f1f2Cmd->payload.promCheck.device = p_payload[F1F2_PD_OFFSET_PROM_DEVICE];
         memcpy(&f1f2Cmd->payload.promCheck.offset, &p_payload[F1F2_PD_OFFSET_PROM_OFFSET], F1F2_PROM_OFFSET_SIZE);
         memcpy(&f1f2Cmd->payload.promCheck.dataLength, &p_payload[F1F2_PD_OFFSET_PROM_LENGTH], F1F2_PROM_LENGTH_SIZE);
         break;

      case F1F2_CMD_PROM_CHECK_RSP:
         if (pdc != (F1F2_PROM_DEVICE_SIZE + F1F2_PROM_OFFSET_SIZE + F1F2_PROM_LENGTH_SIZE + F1F2_PROM_CRC16_SIZE))
         {
            F1F2_ERR("Invalid Payload Data Count (cmd = %d).", f1f2Cmd->cmd);
            return IRC_FAILURE;
         }
         f1f2Cmd->payload.promCheck.device = p_payload[F1F2_PD_OFFSET_PROM_DEVICE];
         memcpy(&f1f2Cmd->payload.promCheck.offset, &p_payload[F1F2_PD_OFFSET_PROM_OFFSET], F1F2_PROM_OFFSET_SIZE);
         memcpy(&f1f2Cmd->payload.promCheck.dataLength, &p_payload[F1F2_PD_OFFSET_PROM_LENGTH], F1F2_PROM_LENGTH_SIZE);
         memcpy(&f1f2Cmd->payload.promCheck.crc16, &p_payload[F1F2_PD_OFFSET_PROM_CRC16], F1F2_PROM_CRC16_SIZE);
         break;

      case F1F2_CMD_PING:
      case F1F2_CMD_FILE_FORMAT:
	  case F1F2_CMD_FILE_USED_SPACE_REQ:
	  case F1F2_CMD_FILE_FREE_SPACE_REQ:
	  case F1F2_CMD_FILE_TOTAL_SPACE_REQ:
         if (pdc != 0)
         {
            F1F2_ERR("Invalid Payload Data Count (cmd = %d).", f1f2Cmd->cmd);
            return IRC_FAILURE;
         }
         break;

	  case F1F2_CMD_FILE_USED_SPACE_RSP:
	  case F1F2_CMD_FILE_FREE_SPACE_RSP:
	  case F1F2_CMD_FILE_TOTAL_SPACE_RSP:
		  if (pdc != F1F2_FILE_SPACE_SIZE)
		  {
			  F1F2_ERR("Invalid Payload Data Count (cmd = %d).", f1f2Cmd->cmd);
			  return IRC_FAILURE;
		  }
		  memcpy(&f1f2Cmd->payload.fileSpace.space, &p_payload[F1F2_PD_OFFSET_FILE_SPACE], F1F2_FILE_SPACE_SIZE);
		  break;

      case F1F2_CMD_NETWORK:
         if (pdc < F1F2_MIN_NET_PAYLOADDATACOUNT)
         {
            F1F2_ERR("Invalid Payload Data Count (cmd = %d).", f1f2Cmd->cmd);
            return IRC_FAILURE;
         }
         f1f2Cmd->isNetwork = 1;
         f1f2Cmd->srcAddr = p_payload[F1F2_PD_OFFSET_NET_SRC_ADDR];
         f1f2Cmd->srcPort = p_payload[F1F2_PD_OFFSET_NET_SRC_PORT];
         f1f2Cmd->destAddr = p_payload[F1F2_PD_OFFSET_NET_DEST_ADDR];
         f1f2Cmd->destPort = p_payload[F1F2_PD_OFFSET_NET_DEST_PORT];
         f1f2Cmd->cmd = p_payload[F1F2_PD_OFFSET_NET_CMD];

         return F1F2_PayloadParser(&p_payload[F1F2_PD_OFFSET_NET_PAYLOAD_DATA],
               pdc - F1F2_MIN_NET_PAYLOADDATACOUNT, f1f2Cmd);
         break;

      default:
         // Unknown command
         F1F2_ERR("Unknown command code (cmd = %d).", f1f2Cmd->cmd);
         return IRC_FAILURE;
   }

   return IRC_SUCCESS;
}

/**
 * Build response to the specified request.
 *
 * @param p_request is the pointer to the request command.
 * @param p_response is the pointer to the response command to fill.
 */
void F1F2_BuildResponse(F1F2Command_t *p_request, F1F2Command_t *p_response)
{
   uint8_t destAddr;
   uint8_t destPort;

   if (p_request->isNetwork == 1)
   {
      // Save request destination address and port in case p_response = p_request
      destAddr = p_request->destAddr;
      destPort = p_request->destPort;

      p_response->isNetwork = 1;
      p_response->destAddr = p_request->srcAddr;
      p_response->destPort = p_request->srcPort;
      p_response->srcAddr = destAddr;
      p_response->srcPort = destPort;
   }
}

/**
 * Build ACK response to the specified request.
 *
 * @param p_request is the pointer to the request command.
 * @param p_response is the pointer to the response command to fill.
 */
void F1F2_BuildACKResponse(F1F2Command_t *p_request, F1F2Command_t *p_response)
{
   F1F2_BuildResponse(p_request, p_response);

   // Fill acknowledged command first in case p_response = p_request
   p_response->payload.ack.cmd = p_request->cmd;
   p_response->cmd = F1F2_CMD_ACK;
}

/**
 * Build NAK response to the specified request.
 *
 * @param p_request is the pointer to the request command.
 * @param p_response is the pointer to the response command to fill.
 */
void F1F2_BuildNAKResponse(F1F2Command_t *p_request, F1F2Command_t *p_response)
{
   F1F2_BuildResponse(p_request, p_response);

   // Fill acknowledged command first in case p_response = p_request
   p_response->payload.ack.cmd = p_request->cmd;
   p_response->cmd = F1F2_CMD_NAK;
}

/**
 * F1F2 protocol command builder function.
 *
 * @param f1f2Cmd is the pointer to the F1F2 command structure that is going to be used to generate transmitted data.
 * @param buffer is a pointer to the command raw data buffer that will be transmitted.
 * @param buflen is the maximum length that can be written into buffer.
 *
 * @return The command length to be transmitted.
 */
uint32_t F1F2_CommandBuilder(F1F2Command_t *f1f2Cmd, uint8_t *buffer, uint16_t buflen)
{
   F1F2PayloadData_t payloadData[11];
   uint16_t dataCount = 0;
   uint8_t cmd = f1f2Cmd->cmd;

   if (f1f2Cmd->isNetwork == 1)
   {
      cmd = F1F2_CMD_NETWORK;
      payloadData[dataCount].p_data = &f1f2Cmd->srcAddr;
      payloadData[dataCount].dataLength = F1F2_NET_SRC_ADDR_SIZE;
      payloadData[dataCount].padLength = 0;
      dataCount++;

      payloadData[dataCount].p_data = &f1f2Cmd->srcPort;
      payloadData[dataCount].dataLength = F1F2_NET_SRC_PORT_SIZE;
      payloadData[dataCount].padLength = 0;
      dataCount++;

      payloadData[dataCount].p_data = &f1f2Cmd->destAddr;
      payloadData[dataCount].dataLength = F1F2_NET_DEST_ADDR_SIZE;
      payloadData[dataCount].padLength = 0;
      dataCount++;

      payloadData[dataCount].p_data = &f1f2Cmd->destPort;
      payloadData[dataCount].dataLength = F1F2_NET_DEST_PORT_SIZE;
      payloadData[dataCount].padLength = 0;
      dataCount++;

      payloadData[dataCount].p_data = &f1f2Cmd->cmd;
      payloadData[dataCount].dataLength = F1F2_CMD_CODE_SIZE;
      payloadData[dataCount].padLength = 0;
      dataCount++;
   }

   switch (f1f2Cmd->cmd)
   {
      case F1F2_CMD_ACK:
      case F1F2_CMD_NAK:
         payloadData[dataCount].p_data = &f1f2Cmd->payload.ack.cmd;
         payloadData[dataCount].dataLength = F1F2_CMD_CODE_SIZE;
         payloadData[dataCount].padLength = 0;
         dataCount++;
         break;

      case F1F2_CMD_REG_READ_REQ:
         payloadData[dataCount].p_data = &f1f2Cmd->payload.regRW.address;
         payloadData[dataCount].dataLength = F1F2_REG_ADDR_SIZE;
         payloadData[dataCount].padLength = 0;
         dataCount++;
         break;

      case F1F2_CMD_REG_READ_RSP:
      case F1F2_CMD_REG_WRITE:
         payloadData[dataCount].p_data = &f1f2Cmd->payload.regRW.address;
         payloadData[dataCount].dataLength = F1F2_REG_ADDR_SIZE;
         payloadData[dataCount].padLength = 0;
         dataCount++;

         payloadData[dataCount].p_data = f1f2Cmd->payload.regRW.data;
         payloadData[dataCount].dataLength = f1f2Cmd->payload.regRW.dataLength;
         payloadData[dataCount].padLength = f1f2Cmd->payload.regRW.padLength;
         dataCount++;
         break;

      case F1F2_CMD_FILE_COUNT_REQ:
         break;

      case F1F2_CMD_FILE_COUNT_RSP:
         payloadData[dataCount].p_data = &f1f2Cmd->payload.fileCount.count;
         payloadData[dataCount].dataLength = F1F2_FILE_COUNT_SIZE;
         payloadData[dataCount].padLength = 0;
         dataCount++;
         break;

      case F1F2_CMD_FILE_INFO_REQ:
      case F1F2_CMD_FILE_CREATE_RSP:
      case F1F2_CMD_FILE_CLOSE:
      case F1F2_CMD_FILE_CHECK_REQ:
      case F1F2_CMD_FILE_DELETE:
         payloadData[dataCount].p_data = &f1f2Cmd->payload.fileIndex.index;
         payloadData[dataCount].dataLength = F1F2_FILE_INDEX_SIZE;
         payloadData[dataCount].padLength = 0;
         dataCount++;
         break;

      case F1F2_CMD_FILE_INFO_RSP:
         payloadData[dataCount].p_data = &f1f2Cmd->payload.fileInfo.index;
         payloadData[dataCount].dataLength = F1F2_FILE_INDEX_SIZE;
         payloadData[dataCount].padLength = 0;
         dataCount++;

         payloadData[dataCount].p_data = f1f2Cmd->payload.fileInfo.name;
         payloadData[dataCount].dataLength = F1F2_FILE_NAME_SIZE;
         payloadData[dataCount].padLength = 0;
         dataCount++;

         payloadData[dataCount].p_data = &f1f2Cmd->payload.fileInfo.size;
         payloadData[dataCount].dataLength = F1F2_FILE_SIZE_SIZE;
         payloadData[dataCount].padLength = 0;
         dataCount++;

         payloadData[dataCount].p_data = &f1f2Cmd->payload.fileInfo.attributes;
         payloadData[dataCount].dataLength = F1F2_FILE_ATTR_SIZE;
         payloadData[dataCount].padLength = 0;
         dataCount++;

         payloadData[dataCount].p_data = &f1f2Cmd->payload.fileInfo.id;
         payloadData[dataCount].dataLength = F1F2_FILE_ID_SIZE;
         payloadData[dataCount].padLength = 0;
         dataCount++;

         payloadData[dataCount].p_data = &f1f2Cmd->payload.fileInfo.type;
         payloadData[dataCount].dataLength = F1F2_FILE_TYPE_SIZE;
         payloadData[dataCount].padLength = 0;
         dataCount++;
         break;

      case F1F2_CMD_FILE_CREATE_REQ:
         payloadData[dataCount].p_data = f1f2Cmd->payload.fileName.name;
         payloadData[dataCount].dataLength = F1F2_FILE_NAME_SIZE;
         payloadData[dataCount].padLength = 0;
         dataCount++;
         break;

      case F1F2_CMD_FILE_READ_REQ:
         payloadData[dataCount].p_data = &f1f2Cmd->payload.fileRW.index;
         payloadData[dataCount].dataLength = F1F2_FILE_INDEX_SIZE;
         payloadData[dataCount].padLength = 0;
         dataCount++;

         payloadData[dataCount].p_data = &f1f2Cmd->payload.fileRW.offset;
         payloadData[dataCount].dataLength = F1F2_FILE_OFFSET_SIZE;
         payloadData[dataCount].padLength = 0;
         dataCount++;

         payloadData[dataCount].p_data = &f1f2Cmd->payload.fileRW.dataLength;
         payloadData[dataCount].dataLength = F1F2_FILE_LENGTH_SIZE;
         payloadData[dataCount].padLength = 0;
         dataCount++;
         break;

      case F1F2_CMD_FILE_READ_RSP:
      case F1F2_CMD_FILE_WRITE:
         payloadData[dataCount].p_data = &f1f2Cmd->payload.fileRW.index;
         payloadData[dataCount].dataLength = F1F2_FILE_INDEX_SIZE;
         payloadData[dataCount].padLength = 0;
         dataCount++;

         payloadData[dataCount].p_data = &f1f2Cmd->payload.fileRW.offset;
         payloadData[dataCount].dataLength = F1F2_FILE_OFFSET_SIZE;
         payloadData[dataCount].padLength = 0;
         dataCount++;

         payloadData[dataCount].p_data = &f1f2Cmd->payload.fileRW.dataLength;
         payloadData[dataCount].dataLength = F1F2_FILE_LENGTH_SIZE;
         payloadData[dataCount].padLength = 0;
         dataCount++;

         payloadData[dataCount].p_data = f1f2Cmd->payload.fileRW.data;
         payloadData[dataCount].dataLength = f1f2Cmd->payload.fileRW.dataLength;
         payloadData[dataCount].padLength = 0;
         dataCount++;
         break;

      case F1F2_CMD_FILE_CHECK_RSP:
         payloadData[dataCount].p_data = &f1f2Cmd->payload.fileCheck.index;
         payloadData[dataCount].dataLength = F1F2_FILE_INDEX_SIZE;
         payloadData[dataCount].padLength = 0;
         dataCount++;

         payloadData[dataCount].p_data = &f1f2Cmd->payload.fileCheck.crc16;
         payloadData[dataCount].dataLength = F1F2_FILE_CRC16_SIZE;
         payloadData[dataCount].padLength = 0;
         dataCount++;
         break;

      case F1F2_CMD_PROM_ERASE:
         payloadData[dataCount].p_data = &f1f2Cmd->payload.promErase.device;
         payloadData[dataCount].dataLength = F1F2_PROM_DEVICE_SIZE;
         payloadData[dataCount].padLength = 0;
         dataCount++;

         payloadData[dataCount].p_data = &f1f2Cmd->payload.promErase.offset;
         payloadData[dataCount].dataLength = F1F2_PROM_OFFSET_SIZE;
         payloadData[dataCount].padLength = 0;
         dataCount++;

         payloadData[dataCount].p_data = &f1f2Cmd->payload.promErase.dataLength;
         payloadData[dataCount].dataLength = F1F2_PROM_LENGTH_SIZE;
         payloadData[dataCount].padLength = 0;
         dataCount++;
         break;

      case F1F2_CMD_PROM_READ_REQ:
         payloadData[dataCount].p_data = &f1f2Cmd->payload.promRW.device;
         payloadData[dataCount].dataLength = F1F2_PROM_DEVICE_SIZE;
         payloadData[dataCount].padLength = 0;
         dataCount++;

         payloadData[dataCount].p_data = &f1f2Cmd->payload.promRW.offset;
         payloadData[dataCount].dataLength = F1F2_PROM_OFFSET_SIZE;
         payloadData[dataCount].padLength = 0;
         dataCount++;

         payloadData[dataCount].p_data = &f1f2Cmd->payload.promRW.dataLength;
         payloadData[dataCount].dataLength = F1F2_PROM_RW_LENGTH_SIZE;
         payloadData[dataCount].padLength = 0;
         dataCount++;
         break;

      case F1F2_CMD_PROM_READ_RSP:
      case F1F2_CMD_PROM_WRITE:
         payloadData[dataCount].p_data = &f1f2Cmd->payload.promRW.device;
         payloadData[dataCount].dataLength = F1F2_PROM_DEVICE_SIZE;
         payloadData[dataCount].padLength = 0;
         dataCount++;

         payloadData[dataCount].p_data = &f1f2Cmd->payload.promRW.offset;
         payloadData[dataCount].dataLength = F1F2_PROM_OFFSET_SIZE;
         payloadData[dataCount].padLength = 0;
         dataCount++;

         payloadData[dataCount].p_data = &f1f2Cmd->payload.promRW.dataLength;
         payloadData[dataCount].dataLength = F1F2_PROM_RW_LENGTH_SIZE;
         payloadData[dataCount].padLength = 0;
         dataCount++;

         payloadData[dataCount].p_data = f1f2Cmd->payload.promRW.data;
         payloadData[dataCount].dataLength = f1f2Cmd->payload.promRW.dataLength;
         payloadData[dataCount].padLength = 0;
         dataCount++;
         break;

      case F1F2_CMD_PROM_CHECK_REQ:
         payloadData[dataCount].p_data = &f1f2Cmd->payload.promCheck.device;
         payloadData[dataCount].dataLength = F1F2_PROM_DEVICE_SIZE;
         payloadData[dataCount].padLength = 0;
         dataCount++;

         payloadData[dataCount].p_data = &f1f2Cmd->payload.promCheck.offset;
         payloadData[dataCount].dataLength = F1F2_PROM_OFFSET_SIZE;
         payloadData[dataCount].padLength = 0;
         dataCount++;

         payloadData[dataCount].p_data = &f1f2Cmd->payload.promCheck.dataLength;
         payloadData[dataCount].dataLength = F1F2_PROM_LENGTH_SIZE;
         payloadData[dataCount].padLength = 0;
         dataCount++;
         break;

      case F1F2_CMD_PROM_CHECK_RSP:
         payloadData[dataCount].p_data = &f1f2Cmd->payload.promCheck.device;
         payloadData[dataCount].dataLength = F1F2_PROM_DEVICE_SIZE;
         payloadData[dataCount].padLength = 0;
         dataCount++;

         payloadData[dataCount].p_data = &f1f2Cmd->payload.promCheck.offset;
         payloadData[dataCount].dataLength = F1F2_PROM_OFFSET_SIZE;
         payloadData[dataCount].padLength = 0;
         dataCount++;

         payloadData[dataCount].p_data = &f1f2Cmd->payload.promCheck.dataLength;
         payloadData[dataCount].dataLength = F1F2_PROM_LENGTH_SIZE;
         payloadData[dataCount].padLength = 0;
         dataCount++;

         payloadData[dataCount].p_data = &f1f2Cmd->payload.promCheck.crc16;
         payloadData[dataCount].dataLength = F1F2_PROM_CRC16_SIZE;
         payloadData[dataCount].padLength = 0;
         dataCount++;
         break;

      case F1F2_CMD_PING:
      case F1F2_CMD_FILE_FORMAT:
	  case F1F2_CMD_FILE_USED_SPACE_REQ:
	  case F1F2_CMD_FILE_FREE_SPACE_REQ:
	  case F1F2_CMD_FILE_TOTAL_SPACE_REQ:
         break;

	  case F1F2_CMD_FILE_USED_SPACE_RSP:
	  case F1F2_CMD_FILE_FREE_SPACE_RSP:
	  case F1F2_CMD_FILE_TOTAL_SPACE_RSP:
		  payloadData[dataCount].p_data = &f1f2Cmd->payload.fileSpace.space;
		  payloadData[dataCount].dataLength = F1F2_FILE_SPACE_SIZE;
		  payloadData[dataCount].padLength = 0;
		  dataCount++;
		  break;

      default:
         F1F2_ERR("Unknown command  (cmd = %d).", f1f2Cmd->cmd);
         // Unknown command
         return 0;
   }

   return F1F2_FrameBuilder(buffer, buflen, cmd, payloadData, dataCount);
}

/**
 * F1F2 protocol frame builder function.
 *
 * @param buffer is a pointer to the command raw data buffer that will be transmitted.
 * @param buflen is the maximum length that can be written into buffer.
 * @param cmd is the F1F2 command code.
 * @param payloadData is the pointer to payload data array.
 * @param payloadDataCount is the number of payload data.
 *
 * @return The command length to be transmitted.
 */
uint32_t F1F2_FrameBuilder(uint8_t *buffer, uint16_t buflen, uint8_t cmd, F1F2PayloadData_t *payloadData, uint16_t payloadDataCount)
{
   uint16_t pdc = 0;    // Payload Data Count
   uint16_t crc16;      // CRC-16 value
   uint16_t i;

   // Write payload data into byte buffer
   pdc = 0;
   for (i = 0; i < payloadDataCount; i++)
   {
      if ((F1F2_MIN_PACKET_SIZE + pdc + payloadData[i].dataLength + payloadData[i].padLength) > buflen)
      {
         return 0;
      }
      memcpy(&buffer[F1F2_OFFSET_PAYLOAD_DATA + pdc], payloadData[i].p_data, payloadData[i].dataLength);
      pdc += payloadData[i].dataLength;
      memset(&buffer[F1F2_OFFSET_PAYLOAD_DATA + pdc], 0, payloadData[i].padLength);
      pdc += payloadData[i].padLength;
   }

   // Write frame data into byte buffer
   buffer[F1F2_OFFSET_SOP] = F1F2_SOP;
   buffer[F1F2_OFFSET_CMD] = cmd;
   memcpy(&buffer[F1F2_OFFSET_PDC], &pdc, sizeof(pdc));
   crc16 = CRC16(0xFFFF, buffer, F1F2_OFFSET_PAYLOAD_DATA + pdc);
   memcpy(&buffer[F1F2_OFFSET_PAYLOAD_DATA + pdc], &crc16, sizeof(crc16));
   buffer[F1F2_OFFSET_PAYLOAD_DATA + pdc + F1F2_CRC16_SIZE] = F1F2_EOP;

   return (F1F2_MIN_PACKET_SIZE + pdc);
}
