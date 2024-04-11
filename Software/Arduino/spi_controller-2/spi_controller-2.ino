/* ****************************************************************************
 * This Arduino Uno firmware is used to control a 12-bit ADC128S052 analog to 
 * digital converter (ADC) IC and an AD5628 digital to analog converter (DAC) 
 * IC.
 * 
 * To communicate with the Arduino use a serial over USB serial connection
 * at 115200 baud, 8 data bits, 1 stop bit, no parity, no flow control. 
 * When connecting to the board you can optionally set the DTR line to LOW for 
 * 50ms to force the Arduino to do a hardware reset.
 * 
 * This firmware uses event-based execution driven by serial interrupts.
 * When the user issues a command the relevant IC is selected, a command is 
 * sent and (in the case of the ADC) the response is read.
 * 
 * To read the voltages of the 8 analog inputs send an ASCII CR character over 
 * the serial connection. The values are returned as a string of zero-padded 
 * 4-digit numbers to provide a constant return string length. 
 *
 * To change an analog output channel's voltage send a string with the channel 
 * number (1 based; 1-8), an ASCII space character, then the 12-bit value
 * (0-4095) that corresponds to the desired voltage. For example, if the AD5628
 * voltage reference pin is set to 5V, sending the command:
 * 
 * 1 2047<CR>
 * 
 * will change the output voltage of output channel 1 to 2.5V.
 * 
 * You can change the voltage output of all of the channels simultaneously by 
 * selecting channel zero, e.g:
 * 
 * 0 0<CR>
 * 
 * sets all output channels to 0V.
 *
 * Strings are returned by the Arduino prefixed with MESG if the information
 * is a message to the user, or with DATA if the following string should be
 * interpreted as analog measurement data.
 * 
 * Dr. Chris Empson, School of Chemistry, University of Leeds 2015.
 * https://github.com/mcmont/arduino-12bit-analog-io
 *
 ******************************************************************************/
#include <SPI.h>
#include "pins_arduino.h"

/* AD5628 register addresses - from table 8 of the datasheet. */
#define AD5628_CMD_WRITE_INPUT_REG 0x00000000 /* Write to input register n */
#define AD5628_CMD_UPDATE_DAC_REG  0x1000000 /* Update DAC register n */
#define AD5628_CMD_WRITE_INPUT_REG_UPDATE 0x2000000 /* Equivalent to software LDAC */
#define AD5628_CMD_WRITE_UPDATE_DAC 0x3000000 /* Write to and update DAC channel n */
#define AD5628_CMD_POWER 0x4000000 /* Power down / up DAC */
#define AD5628_CMD_LOAD_CLR_CODE_REG 0x5000000 /* Load clear code register */
#define AD5628_CMD_LOAD_LDAC_REG 0x6000000 /* Load LDAC register */
#define AD5628_CMD_RESET 0x7000000 /* Reset DAC */
#define AD5628_CMD_SETUP_INTERNAL_REF_REG 0x8000000 /* Setup internal REF register */

/* AD5628 input shift register channels - from table 9 of the datasheet. */
#define AD5628_INPUT_DAC_CHANNEL_A 0x000000
#define AD5628_INPUT_DAC_CHANNEL_B 0x100000
#define AD5628_INPUT_DAC_CHANNEL_C 0x200000
#define AD5628_INPUT_DAC_CHANNEL_D 0x300000
#define AD5628_INPUT_DAC_CHANNEL_E 0x400000 
#define AD5628_INPUT_DAC_CHANNEL_F 0x500000
#define AD5628_INPUT_DAC_CHANNEL_G 0x600000 
#define AD5628_INPUT_DAC_CHANNEL_H 0x700000 
#define AD5628_INPUT_DAC_CHANNEL_ALL 0xF00000

/* AD5628 internal reference register bits, from table 10 of the datasheet */
#define AD5628_INTERNAL_REF_OFF 0x00 /* Internal voltage reference off (default) */
#define AD5628_INTERNAL_REF_ON 0x01 /* Internal voltage reference on */

