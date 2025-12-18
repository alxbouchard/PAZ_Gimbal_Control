
// Author  KMS - Martin Dubois, P.Eng.
// Client  ZAP
// Product Tacking
// File    Includes/ZT/ISystem.h

#pragma once

#include <ZT/IObject.h>
#include <ZT/Result.h>

namespace ZT
{

    class IGamepad;
    class IGimbal;

    class ISystem : public IObject
    {

    public:

        static ISystem * Create();

        virtual Result Gamepads_Detect() = 0;

        virtual IGamepad * Gamepad_Get(unsigned int aIndex) = 0;

        virtual Result Gimbals_Detect() = 0;

        virtual IGimbal * Gimbal_Find_IPv4(const char * aIPv4) = 0;
        virtual IGimbal * Gimbal_Find_IPv4(uint32_t aIPv4) = 0;
        virtual IGimbal * Gimbal_Get(unsigned int aIndex) = 0;

    };

}
