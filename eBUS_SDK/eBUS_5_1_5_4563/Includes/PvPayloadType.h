// *****************************************************************************
//
//     Copyright (c) 2010, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVPAYLOADTYPE_H__
#define __PVPAYLOADTYPE_H__


typedef enum
{ 
    PvPayloadTypeUndefined = -1,
    PvPayloadTypeImage = 0x0001,
    PvPayloadTypeRawData = 0x0002,
    PvPayloadTypeFile = 0x0003,
    PvPayloadTypeChunkData = 0x0004,
	PvPayloadTypeExtendedChunkData = 0x0005,
    PvPayloadTypeJPEG = 0x0006,
    PvPayloadTypeJPEG2000 = 0x0007,
    PvPayloadTypeH264 = 0x0008,
    PvPayloadTypeMultiZoneImage = 0x0009,
	PvPayloadTypeDeviceSpecificBase = 0x8000

} PvPayloadType;


#endif