/* AD5628 power-down modes of operation, from table 12 of the datasheet. */
#define AD5628_POWER_MODE_NORMAL 0x000
#define AD5628_POWER_MODE_1KOHM 0x100
#define AD5628_POWER_MODE_100KOHM 0x200
#define AD5628_POWER_MODE_3_STATE 0x300

#define AD5628_POWER_CH_A 0x01
#define AD5628_POWER_CH_B 0x02
#define AD5628_POWER_CH_C 0x04
#define AD5628_POWER_CH_D 0x08
#define AD5628_POWER_CH_E 0x10
#define AD5628_POWER_CH_F 0x20
#define AD5628_POWER_CH_G 0x40
#define AD5628_POWER_CH_H 0x80

/* AD5628 clear code register settings. */
#define AD5628_CC_ZERO 0x00
#define AD5628_CC_MID 0x01
#define AD5628_CC_FULL 0x02
#define AD5628_CC_NO_OP 0x03

/* AD5628 LDAC register. */
#define AD5628_LDAC_CH_A 0x01
#define AD5628_LDAC_CH_B 0x02
#define AD5628_LDAC_CH_C 0x04
#define AD5628_LDAC_CH_D 0x08
#define AD5628_LDAC_CH_E 0x10
#define AD5628_LDAC_CH_F 0x20
#define AD5628_LDAC_CH_G 0x40
#define AD5628_LDAC_CH_H 0x80

/* ADC128S052 Input shift register channels, derived from tables 1 - 3 
 * of the datasheet.
 *
 * Note: There seems to be an error in the datasheet. Pin 11 is shown as being 
 * being input 7, however in reality it appears to be input 0! Pin 4 (IN0) is 
 * actually IN1, pin 5 (IN1) is actually IN2, and so on. 
 *
 * The definitions below have been altered to make the channel numbers match 
 * the pins on the connection diagram on page 1 of the datasheet, so what is shown
 * as pin IN0 can be referred to as ADC128S052_CHANNEL_0 in this Arduino software. */
#define ADC128S052_CHANNEL_7 0x0000
#define ADC128S052_CHANNEL_0 0x0800
#define ADC128S052_CHANNEL_1 0x1000
#define ADC128S052_CHANNEL_2 0x1800
#define ADC128S052_CHANNEL_3 0x2000
#define ADC128S052_CHANNEL_4 0x2800
#define ADC128S052_CHANNEL_5 0x3000
#define ADC128S052_CHANNEL_6 0x3800 

/* Define pins ***************************************************************/

/* We connect an activity LED to pin 2. */
#define ACTIVITY_LED 2

/* Define the SPI slave select (SS) pins for the AD5628 and the ADC128S052. */
#define SS_ADC 8
#define SS_DAC 9

/* Define the DAC power pin. This is usually the Arduino's SS pin, but we need two SS pins, not one. */
#define DAC_POWER 10
/* Other Arduino pins that must be connected to the ICs are:
MOSI 11 (Instruction signals from the Arduino to both the DAC and ADC ICs)
MISO 12 (ADC signals to Arduino)
SCLK 13 (Clock signals from Arduino to both the DAC and ADC ICs) 
*/

/* Standard boolean definitions. */
#define FALSE 0
#define TRUE 1

/* The ASCII character representing a space */
#define ASCII_SPACE 32
/* The ASCII character representing a new line */
#define ASCII_CR 13

/* Define a union data type for the 16-bit ADC128S052 SPI data packet. 
 *
 * A union allows the same memory location to be addressed as different data types.
 * This means we can use the 'word' value to calculate the 16-bit value
 * to be sent to the ADC128S052, and we can send the same value using two of the 
 * Arduino's 8-bit SPI messages.
 */
union data16 { 
  byte packet[2];
  word value;
};

/* Define a union data type for the 32-bit AD5628 data packet. 
 *
 * A union allows the same memory location to be addressed as different data types.
 * This means we can use the unsigned long value to calculate the 32-bit value
 * to be sent to the AD5628, and we can send the same value using four of the 
 * Arduino's 8-bit SPI messages.
 */
union data32 { 
  byte packet[4];
  unsigned long value;
};

