
// Author  KMS - Martin Dubois, P.Eng.
// Client  ZAP
// Product Tracking
// File    ZT_Lib/DJI.cpp

#include "Component.h"

// ===== ZT_Lib =============================================================
#include "DJI_CRC.h"

#include "DJI.h"

// Public
/////////////////////////////////////////////////////////////////////////////

void DJI_Frame::Init(uint8_t aDataSize_byte, uint8_t aCmdType, uint8_t aCmdSet, uint8_t aCmdId, uint16_t aSerial)
{
    assert((DJI_CMD_SET_THIRD_PARTY == aCmdSet) || (DJI_CMD_SET_DEFAULT == aCmdSet));
    assert(0xff >= aCmdId);

    memset(this, 0, sizeof(DJI_Frame));

    mCmdType = aCmdType;
    mSerial = aSerial;
    mSize_byte = DJI_FRAME_TOTAL_SIZE(aDataSize_byte);
    mSOF = DJI_SOF;

    mData[DJI_DATA_CMD_SET] = aCmdSet;
    mData[DJI_DATA_CMD_ID ] = aCmdId;

    assert(sizeof(DJI_Frame) >= mSize_byte);

    mCRC16 = DJI_CRC_16(&mSOF);
}

void DJI_Frame::Seal()
{
    assert(sizeof(uint32_t) < mSize_byte);

    unsigned int lSize_byte = mSize_byte - sizeof(uint32_t);

    uint32_t lCRC_32 = DJI_CRC_32(&mSOF, lSize_byte);

    memcpy((&mSOF) + lSize_byte, &lCRC_32, sizeof(lCRC_32));
}

double DJI_Frame::Angle_Get(unsigned int aOffset_byte) const
{
    int16_t lAngle;
    
    lAngle  =  mData[aOffset_byte];
    lAngle |= (mData[aOffset_byte + 1] << 8);

    double lResult_deg = lAngle;

    lResult_deg /= 10.0;

    return lResult_deg;
}

void DJI_Frame::Angle_Set(unsigned int aOffset_byte, double aAngle_deg)
{
    assert(sizeof(mData) > aOffset_byte);

    int16_t lAngle = aAngle_deg * 10.0;

    mData[aOffset_byte    ] = lAngle & 0xff;
    mData[aOffset_byte + 1] = lAngle >> 8;
}

void DJI_Frame::Speed_Set(unsigned int aOffset_byte, double aSpeed_deg_s)
{
    assert(sizeof(mData) > aOffset_byte);

    int16_t lSpeed = aSpeed_deg_s * 10.0;

    mData[aOffset_byte    ] = lSpeed & 0xff;
    mData[aOffset_byte + 1] = lSpeed >> 8;
}

void DJI_Frame::Display(FILE * aOut) const
{
    assert(NULL != aOut);

    fprintf(aOut, "SOF      : 0x%02x\n"  , mSOF);
    fprintf(aOut, "Size     : %u bytes\n", mSize_byte);
    fprintf(aOut, "Version  : 0x%02x\n"  , mVersion);
    fprintf(aOut, "Cmd Type : 0x%02x\n"  , mCmdType);
    fprintf(aOut, "Encoded  : 0x%02x\n"  , mEncoded);
    fprintf(aOut, "Serial   : 0x%04x\n"  , mSerial);
    fprintf(aOut, "CRC 16   : 0x%04x\n"  , mCRC16);
    fprintf(aOut, "Data     :");

    unsigned int lDataSize_byte = mSize_byte - DJI_HEADER_SIZE_byte - DJI_FOOTER_SIZE_byte;
    for (unsigned int i = 0; i < lDataSize_byte; i++)
    {
        fprintf(aOut, " 0x%02x", mData[i]);
    }
    fprintf(aOut, "\n");

    uint32_t lCRC32;
    memcpy(&lCRC32, mData + lDataSize_byte, sizeof(lCRC32));

    fprintf(aOut, "CRC 32   : 0x%08x\n", lCRC32);
}
