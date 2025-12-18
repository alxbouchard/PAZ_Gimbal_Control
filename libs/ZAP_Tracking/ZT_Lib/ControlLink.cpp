
// Author  KMS - Martin Dubois, P. Eng.
// Client  ZAP
// Product Tacking
// File    ZT_Lib/ControlLink.cpp

// CODE REVIEW 2021-07-26 KMS - Martin Dubois, P.Eng.

#include "Component.h"

// ===== C ==================================================================
#include <unistd.h>

// ===== Includes ===========================================================
#include <ZT/ISystem.h>

// ===== ZT_Lib =============================================================
#include "Value.h"

#include "ControlLink.h"

// Constants
// //////////////////////////////////////////////////////////////////////////

static const double BOOST_AXIS[ZT::IGimbal::AXIS_QTY] = { 0.5, 0.0, 1.0 };

static const char * FUNCTION_NAMES[ControlLink::FUNCTION_QTY] =
{
    "ATEM_APERTURE_ABSOLUTE",
    "ATEM_APERTURE_AUTO",
    "ATEM_FOCUS_ABSOLUTE",
    "ATEM_FOCUS_AUTO",
    "ATEM_GAIN_ABSOLUTE",
    "ATEM_ZOOM",
    "ATEM_ZOOM_ABSOLUTE",
    "FOCUS",
    "FOCUS_ABSOLUTE",
    "FOCUS_CALIBRATION",
    "FORWARD",
    "GIMBAL_FIRST",
    "GIMBAL_LAST",
    "GIMBAL_NEXT",
    "GIMBAL_NEXT_LOOP",
    "GIMBAL_PREVIOUS",
    "GIMBAL_PREVIOUS_LOOP",
    "GIMBAL_SELECT",
    "HOME",
    "HOME_PITCH",
    "HOME_SET",
    "HOME_YAW",
    "PITCH",
    "PITCH_ABSOLUTE",
    "ROLL",
    "ROLL_ABSOLUTE",
    "SPEED_BOOST",
    "TRACK_SWITCH",
    "YAW",
    "YAW_ABSOLUTE",
    "ZOOM",
    "ZOOM_ABSOLUTE",
    "ZOOM_CALIBRATION",
};

#define FACTOR_MAX ( 360.0)
#define FACTOR_MIN (-360.0)

#define MSG_GAMEPAD (1)

#define OFFSET_MAX ( 180.0)
#define OFFSET_MIN (-180.0)

// Macros
// //////////////////////////////////////////////////////////////////////////

#define CURRENT_ATEM                             \
    assert(mGimbals.size() > mGimbalIndex);      \
    GimbalInfo & lInfo = mGimbals[mGimbalIndex]; \
    if (0 >= lInfo.mAtemPort) { return; }        \
    if (NULL == mAtem) { Error("No ATEM"); return; }

#define CURRENT_GIMBAL                           \
    assert(mGimbals.size() > mGimbalIndex);      \
    GimbalInfo & lInfo = mGimbals[mGimbalIndex]; \
    if (NULL == lInfo.mGimbal) { return; }

// Static fonction declarations
// //////////////////////////////////////////////////////////////////////////

static ZT::Result ActionFromName  (const char * aIn, ZT::IGamepad::Action  * aOut);
static ZT::Result ControlFromName (const char * aIn, ZT::IGamepad::Control * aOut);
static ZT::Result FunctionFromName(const char * aIn, ControlLink::Function * aOut);

static void Error(const char * aMsg);

static void VerifyResult(ZT::Result aResult, unsigned int aLine);

// Public
// //////////////////////////////////////////////////////////////////////////

ControlLink::ControlLink()
    : mAtem(NULL)
    , mGamepad(NULL)
    , mGimbalIndex(0)
    , mReceiver(NULL)
    , mReceiver_Configured(0)
    , mReceiver_Unknown   (0)
    , mRefCount(1)
    , mSpeedBoost(0.0)
{
    OnGimbalChanged();

    Table_Init();
}

// ===== ZT::IControlLink ===================================================

ZT::Result ControlLink::ReadConfigFile(const char * aFileName)
{
    assert(NULL != aFileName);

    FILE * lFile = fopen(aFileName, "rb");
    if (NULL == lFile)
    {
        return ZT::ZT_ERROR_FILE_OPEN;
    }

    char       lLine[1024];
    ZT::Result lResult = ZT::ZT_OK;

    while ((ZT::ZT_OK == lResult) && (NULL != fgets(lLine, sizeof(lLine), lFile)))
    {
        lResult = ParseConfigLine(lLine);
    }

    int lRet = fclose(lFile);
    assert(0 == lRet);

    return lResult;
}

ZT::Result ControlLink::Gamepad_Set(ZT::IGamepad * aGamepad)
{
    assert(NULL != aGamepad);

    mGamepad = aGamepad;

    return ZT::ZT_OK;
}

ZT::Result ControlLink::Gimbals_Set(ZT::ISystem * aSystem)
{
    ZT::Result lResult = ZT::ZT_OK;

    if (0 < mGimbalIds.size())
    {
        for (StringList::iterator lIt = mGimbalIds.begin(); (ZT::ZT_OK == lResult) && (lIt != mGimbalIds.end()); lIt ++)
        {
            lResult = Gimbal_Set(aSystem, lIt->c_str());
        }
    }
    else
    {
        lResult = Gimbal_Set(aSystem, "");
    }

    return lResult;
}