/* Function to send data to the DAC */
void DAC_SPI_send_data(union data32 data) {
  short i;
  
  /* Turn on the activity LED */
  digitalWrite(ACTIVITY_LED, HIGH);
  
  /* The AD5628 uses CPOL=0 CPHA=1, so set the transfer mode accordingly. */
  SPI.setDataMode(SPI_MODE1); 
  
  /* Set the DAC slave select pin low, to make it ready to receive SPI data */ 
  digitalWrite(SS_DAC, LOW); 
  /* Send the 32-bit data to the AD5628 in four 8-bit packets.
   * The AD5628 requires the most significant bits to be transferred first, 
   * so in addition to setting the Arduino's SPI protocol to MSBFIRST,
   * we need to send the data.packet[]s in reverse order [3]->[2]->[1]->[0].  */
  for (i=3;i>=0;i--) {
    SPI.transfer(data.packet[i]);
  }
  /* Set the slave select pin for the DAC high to make it ignore the SPI bus */
  digitalWrite(SS_DAC, HIGH);
  /* Turn off the activity LED */
  digitalWrite(ACTIVITY_LED, LOW);
}


/* Set the output voltage for a given channel.
 *
 * Unfortunately the Arduino doesn't have enough floating point precision to enable
 * us to access the full range of the 12-bit output by passing floating point numbers
 * over the serial interface. So, we have to pass the Arduino
 * the 12-bit number over the serial connection, then use the number as 
 * the set-point. */
void DAC_set_output(unsigned long channel, unsigned long setting) {
  short i;
  data32 data;
  
  /* Make sure that the voltage is within the allowed range, to prevent the
   * Arduino from sending invalid data to the IC */
  if (setting > 4095) {
    setting = 4095;
  }
  if (setting < 0) {
    setting = 0;
  }
  
  /* Shift the setting up by 8 bits, to match the input register contents diagram
   * in figure 59 of the datasheet. The 8 least significant bits are "don't care"
   * bits, and shifting them in this way will make them zeros. */
  setting = setting << 8;

  /* Use boolean OR operations to build the 32-bit data to be sent to the DAC*/
  data.value = 0;
  data.value = AD5628_CMD_WRITE_INPUT_REG | channel | setting;

  /* Send the data to the DAC */
  DAC_SPI_send_data(data);
}


/* Write to a register on the DAC */
void DAC_register_write(unsigned long reg, unsigned long setting) {
  short i;
  data32 data;

  data.value = 0;
  data.value = reg | setting;
  
  /* Send the data to the DAC */
  DAC_SPI_send_data(data);
}

/* Run A/D conversion for a given channel and return the voltage */
word ADC_convert(word channel) {
  word result = 0;
  data16 data;

  /* Carry out a bitwise OR to load the channel bits into the data */
  data.value = 0 | channel;

  /* Turn on the activity LED */
  digitalWrite(ACTIVITY_LED, HIGH);

  /* The ADC128S052 uses CPOL=0 CPHA=0, so set the transfer mode accordingly. */
  SPI.setDataMode(SPI_MODE0);

  /* Set ADC slave select pin LOW */
  digitalWrite(SS_ADC, LOW);
  
  /* Start A/D conversion.
   *
   * Send the 16-bit data word to the ADC128S052 in two 8-bit packets.
   * The ADC128S052 requires the most significant bits to be transferred first, 
   * so in addition to setting the Arduino's SPI protocol to MSBFIRST,
   * we need to send the data.packet[]s in reverse order [1]->[0].  */
  result = 0;
  result = SPI.transfer(data.packet[1]); 
  /* Shift the result up by 8 positions. */
  result = result << 8;
  /* Use a logical OR to build the complete data word returned by the ADC128S052.*/
  result |= SPI.transfer(data.packet[0]);
  
  /* Set ADC chip select pin HIGH to power it down. */   
  digitalWrite(SS_ADC, HIGH); 
  /* Turn off the activity LED */
  digitalWrite(ACTIVITY_LED, LOW);  
  /* Return the result (0-4095) */
  return result;
}

