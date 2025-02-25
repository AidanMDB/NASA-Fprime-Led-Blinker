module Components {
    @ Component to blink an LED driven by a rate group
    active component Led {

        @ Command to turn on or off the blinking LED.
        async command BLINKING_ON_OFF(
            onOff: Fw.On    @< indicates whether the blinking led should be on or off
        )

        @ Telemetry channel to report blinking state.
        telemetry BlinkingState: Fw.On

        @ Telemetry channel to report led transitions.
        telemetry LedTransitions: U64

        @ Blinking interval in rate group ticks
        param BLINK_INTERVAL: U32 default 1

        @ Reports the state we set to blinking.
        event SetBlinkingState($state: Fw.On) \
            severity activity high \
            format "Set Blinking state to {}."

        @ Sets the interval at which to blink the LED.
        event BlinkIntervalSet(interval: U32) \
            severity activity high \
            format "LED blink interval set to {}"

        @ Indicates the LED has been driven to a different state
        event LedState(onOff: Fw.On) \
            severity activity low \
            format "LED is {}"

        @ Port receiving calls from rate group 
        async input port run: Svc.Sched

        @ Port sending calls to the GPIO driver
        output port gpioSet: Drv.GpioWrite
        ##############################################################################
        #### Uncomment the following examples to start customizing your component ####
        ##############################################################################

        # @ Example async command
        # async command COMMAND_NAME(param_name: U32)

        # @ Example telemetry counter
        # telemetry ExampleCounter: U64

        # @ Example event
        # event ExampleStateEvent(example_state: Fw.On) severity activity high id 0 format "State set to {}"

        # @ Example port: receiving calls from the rate group
        # sync input port run: Svc.Sched

        # @ Example parameter
        # param PARAMETER_NAME: U32

        ###############################################################################
        # Standard AC Ports: Required for Channels, Events, Commands, and Parameters  #
        ###############################################################################
        @ Port for requesting the current time
        time get port timeCaller

        @ Port for sending command registrations
        command reg port cmdRegOut

        @ Port for receiving commands
        command recv port cmdIn

        @ Port for sending command responses
        command resp port cmdResponseOut

        @ Port for sending textual representation of events
        text event port logTextOut

        @ Port for sending events to downlink
        event port logOut

        @ Port for sending telemetry channels to downlink
        telemetry port tlmOut

        @ Port to return the value of a parameter
        param get port prmGetOut

        @Port to set the value of a parameter
        param set port prmSetOut

    }
}