// Based off of openMelt2 melty_config.h
// Which was used to alter commonly changed settings
// May not work on Teensy 4.0 as it is an ARM processor

#ifndef MELTY_CONFIG_GUARD
#define MELTY_CONFIG_GUARD

// _____DIAGNOSTICS_____
// #define DO_DIAGNOSTICS_LOOP
// disable robot -> display config, battery voltage, and RC info via serial 

// _____EEPROM_____
#define ENABLE_EEPROM_STORAGE
#define EEPROM_WRITTEN_SENTINEL_VALUE 42        // Changing this revert EEPROM values to default

// Drift Settings
#define DEFAULT_ACCEL_MOUNT_RADIUS_CM 10        // radius of accelerometer from centre
#define DEFAULT_LED_OFFSET_PERCENT 0            // adjusts heading led direction 0-99 moving clockwise

#define DEFAULT_ACCEL_ZERO_G_OFFSET 0.0f        //Value accelerometer returns with robot at rest (in G) - adjusts for any offset
                                                //H3LIS331 claims +/-1g DC offset - typical - but +/-2.5 has been observed at +/-400g setting (enough to cause tracking error)
                                                //Just enterring and exiting config mode will automatically set this value / save to EEPROM (based on current accel reading reflecting 0g)
                                                //For small-radius bots - try changing to H3LIS331 to +/-200g range for improved accuracy (accel_handler.h)

#define LEFT_RIGHT_HEADING_CONTROL_DIVISOR 1.5f //How quick steering is (larger values = slower)

#define MIN_TRANSLATION_RPM 400                 //full power spin in below this - increasing reduces spin up time

// _____PIN MAPPINGS_____
// RC -> interrupt pins
// 3 interrupt pins needed
// Common settings: CH1 = LEFTRIGHT, CH2 = FORBACK, CH3 - THROTTLE
// (OpenMelt specific, may not work) - accelerometer connect with default SDA / SCL pins

// Change pins if they are different on Teensy 4.0

#define LEFTRIGHT_RC_CHANNEL_PIN 7                //To Left / Right on RC receiver
#define FORBACK_RC_CHANNEL_PIN 1                  //To Forward / Back on RC receiver (Pin 1 on Arduino Micro labelled as "TX" - https://docs.arduino.cc/hacking/hardware/PinMapping32u4)
#define THROTTLE_RC_CHANNEL_PIN 0                 //To Throttle on RC receiver (Pin 0 on Arduino Micro labelled as "RX" - https://docs.arduino.cc/hacking/hardware/PinMapping32u4)

#define HEADING_LED_PIN	8                         //To heading LED (pin 13 is on-board Arduino LED)

//no configuration changes are needed if only 1 motor is used!
#define MOTOR_PIN1 9                              //Pin for Motor 1 driver
#define MOTOR_PIN2 10                             //Pin for Motor 2 driver

#define BATTERY_ADC_PIN A0                        //Pin for battery monitor (if enabled)


// _____THROTTLE CONFIGURATION_____
//THROTTLE_TYPE / High-speed PWM motor driver support:
//Setting THROTTLE_TYPE to FIXED_PWM_THROTTLE or DYNAMIC_PWM_THROTTLE pulses 490Hz PWM signal on motor drive pins at specified duty cycle (0-255)
//Can be used for 2 possible purposes:
//  1. Used as control signal for a brushless ESC supporting high speed (490Hz) PWM (tested with Hobbypower 30A / "Simonk" firmware)
//          Assumes Arduino PWM output is 490Hz (such as Arduino Micro pins 9 and 10) - should be expected NOT to work with non-AVR Arduino's without changes
//  2. 490hz signal maybe fed into a MOSFET or other on / off motor driver to control drive strength (relatively low frequency)
//Thanks to Richard Wong for his research in implementing brushless ESC support

enum throttle_modes {
  BINARY_THROTTLE,      //Motors pins are fully on/off - throttle controlled by portion of each rotation motor is on (no PWM)

  FIXED_PWM_THROTTLE,   //Motors pins are PWM at PWM_MOTOR_ON, PWM_MOTOR_COAST or PWM_MOTOR_OFF
                        //throttle controlled by portion of each rotation motor is on

  DYNAMIC_PWM_THROTTLE  //Scales PWM throttling between PWM_MOTOR_COAST and PWM_MOTOR_ON
                        //Range of throttle scaled over is determined by DYNAMIC_PWM_THROTTLE_PERCENT_MAX
                        //PWM is locked at PWM_MOTOR_ON for throttle positions higher than DYNAMIC_PWM_THROTTLE_PERCENT_MAX
                        //Robot speed is additionally controlled by portion of each rotation motor is on (unless DYNAMIC_PWM_MOTOR_ON_PORTION is defined)
                        //This mode reduces current levels during spin up at part throttle
};

#define THROTTLE_TYPE BINARY_THROTTLE      //<---Throttle type set here!

#define DYNAMIC_PWM_MOTOR_ON_PORTION 0.5f       //if defined (and DYNAMIC_PWM_THROTTLE is set) portion of each rotation motor is on is fixed at this value
                                                //About 0.5f for best translation (higher for increased RPM)

#define DYNAMIC_PWM_THROTTLE_PERCENT_MAX 1.0f   //Range of RC throttle DYNAMIC_PWM_THROTTLE is applied to 
                                                //0.5f for 0-50% throttle (full PWM_MOTOR_ON used for >50% throttle)
                                                //1.0f for 0-100% throttle