/* The setup() function is run automatically as soon as the Arduino is switched on. */
void setup() {

  /* Power up the DAC */
  pinMode(DAC_POWER, OUTPUT);
  digitalWrite(DAC_POWER, HIGH);
  
  /* Switch on the activity LED */
  pinMode(ACTIVITY_LED, OUTPUT);
  digitalWrite(ACTIVITY_LED, HIGH);

  //Serial.begin(115200);
  Serial.begin(9600);
  Serial.println("MESG Firmware version 1.2.2");
  Serial.print("MESG Initialising... ");
   
  /* The Arduino's SPI interface doesn't seem to wake up straight away, so wait for 
   * 3 seconds to give it chance to initialise. */
  delay(3000);
  /* Set SS pin to OUTPUT to prevent the Arduino from entering SPI slave mode. */
  pinMode(SS, OUTPUT);
 
  /* Configure SPI hardware. */
  /* Start SPI hardware. */
  SPI.begin();
  /* Set the bit order to Most Significant Bit first. */
  SPI.setBitOrder(MSBFIRST); 
  /* Divide the Arduino's 16MHz clock down to 4MHz for SCLK. */
  SPI.setClockDivider(SPI_CLOCK_DIV4);
  /* The ADC128S052 uses CPOL=0 CPHA=0, so set the transfer mode accordingly. */
  SPI.setDataMode(SPI_MODE0);
  
  /* Setup the slave select pins for the DAC and ADC. */
  pinMode(SS_DAC, OUTPUT);
  pinMode(SS_ADC, OUTPUT);
  /* Disable the DAC and ADC initially. */
  digitalWrite(SS_DAC, HIGH);  
  digitalWrite(SS_ADC, HIGH);
  
  /* Turn off the AD5628's internal 5V reference. In actual fact it doesn't really 
   * matter whether this is on or off, the accuracy is basically the same 
   * either way. */
  //DAC_register_write(AD5628_CMD_SETUP_INTERNAL_REF_REG, AD5628_INTERNAL_REF_OFF);
  DAC_register_write(AD5628_CMD_SETUP_INTERNAL_REF_REG, AD5628_INTERNAL_REF_ON);

  /* Initialise all flow rates to zero. */
  DAC_set_output(0, 0);
  
  /* Report that the device is ready. */
  Serial.println("Ready.");
  /* Switch off the activity LED to signify that setup is complete. */
  digitalWrite(ACTIVITY_LED, LOW);
}

/* This function is run by the Arduino after setup(). It is empty, 
 * because the firmware runs on serial interrupts. */
void loop() {
}

/* This function prints the values of all 8 A/D channels to the serial port.
 * The list of values from 0-4095 is prefixed by the keyword "DATA " */
void printValues() {

  char padded_int[5];
  Serial.print("DATA ");
  sprintf(padded_int, "%04i", ADC_convert(ADC128S052_CHANNEL_0));
  Serial.print(padded_int);
  Serial.print(" ");  
  sprintf(padded_int, "%04i", ADC_convert(ADC128S052_CHANNEL_1));  
  Serial.print(padded_int);
  Serial.print(" ");  
  sprintf(padded_int, "%04i", ADC_convert(ADC128S052_CHANNEL_2));
  Serial.print(padded_int);
  Serial.print(" ");  
  sprintf(padded_int, "%04i", ADC_convert(ADC128S052_CHANNEL_3));  
  Serial.print(padded_int);
  Serial.print(" ");  
  sprintf(padded_int, "%04i", ADC_convert(ADC128S052_CHANNEL_4));
  Serial.print(padded_int);
  Serial.print(" "); 
  sprintf(padded_int, "%04i", ADC_convert(ADC128S052_CHANNEL_5));
  Serial.print(padded_int);
  Serial.print(" ");  
  sprintf(padded_int, "%04i", ADC_convert(ADC128S052_CHANNEL_6));
  Serial.print(padded_int);
  Serial.print(" ");   
  sprintf(padded_int, "%04i", ADC_convert(ADC128S052_CHANNEL_7));  
  Serial.println(padded_int);
}


/* The serialEvent() function is called automatically after each iteration of 
 * loop() if there is incoming data waiting at the serial port. */