ZT::Result ControlLink::Receiver_Set(ZT::IMessageReceiver * aReceiver, unsigned int aConfigured, unsigned int aUnknown)
{
    if (NULL == aReceiver)
    {
        if ((0 != aConfigured) || (0 != aUnknown))
        {
            return ZT::ZT_ERROR_CODE;
        }
    }
    else
    {
        if ((0 == aConfigured) && (0 == aUnknown))
        {
            return ZT::ZT_ERROR_CODE;
        }
    }

    mReceiver = aReceiver;

    mReceiver_Configured = aConfigured;
    mReceiver_Unknown    = aUnknown;

    return ZT::ZT_OK;
}

ZT::Result ControlLink::Start()
{
    assert(NULL != mGamepad);

    ZT::Result lResult = ZT::ZT_OK;

    for (GimbalList::iterator lIt = mGimbals.begin(); (ZT::ZT_OK == lResult) && (lIt != mGimbals.end()); lIt++)
    {
        if (NULL != lIt->mGimbal)
        {
            lResult = lIt->mGimbal->Activate();
        }
    }

    if (ZT::ZT_OK == lResult)
    {
        lResult = mGamepad->Receiver_Start(this, MSG_GAMEPAD);
    }

    return lResult;
}

ZT::Result ControlLink::Stop()
{
    assert(NULL != mGamepad);

    return mGamepad->Receiver_Stop();
}

// ===== ZT::IObject ========================================================

void ControlLink::Release()
{
    if (1 == mRefCount)
    {
        delete this;
    }

    mRefCount --;
}

// ===== ZT::IMessageReceiver ===========================================
    
bool ControlLink::ProcessMessage(void * aSender, unsigned int aCode, const void * aData)
{
    assert(NULL != aData);

    bool lResult = false;

    switch (aCode)
    {
    case MSG_GAMEPAD: lResult = OnGamepadEvent(*reinterpret_cast<const ZT::IGamepad::Event *>(aData)); break;

    default: assert(false);
    }

    return lResult;
}

// Private
// //////////////////////////////////////////////////////////////////////////

unsigned int ControlLink::ComputeHomeDuration(double aFactor)
{
    double lDuration_ms = aFactor * 1000.0;
    if (1.0 < mSpeedBoost)
    {
        lDuration_ms /= mSpeedBoost;
    }

    return lDuration_ms;
}

ZT::Result ControlLink::Gimbal_Set(ZT::ISystem * aSystem, const char * aId)
{
    assert(NULL != aSystem);
    assert(NULL != aId);

    char          lId[64];
    unsigned int  lIndex;
    GimbalInfo    lInfo;
    bool          lTestGimbal = true;

    memset(&lId  , 0, sizeof(lId  ));
    memset(&lInfo, 0, sizeof(lInfo));

    if (1 == sscanf(aId, "NONE ATEM = %u", &lInfo.mAtemPort))
    {
        if (0 >= lInfo.mAtemPort)
        {
            fprintf(stderr, "ERROR  Invalid gimbal id (%s)\n", aId);
            return ZT::ZT_ERROR_CONFIG;
        }

        lTestGimbal = false;
    }
    else if ((0 == strcmp("", aId))
        ||   (1 == sscanf(aId, "ATEM = %u", &lInfo.mAtemPort)))
    {
        lInfo.mGimbal = aSystem->Gimbal_Get(0);
    }
    else if ((2 == sscanf(aId, "ATEM = %u INDEX = %u", &lInfo.mAtemPort, &lIndex))
        ||   (2 == sscanf(aId, "INDEX = %u ATEM = %u", &lIndex, &lInfo.mAtemPort))
        ||   (1 == sscanf(aId, "INDEX = %u", &lIndex)))
    {
        lInfo.mGimbal = aSystem->Gimbal_Get(lIndex);
    }
    else if ((2 == sscanf(aId, "ATEM = %u IPv4 = %[0-9.]", &lInfo.mAtemPort, lId))
        ||   (2 == sscanf(aId, "IPv4 = %[0-9.] ATEM = %u", lId, &lInfo.mAtemPort))
        ||   (1 == sscanf(aId, "IPv4 = %[0-9.]", lId)))
    {
        lInfo.mGimbal = aSystem->Gimbal_Find_IPv4(lId);
    }
    else
    {
        fprintf(stderr, "ERROR  Invalid gimbal id (%s)\n", aId);
        return ZT::ZT_ERROR_CONFIG;
    }

    switch (lInfo.mAtemPort / 10)
    {
    case 0 : lInfo.mAtemCameraType = Atem::CAMERA_MFT; break;
    case 1 : lInfo.mAtemCameraType = Atem::CAMERA_EF ; break;
    
    default:
        Error("Invalid Atem camera type");
        return ZT::ZT_ERROR_CONFIG;
    }

    lInfo.mAtemPort %= 10;

    if (lTestGimbal && (NULL == lInfo.mGimbal))
    {
        fprintf(stderr, "ERROR  System::Gimbal_Get( %u ) or System::Gimbal_Find_IPv4( \"%s\" ) failed\n", lIndex, lId);
        return ZT::ZT_ERROR_GIMBAL_OFF;
    }

    mGimbals.push_back(lInfo);

    return ZT::ZT_OK;
}

