
// Author  KMS - Martin Dubois, P.Eng.
// Client  ZAP
// Product Tacking
// File    ZT_Lib/DJI_CRC.h

#pragma once

// Functions
/////////////////////////////////////////////////////////////////////////////

extern uint16_t DJI_CRC_16(const uint8_t * aIn);
extern uint32_t DJI_CRC_32(const uint8_t * aIn, unsigned int aSize_byte);
