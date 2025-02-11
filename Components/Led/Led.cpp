// ======================================================================
// \title  Led.cpp
// \author aidandb
// \brief  cpp file for Led component implementation class
// ======================================================================

#include "Components/Led/Led.hpp"
#include "FpConfig.hpp"

namespace Components {

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  Led ::
    Led(const char* const compName) :
      LedComponentBase(compName)
  {

  }

  Led ::
    ~Led()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for commands
  // ----------------------------------------------------------------------
 
  void Led ::
  run_handler(
      FwIndexType portNum,
      U32 context
  )
  {
    // TODO
    Fw::ParamValid isValid = Fw::ParamValid::INVALID;
    U32 interval = this->paramGet_BLINK_INTERVAL(isValid);
    FW_ASSERT((isValid != Fw::ParamValid::INVALID) && (isValid != Fw::ParamValid::UNINIT), 
              static_cast<FwAssertArgType>(isValid));

    // only perform actions when set to blinking
    if (this->m_blinking && (interval) != 0) {
      // if toggling state
      if (this->m_toggleCounter == 0) {
        // toggle state
        this->m_state = (this->m_state == Fw::On::ON) ? Fw::On::OFF : Fw::On::ON;
        this->m_transitions++;

        // TODO: report the number of LED transitions on channel LedTransitions
        this->tlmWrite_LedTransitions(this->m_transitions);

        // Port may not be connected, so check before sending output
        if (this->isConnected_gpioSet_OutputPort(0)) {
          this->gpioSet_out(0, (Fw::On::ON == this->m_state) ? Fw::Logic::HIGH : Fw::Logic::LOW);
        }

        // TODO: Emit an event LedState to report the LED state (this->m_state)
        this->log_ACTIVITY_LO_LedState(this->m_state);
      }
      this->m_toggleCounter = (this->m_toggleCounter + 1) % interval;
    }
    // We are not blinking
    else {    
      if (this->m_state == Fw::On::ON) {
        // Port may not be connected, so check before sending output
        if (this->isConnected_gpioSet_OutputPort(0)) {
          this->gpioSet_out(0, Fw::Logic::LOW);
        }

        this->m_state = Fw::On::OFF;

        // TODO: Emit an event LedState to report the LED state
        this->log_ACTIVITY_LO_LedState(this->m_state);
      }
    }
  }

  
  void Led :: BLINKING_ON_OFF_cmdHandler(
      FwOpcodeType opCode,
      U32 cmdSeq,
      Fw::On onOff
    )
  {
    this->m_toggleCounter = 0;                // Reset count on any successful command
    this->m_blinking = Fw::On::ON == onOff;   // Update blinking state

    // Emit an event SetBlinkingState to report the blinking state (onOff).
    this->log_ACTIVITY_HI_SetBlinkingState(onOff);

    // Report the blinking state (onOff) on channel BlinkingState.
    this->tlmWrite_BlinkingState(onOff);

    // provide command response
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
  }


  void Led ::parameterUpdated(FwPrmIdType id) {
    Fw::ParamValid isValid = Fw::ParamValid::INVALID;
    switch (id) {
      case PARAMID_BLINK_INTERVAL: {
        // read back parameter value
        const U32 interval = this->paramGet_BLINK_INTERVAL(isValid);
        // NOTE: isValid is always VALID in parameterUpdated as it was just properly set
        FW_ASSERT(isValid == Fw::ParamValid::VALID, static_cast<FwAssertArgType>(isValid));

        // Emit the blink interval set event
        // TODO: Emit an event with, severity activity high, named BlinkIntervalSet that takes in an argument of
        // type U32 to report the blink interval.
        this->log_ACTIVITY_HI_BlinkIntervalSet(interval);
        break;
      }
      default:
        FW_ASSERT(0, static_cast<FwAssertArgType>(id));
        break;
    }
  }

}