bool ControlLink::OnGamepadEvent(const ZT::IGamepad::Event & aEvent)
{
    bool lResult = true;

    const TableEntry * lEntry = Table_FindEntry(aEvent.mAction, aEvent.mControl);
    if (NULL != lEntry)
    {
        // printf("%s %f %%\n", FUNCTION_NAMES[lEntry->mFunction], aEvent.mValue_pc);

        switch (lEntry->mFunction)
        {
        case FUNCTION_GIMBAL_SELECT: Function_Gimbal_Select(lEntry->mFactor); break;
        case FUNCTION_HOME         : Function_Home         (lEntry->mFactor); break;
        case FUNCTION_HOME_PITCH   : Function_Home_Pitch   (lEntry->mFactor); break;
        case FUNCTION_HOME_YAW     : Function_Home_Yaw     (lEntry->mFactor); break;

        case FUNCTION_ATEM_ZOOM: Function_Atem_Zoom(lEntry->mFactor, aEvent.mValue_pc); break;
        case FUNCTION_FOCUS: Function_Focus(lEntry->mFactor, aEvent.mValue_pc); break;
        case FUNCTION_PITCH: Function_Pitch(lEntry->mFactor, aEvent.mValue_pc); break;
        case FUNCTION_ROLL : Function_Roll (lEntry->mFactor, aEvent.mValue_pc); break;
        case FUNCTION_SPEED_BOOST: Function_Speed_Boost(lEntry->mFactor, aEvent.mValue_pc); break;
        case FUNCTION_YAW  : Function_Yaw  (lEntry->mFactor, aEvent.mValue_pc); break;
        case FUNCTION_ZOOM : Function_Zoom (lEntry->mFactor, aEvent.mValue_pc); break;

        case FUNCTION_ATEM_APERTURE_ABSOLUTE: Function_Atem_Aperture_Absolute(lEntry->mFactor, lEntry->mOffset, aEvent.mValue_pc); break;
        case FUNCTION_ATEM_FOCUS_ABSOLUTE   : Function_Atem_Focus_Absolute   (lEntry->mFactor, lEntry->mOffset, aEvent.mValue_pc); break;
        case FUNCTION_ATEM_GAIN_ABSOLUTE    : Function_Atem_Gain_Absolute    (lEntry->mFactor, lEntry->mOffset, aEvent.mValue_pc); break;
        case FUNCTION_ATEM_ZOOM_ABSOLUTE    : Function_Atem_Zoom_Absolute    (lEntry->mFactor, lEntry->mOffset, aEvent.mValue_pc); break;
        case FUNCTION_FOCUS_ABSOLUTE: Function_Focus_Absolute(lEntry->mFactor, lEntry->mOffset, aEvent.mValue_pc); break;
        case FUNCTION_PITCH_ABSOLUTE: Function_Pitch_Absolute(lEntry->mFactor, lEntry->mOffset, aEvent.mValue_pc); break;
        case FUNCTION_ROLL_ABSOLUTE : Function_Roll_Absolute (lEntry->mFactor, lEntry->mOffset, aEvent.mValue_pc); break;
        case FUNCTION_YAW_ABSOLUTE  : Function_Yaw_Absolute  (lEntry->mFactor, lEntry->mOffset, aEvent.mValue_pc); break;
        case FUNCTION_ZOOM_ABSOLUTE : Function_Zoom_Absolute (lEntry->mFactor, lEntry->mOffset, aEvent.mValue_pc); break;

        case FUNCTION_FORWARD: lResult = Function_Forward(aEvent); break;

        case FUNCTION_ATEM_APERTURE_AUTO  : Function_Atem_Aperture_Auto  (); break;
        case FUNCTION_ATEM_FOCUS_AUTO     : Function_Atem_Focus_Auto     (); break;
        case FUNCTION_FOCUS_CALIBRATION   : Function_Focus_Calibration   (); break;
        case FUNCTION_GIMBAL_FIRST        : Function_Gimbal_First        (); break;
        case FUNCTION_GIMBAL_LAST         : Function_Gimbal_Last         (); break;
        case FUNCTION_GIMBAL_NEXT         : Function_Gimbal_Next         (); break;
        case FUNCTION_GIMBAL_NEXT_LOOP    : Function_Gimbal_Next_Loop    (); break;
        case FUNCTION_GIMBAL_PREVIOUS     : Function_Gimbal_Previous     (); break;
        case FUNCTION_GIMBAL_PREVIOUS_LOOP: Function_Gimbal_Previous_Loop(); break;
        case FUNCTION_HOME_SET            : Function_Home_Set            (); break;
        case FUNCTION_TRACK_SWITCH        : Function_Track_Switch        (); break;
        case FUNCTION_ZOOM_CALIBRATION    : Function_Zoom_Calibration    (); break;
        
        default: assert(false); lResult = false;
        }
    }
    else
    {
        if ((NULL != mReceiver) && (0 != mReceiver_Unknown))
        {
            lResult = mReceiver->ProcessMessage(this, mReceiver_Unknown, &aEvent);
        }
    }

    return lResult;
}