void serialEvent() {

  short i;
  char c;
  String input = "";
  unsigned long channel;
  unsigned long setting;
  byte thisByte;
  boolean spacePresent;
  boolean crPresent;
  
  /* Turn on the LED to show that something's happening. */
  digitalWrite(ACTIVITY_LED, HIGH);
  /* Floating point limitations on the Arduino Uno mean that floating point 
   * numbers are only stored with 2 decimal places of precision. We can't therefore
   * pass floating point numbers to the Arduino with the necessary precision.
   * 
   * We're going to set up the Arduino to receive pairs of data. The channel 
   * number followed by the setting, e.g. "0 1024", "4 22", "7 4095". 
   * So, we need to split these two values by the space between them. We know 
   * that the first byte will always be the channel number, the byte after that 
   * will be the space, and all of the bytes after that will be the setting. */
  spacePresent = FALSE;
  crPresent = FALSE;
  
  /* If any instructions have been passed over the USB serial
   * connection, parse them. */
  while (Serial.available()) { 
    c = Serial.read();
    input.concat(c);
    /* Delay by a couple of ms to allow the data transfer process 
     * to occur properly. We don't want to miss any bytes. */
    delay(1);
  }

  /* Determine size of the array. 
   * We have to add 1 to account for the NULL character. */
  char inputCharArray[input.length()+1];
  input.toCharArray(inputCharArray, sizeof(inputCharArray));
  for (i=0;i<sizeof(inputCharArray);i++) {
    thisByte = inputCharArray[i];
    if (thisByte == ASCII_SPACE) {
      spacePresent = TRUE;
    }
    if (thisByte == ASCII_CR) {
      crPresent = TRUE;
    }
  }
  
  if (spacePresent) {
    channel = atol(&inputCharArray[0]);
    if ((channel < 0) || (channel > 8)) {
      Serial.println("MESG Invalid channel. Specify a channel number (1-8, or 0 for all channels) followed by a 12-bit set point (0-4095).");
    } else {
      /* Create a character array to hold the setting information */
      char settingCharArray[sizeof(inputCharArray)-1];

      /* Copy the setting data from the settingCharArray[] */
      for (i=0;i<sizeof(settingCharArray);i++) {
        settingCharArray[i] = inputCharArray[i+1];
      }

      /* Convert the settingCharArray[] into an integer */
      setting = atoi(settingCharArray);
      /* Make sure that the setting is in the valid range */
      if (setting > 4095) {
        setting = 4095;
      }
      if (setting < 0) {
        setting = 0;
      }
      Serial.print("MESG Setting channel ");
      Serial.print(channel);
      Serial.print(" to value ");
      Serial.println(setting); 
        
      /* Select the appropriate channel constant. */
      switch(channel) {
        case 0:
          /* Setting a value for channel zero will apply the setting to 
           * all of the channels. */
          channel = AD5628_INPUT_DAC_CHANNEL_ALL;
          break;
        case 1:
          channel = AD5628_INPUT_DAC_CHANNEL_A;
          break;
        case 2:
          channel = AD5628_INPUT_DAC_CHANNEL_B;
          break;
        case 3:
          channel = AD5628_INPUT_DAC_CHANNEL_C;
          break;
        case 4:
          channel = AD5628_INPUT_DAC_CHANNEL_D;
          break;
        case 5:
          channel = AD5628_INPUT_DAC_CHANNEL_E;
          break;
        case 6:
          channel = AD5628_INPUT_DAC_CHANNEL_F;  
          break;
        case 7:
          channel = AD5628_INPUT_DAC_CHANNEL_G;
          break;
        case 8:
          channel = AD5628_INPUT_DAC_CHANNEL_H;
          break;
        default:
          /* We should've already picked up any invalid channel identifiers, 
           * but it's good practice to add a handler here just in case. */
          Serial.println("MESG Invalid channel. Specify a channel number (1-8, or 0 for all channels) followed by a 12-bit set point (0-4095).");
          break;
      }
      /* Set the appropriate channel to the desired setting */
      DAC_set_output(channel, setting);
    }
  } else {
    if (crPresent) {
      printValues();
    } else {
      Serial.println("MESG Invalid input. Specify a channel number (1-8, or 0 for all channels) followed by a 12-bit set point (0-4095).");
    }
  } /* End "if (spacePresent) ... " */

  /* Turn off the LED to show that we've finished changing the channel setting. */
  digitalWrite(ACTIVITY_LED, LOW);
} /* End serialEvent() */
