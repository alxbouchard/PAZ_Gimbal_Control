
// Author  KMS - Martin Dubois, P.Eng.
// Client  ZAP
// Product Tacking
// File    ZT_Lib/ControlLink.h

#pragma once

// ===== C++ ================================================================
#include <list>
#include <string>
#include <vector>

// ===== Includes ===========================================================
#include <ZT/IControlLink.h>
#include <ZT/IGamepad.h>
#include <ZT/IGimbal.h>
#include <ZT/IMessageReceiver.h>

// ===== ZT_Lib =============================================================
#include "Atem.h"

class ControlLink : public ZT::IControlLink, public ZT::IMessageReceiver
{

public:

    ControlLink();

    // ===== ZT::IControlLink ===============================================

    virtual ZT::Result ReadConfigFile(const char * aFileName);

    virtual ZT::Result Gamepad_Set(ZT::IGamepad * aGamepad);

    virtual ZT::Result Gimbals_Set(ZT::ISystem * aSystem);

    virtual ZT::Result Receiver_Set(ZT::IMessageReceiver * aReceiver, unsigned int aConfigured, unsigned int aUnknown);

    virtual ZT::Result Start();
    virtual ZT::Result Stop();

    // ===== ZT::IObject ====================================================

    virtual void Release();

    // ===== ZT::IMessageReceiver ===========================================

    virtual bool ProcessMessage(void * aSender, unsigned int aCode, const void * aData);

// Internal

    typedef enum
    {
        FUNCTION_ATEM_APERTURE_ABSOLUTE,
        FUNCTION_ATEM_APERTURE_AUTO,
        FUNCTION_ATEM_FOCUS_ABSOLUTE,
        FUNCTION_ATEM_FOCUS_AUTO,
        FUNCTION_ATEM_GAIN_ABSOLUTE,
        FUNCTION_ATEM_ZOOM,
        FUNCTION_ATEM_ZOOM_ABSOLUTE,
        FUNCTION_FOCUS,
        FUNCTION_FOCUS_ABSOLUTE,
        FUNCTION_FOCUS_CALIBRATION,
        FUNCTION_FORWARD,
        FUNCTION_GIMBAL_FIRST,
        FUNCTION_GIMBAL_LAST,
        FUNCTION_GIMBAL_NEXT,
        FUNCTION_GIMBAL_NEXT_LOOP,
        FUNCTION_GIMBAL_PREVIOUS,
        FUNCTION_GIMBAL_PREVIOUS_LOOP,
        FUNCTION_GIMBAL_SELECT,
        FUNCTION_HOME,
        FUNCTION_HOME_PITCH,
        FUNCTION_HOME_SET,
        FUNCTION_HOME_YAW,
        FUNCTION_PITCH,
        FUNCTION_PITCH_ABSOLUTE,
        FUNCTION_ROLL,
        FUNCTION_ROLL_ABSOLUTE,
        FUNCTION_SPEED_BOOST,
        FUNCTION_TRACK_SWITCH,
        FUNCTION_YAW,
        FUNCTION_YAW_ABSOLUTE,
        FUNCTION_ZOOM,
        FUNCTION_ZOOM_ABSOLUTE,
        FUNCTION_ZOOM_CALIBRATION,

        FUNCTION_QTY
    }
    Function;

private:

    typedef struct
    {
        Atem::CameraType mAtemCameraType;
        unsigned int  mAtemPort;
        ZT::IGimbal * mGimbal;
        ZT::IGimbal::Position mHome;
    }
    GimbalInfo;
    
    typedef struct
    {
        ZT::IGamepad::Action  mAction;
        ZT::IGamepad::Control mControl;

        Function mFunction;

        double mFactor;
        double mOffset;
    }
    TableEntry;

    typedef std::vector<GimbalInfo>    GimbalList;
    typedef std::list<std::string>     StringList;
    typedef std::list<TableEntry>      Table;

    unsigned int ComputeHomeDuration(double aFactor);
    