void ControlLink::OnGimbalChanged()
{
    memset(&mSpeedCommand, 0, sizeof(mSpeedCommand));
}

// # Comment
// CLEAR
// GIMBAL [ATEM = y] [INDEX = x]
// GIMBAL [ATEM = y] [IPv4 = A.B.C.D]
// {Action} {Control} {Function} [SpeedFactor]       
ZT::Result ControlLink::ParseConfigLine(const char * aLine)
{
    assert(NULL != aLine);

    ZT::Result lResult = ZT::ZT_OK;

    switch (aLine[0])
    {
    case '#': break; // Comment

    case ' ': // Line begining with a blank
    case '\t':
        break;

    case '\n': // Empty lines
    case '\r':
        break;

    default:
        char lId[1024];

        if (1 == sscanf(aLine, "ATEM %[^\n\r\t]", lId))
        {
            mAtem = Atem::FindOrCreate(lId);
            if (NULL == mAtem)
            {
                fprintf(stderr, "ERROR  Atem::FindOrCreate( \"%s\" ) failed\n", lId);
                lResult = ZT::ZT_ERROR_CONFIG;
            }
        }
        else if (0 == strncmp("CLEAR", aLine, 5))
        {
            mTable.clear();
        }
        else if (1 == sscanf(aLine, "GIMBAL %[^\n\r\t]", lId))
        {
            mGimbalIds.push_back(lId);
        }
        else if (0 == strncmp("GIMBAL", aLine, 6))
        {
            mGimbalIds.push_back("");
        }
        else
        {
            char lAction  [1024];
            char lControl [1024];
            char lFunction[1024];

            double lFactor;
            double lOffset;

            switch (sscanf(aLine, "%[A-Z_] %[A-Z0-9_] %[A-Z_] %lf %lf", lAction, lControl, lFunction, &lFactor, &lOffset))
            {
            case 2: lResult = Table_RemoveEntry(lAction, lControl); break;

            case 3: lResult = Table_AddEntry(lAction, lControl, lFunction); break;
            case 4: lResult = Table_AddEntry(lAction, lControl, lFunction, lFactor); break;
            case 5: lResult = Table_AddEntry(lAction, lControl, lFunction, lFactor, lOffset); break;

            default:
                fprintf(stderr, "ERROR  Invalid configuration line (%s)\n", aLine);
                lResult = ZT::ZT_ERROR_CONFIG;
            }
        }
    }

    return lResult;
}

ZT::Result ControlLink::Table_AddEntry(ZT::IGamepad::Action aAction, ZT::IGamepad::Control aControl, Function aFunction, double aFactor, double aOffset)
{
    assert(ZT::IGamepad::ACTION_QTY > aAction);
    assert(ZT::IGamepad::CONTROL_QTY > aControl);
    assert(FUNCTION_QTY > aFunction);

    ZT::Result lResult = Value_Validate(aFactor, FACTOR_MIN, FACTOR_MAX);
    if (ZT::ZT_OK == lResult)
    {
        lResult = Value_Validate(aOffset, OFFSET_MIN, OFFSET_MAX);
        if (ZT::ZT_OK == lResult)
        {
            TableEntry * lEntry = Table_FindEntry(aAction, aControl);
            if (NULL == lEntry)
            {
                TableEntry lNewEntry;

                lNewEntry.mAction = aAction;
                lNewEntry.mControl = aControl;
                lNewEntry.mFunction = aFunction;
                lNewEntry.mOffset = aOffset;
                lNewEntry.mFactor = aFactor;

                mTable.push_back(lNewEntry);
            }
            else
            {
                lEntry->mFunction = aFunction;
                lEntry->mOffset = aOffset;
                lEntry->mFactor = aFactor;
            }
        }
    }

    return lResult;
}

ZT::Result ControlLink::Table_AddEntry(const char * aAction, const char * aControl, const char * aFunction, double aFactor, double aOffset)
{
    ZT::IGamepad::Action lAction;

    ZT::Result lResult = ActionFromName(aAction, &lAction);
    if (ZT::ZT_OK == lResult)
    {
        ZT::IGamepad::Control lControl;

        lResult = ControlFromName(aControl, &lControl);
        if (ZT::ZT_OK == lResult)
        {
            Function lFunction;

            lResult = FunctionFromName(aFunction, &lFunction);
            if (ZT::ZT_OK == lResult)
            {
                lResult = Table_AddEntry(lAction, lControl, lFunction, aFactor, aOffset);
            }
        }
    }

    return lResult;
}

ControlLink::TableEntry * ControlLink::Table_FindEntry(ZT::IGamepad::Action aAction, ZT::IGamepad::Control aControl)
{
    assert(ZT::IGamepad::ACTION_QTY > aAction);
    assert(ZT::IGamepad::CONTROL_QTY > aControl);

    for (Table::iterator lIt = mTable.begin(); lIt != mTable.end(); lIt++)
    {
        if ((aAction == lIt->mAction) && (aControl == lIt->mControl))
        {
            return &(*lIt);
        }
    }

    return NULL;
}

