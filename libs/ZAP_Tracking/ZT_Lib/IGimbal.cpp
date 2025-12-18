
// Author  KMS - Martin Dubois, P.Eng.
// Client  ZAP
// Product Tacking
// File    ZT_Lib/IGimbal.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <ZT/IGimbal.h>

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

static void Display_IPv4   (FILE * aOut, uint32_t aIn);
static void Display_Version(FILE * aOut, const uint8_t * aIn);

namespace ZT
{

    // Public
    // //////////////////////////////////////////////////////////////////////

    const double IGimbal::FOCUS_POSITION_MAX_pc = 100.0;
    const double IGimbal::FOCUS_POSITION_MIN_pc =   0.0;

    const double IGimbal::FOCUS_SPEED_MIN_pc_s  = - 100.0;
    const double IGimbal::FOCUS_SPEED_MAX_pc_s  =   100.0;
    const double IGimbal::FOCUS_SPEED_STOP_pc_s =     0.0;

    const double IGimbal::POSITION_MAX_deg =   180.5;
    const double IGimbal::POSITION_MIN_deg = - 180.5;

    const double IGimbal::SPEED_MAX_deg_s  =   360.0;
    const double IGimbal::SPEED_MIN_deg_s  = - 360.0;
    const double IGimbal::SPEED_STOP_deg_s =     0.0;

    void IGimbal::Display(void * aOut, Axis aIn)
    {
        FILE * lOut = (NULL == aOut) ? stdout : reinterpret_cast<FILE *>(aOut);

        switch (aIn)
        {
        case AXIS_PITCH: fprintf(lOut, "AXIS_PITCH\n"); break;
        case AXIS_ROLL : fprintf(lOut, "AXIS_ROLL\n" ); break;
        case AXIS_YAW  : fprintf(lOut, "AXIS_YAW\n"  ); break;

        default: fprintf(lOut, "Invalid Axis value (%u)\n", aIn);
        }
    }

    void IGimbal::Display(void * aOut, const Config & aIn)
    {
        FILE * lOut = (NULL == aOut) ? stdout : reinterpret_cast<FILE *>(aOut);

        for (unsigned int a = 0; a < AXIS_QTY; a++)
        {
            Display(lOut, static_cast<Axis>(a));
            Display(lOut, aIn.mAxis[a]);            
        }
    }

    void IGimbal::Display(void * aOut, const Config_Axis & aIn)
    {
        FILE * lOut = (NULL == aOut) ? stdout : reinterpret_cast<FILE *>(aOut);

        fprintf(lOut, "    Max       : %f deg\n"  , aIn.mMax_deg     );
        fprintf(lOut, "    Min       : %f deg\n"  , aIn.mMin_deg     );
        fprintf(lOut, "    Offset    : %f deg\n"  , aIn.mOffset_deg  );
        fprintf(lOut, "    Speed     : %f deg/s\n", aIn.mSpeed_deg_s );
        fprintf(lOut, "    Stiffness : %f %%\n"   , aIn.mStiffness_pc);
    }

    void IGimbal::Display(void * aOut, const Info & aIn)
    {
        FILE * lOut = (NULL == aOut) ? stdout : reinterpret_cast<FILE *>(aOut);

        fprintf(lOut, "Name         : %s\n", aIn.mName);
        fprintf(lOut, "IPv4 Address : "); Display_IPv4(lOut, aIn.mIPv4_Address);
        fprintf(lOut, "     Gateway : "); Display_IPv4(lOut, aIn.mIPv4_Gateway);
        fprintf(lOut, "     Netmask : "); Display_IPv4(lOut, aIn.mIPv4_Gateway);
        fprintf(lOut, "Version      : "); Display_Version(lOut, aIn.mVersion);

        for (unsigned int a = 0; a < AXIS_QTY; a++)
        {
            Display(lOut, static_cast<Axis>(a));
            Display(lOut, aIn.mAxis[a]);
        }
    }

    void IGimbal::Display(void * aOut, const Info_Axis & aIn)
    {
        FILE * lOut = (NULL == aOut) ? stdout : reinterpret_cast<FILE *>(aOut);

        fprintf(lOut, "    Speed Max. : %f deg/s\n", aIn.mSpeed_Max_deg_s);
    }

    void IGimbal::Display(void * aOut, Operation aIn)
    {
        FILE * lOut = (NULL == aOut) ? stdout : reinterpret_cast<FILE *>(aOut);

        switch (aIn)
        {
        case OPERATION_CAL_AUTO_ENABLE  : fprintf(lOut, "OPERATION_CAL_AUTO_ENABLE\n"  ); break;
        case OPERATION_CAL_MANUAL_ENABLE: fprintf(lOut, "OPERATION_CAL_MANUAL_ENABLE\n"); break;
        case OPERATION_CAL_SET_MAX      : fprintf(lOut, "OPERATION_CAL_SET_MAX\n"      ); break;
        case OPERATION_CAL_SET_MIN      : fprintf(lOut, "OPERATION_CAL_SET_MIN\n"      ); break;
        case OPERATION_CAL_STOP         : fprintf(lOut, "OPERATION_CAL_STOP\n"         ); break;

        default: fprintf(lOut, "Invalid Operation value (%u)\n", aIn);
        }
    }

    void IGimbal::Display(void * aOut, const Position & aIn)
    {
        FILE * lOut = (NULL == aOut) ? stdout : reinterpret_cast<FILE *>(aOut);

        for (unsigned int a = 0; a < AXIS_QTY; a++)
        {
            Display(lOut, static_cast<Axis>(a));
            fprintf(lOut, "    %f deg\n", aIn.mAxis_deg[a]);
        }
    }

    void IGimbal::Display(void * aOut, const Speed & aIn)
    {
        FILE * lOut = (NULL == aOut) ? stdout : reinterpret_cast<FILE *>(aOut);

        for (unsigned int a = 0; a < AXIS_QTY; a++)
        {
            Display(lOut, static_cast<Axis>(a));
            fprintf(lOut, "    %f deg/s\n", aIn.mAxis_deg_s[a]);
        }
    }

}

// Static functions
// //////////////////////////////////////////////////////////////////////////

void Display_IPv4(FILE * aOut, uint32_t aIn)
{
    assert(NULL != aOut);

    const uint8_t * lIn = reinterpret_cast<uint8_t *>(&aIn);

    fprintf(aOut, "%u.%u.%u.%u\n", lIn[0], lIn[1], lIn[2], lIn[3]);
}

void Display_Version(FILE * aOut, const uint8_t * aIn)
{
    assert(NULL != aOut);

    fprintf(aOut, "%u.%u.%u.%u\n", aIn[0], aIn[1], aIn[2], aIn[3]);
}
