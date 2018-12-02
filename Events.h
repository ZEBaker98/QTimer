/*
 *    This file is part of QTimer.

 *    QTimer is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.

 *    QTimer is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.

 *    You should have received a copy of the GNU General Public License
 *    along with QTimer.  If not, see <https://www.gnu.org/licenses/>.
 */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
 Written by Zach Baker
* * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef Events_h
#define Events_h

#include "Arduino.h"
#include "stdint.h"

class BaseEvent {
  public:
    uint8_t id;
    BaseEvent *prev, *next; // pointers to prev & next nodes
    uint32_t start, period; // start time in ms & time between triggers
    uint16_t repeatCount; // how many times to repeat event (negative means forever)

    // Constructors
    BaseEvent(uint8_t id, uint32_t start, uint32_t period, uint16_t repeatCount);

    // trigger method, base class does nothing when triggered
    virtual void trigger();
};

class CallbackEvent : public BaseEvent {
  private:
    void (*callback)(); // holds pointer to callback function

  public:
    // Constructors
    CallbackEvent(uint8_t id, void (*callback)(), uint32_t start, uint32_t period, uint16_t repeatCount);

    // Override of trigger function calls callback function
    void trigger();
};

class PinEvent : public BaseEvent {
  private:
    uint8_t pin; // pin to be toggled
    uint8_t state; // holds state of pin

  public:
    // Constructors
    PinEvent(uint8_t id, uint8_t pin, uint8_t state, uint32_t start, uint32_t period, uint16_t repeatCount);

    // Override of trigger function toggle pin state
    void trigger();
};

#endif