void ControlLink::Table_Init()
{
    Table_AddEntry(ZT::IGamepad::ACTION_CHANGED, ZT::IGamepad::ANALOG_0_X, FUNCTION_YAW  , 2.0);
    Table_AddEntry(ZT::IGamepad::ACTION_CHANGED, ZT::IGamepad::ANALOG_1_Y, FUNCTION_PITCH, 2.0);

    Table_AddEntry(ZT::IGamepad::ACTION_CHANGED, ZT::IGamepad::TRIGGER_LEFT , FUNCTION_FOCUS, - 2.0);
    Table_AddEntry(ZT::IGamepad::ACTION_CHANGED, ZT::IGamepad::TRIGGER_RIGHT, FUNCTION_FOCUS,   2.0);

    Table_AddEntry(ZT::IGamepad::ACTION_DISCONNECTED, ZT::IGamepad::CONTROL_NONE, FUNCTION_FORWARD);

    Table_AddEntry(ZT::IGamepad::ACTION_PRESSED, ZT::IGamepad::BUTTON_A    , FUNCTION_HOME_SET);
    Table_AddEntry(ZT::IGamepad::ACTION_PRESSED, ZT::IGamepad::BUTTON_B    , FUNCTION_HOME);
    Table_AddEntry(ZT::IGamepad::ACTION_PRESSED, ZT::IGamepad::BUTTON_BACK , FUNCTION_FORWARD);
    Table_AddEntry(ZT::IGamepad::ACTION_PRESSED, ZT::IGamepad::BUTTON_LEFT , FUNCTION_TRACK_SWITCH);
    Table_AddEntry(ZT::IGamepad::ACTION_PRESSED, ZT::IGamepad::BUTTON_START, FUNCTION_FOCUS_CALIBRATION);
    Table_AddEntry(ZT::IGamepad::ACTION_PRESSED, ZT::IGamepad::BUTTON_X    , FUNCTION_HOME_YAW);
    Table_AddEntry(ZT::IGamepad::ACTION_PRESSED, ZT::IGamepad::BUTTON_Y    , FUNCTION_HOME_PITCH);

    Table_AddEntry(ZT::IGamepad::ACTION_PRESSED, ZT::IGamepad::PAD_BOTTOM, FUNCTION_GIMBAL_FIRST);
    Table_AddEntry(ZT::IGamepad::ACTION_PRESSED, ZT::IGamepad::PAD_LEFT  , FUNCTION_GIMBAL_PREVIOUS);
    Table_AddEntry(ZT::IGamepad::ACTION_PRESSED, ZT::IGamepad::PAD_RIGHT , FUNCTION_GIMBAL_NEXT);
    Table_AddEntry(ZT::IGamepad::ACTION_PRESSED, ZT::IGamepad::PAD_TOP   , FUNCTION_GIMBAL_LAST);
}

void ControlLink::Table_RemoveEntry(ZT::IGamepad::Action aAction, ZT::IGamepad::Control aControl)
{
    assert(ZT::IGamepad::ACTION_QTY > aAction);
    assert(ZT::IGamepad::CONTROL_QTY > aControl);

    for (Table::iterator lIt = mTable.begin(); lIt != mTable.end(); lIt++)
    {
        if ((aAction == lIt->mAction) && (aControl == lIt->mControl))
        {
            mTable.erase(lIt);
            break;
        }
    }
}

ZT::Result ControlLink::Table_RemoveEntry(const char * aAction, const char * aControl)
{
    ZT::IGamepad::Action lAction;

    ZT::Result lResult = ActionFromName(aAction, &lAction);
    if (ZT::ZT_OK == lResult)
    {
        ZT::IGamepad::Control lControl;

        lResult = ControlFromName(aControl, &lControl);
        if (ZT::ZT_OK == lResult)
        {
            Table_RemoveEntry(lAction, lControl);
        }
    }

    return lResult;
}

// ===== Functions ==========================================================

static unsigned int FLAGS[ZT::IGimbal::AXIS_QTY] =
{
    ZT_FLAG_IGNORE_ROLL  | ZT_FLAG_IGNORE_YAW ,
    ZT_FLAG_IGNORE_PITCH | ZT_FLAG_IGNORE_YAW ,
    ZT_FLAG_IGNORE_PITCH | ZT_FLAG_IGNORE_ROLL,
};

void ControlLink::Function_Axis(ZT::IGimbal::Axis aAxis, double aFactor, double aValue_pc)
{
    CURRENT_GIMBAL;

    double lFactor = (aFactor + mSpeedBoost * BOOST_AXIS[aAxis]);

    ZT::IGimbal::Speed lSpeed;

    lSpeed.mAxis_deg_s[aAxis] = Value_Limit(lFactor * aValue_pc, ZT::IGimbal::SPEED_MIN_deg_s, ZT::IGimbal::SPEED_MAX_deg_s);

    ZT::Result lRet = lInfo.mGimbal->Speed_Set(lSpeed, FLAGS[aAxis]);
    assert(ZT::ZT_OK == lRet);

    mSpeedCommand.mAxis_deg_s[aAxis] = lSpeed.mAxis_deg_s[aAxis] / lFactor;
}
    
