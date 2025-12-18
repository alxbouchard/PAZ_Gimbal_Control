
// Author  KMS - Martin Dubois, P.Eng.
// Client  ZAP
// Product Tacking
// File    ZT_Lib/Atem.h

#pragma once

class Atem
{

public:

    typedef enum
    {
        CAMERA_EF,
        CAMERA_MFT,

        CAMERA_QTY
    }
    CameraType;

    static Atem * FindOrCreate(const char * aId);

    Atem();

    ~Atem();

    bool Aperture_Absolute(unsigned int aPort, double aValue_pc);
    bool Focus_Absolute   (unsigned int aPort, double aValue_pc, CameraType aCameraType);
    bool Gain_Absolute    (unsigned int aPort, double aValue_pc);
    bool Zoom             (unsigned int aPort, double aValue_pc);
    bool Zoom_Absolute    (unsigned int aPort, double aValue_pc);

    bool Aperture_Auto(unsigned int aPort);
    bool Focus_Auto   (unsigned int aPort);
    
private:

    enum
    {
        PORT_QTY = 8,
    };

    bool Connect(const char * aId);
    
    bool Connect_IPv4(const char * aIPv4);

    void * mCameraControl;
    void * mSwitcher;

    double mFocus_Positions[PORT_QTY];

};
