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


#ifndef QTimer_h
#define QTimer_h

#include "Arduino.h"
#include "stdint.h"

#include "EventDeque.h"
#include "Events.h"

class QTimer {
  private:
    uint8_t eventCount; // counts events created
    EventDeque ed; // EventDeque to hold events

    // private callback event creator used by public event creators
    CallbackEvent* newCallbackEvent(uint32_t  period, void (*callback)(), uint16_t repeatCount);

    // private pin event creator used by other public event creators
    PinEvent* newPinEvent(uint8_t pin, uint32_t  period, uint8_t startingState, uint16_t toggleCount);

    // private updater used by public updators
    // updates a target and all following targets recursively
    void update(uint32_t  now, BaseEvent *target);

  public:
    // Constructor
    QTimer()
      : eventCount(0)
    {
    }

    // public event creators
    // duration / period is given in milliseconds
    // each returns the id of the event created
    // after -> creates an event that triggers once after duration
    uint8_t after(uint32_t duration, void (*callback)());
    // every -> creates an event that triggers every period
    uint8_t every(uint32_t period, void (*callback)());
    // every -> creates an event that triggers every period a specific number of times
    uint8_t every(uint32_t period, void (*callback)(),uint16_t repeatCount);
    // oscillate -> toggles the state between high and low on a pin every period
    uint8_t oscillate(uint8_t pin, uint32_t period, uint8_t startingState);
    // oscillate -> toggles the state between high and low on a pin every period a specific number of times
    uint8_t oscillate(uint8_t pin, uint32_t period, uint8_t startingState, uint16_t repeatCount);
    // pulse -> generates a pulse of length period, one period after call
    uint8_t pulse(uint8_t pin, uint32_t period, uint8_t startingState);

    // cancels an event when passed an event id, it will be cleaned up from memory next time the timer is updated
    void stop(uint8_t targetID); 

    // stops all timer events
    void stopAll();

    // updates the timer, triggering events that fall at the current time
    void update();

    // updates the timer, triggering events that fall at a given time
    void update(uint32_t now);
};

#endif