void ControlLink::Function_Axis_Absolute(ZT::IGimbal::Axis aAxis, double aFactor, double aOffset, double aValue_pc)
{
    CURRENT_GIMBAL;

    ZT::IGimbal::Position lPosition;

    lPosition.mAxis_deg[aAxis] = Value_Limit(aOffset + aFactor * aValue_pc, ZT::IGimbal::POSITION_MIN_deg, ZT::IGimbal::POSITION_MAX_deg);

    ZT::Result lRet = lInfo.mGimbal->Position_Set(lPosition, FLAGS[aAxis]);
    VerifyResult(lRet, __LINE__);
}
    
void ControlLink::Function_Home_Axis(ZT::IGimbal::Axis aAxis, double aFactor)
{
    CURRENT_GIMBAL;

    unsigned int lDuration_ms = ComputeHomeDuration(aFactor);

    ZT::Result lRet = lInfo.mGimbal->Position_Set(lInfo.mHome, FLAGS[aAxis], lDuration_ms);
    assert(ZT::ZT_OK == lRet);
}

void ControlLink::Function_Gimbal_Select(double aFactor)
{
    mGimbalIndex = aFactor;

    if (mGimbals.size() <= mGimbalIndex)
    {
        mGimbalIndex = 0;
    }
}

void ControlLink::Function_Home(double aFactor)
{
    CURRENT_GIMBAL;

    unsigned int lDuration_ms = ComputeHomeDuration(aFactor);

    ZT::Result lRet = lInfo.mGimbal->Position_Set(lInfo.mHome, 0, lDuration_ms);
    assert(ZT::ZT_OK == lRet);
}

void ControlLink::Function_Home_Pitch(double aFactor)
{
    Function_Home_Axis(ZT::IGimbal::AXIS_PITCH, aFactor);
}

void ControlLink::Function_Home_Yaw(double aFactor)
{
    Function_Home_Axis(ZT::IGimbal::AXIS_YAW, aFactor);
}

void ControlLink::Function_Atem_Zoom(double aFactor, double aValue_pc)
{
    CURRENT_ATEM;

    double lValue_pc = Value_Limit(aFactor * aValue_pc, -100.0, 100.0);

    if (!mAtem->Zoom(lInfo.mAtemPort, lValue_pc))
    {
        fprintf(stderr, "ERROR  Atem::Zoom( %u, %f ) failed\n", lInfo.mAtemPort, lValue_pc);
    }
}

void ControlLink::Function_Focus(double aFactor, double aValue_pc)
{
    CURRENT_GIMBAL;

    double lSpeed_pc_s = Value_Limit(aFactor * aValue_pc, ZT::IGimbal::FOCUS_SPEED_MIN_pc_s, ZT::IGimbal::FOCUS_SPEED_MAX_pc_s);

    ZT::Result lRet = lInfo.mGimbal->Focus_Speed_Set(lSpeed_pc_s);
    assert(ZT::ZT_OK == lRet);
}

void ControlLink::Function_Pitch(double aFactor, double aValue_pc)
{
    Function_Axis(ZT::IGimbal::AXIS_PITCH, aFactor, aValue_pc);
}

void ControlLink::Function_Roll(double aFactor, double aValue_pc)
{
    Function_Axis(ZT::IGimbal::AXIS_ROLL, aFactor, aValue_pc);
}

void ControlLink::Function_Speed_Boost(double aFactor, double aValue_pc)
{
    CURRENT_GIMBAL;

    ZT::Result lRet;

    double lSpeedBoost = mSpeedBoost;

    mSpeedBoost = aFactor * aValue_pc / 100.0;

    double lDelta = mSpeedBoost - lSpeedBoost;
    if (0.0 != lDelta)
    {
        ZT::IGimbal::Speed lSpeed;

        lRet = lInfo.mGimbal->Speed_Get(&lSpeed);
        if (ZT::ZT_OK == lRet)
        {
            for (unsigned int a = 0; a < ZT::IGimbal::AXIS_QTY; a ++)
            {
                if ((0.0 != lSpeed.mAxis_deg_s[a]) && (0.0 != mSpeedCommand.mAxis_deg_s[a]))
                {
                    lSpeed.mAxis_deg_s[a] += lDelta * BOOST_AXIS[a] * mSpeedCommand.mAxis_deg_s[a];
                }
            }

            lRet = lInfo.mGimbal->Speed_Set(lSpeed);
            assert(ZT::ZT_OK == lRet);
        }
    }

    lRet = lInfo.mGimbal->Track_Speed_Set(aValue_pc);
    assert(ZT::ZT_OK == lRet);
}

void ControlLink::Function_Yaw(double aFactor, double aValue_pc)
{
    Function_Axis(ZT::IGimbal::AXIS_YAW, aFactor, aValue_pc);
}