    ZT::Result Gimbal_Set(ZT::ISystem * aSystem, const char * aId);

    bool OnGamepadEvent(const ZT::IGamepad::Event & aEvent);

    void OnGimbalChanged();

    ZT::Result ParseConfigLine(const char * aLine);

    ZT::Result   Table_AddEntry(ZT::IGamepad::Action aAction, ZT::IGamepad::Control aControl, Function aFunction, double aFactor = 0.0, double aOffset = 0.0);
    ZT::Result   Table_AddEntry(const char * aAction, const char * aControl, const char * aFunction, double aFactor = 0.0, double aOffset = 0.0);
    TableEntry * Table_FindEntry(ZT::IGamepad::Action aAction, ZT::IGamepad::Control aControl);
    void         Table_Init();
    void         Table_RemoveEntry(ZT::IGamepad::Action aAction, ZT::IGamepad::Control aControl);
    ZT::Result   Table_RemoveEntry(const char * aAction, const char * aControl);

    // ===== Functions ======================================================

    void Function_Axis(ZT::IGimbal::Axis aAxis, double aFactor, double aValue_pc);

    void Function_Axis_Absolute(ZT::IGimbal::Axis aAxis, double aFactor, double aOffset, double aValue_pc);
    
    void Function_Home_Axis(ZT::IGimbal::Axis aAxis, double aFactor);

    void Function_Gimbal_Select(double aFactor);
    void Function_Home         (double aFactor);
    void Function_Home_Pitch   (double aFactor);
    void Function_Home_Yaw     (double aFactor);

    void Function_Atem_Zoom(double aFactor, double aValue_pc);
    void Function_Focus(double aFactor, double aValue_pc);
    void Function_Pitch(double aFactor, double aValue_pc);
    void Function_Roll (double aFactor, double aValue_pc);
    void Function_Speed_Boost(double aFactor, double aValue_pc);
    void Function_Yaw  (double aFactor, double aValue_pc);
    void Function_Zoom (double aFactor, double aValue_pc);

    void Function_Atem_Aperture_Absolute(double aFactor, double aOffset, double aValue_pc);
    void Function_Atem_Focus_Absolute   (double aFactor, double aOffset, double aValue_pc);
    void Function_Atem_Gain_Absolute    (double aFactor, double aOffset, double aValue_pc);
    void Function_Atem_Zoom_Absolute    (double aFactor, double aOffset, double aValue_pc);
    void Function_Focus_Absolute(double aFactor, double aOffset, double aValue_pc);
    void Function_Pitch_Absolute(double aFactor, double aOffset, double aValue_pc);
    void Function_Roll_Absolute (double aFactor, double aOffset, double aValue_pc);
    void Function_Yaw_Absolute  (double aFactor, double aOffset, double aValue_pc);
    void Function_Zoom_Absolute (double aFactor, double aOffset, double aValue_pc);

    bool Function_Forward(const ZT::IGamepad::Event & aEvent);

    void Function_Atem_Aperture_Auto  ();
    void Function_Atem_Focus_Auto     ();
    void Function_Focus_Calibration   ();
    void Function_Gimbal_First        ();
    void Function_Gimbal_Last         ();
    void Function_Gimbal_Next         ();
    void Function_Gimbal_Next_Loop    ();
    void Function_Gimbal_Previous     ();
    void Function_Gimbal_Previous_Loop();
    void Function_Home_Set            ();
    void Function_Track_Switch        ();
    void Function_Zoom_Calibration    ();

    Atem         * mAtem;
    ZT::IGamepad * mGamepad;

    GimbalList   mGimbals;
    StringList   mGimbalIds;
    unsigned int mGimbalIndex;

    ZT::IGimbal::Speed    mSpeedCommand;

    ZT::IMessageReceiver * mReceiver;

    unsigned int mReceiver_Configured;
    unsigned int mReceiver_Unknown;

    unsigned int mRefCount;

    double mSpeedBoost;

    Table mTable;

};
