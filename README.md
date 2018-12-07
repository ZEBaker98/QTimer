# QTimer
An Alternative Timer for Arduino

Timers can be used to schedule tasks for your arduino to do in the future. QTimer is a non blocking timer which means you can execute your own code along side QTimer. An timer update function is provided for you to integrate your code. Whenever an update is called, QTimer will use the `millis()` function to figure out what events need to be triggered. It's not perfectly acurate but it tries it's best to trigger your actions as close to the correct time as it can.

# Why use QTimer?
QTimer is different than other arduino timers or task schedulers. Instead of using an array to store events, QTimer uses a queue-like data structure, hence the name QTimer. This sturcture  only uses memory when you add events to it and is optimized to check all events as fast as it can. With QTimer you have up to 255 events running simultaneously!

## Things to be aware of
QTimer when used incorretly can act strangely. Events in QTimer are stored in dynamically allocated memory which means that there is nothing stopping you from creating more events than you have ram for. When this happens your program is likely to crash or start behaving strangely. This isn't necessarily a bad thing, its just a limit of the program and something you need to account for when using it. Most likely QTimer will be able to handle all the events you need it to without a problem. Look at the Memory Test example for more information.

# Using QTimer

A timer can be created in your sketch using
```cpp
#include "QTimer.h"

QTimer timer;
```

Once you have created a timer you can add events to it. In order for events to trigger the timer must be updated using
```cpp
timer.update()
```

As long as this function is called frequelty you can have your own code run allong side QTimer.

## Creating Events
QTimer currently supports 2 kinds of events, Callback Events & Pin Events. Callback events are used to call functions after specific durations or at specific intervals. Pin Events are used to schedule pin changes after durations or at specific intervals.  Durations and intervals should be passed to QTimer in milliseconds.

### Callback Events
Callback events are events that call functions when they are triggered. 

#### Single time events
The most basic callback event can be created using
```cpp
timer.after(duration, callback);
```
`duration` is an `unsigned long` representing the time in milliseconds after you create the event that it should trigger.
`callback` is the name of a function you want to trigger. In order for it to work properly the function should return void and take no arguments.

For example
```cpp
// returns void, no arguments
void myCallbackFunction() {
  // does something
}
```
can be called after a 1 second delay by 
```cpp
timer.after(1000, myCallbackFunction);
```

Another thing to remember is that `timer.after()` returns a `byte` containing the id of the event created. This id will be 0 if the event failed to be created.

#### Repeating events
Another way to create a callback function is using
```cpp
timer.every(interval, callback);
```

This is different from `timer.after()` because `timer.every()` repeats itself every interval. `interval` is also an `unsigned long` that represents how frequently your event should occour.

If you wanted `myCallbackFunction` from earlier to trigger every second you could use
```cpp
timer.every(1000, myCallbackFunction);
```
which would trigger `myCallbackFunction` after 1 second, 2 seconds, 3, 4, 5 and so on.

If you want your evenrts to repeat but only a limited number of times you can supply a repeatCount using
```cpp
timer.every(interval, callback, repeatCount);
```

This is exactly the same as the previous `timer.every()` but now the event will stop after `repeatCount` times. `repeatCount` is an `unsigned int`.

### Pin Events
Pin events trigger changes on pins for your arduino when they trigger.

#### Oscilating a pin

A pin can be set to oscillate using
```cpp
timer.oscillate(pin, interval, startingState);
```

`pin` is a `byte` representing which arduino pin the event should be passed to. `interval` is an `unsigned long` represengint how long the pulse should last. `startingState` is a `bool` (HIGH or LOW) that states which state the pin should start at.

For example
```cpp
timer.oscillate(4, 2000, HIGH);
```
will set pin 4 `HIGH` for two seconds and then drop it to `LOW` for two seconds and repeat.
```cpp
timer.oscillate(7, 250, LOW);
```
will set pin 7 `LOW` for a quarter of a second and then set it to `HIGH` for a quarter of a second and then repeat.

Oscillations can be set to only repeat for a specific number of cycles by providing a repeatCount using
```cpp
timer.oscillate(pin, interval, startingState, repeatCount);
```
`repeatCount` is an `unsigned int`. One repeat counts as one high interval and one low interval combined. Oscilations will allways set the pin to an end state of `!startingState`
    

#### Pulsing a pin

A pulse can be generated on a pin using
```cpp
timer.pulse(pin, interval, startingState);
```

`pin` is a `byte` representing which arduino pin the event should be passed to. `interval` is an `unsigned long` represengint how long the pulse should last. `startingState` is a `bool` (HIGH or LOW) that states which state the pin should start at.

For example
```cpp
timer.pulse(13, 1000, HIGH);
```
will set pin 13 `HIGH` for one second and then drop it to `LOW`and leave it there.
```cpp
timer.pulse(10, 500, LOW);
```
will set pin 10 `LOW` for half a second and then set it to `HIGH` and leave it there.

`timer.pulse()` will leave the pin int the state `!startingState`.

## Stopping Events

All event creators return a `byte` representing the id of the event created. That id can be passed to 
```cpp
timer.stop(id);
```
to stop an event. When an event is stopped, the memory it uses won't be freed until the next time the timer updates. Event ids are reused and new events, when created, find the lowest id not being used by another event and take it for themselves.

All of a timers events can be stopped using
```cpp
timer.stopAll();
```
