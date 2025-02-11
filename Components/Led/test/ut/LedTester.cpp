// ======================================================================
// \title  LedTester.cpp
// \author aidandb
// \brief  cpp file for Led component test harness implementation class
// ======================================================================

#include "LedTester.hpp"

namespace Components {

  // ----------------------------------------------------------------------
  // Construction and destruction
  // ----------------------------------------------------------------------

  LedTester ::
    LedTester() :
      LedGTestBase("LedTester", LedTester::MAX_HISTORY_SIZE),
      component("Led")
  {
    this->initComponents();
    this->connectPorts();
  }

  LedTester ::
    ~LedTester()
  {

  }

  // ----------------------------------------------------------------------
  // Tests
  // ----------------------------------------------------------------------

  void LedTester ::
    testBlinking()
  {
    // test makes use of parameters so we need to load them
    this->component.loadParameters();

    // Ensure LED stays off when blinking is disabled
    // The Led component defaults to blinking off
    this->invoke_to_run(0, 0);        // invoke 'run' port to simulate running one cycle
    this->component.doDispatch();     // trigger execution of async port

    ASSERT_EVENTS_LedState_SIZE(0);     // ensure no LedState change events were set 
    ASSERT_from_gpioSet_SIZE(0);        // ensure gpio LED wasn't set
    ASSERT_TLM_LedTransitions_SIZE(0);  // ensure no LedTransitions were recorded.

    // set command to enable blinking
    this->sendCmd_BLINKING_ON_OFF(0, 0, Fw::On::ON);
    this->component.doDispatch();       // trigger execution of async command
    ASSERT_CMD_RESPONSE_SIZE(1);        // ensure a command response emitted
    ASSERT_CMD_RESPONSE(0, Led::OPCODE_BLINKING_ON_OFF, 0, Fw::CmdResponse::OK);    // ensure expected command response emitted

    // Step through 3 run cycles to observe LED turning on and off 3 times
    // Cycle 1: LED initialization->On
    this->invoke_to_run(0, 0);
    this->component.doDispatch();     // Trigger execution of async port
    ASSERT_EVENTS_LedState_SIZE(1);
    ASSERT_EVENTS_LedState(0, Fw::On::ON);
    ASSERT_from_gpioSet_SIZE(1);
    ASSERT_from_gpioSet(0, Fw::Logic::HIGH);
    ASSERT_TLM_LedTransitions_SIZE(1);
    ASSERT_TLM_LedTransitions(0, 1);

    // Cycle 2: LED On->Off
    this->invoke_to_run(0, 0);
    this->component.doDispatch();  // Trigger execution of async port
    ASSERT_EVENTS_LedState_SIZE(2);
    ASSERT_EVENTS_LedState(1, Fw::On::OFF);
    ASSERT_from_gpioSet_SIZE(2);
    ASSERT_from_gpioSet(1, Fw::Logic::LOW);
    // TODO: Add assertions for LedTransitions telemetry
    ASSERT_TLM_LedTransitions_SIZE(2);
    ASSERT_TLM_LedTransitions(1, 2);


    // Cycle 3: LED Off->On
    this->invoke_to_run(0, 0);
    this->component.doDispatch();  // Trigger execution of async port
    //TODO: Write assertions for third cycle
    ASSERT_EVENTS_LedState_SIZE(3);
    ASSERT_EVENTS_LedState(2, Fw::On::ON);
    ASSERT_from_gpioSet_SIZE(3);
    ASSERT_from_gpioSet(2, Fw::Logic::HIGH);
    ASSERT_TLM_LedTransitions_SIZE(3);
    ASSERT_TLM_LedTransitions(2, 3);
  }


  void LedTester::testBlinkInterval() {
    // Enable LED Blinking
    this->sendCmd_BLINKING_ON_OFF(0, 0, Fw::On::ON);
    this->component.doDispatch();

    // adjust interval to 4 cycles
    U32 blinkInterval = 4;
    this->paramSet_BLINK_INTERVAL(blinkInterval, Fw::ParamValid::VALID);
    this->paramSend_BLINK_INTERVAL(0, 0);
    ASSERT_EVENTS_BlinkIntervalSet_SIZE(1);

    // logic to test adjusted blink interval
    ASSERT_EVENTS_BlinkIntervalSet(0, blinkInterval);
  }

}