void ControlLink::Function_Zoom(double aFactor, double aValue_pc)
{
    CURRENT_GIMBAL;

    double lSpeed_pc_s = Value_Limit(aFactor * aValue_pc, ZT::IGimbal::FOCUS_SPEED_MIN_pc_s, ZT::IGimbal::FOCUS_SPEED_MAX_pc_s);

    ZT::Result lRet = lInfo.mGimbal->Focus_Speed_Set(lSpeed_pc_s);
    assert(ZT::ZT_OK == lRet);
}

void ControlLink::Function_Atem_Aperture_Absolute(double aFactor, double aOffset, double aValue_pc)
{
    CURRENT_ATEM;

    double lValue_pc = Value_Limit(aFactor * aValue_pc + aOffset, 0.0, 100.0);

    if (!mAtem->Aperture_Absolute(lInfo.mAtemPort, lValue_pc))
    {
        fprintf(stderr, "ERROR  Atem::Aperture_Absolute( %u, %f ) failed\n", lInfo.mAtemPort, lValue_pc);
    }
}

void ControlLink::Function_Atem_Focus_Absolute(double aFactor, double aOffset, double aValue_pc)
{
    CURRENT_ATEM;

    double lValue_pc = Value_Limit(aFactor * aValue_pc + aOffset, 0.0, 100.0);

    if (!mAtem->Focus_Absolute(lInfo.mAtemPort, lValue_pc, lInfo.mAtemCameraType))
    {
        fprintf(stderr, "ERROR  Atem::Focus_Absolute( %u, %f, %u ) failed\n", lInfo.mAtemPort, lValue_pc, lInfo.mAtemCameraType);
    }
}

void ControlLink::Function_Atem_Gain_Absolute(double aFactor, double aOffset, double aValue_pc)
{
    CURRENT_ATEM;

    double lValue_pc = Value_Limit(aFactor * aValue_pc + aOffset, 0.0, 100.0);

    if (!mAtem->Gain_Absolute(lInfo.mAtemPort, lValue_pc))
    {
        fprintf(stderr, "ERROR  Atem::Gain_Absolute( %u, %f ) failed\n", lInfo.mAtemPort, lValue_pc);
    }
}

void ControlLink::Function_Atem_Zoom_Absolute(double aFactor, double aOffset, double aValue_pc)
{
    CURRENT_ATEM;

    double lValue_pc = Value_Limit(aOffset + aFactor * aValue_pc, 0.0, 100.0);

    if (!mAtem->Zoom_Absolute(lInfo.mAtemPort, lValue_pc))
    {
        fprintf(stderr, "ERROR  Atem::Zoom_Absolute( %u, %f ) failed\n", lInfo.mAtemPort, lValue_pc);
    }
}

void ControlLink::Function_Focus_Absolute(double aFactor, double aOffset, double aValue_pc)
{
    CURRENT_GIMBAL;

    double lPosition_pc = Value_Limit(aOffset + aFactor * aValue_pc, ZT::IGimbal::FOCUS_POSITION_MIN_pc, ZT::IGimbal::FOCUS_POSITION_MAX_pc);

    ZT::Result lRet = lInfo.mGimbal->Focus_Position_Set(lPosition_pc);
    assert(ZT::ZT_OK == lRet);
}

void ControlLink::Function_Pitch_Absolute(double aFactor, double aOffset, double aValue_pc)
{
    Function_Axis_Absolute(ZT::IGimbal::AXIS_PITCH, aFactor, aOffset, aValue_pc);
}

void ControlLink::Function_Roll_Absolute(double aFactor, double aOffset, double aValue_pc)
{
    Function_Axis_Absolute(ZT::IGimbal::AXIS_ROLL, aFactor, aOffset, aValue_pc);
}

void ControlLink::Function_Yaw_Absolute(double aFactor, double aOffset, double aValue_pc)
{
    Function_Axis_Absolute(ZT::IGimbal::AXIS_YAW, aFactor, aOffset, aValue_pc);
}

void ControlLink::Function_Zoom_Absolute(double aFactor, double aOffset, double aValue_pc)
{
    CURRENT_GIMBAL;

    double lPosition_pc = Value_Limit(aOffset + aFactor * aValue_pc, ZT::IGimbal::FOCUS_POSITION_MIN_pc, ZT::IGimbal::FOCUS_POSITION_MAX_pc);

    ZT::Result lRet = lInfo.mGimbal->Focus_Position_Set(lPosition_pc);
    assert(ZT::ZT_OK == lRet);
}

bool ControlLink::Function_Forward(const ZT::IGamepad::Event & aEvent)
{
    bool lResult = true;

    if ((NULL != mReceiver) && (0 != mReceiver_Configured))
    {
        lResult = mReceiver->ProcessMessage(this, mReceiver_Configured, &aEvent);
    }

    return lResult;
}

void ControlLink::Function_Atem_Aperture_Auto()
{
    CURRENT_ATEM;

    if (!mAtem->Aperture_Auto(lInfo.mAtemPort))
    {
        fprintf(stderr, "ERROR  Atem::Aperture_Auto( %u ) failed\n", lInfo.mAtemPort);
    }
}

void ControlLink::Function_Atem_Focus_Auto()
{
    CURRENT_ATEM;

    if (!mAtem->Focus_Auto(lInfo.mAtemPort))
    {
        fprintf(stderr, "ERROR  Atem::Focus_Auto( %u ) failed\n", lInfo.mAtemPort);
    }
}

