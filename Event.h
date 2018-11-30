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


#ifndef Event_h
#define Event_h

struct Event {
  // pointers to previos and next nodes
  Event *prev, *next;

  // start -> time in milliseconds after program start when Event was created / last time event was called
  // period -> time in millisecond after start until callback call
  unsigned long start, period;

  // how many times to repeat, Event is distroyed when it reaches 0
  // if negative event will repeat forever
  int repeatCount; 

  // function called with the event is triggered
  void (*callback)();
};

#endif
