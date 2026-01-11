/*
 * ZT_Python - C wrapper for ZT_Lib to be called from Python via ctypes
 *
 * Author: ZAP Gimbal UI
 *
 * This file provides a C-compatible interface to ZT_Lib for Python integration.
 * Compile with: make
 */

#include <cstdio>
#include <cstring>
#include <cstdint>

#include <ZT/IGimbal.h>
#include <ZT/ISystem.h>
#include "../ZT_Lib/Atem.h"

// ============== C API ==============

extern "C" {

// System functions
void* ZTP_System_Create() {
    return ZT::ISystem::Create();
}

void ZTP_System_Release(void* system) {
    if (system) {
        static_cast<ZT::ISystem*>(system)->Release();
    }
}

int ZTP_System_Gimbals_Detect(void* system) {
    if (!system) return -1;
    return static_cast<int>(static_cast<ZT::ISystem*>(system)->Gimbals_Detect());
}

void* ZTP_System_Gimbal_Get(void* system, int index) {
    if (!system) return nullptr;
    return static_cast<ZT::ISystem*>(system)->Gimbal_Get(index);
}

// Gimbal functions
int ZTP_Gimbal_Activate(void* gimbal) {
    if (!gimbal) return -1;
    return static_cast<int>(static_cast<ZT::IGimbal*>(gimbal)->Activate());
}

void ZTP_Gimbal_Release(void* gimbal) {
    if (gimbal) {
        static_cast<ZT::IGimbal*>(gimbal)->Release();
    }
}

void ZTP_Gimbal_Debug(void* gimbal) {
    if (gimbal) {
        static_cast<ZT::IGimbal*>(gimbal)->Debug();
    }
}

// Position functions
typedef struct {
    double pitch_deg;
    double roll_deg;
    double yaw_deg;
} ZTP_Position;

int ZTP_Gimbal_Position_Get(void* gimbal, ZTP_Position* pos) {
    if (!gimbal || !pos) return -1;

    ZT::IGimbal::Position zt_pos;
    ZT::Result result = static_cast<ZT::IGimbal*>(gimbal)->Position_Get(&zt_pos);

    if (result == ZT::ZT_OK) {
        pos->pitch_deg = zt_pos.mAxis_deg[ZT::IGimbal::AXIS_PITCH];
        pos->roll_deg  = zt_pos.mAxis_deg[ZT::IGimbal::AXIS_ROLL];
        pos->yaw_deg   = zt_pos.mAxis_deg[ZT::IGimbal::AXIS_YAW];
    }

    return static_cast<int>(result);
}

int ZTP_Gimbal_Position_Set(void* gimbal, double pitch_deg, double roll_deg, double yaw_deg) {
    if (!gimbal) return -1;

    ZT::IGimbal::Position zt_pos;
    zt_pos.mAxis_deg[ZT::IGimbal::AXIS_PITCH] = pitch_deg;
    zt_pos.mAxis_deg[ZT::IGimbal::AXIS_ROLL]  = roll_deg;
    zt_pos.mAxis_deg[ZT::IGimbal::AXIS_YAW]   = yaw_deg;

    return static_cast<int>(static_cast<ZT::IGimbal*>(gimbal)->Position_Set(zt_pos));
}

// Speed functions
typedef struct {
    double pitch_deg_s;
    double roll_deg_s;
    double yaw_deg_s;
} ZTP_Speed;

int ZTP_Gimbal_Speed_Set(void* gimbal, double pitch_deg_s, double roll_deg_s, double yaw_deg_s) {
    if (!gimbal) return -1;

    ZT::IGimbal::Speed zt_speed;
    zt_speed.mAxis_deg_s[ZT::IGimbal::AXIS_PITCH] = pitch_deg_s;
    zt_speed.mAxis_deg_s[ZT::IGimbal::AXIS_ROLL]  = roll_deg_s;
    zt_speed.mAxis_deg_s[ZT::IGimbal::AXIS_YAW]   = yaw_deg_s;

    return static_cast<int>(static_cast<ZT::IGimbal*>(gimbal)->Speed_Set(zt_speed));
}

int ZTP_Gimbal_Speed_Stop(void* gimbal) {
    if (!gimbal) return -1;
    return static_cast<int>(static_cast<ZT::IGimbal*>(gimbal)->Speed_Stop());
}

// Config functions
typedef struct {
    double pitch_min_deg;
    double pitch_max_deg;
    double roll_min_deg;
    double roll_max_deg;
    double yaw_min_deg;
    double yaw_max_deg;
} ZTP_Config;

void ZTP_Gimbal_Config_Get(void* gimbal, ZTP_Config* config) {
    if (!gimbal || !config) return;

    ZT::IGimbal::Config zt_config;
    static_cast<ZT::IGimbal*>(gimbal)->Config_Get(&zt_config);

    config->pitch_min_deg = zt_config.mAxis[ZT::IGimbal::AXIS_PITCH].mMin_deg;
    config->pitch_max_deg = zt_config.mAxis[ZT::IGimbal::AXIS_PITCH].mMax_deg;
    config->roll_min_deg  = zt_config.mAxis[ZT::IGimbal::AXIS_ROLL].mMin_deg;
    config->roll_max_deg  = zt_config.mAxis[ZT::IGimbal::AXIS_ROLL].mMax_deg;
    config->yaw_min_deg   = zt_config.mAxis[ZT::IGimbal::AXIS_YAW].mMin_deg;
    config->yaw_max_deg   = zt_config.mAxis[ZT::IGimbal::AXIS_YAW].mMax_deg;
}

int ZTP_Gimbal_Config_Set(void* gimbal, const ZTP_Config* config) {
    if (!gimbal || !config) return -1;

    ZT::IGimbal::Config zt_config;
    // First get current config to preserve other fields
    static_cast<ZT::IGimbal*>(gimbal)->Config_Get(&zt_config);

    zt_config.mAxis[ZT::IGimbal::AXIS_PITCH].mMin_deg = config->pitch_min_deg;
    zt_config.mAxis[ZT::IGimbal::AXIS_PITCH].mMax_deg = config->pitch_max_deg;
    zt_config.mAxis[ZT::IGimbal::AXIS_ROLL].mMin_deg  = config->roll_min_deg;
    zt_config.mAxis[ZT::IGimbal::AXIS_ROLL].mMax_deg  = config->roll_max_deg;
    zt_config.mAxis[ZT::IGimbal::AXIS_YAW].mMin_deg   = config->yaw_min_deg;
    zt_config.mAxis[ZT::IGimbal::AXIS_YAW].mMax_deg   = config->yaw_max_deg;

    return static_cast<int>(static_cast<ZT::IGimbal*>(gimbal)->Config_Set(zt_config));
}

// Info functions
typedef struct {
    char name[16];
    unsigned int ipv4_address;
    unsigned int ipv4_gateway;
    unsigned int ipv4_netmask;
    unsigned char version[4];
} ZTP_Info;

void ZTP_Gimbal_Info_Get(void* gimbal, ZTP_Info* info) {
    if (!gimbal || !info) return;

    ZT::IGimbal::Info zt_info;
    static_cast<ZT::IGimbal*>(gimbal)->Info_Get(&zt_info);

    strncpy(info->name, zt_info.mName, sizeof(info->name) - 1);
    info->name[sizeof(info->name) - 1] = '\0';
    info->ipv4_address = zt_info.mIPv4_Address;
    info->ipv4_gateway = zt_info.mIPv4_Gateway;
    info->ipv4_netmask = zt_info.mIPv4_NetMask;
    memcpy(info->version, zt_info.mVersion, 4);
}

// Focus functions
int ZTP_Gimbal_Focus_Position_Set(void* gimbal, double position) {
    if (!gimbal) return -1;
    return static_cast<int>(static_cast<ZT::IGimbal*>(gimbal)->Focus_Position_Set(position));
}

int ZTP_Gimbal_Focus_Speed_Set(void* gimbal, double speed) {
    if (!gimbal) return -1;
    return static_cast<int>(static_cast<ZT::IGimbal*>(gimbal)->Focus_Speed_Set(speed));
}

int ZTP_Gimbal_Focus_Cal(void* gimbal, int operation) {
    if (!gimbal) return -1;
    return static_cast<int>(static_cast<ZT::IGimbal*>(gimbal)->Focus_Cal(static_cast<ZT::IGimbal::Operation>(operation)));
}

// Track functions
int ZTP_Gimbal_Track_Switch(void* gimbal) {
    if (!gimbal) return -1;
    return static_cast<int>(static_cast<ZT::IGimbal*>(gimbal)->Track_Switch());
}

int ZTP_Gimbal_Track_Speed_Set(void* gimbal, double speed) {
    if (!gimbal) return -1;
    return static_cast<int>(static_cast<ZT::IGimbal*>(gimbal)->Track_Speed_Set(speed));
}

// Result name
const char* ZTP_Result_GetName(int result) {
    return ZT::Result_GetName(static_cast<ZT::Result>(result));
}

// Version info
const char* ZTP_GetVersion() {
    return "1.1.0";
}

// ============== ATEM Camera Control Functions ==============

// Camera type enum (matches Atem::CameraType)
// 0 = CAMERA_EF (Canon EF mount - offset-based focus)
// 1 = CAMERA_MFT (Micro Four Thirds - absolute focus)

void* ZTP_Atem_FindOrCreate(const char* ipv4_address) {
    if (!ipv4_address) return nullptr;

    // Format the ID string as expected by Atem class
    char id[64];
    snprintf(id, sizeof(id), "IPv4 = %s", ipv4_address);

    return Atem::FindOrCreate(id);
}

int ZTP_Atem_Focus_Absolute(void* atem, unsigned int port, double value_pc, int camera_type) {
    if (!atem) return -1;
    if (port < 1 || port > 8) return -2;
    if (value_pc < 0.0 || value_pc > 100.0) return -3;

    bool result = static_cast<Atem*>(atem)->Focus_Absolute(
        port,
        value_pc,
        static_cast<Atem::CameraType>(camera_type)
    );
    return result ? 0 : -4;
}

int ZTP_Atem_Focus_Auto(void* atem, unsigned int port) {
    if (!atem) return -1;
    if (port < 1 || port > 8) return -2;

    bool result = static_cast<Atem*>(atem)->Focus_Auto(port);
    return result ? 0 : -3;
}

int ZTP_Atem_Aperture_Absolute(void* atem, unsigned int port, double value_pc) {
    if (!atem) return -1;
    if (port < 1 || port > 8) return -2;
    if (value_pc < 0.0 || value_pc > 100.0) return -3;

    bool result = static_cast<Atem*>(atem)->Aperture_Absolute(port, value_pc);
    return result ? 0 : -4;
}

int ZTP_Atem_Aperture_Auto(void* atem, unsigned int port) {
    if (!atem) return -1;
    if (port < 1 || port > 8) return -2;

    bool result = static_cast<Atem*>(atem)->Aperture_Auto(port);
    return result ? 0 : -3;
}

int ZTP_Atem_Gain_Absolute(void* atem, unsigned int port, double value_pc) {
    if (!atem) return -1;
    if (port < 1 || port > 8) return -2;
    if (value_pc < 0.0 || value_pc > 100.0) return -3;

    bool result = static_cast<Atem*>(atem)->Gain_Absolute(port, value_pc);
    return result ? 0 : -4;
}

int ZTP_Atem_Zoom(void* atem, unsigned int port, double value_pc) {
    if (!atem) return -1;
    if (port < 1 || port > 8) return -2;
    if (value_pc < 0.0 || value_pc > 100.0) return -3;

    bool result = static_cast<Atem*>(atem)->Zoom(port, value_pc);
    return result ? 0 : -4;
}

int ZTP_Atem_Zoom_Absolute(void* atem, unsigned int port, double value_pc) {
    if (!atem) return -1;
    if (port < 1 || port > 8) return -2;
    if (value_pc < 0.0 || value_pc > 100.0) return -3;

    bool result = static_cast<Atem*>(atem)->Zoom_Absolute(port, value_pc);
    return result ? 0 : -4;
}

} // extern "C"
