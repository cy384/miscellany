#include "keys.h"
#include "LowPower.h"

//////////////////////////////////////////
#include <Arduino.h>
#include <SPI.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"

#include "bluefruit_config.h"

    #define FACTORYRESET_ENABLE         0
    #define MINIMUM_FIRMWARE_VERSION    "0.6.6"
Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);
// A small helper
void error(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
}
///////////////////////////////////////////
#define LEDPIN 13

int rows[] = {A5, 6, 1 , 2};
int cols[] = {A0, A1, A2, A3, A4, 5, 9, 10, 11, 12, 13};

uint8_t debounce[4][11] = {{0},{0},{0},{0}};

uint8_t debounce_mask = 0b00111111;
uint8_t press_mask    = 0b00011111;
uint8_t release_mask  = 0b00110000;

int fn = 0;

uint8_t KEY_FN = 0xFF;

bool powersave = false;

uint8_t usb_hid[8] = {0, 0, 0, 0, 0, 0, 0, 0};


/* AT+FACTORYRESET
 *  clear bonds etc.
 * AT+BAUDRATE=9600
 * get info
 * ATI
 * AT+HWMODELED=DISABLE
 * AT+HWMODELED=MANUAL,ON
 * AT+HWMODELED=MANUAL,TOGGLE
 * AT+HWMODELED=MANUAL,OFF
 * max power
 * AT+BLEPOWERLEVEL=4
 * min power
 * AT+BLEPOWERLEVEL=-40
 * AT+BLEGETRSSI
 * gets connection quality
 * 0 no connect
 * more negative: good
 * AT+BLEKEYBOARDEN=1
 * enable ble keyboard
 * must then reset via ATZ
 * AT+BLEKEYBOARD=a
 * send a, must escape some
 * AT+BLEBATTEN=1
 * AT+BLEBATTVAL=72
 * in percent
 * AT+GAPDISCONNECT
 * disconnect
 * AT+GAPDEVNAME=BLEFriend
 * set name
 * must do a reset aftewards ATZ
 * AT+GAPINTERVALS
 * look at the docs for this
*/

// see https://learn.adafruit.com/adafruit-feather-32u4-bluefruit-le/ble-services#at-plus-blekeyboardcode-14-25

float measure_bat(void)
{
    #define VBATPIN A9

    clear_pins();
    
    pinMode(A9, INPUT);
       
    float measuredvbat = analogRead(VBATPIN);
    measuredvbat *= 2;    // we divided by 2, so multiply back
    measuredvbat *= 3.3;  // Multiply by 3.3V, our reference voltage
    measuredvbat /= 1024; // convert to voltage

    clear_pins();
    return measuredvbat;
}

// very approximate, for low-load situation
// could take actual measurements to find out...
int voltage_to_percent(float vbat)
{
  if (vbat > 4.1f) return 95;
  if (vbat > 4.0f) return 90;
  if (vbat > 3.9f) return 80;
  if (vbat > 3.8f) return 60;
  if (vbat > 3.7f) return 40;
  if (vbat > 3.6f) return 20;
  return 5;
}

uint8_t keymap[2][4][11] = 
{
  {
    {KEY_P, KEY_O, KEY_I, KEY_U, KEY_Y, 0, KEY_T, KEY_R, KEY_E, KEY_W, KEY_Q},
    {KEY_SEMICOLON, KEY_L, KEY_K, KEY_J, KEY_H, 0, KEY_G, KEY_F, KEY_D, KEY_S, KEY_A},
    {KEY_SLASH, KEY_DOT, KEY_COMMA, KEY_M, KEY_N, KEY_LEFTALT, KEY_B, KEY_V, KEY_C, KEY_X, KEY_Z},
    {KEY_ENTER, KEY_APOSTROPHE, KEY_MINUS, KEY_FN, KEY_SPACE, KEY_LEFTCTRL, KEY_BACKSPACE, KEY_LEFTSHIFT, KEY_LEFTMETA, KEY_TAB, KEY_ESC}

  },
  {
    {KEY_0, KEY_9, KEY_8, KEY_7, KEY_6, 0, KEY_5, KEY_4, KEY_3, KEY_2, KEY_1},
    {0xc9, KEY_RIGHT, KEY_UP, KEY_LEFT, KEY_EQUAL, 0, KEY_GRAVE, KEY_KPRIGHTPAREN, KEY_KPLEFTPAREN, 0xb9, 0xb8},
    {KEY_BACKSLASH, 0xc7, KEY_DOWN, KEY_KPASTERISK, KEY_KPPLUS, KEY_LEFTALT, KEY_GRAVE, KEY_RIGHTBRACE, KEY_LEFTBRACE, 0xc3, 0xc4},
    {KEY_ENTER, KEY_APOSTROPHE, KEY_MINUS, KEY_FN, KEY_SPACE, KEY_LEFTCTRL, KEY_DELETE, KEY_LEFTSHIFT, KEY_LEFTMETA, KEY_TAB, KEY_ESC}
  }
};

