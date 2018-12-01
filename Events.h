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
    // pointers to prev & next nodes
    BaseEvent *prev, *next;

    // start -> time in milliseconds after program start when Event was created / last time event was called
    // period -> time in millisecond after start until callback call
    uint32_t start, period;

    // repeatCount -> how many times to repeat event (negative means forever)
    uint16_t repeatCount;

    // Default constructor sets pointers to nullptr
    BaseEvent()
      : prev(nullptr), next(nullptr) 
    {
    }

    // trigger method, base class does nothing when triggered
    virtual void trigger() {
      return;
    }
};

class CallbackEvent : public BaseEvent {
  private:
    // holds pointer to callback function
    void (*callback)();

  public:
    // Constructor sets callback function and calls base constructor
    CallbackEvent(void (*callback)())
      : callback(callback)
    {
    }

    // Override of trigger function calls callback function
    void trigger() { 
      if (this->callback != nullptr) (*this->callback)();
    }
};

#endif
