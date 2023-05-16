## Application description

The BLE\_fit\_ds\_int project demonstrates how to handle an event signaled using a GPIO interrupt without disrupting the BLE scheduler.

A "user event" is signaled via a GPIO interrupt. By default, this is configured to happen when BTN0 on the EvKit is released. The interrupt handler (`UserEvent()`) sets a flag notifying the main loop that an event is pending. When the main loop detects that an event is pending and that there is enough time to process the event, the user event handler (`HandleUserEvent()`) is called. This simulates a delay (`EVENT_TIME_SIM_DELAY`) and clears the event flag.

To adapt this application to a real world application, the user must tweak the `EVENT_TIME_REQUIREMENT` and `EVENT_TIME_SIM_DELAY` constants (see main.c).  `EVENT_TIME_SIM_DELAY` must be set to a duration at least as long as the maximum time required to handle an interrupt. `EVENT_TIME_REQUIREMENT` must be equal to `EVENT_TIME_SIM_DELAY` plus some margin to make sure the event handler isn't run unless there really is enough time available.

Alternatively, the simulated delay may be replaced with an actual event handler, in which case `EVENT_TIME_SIM_DELAY` is not required to be set. In all cases, `EVENT_TIME_REQUIREMENT` must be set to a value larger than the maximum time required to handle a user event plus a safety margin to ensure the scheduler has a large enough block of idle time.

If the user event handler has a very brief runtime, then it may make sense to undefine both `EVENT_TIME_REQUIREMENT` and `EVENT_TIME_SIM_DELAY`. An example would be a user event handler that simply strobes a GPIO pin. Some time is required to check whether the `EVENT_TIME_REQUIREMENT` may be met, and for user events that can be handled extremely quickly, this check may not be necessary.

The provided example has 2 user event demos. `USER_GPIO_TOGGLE_DEMO` toggles the output GPIO pin every time an interrupt is handled. `USER_GPIO_TIMING_DEMO` pulls the output GPIO pin low when the input event is registered, and drives the pin high again once the event has been handled.

### Note:

The BLE\_fit\_ds\_int application may change depending on the frequency of the incomming GPIO "user events" as well as the application \#defines that are enabled. It is necessary to test the provided code with the indended "user event" handler code to ensure that it will work.

### Errata:

The BLE\_fit\_ds\_int application currently functions when the SDMA is enabled and when "user events" arrive with a period of 80ms. All other scenerios have not been demonstrated to work reliably.

