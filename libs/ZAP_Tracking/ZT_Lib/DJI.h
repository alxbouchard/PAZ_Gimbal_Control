
// Author  KMS - Martin Dubois, P.Eng.
// Client  ZAP
// Product Tracking
// File    ZT_Lib/DJI.h

#pragma once

// Constants
/////////////////////////////////////////////////////////////////////////////

#define DJI_FOOTER_SIZE_byte (4)
#define DJI_HEADER_SIZE_byte (12)

#define DJI_SOF (0xaa)

// CAN ID

#define DJI_CAN_ID_RX (0x222)
#define DJI_CAN_ID_TX (0x223)

// Default command set

#define DJI_CMD_SET_DEFAULT (0x0e)

#define DJI_CMD_POSITION_SET        (0x00)
#define DJI_CMD_SPEED_SET           (0x01)
#define DJI_CMD_ANGLE_GET           (0x02)
#define DJI_CMD_ANGLE_LIMIT_SET     (0x03)
#define DJI_CMD_ANGLE_LIMIT_GET     (0x04)
#define DJI_CMD_MOTOR_STIFFNESS_SET (0x05)
#define DJI_CMD_MOTOR_STIFFNESS_GET (0x06)
#define DJI_CMD_VERSION             (0x09)
#define DJI_CMD_CONTROL             (0x0a)
#define DJI_CMD_TLV_SET             (0x0c)
#define DJI_CMD_CALIBRATION         (0x0f)
#define DJI_CMD_TRACK_SWITCH        (0x11)
#define DJI_CMD_FOCUS               (0x12)

#define DJI_CMD_FOCUS_SET (0x01)
#define DJI_CMD_FOCUS_CAL (0x02)

// Thrird party command set

#define DJI_CMD_SET_THIRD_PARTY (0x0d)

#define DJI_CMD_MOTION     (0x00)
#define DJI_CMD_STATUS_GET (0x01)

// Command type

#define DJI_CMD_TYPE_DO_REPLY (0x03)
#define DJI_CMD_TYPE_NO_REPLY (0x00)
#define DJI_CMD_TYPE_REPLY    (0x20)

// Result code
#define DJI_OK          (0x00)
#define DJI_ERROR_PARSE (0x01)
#define DJI_ERROR_FAIL  (0x02)

// Data offset

#define DJI_DATA_CMD_SET (0)
#define DJI_DATA_CMD_ID  (1)

#define DJI_REPLY_RESULT (2)

// Macros
/////////////////////////////////////////////////////////////////////////////

#define DJI_FRAME_TOTAL_SIZE(D) (DJI_HEADER_SIZE_byte + (D) + DJI_FOOTER_SIZE_byte)

// Class
/////////////////////////////////////////////////////////////////////////////

class DJI_Frame
{

public:

    void Init(uint8_t aDataSize_byte, uint8_t aCmdType, uint8_t aCmdSet, uint8_t aCmdId, uint16_t aSerial);

    void Seal();

    double Angle_Get(unsigned int aOffset_byte) const;
    void   Angle_Set(unsigned int aOffset_byte, double aAngle_deg);

    void Speed_Set(unsigned int aOffset_byte, double aSpeed_deg_s);

    void Display(FILE * aOut) const;

    uint8_t mSOF;
    uint8_t mSize_byte;
    uint8_t mVersion;
    uint8_t mCmdType;
    uint8_t mEncoded;

    uint8_t mReserved0[3];

    uint16_t mSerial;
    uint16_t mCRC16;

    uint8_t mData[16];

};