//----------PWM MOTOR SETTINGS---------- 
//(only used if a PWM throttle mode is chosen)
//PWM values are 0-255 duty cycle
#define PWM_MOTOR_ON 230                          //Motor PWM ON duty cycle (Simonk: 140 seems barely on / 230 seems a good near-full-throttle value)
#define PWM_MOTOR_COAST 100                       //Motor PWM COAST duty cycle - set to same as PWM_ESC_MOTOR_OFF for fully unpowered (best translation?)
#define PWM_MOTOR_OFF 100                         //Motor PWM OFF duty cycle (Simonk: 100 worked well in testing - if this is too low - ESC may not init)


//----------BATTERY MONITOR----------
#define BATTERY_ALERT_ENABLED                     //if enabled - heading LED will flicker when battery voltage is low
#define VOLTAGE_DIVIDER 11                        //(~10:1 works well - 10kohm to GND, 100kohm to Bat+).  Resistors have tolerances!  Adjust as needed...
#define BATTERY_ADC_WARN_VOLTAGE_THRESHOLD 7.0f  //If voltage drops below this value - then alert is triggered
#define ARDUINIO_VOLTAGE 5.0f                     //Needed for ADC maths for battery monitor
#define LOW_BAT_REPEAT_READS_BEFORE_ALARM 20      //Requires this many ADC reads below threshold before alarming


//----------SAFETY----------
#define ENABLE_WATCHDOG                           //Uses Adafruit's sleepdog to enable watchdog / reset (tested on AVR - should work for ARM https://github.com/adafruit/Adafruit_SleepyDog)
#define WATCH_DOG_TIMEOUT_MS 2000                 //Timeout value for watchdog (not all values are supported - 2000ms verified with Arudino Micro)
#define VERIFY_RC_THROTTLE_ZERO_AT_BOOT           //Requires RC throttle be 0% at boot to allow spin-up for duration of MAX_MS_BETWEEN_RC_UPDATES (about 1 second)
                                                  //Intended as safety feature to prevent bot from spinning up at power-on if RC was inadvertently left on.
                                                  //Downside is if unexpected reboot occurs during a fight - driver will need to set throttle to zero before power 


#include <EEPROM.h>
#include <arduino.h>
#define EEPROM_SENTINEL_VALUE1 EEPROM_SENTINEL_VALUE
#define EEPROM_SENTINEL_VALUE2 EEPROM_SENTINEL_VALUE

#define EEPROM_WRITTEN_SENTINEL_BYTE1_LOC 0
#define EEPROM_WRITTEN_SENTINEL_BYTE2_LOC 1

#define EEPROM_HEADING_LED_LOC 2

//bytes for float
#define EEPROM_ACCEL_RADIUS_BYTE1_LOC 3
#define EEPROM_ACCEL_RADIUS_BYTE2_LOC 4
#define EEPROM_ACCEL_RADIUS_BYTE3_LOC 5
#define EEPROM_ACCEL_RADIUS_BYTE4_LOC 6

//bytes for float
#define EEPROM_ACCEL_OFFSET_BYTE1_LOC 7
#define EEPROM_ACCEL_OFFSET_BYTE2_LOC 8
#define EEPROM_ACCEL_OFFSET_BYTE3_LOC 9
#define EEPROM_ACCEL_OFFSET_BYTE4_LOC 10


//indicates values saved to EEPROM (doing 2x)
static void write_sentinel() {
  EEPROM.write(EEPROM_WRITTEN_SENTINEL_BYTE1_LOC, EEPROM_WRITTEN_SENTINEL1_VALUE);
  EEPROM.write(EEPROM_WRITTEN_SENTINEL_BYTE2_LOC, EEPROM_WRITTEN_SENTINEL2_VALUE);
}

//make sure we actually wrote values before trying to read... (doing 2x)
//if EEPROM_WRITTEN_SENTINEL_VALUE is changed - will cause EEPROM values to invalidate / use default values
static int check_sentinel() {
  if (EEPROM.read(EEPROM_WRITTEN_SENTINEL_BYTE1_LOC) != EEPROM_WRITTEN_SENTINEL1_VALUE) return 0;
  if (EEPROM.read(EEPROM_WRITTEN_SENTINEL_BYTE2_LOC) != EEPROM_WRITTEN_SENTINEL2_VALUE) return 0;
  return 1;
}

void save_settings_to_eeprom(int led_offset, float accel_radius, float accel_zero_g_offset) {
  EEPROM.write(EEPROM_HEADING_LED_LOC, led_offset);
  EEPROM.put(EEPROM_ACCEL_RADIUS_BYTE1_LOC, accel_radius);
  EEPROM.put(EEPROM_ACCEL_OFFSET_BYTE1_LOC, accel_zero_g_offset);
  write_sentinel();
}

int load_heading_led_offset() {
  //if value hasn't been saved previously - return the default
  if (check_sentinel() != 1) return DEFAULT_LED_OFFSET_PERCENT;
  return EEPROM.read(EEPROM_HEADING_LED_LOC);
}

float load_accel_zero_g_offset() {
  //if value hasn't been saved previously - return the default
  if (check_sentinel() != 1) return DEFAULT_ACCEL_ZERO_G_OFFSET;
  float accel_zero_g_offset;
  accel_zero_g_offset = EEPROM.get(EEPROM_ACCEL_OFFSET_BYTE1_LOC, accel_zero_g_offset);
  return accel_zero_g_offset;
}

float load_accel_mount_radius() {
  //if value hasn't been saved previously - return the default
  if (check_sentinel() != 1) return DEFAULT_ACCEL_MOUNT_RADIUS_CM;
  float accel_radius;
  accel_radius = EEPROM.get(EEPROM_ACCEL_RADIUS_BYTE1_LOC, accel_radius);
  return accel_radius;
}

#endif

// config_storage hard coded here, as ARM processors work differently to AVR
