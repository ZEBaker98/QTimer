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
#include "EventDeque.h"
#include "Event.h"

class QTimer {
  private:
    // EventDeque to hold events
    EventDeque ed;

    // private event creator used by public event creators
    Event* newEvent(unsigned long period, void (*callback)(), int repeatCount);

    // private updater used by public updators
    // updates a target and all following targets recursively
    void update(unsigned long now, Event *target);

  public:
    // public event creators
    // duration / period is given in milliseconds
    // each returns a pointer to the Event created
    // after -> creates an event that triggers once after duration
    Event* after(unsigned long duration, void (*callback)());
    // every -> creates an event that triggers every period
    Event* every(unsigned long period, void (*callback)());
    // every -> creates an event that triggers every period a specific number of times
    Event* every(unsigned long period, void (*callback)(), int repeatCount);

    // cancels an event, it will be cleaned up from memory next time the timer is updated
    void stop(Event *target); 

    // updates the timer, triggering events that fall at current time
    void update();

    // updates the timer, triggering events that fall at a given time
    void update(unsigned long now);
};

#endif
