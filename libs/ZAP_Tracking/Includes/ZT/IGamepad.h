
// Author  KMS - Martin Dubois, P.Eng.
// Client  ZAP
// Product Tacking
// File    Includes/ZT/IGamepad.h

#pragma once

#include <ZT/IObject.h>
#include <ZT/Result.h>

namespace ZT
{

    class IMessageReceiver;

    class IGamepad : public IObject
    {

    public:

        typedef enum
        {
            ACTION_CHANGED,
            ACTION_DISCONNECTED,
            ACTION_PRESSED,
            ACTION_RELEASED,

            ACTION_QTY
        }
        Action;

        typedef enum
        {
            CONTROL_NONE,
            
            ANALOG_0_X,
            ANALOG_0_Y,
            ANALOG_1_X,
            ANALOG_1_Y,
            BUTTON_A,
            BUTTON_ANALOG_0,
            BUTTON_ANALOG_1,
            BUTTON_B,
            BUTTON_BACK,
            BUTTON_LEFT,
            BUTTON_RIGHT,
            BUTTON_START,
            BUTTON_X,
            BUTTON_Y,
            PAD_BOTTOM,
            PAD_LEFT,
            PAD_RIGHT,
            PAD_TOP,
            TRIGGER_LEFT,
            TRIGGER_RIGHT,

            CONTROL_QTY,
        }
        Control;

        typedef struct
        {
            Action  mAction ;
            Control mControl;

            double mValue_pc;
        }
        Event;

        static const char * ACTION_NAMES [ACTION_QTY ];
        static const char * CONTROL_NAMES[CONTROL_QTY];

        virtual void Debug(void * aOut = NULL) = 0;

        virtual ZT::Result Receiver_Start(IMessageReceiver * aReceiver, unsigned int aCode) = 0;

        virtual ZT::Result Receiver_Stop() = 0;

    };

}