void clear_pins()
{
  for (int i = 0; i < 4; i++) pinMode(rows[i], INPUT);
  for (int i = 0; i < 11; i++) pinMode(cols[i], INPUT_PULLUP);
}

void sleep_pins()
{
  for (int i = 0; i < 4; i++) pinMode(rows[i], INPUT);
  for (int i = 0; i < 11; i++) pinMode(cols[i], INPUT);
}

void send_hid()
{
  //Serial.print("hid: ");
/*
  for (int i = 0; i < 8; i++)
  {
    for (int j = 7; j >= 0; j--)
    {
      int b = (usb_hid[i] >> j) & 1;
      //Serial.print(b);
    }
    
    //Serial.print(" ");
    }
*/
  //Serial.print("\n");
/*
  // example string: "AT+BLEKEYBOARDCODE=02-00-04-05-06-00-00"
  //ble.print("AT+BLEKEYBOARDCODE=");
  Serial.print("AT+BLEKEYBOARDCODE=");
  for (int i = 0; i < 8; i++)
  {
    //ble.print(usb_hid[i], HEX);
    Serial.print(usb_hid[i], HEX);
    //if (i != 7) ble.print("-");
    if (i != 7) Serial.print("-");
  }
  Serial.print("\n");
  //ble.print("\n");*/
  char buffer[48] = {0};
  snprintf(buffer, 48, "AT+BLEKEYBOARDCODE=%02X-%02X-%02X-%02X-%02X-%02X-%02X\n", usb_hid[0], usb_hid[1], usb_hid[2], usb_hid[3], usb_hid[4], usb_hid[5], usb_hid[6]);
  ble.print(buffer);
  //Serial.print(buffer);
}

void press_key(uint8_t k)
{
  switch(k)
  {
    case KEY_LEFTCTRL:
      usb_hid[0] |= (uint8_t) KEY_MOD_LCTRL;
      break;
    case KEY_LEFTSHIFT:
      usb_hid[0] |= (uint8_t) KEY_MOD_LSHIFT;
      break;
    case KEY_LEFTALT:
      usb_hid[0] |= (uint8_t) KEY_MOD_LALT;
      break;
    case KEY_LEFTMETA:
      usb_hid[0] |= (uint8_t) KEY_MOD_LMETA;
      break;
    case KEY_RIGHTCTRL:
      usb_hid[0] |= (uint8_t) KEY_MOD_RCTRL;
      break;
    case KEY_RIGHTSHIFT:
      usb_hid[0] |= (uint8_t) KEY_MOD_RSHIFT;
      break;
    case KEY_RIGHTALT:
      usb_hid[0] |= (uint8_t) KEY_MOD_RALT;
      break;
    case KEY_RIGHTMETA:
      usb_hid[0] |= (uint8_t) KEY_MOD_RMETA;
      break;
    default:
      for (int i = 2; i < 8; i++)
      {
        if (usb_hid[i] == 0)
        {
          usb_hid[i] = k;
          break;
        }
      }
    break;
  }
}

void release_key(uint8_t k)
{
  switch(k)
  {
    case KEY_LEFTCTRL:
      usb_hid[0] &= ~(KEY_MOD_LCTRL);
      break;
    case KEY_LEFTSHIFT:
      usb_hid[0] &= ~(KEY_MOD_LSHIFT);
      break;
    case KEY_LEFTALT:
      usb_hid[0] &= ~(KEY_MOD_LALT);
      break;
    case KEY_LEFTMETA:
      usb_hid[0] &= ~(KEY_MOD_LMETA);
      break;
    case KEY_RIGHTCTRL:
      usb_hid[0] &= ~(KEY_MOD_RCTRL);
      break;
    case KEY_RIGHTSHIFT:
      usb_hid[0] &= ~(KEY_MOD_RSHIFT);
      break;
    case KEY_RIGHTALT:
      usb_hid[0] &= ~(KEY_MOD_RALT);
      break;
    case KEY_RIGHTMETA:
      usb_hid[0] &= ~(KEY_MOD_RMETA);
      break;
    default:
      for (int i = 2; i < 8; i++)
      {
        if (usb_hid[i] == k)
        {
          usb_hid[i] = 0;
          for (int j = i; j < 7; j++) usb_hid[j] = usb_hid[j+1];
          usb_hid[7] = 0;
        }
      }
    break;
  }
}