void ControlLink::Function_Focus_Calibration()
{
    CURRENT_GIMBAL;

    ZT::Result lRet = lInfo.mGimbal->Focus_Cal(ZT::IGimbal::OPERATION_CAL_AUTO_ENABLE);
    if (ZT::ZT_OK == lRet)
    {
        sleep(1);
    }
    else
    {
        printf("Focus_Cal(  ) failed - %u\n", lRet);
    }

    lRet = lInfo.mGimbal->Focus_Cal(ZT::IGimbal::OPERATION_CAL_STOP);
    assert(ZT::ZT_OK == lRet);
}

void ControlLink::Function_Gimbal_First()
{
    mGimbalIndex = 0;

    OnGimbalChanged();
}

void ControlLink::Function_Gimbal_Last()
{
    assert(0 < mGimbals.size());

    mGimbalIndex = mGimbals.size() - 1;

    OnGimbalChanged();
}

void ControlLink::Function_Gimbal_Next()
{
    mGimbalIndex++;

    if (mGimbals.size() <= mGimbalIndex)
    {
        mGimbalIndex = mGimbals.size() - 1;
    }

    OnGimbalChanged();
}

void ControlLink::Function_Gimbal_Next_Loop()
{
    mGimbalIndex++;

    if (mGimbals.size() <= mGimbalIndex)
    {
        mGimbalIndex = 0;
    }

    OnGimbalChanged();
}

void ControlLink::Function_Gimbal_Previous()
{
    if (0 < mGimbalIndex)
    {
        mGimbalIndex --;

        OnGimbalChanged();
    }
}

void ControlLink::Function_Gimbal_Previous_Loop()
{
    if (0 < mGimbalIndex)
    {
        mGimbalIndex --;
    }
    else
    {
        mGimbalIndex = mGimbals.size() - 1;
    }

    OnGimbalChanged();
}


void ControlLink::Function_Home_Set()
{
    CURRENT_GIMBAL;

    ZT::Result lRet = lInfo.mGimbal->Position_Get(&lInfo.mHome);
    assert(ZT::ZT_OK == lRet);
}

void ControlLink::Function_Track_Switch()
{
    CURRENT_GIMBAL;

    ZT::Result lRet = lInfo.mGimbal->Track_Switch();
    assert(ZT::ZT_OK == lRet);
}

void ControlLink::Function_Zoom_Calibration()
{
    CURRENT_GIMBAL;

    ZT::Result lRet = lInfo.mGimbal->Focus_Cal(ZT::IGimbal::OPERATION_CAL_AUTO_ENABLE);
    if (ZT::ZT_OK == lRet)
    {
        sleep(1);
    }
    else
    {
        printf("Focus_Cal(  ) failed - %u\n", lRet);
    }

    lRet = lInfo.mGimbal->Focus_Cal(ZT::IGimbal::OPERATION_CAL_STOP);
    assert(ZT::ZT_OK == lRet);
}

// Static fonction declarations
// //////////////////////////////////////////////////////////////////////////

ZT::Result ActionFromName(const char * aIn, ZT::IGamepad::Action * aOut)
{
    assert(NULL != aIn);
    assert(NULL != aOut);

    for (unsigned int i = 0; i < ZT::IGamepad::ACTION_QTY; i++)
    {
        if (0 == strcmp(ZT::IGamepad::ACTION_NAMES[i], aIn))
        {
            *aOut = static_cast<ZT::IGamepad::Action>(i);
            return ZT::ZT_OK;
        }
    }

    return ZT::ZT_ERROR_ACTION;
}

ZT::Result ControlFromName(const char * aIn, ZT::IGamepad::Control * aOut)
{
    assert(NULL != aIn);
    assert(NULL != aOut);

    for (unsigned int i = 0; i < ZT::IGamepad::CONTROL_QTY; i++)
    {
        if (0 == strcmp(ZT::IGamepad::CONTROL_NAMES[i], aIn))
        {
            *aOut = static_cast<ZT::IGamepad::Control>(i);
            return ZT::ZT_OK;
        }
    }

    return ZT::ZT_ERROR_CONTROL;
}

ZT::Result FunctionFromName(const char * aIn, ControlLink::Function * aOut)
{
    assert(NULL != aIn);
    assert(NULL != aOut);

    for (unsigned int i = 0; i < ControlLink::FUNCTION_QTY; i++)
    {
        if (0 == strcmp(FUNCTION_NAMES[i], aIn))
        {
            *aOut = static_cast<ControlLink::Function>(i);
            return ZT::ZT_OK;
        }
    }

    return ZT::ZT_ERROR_FUNCTION;
}

void Error(const char * aMsg)
{
    assert(NULL != aMsg);

    fprintf(stderr, "ERROR  %s\n", aMsg);
}

void VerifyResult(ZT::Result aResult, unsigned int aLine)
{
    switch (aResult)
    {
    case ZT::ZT_OK: break;

    default: fprintf(stderr, "ERROR  VerifyResult( %s, %u )\n", ZT::Result_GetName(aResult), aLine);
    }
}
