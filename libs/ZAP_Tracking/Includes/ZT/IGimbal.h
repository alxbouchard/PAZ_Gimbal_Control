
// Author  KMS - Martin Dubois, P. Eng.
// Client  ZAP
// Product Tacking
// File    Includes/ZT/IGimbal.h

#pragma once

#include <ZT/IObject.h>
#include <ZT/Result.h>

#define ZT_FLAG_IGNORE_PITCH (0x01)
#define ZT_FLAG_IGNORE_ROLL  (0x02)
#define ZT_FLAG_IGNORE_YAW   (0x04)

#define ZT_FLAG_IGNORE_ALL   (0x07)

#define ZT_FLAG_IGNORE(A) (1<<(A))

namespace ZT
{

    class IGimbal : public IObject
    {

    public:

        typedef enum
        {
            AXIS_PITCH,
            AXIS_ROLL,
            AXIS_YAW,

            AXIS_QTY
        }
        Axis;

        typedef struct
        {
            double mMax_deg;
            double mMin_deg;
            double mOffset_deg;
            double mSpeed_deg_s;
            double mStiffness_pc;

            uint8_t mReserved0[24];
        }
        Config_Axis;

        typedef struct
        {
            Config_Axis mAxis[AXIS_QTY];

            uint8_t mReserved0[64];
        }
        Config;

        typedef struct
        {
            double mSpeed_Max_deg_s;

            uint8_t mReserved0[24];
        }
        Info_Axis;

        typedef struct
        {
            char mName[16];

            uint32_t mIPv4_Address;
            uint32_t mIPv4_Gateway;
            uint32_t mIPv4_NetMask;

            uint8_t mVersion[4];

            uint8_t mReserved0[32];

            Info_Axis mAxis[AXIS_QTY];

            uint8_t mReserved3[32];
        }
        Info;

        typedef enum
        {
            OPERATION_CAL_AUTO_ENABLE,
            OPERATION_CAL_MANUAL_ENABLE,
            OPERATION_CAL_SET_MAX,
            OPERATION_CAL_SET_MIN,
            OPERATION_CAL_STOP,

            OPERATION_QTY
        }
        Operation;

        typedef struct
        {
            double mAxis_deg[AXIS_QTY];
        }
        Position;

        typedef struct
        {
            double mAxis_deg_s[AXIS_QTY];
        }
        Speed;

        static const double POSITION_MAX_deg;
        static const double POSITION_MIN_deg;

        static const double FOCUS_POSITION_MAX_pc;
        static const double FOCUS_POSITION_MIN_pc;

        static const double FOCUS_SPEED_MAX_pc_s;
        static const double FOCUS_SPEED_MIN_pc_s;
        static const double FOCUS_SPEED_STOP_pc_s;

        static const double SPEED_MAX_deg_s;
        static const double SPEED_MIN_deg_s;
        static const double SPEED_STOP_deg_s;

        static void Display(void * aOut, Axis aIn);
        static void Display(void * aOut, const Config & aIn);
        static void Display(void * aOut, const Config_Axis & aIn);
        static void Display(void * aOut, const Info & aIn);
        static void Display(void * aOut, const Info_Axis & aIn);
        static void Display(void * aOut, Operation aIn);
        static void Display(void * aOut, const Position & aIn);
        static void Display(void * aOut, const Speed & aIn);

        virtual Result Activate() = 0;

        virtual void   Config_Get(Config * aOut) const = 0;
        virtual Result Config_Set(const Config & aIn) = 0;

        virtual Result Focus_Cal(Operation aOperation) = 0;
        virtual Result Focus_Position_Set(double aPosition_pc) = 0;
        virtual Result Focus_Speed_Set   (double aSpeed_pc_s) = 0;

        virtual void Info_Get(Info * aOut) const = 0;

        virtual Result Position_Get(Position * aOut) = 0;
        virtual Result Position_Set(const Position & aIn, unsigned int aFlags = 0, unsigned int aDuration_ms = 0) = 0;

        virtual Result Speed_Get(Speed * aOut) = 0;
        virtual Result Speed_Set(const Speed & aIn, unsigned int aFlags = 0) = 0;
        virtual Result Speed_Stop() = 0;

        virtual Result Track_Speed_Set(double aSpeed_pc) = 0;
        virtual Result Track_Switch() = 0;

        virtual void Debug(void * aOut = NULL) = 0;

    };

}