void clear_hid(void)
{
  for (int i = 0; i < 7; i++)
  {
    usb_hid[i] = 0;
  }
}

void setup(void)
{
  clear_pins();

  Serial.begin(57600);
  
  for (int i = 0; i < 10; i++)
  {
    Serial.print(i);
    delay(1000);
  }
  
  Serial.print(" ready.\n");
  
  clear_hid();

////////////////////////////////////////////////////////////
  if ( !ble.begin(VERBOSE_MODE) )
  {
    error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
  }
  if ( FACTORYRESET_ENABLE )
  {
    /* Perform a factory reset to make sure everything is in a known state */
    Serial.println(F("Performing a factory reset: "));
    if ( ! ble.factoryReset() ){
      error(F("Couldn't factory reset"));
    }
  }

  /* Disable command echo from Bluefruit */
  ble.echo(false);

  Serial.println("Requesting Bluefruit info:");
  /* Print Bluefruit information */
  ble.info();
  /* Change the device name to make it easier to find */
  Serial.println(F("Setting device name to 'blue_atreus': "));
  if (! ble.sendCommandCheckOK(F( "AT+GAPDEVNAME=blue_atreus" )) ) {
    error(F("Could not set device name?"));
  }

  Serial.println(F("Disabling status LED: "));
  if (! ble.sendCommandCheckOK(F("AT+HWMODELED=DISABLE")))
  {
    error(F("Could not disable status LED?"));
  }

  /* Enable HID Service */
  Serial.println(F("Enable HID Service (including Keyboard): "));
  if ( ble.isVersionAtLeast(MINIMUM_FIRMWARE_VERSION) )
  {
    if ( !ble.sendCommandCheckOK(F( "AT+BleHIDEn=On" ))) {
      error(F("Could not enable Keyboard"));
    }
  }else
  {
    if (! ble.sendCommandCheckOK(F( "AT+BleKeyboardEn=On"  ))) {
      error(F("Could not enable Keyboard"));
    }
  }

  /* Add or remove service requires a reset */
  Serial.println(F("Performing a SW reset (service changes require a reset): "));
  if (! ble.reset() ) {
    error(F("Couldn't reset??"));
  }
////////////////////////////////////////////////////////////
}

bool scan_matrix()
{
  bool change = false;
  
    for (int i = 0; i < 4; i++)
    {
      pinMode(rows[i], OUTPUT);

      digitalWrite(rows[i], LOW);

      for (int j = 0; j < 11; j++)
      {
        debounce[i][j] <<= 1;

        if (digitalRead(cols[j]) == LOW)
        {
          debounce[i][j] |= 0b00000001;
        }

        if ((debounce[i][j] & debounce_mask) == press_mask)
        {
          if (keymap[0][i][j] == KEY_FN)
          {
            fn = 1;
          }
          else
          {
            press_key(keymap[fn][i][j]);
            change = true;
          }
        }
        else if ((debounce[i][j] & debounce_mask) == release_mask)
        {
          if (keymap[0][i][j] == KEY_FN)
          {
            fn = 0;
          }
          else
          {
            release_key(keymap[0][i][j]);
            release_key(keymap[1][i][j]);
            change = true;
          }
        }
      }

      pinMode(rows[i], INPUT);
    }

    return change;
}

period_t sleep_amount(unsigned long last_event)
{
  period_t sleep_amounts[11] = { SLEEP_15MS, SLEEP_30MS, SLEEP_60MS, SLEEP_120MS, SLEEP_250MS, SLEEP_500MS, SLEEP_1S, SLEEP_2S, SLEEP_4S, SLEEP_8S, SLEEP_FOREVER};

  return sleep_amounts[3];
}

void loop()
{
  unsigned long last_event = millis();
  unsigned long last_led_off = millis();

  while (1)
  {
    if (scan_matrix())
    {
      last_event = millis();
      send_hid();
    }
    else if (millis() - last_event > 30000)
    {
      clear_hid();
      send_hid();
      sleep_pins();
      LowPower.idle(sleep_amount(last_event), ADC_OFF, TIMER4_OFF, TIMER3_OFF, TIMER1_OFF, TIMER0_OFF, SPI_OFF, USART1_OFF, TWI_OFF, USB_OFF);
      send_hid();
      clear_pins();
    }

    if (millis() - last_led_off > 30000)
    {
      last_led_off = millis();
      //AT+HWGPIOMODE=14,1
      Serial.println(F("Turning off connected LED"));
      if (!ble.sendCommandCheckOK(F("AT+HWGPIO=19,0")))
      {
        Serial.println(F("Failed to turn off connected LED"));
      }
    }
  }
}
