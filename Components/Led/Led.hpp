// ======================================================================
// \title  Led.hpp
// \author aidandb
// \brief  hpp file for Led component implementation class
// ======================================================================

#ifndef Components_Led_HPP
#define Components_Led_HPP

#include "Components/Led/LedComponentAc.hpp"

namespace Components {

  class Led :
    public LedComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Component construction and destruction
      // ----------------------------------------------------------------------

      //! Construct Led object
      Led(
          const char* const compName //!< The component name
      );

      //! Destroy Led object
      ~Led();

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for commands
      // ----------------------------------------------------------------------

      //! Handler implementation for run
      //!
      //! Port receiving calls from rate group
      void run_handler(
          FwIndexType portNum, //!< The port number
          U32 context //!< The call order
      ) override;


      //! Handler implementation for command BLINKING_ON_OFF
      //!
      //! Command to turn on or off the blinking LED
      void BLINKING_ON_OFF_cmdHandler(
          FwOpcodeType opCode, //!< The opcode
          U32 cmdSeq, //!< The command sequence number
          Fw::On onOff //!< indicates whether the blinking led should be on or off
      ) override;

      void parameterUpdated(
        FwPrmIdType id  //! < The parameter ID
      ) override;

      Fw::On m_state = Fw::On::OFF; //! keeps track if LED is on or off
      U64 m_transitions = 0;        //! The number of on/off transitions that have occured from FSW boot up
      U32 m_toggleCounter = 0;      //! Keeps track of how many ticks the led has been on for
      bool m_blinking = false;      //! Flag: if true then LED blinking will occur else no blinking will happen
  };

}

#endif
