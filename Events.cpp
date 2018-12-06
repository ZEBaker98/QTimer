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

#include "Arduino.h"
#include "stdint.h"

#include "Events.h"

BaseEvent::BaseEvent(uint8_t id, uint32_t start, uint32_t period, uint16_t repeatCount) 
  : id(id), prev(nullptr), next(nullptr), start(start), period(period), repeatCount(repeatCount) 
{
}

// Base event trigger does nothing
void BaseEvent::trigger() {};

CallbackEvent::CallbackEvent(uint8_t id, void (*callback)(), uint32_t start, uint32_t period, uint16_t repeatCount)
  : BaseEvent(id, start, period, repeatCount), callback(callback) 
{
}

// Callback event calls function pointer when triggered
void CallbackEvent::trigger() { 
  if (this->callback != nullptr) (*this->callback)();
}

PinEvent::PinEvent(uint8_t id, uint8_t pin, uint8_t state, uint32_t start, uint32_t period, uint16_t repeatCount)
  : BaseEvent(id, start, period, repeatCount), pin(pin), state(state)
{
}
 
// Pin event toggles pin when triggered
void PinEvent::trigger() {
  digitalWrite(pin, state);
  state = !state;
}
