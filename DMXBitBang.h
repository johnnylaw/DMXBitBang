#pragma once

#include <Arduino.h>

namespace DMXBitBang
{
  #ifndef _DMX_BIT_BANG_RGB_COLOR
  #define _DMX_BIT_BANG_RGB_COLOR
  typedef struct RGBColor
  {
    unsigned char red, green, blue;
  } RGBColor;
  #endif

  class DMXUniverseBase
  {
    public:
      void virtual write(unsigned char *, unsigned int count) = 0;
    private:

  };

  template<unsigned char pin> class DMXUniverse : public DMXUniverseBase
  {
    public:
      void virtual write(unsigned char *, unsigned int count);
  };


  template<unsigned char pin> void __attribute__((aligned(16))) DMXUniverse<pin>::write(unsigned char * values, unsigned int count)
  {
    Pio * port = g_APinDescription[pin].pPort;
    uint32_t pinValue = g_APinDescription[pin].ulPin;
    PIO_SetOutput(port, pinValue, LOW, 0, 0);

    __disable_irq();   // Disable interrupts temporarily because we don't want our pulse timing to be messed up.

    // Hold low for at least 100 µs (DMX Break)
    delayMicroseconds(100);

    // Hold hi for at least 12 µs (DMX Mark-After-Break)
    PIO_Set(port, pinValue);
    delayMicroseconds(12);

    // while(count--)
    while(true)
    {
      // Send a color to the LED strip.
      // The assembly below also increments the 'colors' pointer,
      // it will be pointing to the next color at the end of this loop.
      asm volatile(
        "ldrb r12, [%0, #0]\n"  // Load value
        "lsls r12, r12, #24\n"  // Put value in MSB of color register
        "rbit r12, r12\n"       // Reverse the bits so we can use right rotations.
        "adds %0, %0, #1\n"     // Advance pointer to next address.
        "movs r3, #8\n"         // Initialize the loop counter register.

        "send_dmx_bit_pair%=:\n"     // Temp only

        // Send one bit low (4 µs = 336 cycles @ 84MHz)
        "str %[val], %[clear]\n"            // Drive the line low.

        "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
        "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
        "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
        "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
        "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
        "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
        "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
        "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
        "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
        "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"

        "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
        "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
        "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
        "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
        "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
        "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
        "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
        "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
        "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
        "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"

        "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
        "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
        "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
        "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
        "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
        "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
        "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
        "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
        "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
        "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"

        // "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" //"nop\n" "nop\n" "nop\n"// "nop\n" "nop\n"
        // "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
        // "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
        // "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"

        // "send_dmx_bit%=:\n"     // Begin looping through 8 bits


        // Send two bits high (8 µs = 672 cycles @ 84MHz)
        "str %[val], %[set]\n"            // Drive the line high.
        // 671 nops
        "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
        "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
        "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
        "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
        "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
        "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
        "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
        "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
        "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
        "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"

        "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
        "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
        "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
        "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
        "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
        "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
        "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
        "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
        "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
        "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"

        "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
        "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
        "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
        "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
        "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
        "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
        "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
        "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
        "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
        "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" // less one for branch

        // "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" //"nop\n" "nop\n" "nop\n" //"nop\n" "nop\n"
        // "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
        // "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
        // "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"





        "b send_dmx_bit_pair%=\n"             // Temporary
        // "sub r3, r3, #1\n"                // Decrement the loop counter.
        // "cbz r3, dmx_asm_end%=\n"         // If we have sent 8 bits, go to the end.
        // "b send_dmx_bit%=\n"
        //
        // "dmx_asm_end%=:\n"

        : "=r" (values)
        : "0" (values),
          [set] "m" (port->PIO_SODR),
          [clear] "m" (port->PIO_CODR),
          [val] "r" (pinValue)
        : "r3", "r12", "cc"
      );
    }
    __enable_irq();
  }
}

using namespace DMXBitBang;


//
//
//         "ldrb r12, [%0, #0]\n"    // Load red.
//         "lsls r12, r12, #24\n"    // Put red in MSB of color register.
//         "ldrb r3, [%0, #1]\n"     // Load green.
//         "lsls r3, r3, #16\n"
//         "orrs r12, r12, r3\n"     // Put green in color register.
//         "ldrb r3, [%0, #2]\n"     // Load blue.
//         "lsls r3, r3, #8\n"
//         "orrs r12, r12, r3\n"     // Put blue in LSB of color register.
//         "rbit r12, r12\n"         // Reverse the bits so we can use right rotations.
//         "adds  %0, %0, #3\n"      // Advance pointer to next color.
//
//         "mov r3, #24\n"           // Initialize the loop counter register.
//
//         "send_dmx_bit%=:\n"
//         "str %[val], %[set]\n"            // Drive the line high.
//         "rrxs r12, r12\n"                 // Rotate right through carry.
//
//         "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
//         "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
//         "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
//
//         "it cc\n" "strcc %[val], %[clear]\n"  // If the bit to send is 0, set the line low now.
//
//         "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
//         "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
//         "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
//         "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
//
//         "it cs\n" "strcs %[val], %[clear]\n"  // If the bit to send is 1, set the line low now.
//
//         "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
//         "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
//
//         "sub r3, r3, #1\n"                // Decrement the loop counter.
//         "cbz r3, dmx_asm_end%=\n"   // If we have sent 24 bits, go to the end.
//         "b send_dmx_bit%=\n"
//
//         "dmx_asm_end%=:\n"
//
//       : "=r" (colors)
//       : "0" (colors),
//         [set] "m" (port->PIO_SODR),
//         [clear] "m" (port->PIO_CODR),
//         [val] "r" (pinValue)
//       : "r3", "r12", "cc"
//       );
//     }
//     __enable_irq();         // Re-enable interrupts now that we are done.
//   }
// }
//
