#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include <stdint.h>
#include "interrupts.h"
#include "port.h"

class KeyboardEventHandler
{
    public:
    KeyboardEventHandler();

    virtual void OnKeyDown(char);
    virtual void OnKeyUp(char);
};

class KeyboardDriver : public InterruptHandler
{
    Port8Bit dataport;
    Port8Bit commandport;

    KeyboardEventHandler* handler;

public:
    KeyboardDriver(InterruptManager * manager, KeyboardEventHandler *handler);
    ~KeyboardDriver();
    virtual uint32_t HandleInterrupt(uint32_t esp);

};


#endif
