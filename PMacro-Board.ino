// PMacro-Board.ino
//
// Custom Driver for Hotkeys and Scripting
// For use with the Programmable Macro Board by loklokfafa
// 
// https://loklokfafa.com/products/programmable-macro-board
// Copyright (c) 2022 loklokfafa
//
// Last Update: 4/21/2022
// Board Version: v2
// Controls: x7 Mechanical Keys, x2 Rotary Encoders, x2 Buttons (on Encoders)

// Import Dependencies
#include <Keyboard.h>

/////////////////////////////////////////////////////////////////
// CHANGE THIS PART FOR CUSTOM COMMANDS //
#define BT_A_CMD 'd' // Leftmost 1U Key Command
#define BT_B_CMD 'f' // Middle left 1U Key Command
#define BT_C_CMD 'j' // Middle right 1U Key Command
#define BT_D_CMD 'k' // Rightmost 1U Key Command

#define FX_L_CMD 'c'   // Bottom Left 2U Key Command
#define FX_R_CMD 'm'   // Bottom Right 2U Key Command

#define BT_ST_CMD 'y'  // Start Button Key Command

#define ENC_LL_CMD 'w' // Left Encoder Left Turn Command
#define ENC_LR_CMD 'e' // Left Encoder Right Turn Command
#define ENC_RL_CMD 'o' // Right Encoder Left Turn Command
#define ENC_RR_CMD 'p' // Right Encoder Right Turn Command

char ENC_BTNL_CMD = KEY_LEFT_ARROW;   // Left Encoder Button Command
char ENC_BTNR_CMD = KEY_RIGHT_ARROW;  // Right Encoder Button Command
/////////////////////////////////////////////////////////////////

// Define Mechanical Keys and Button Connections
// Specific to the P-Macro Board. DO NOT CHANGE
#define BT_A 9
#define BT_B 8
#define BT_C 7
#define BT_D 6
#define FX_L 14
#define FX_R 15
#define BT_ST 16
#define enc_btn_L 5
#define enc_btn_R 4

// Define Encoders according to specified library
// Specific to the P-Macro Board. DO NOT CHANGE
#include "Encoder.h"
Encoder enc1(2,3);
Encoder enc2(0,1);
int16_t knob1 = 0;
int16_t knob2 = 0;
int8_t old_knob1 = 0;
int8_t old_knob2 = 0;
long timesincelastrot1 = 0;
long timesincelastrot2 = 0;

struct button{
  int pin;
  char key;
} buttons[7];

// Setup Function: Runs one time when the device powers on
void setup()
{
  buttons[0] = {BT_A,  BT_A_CMD};
  buttons[1] = {BT_B,  BT_B_CMD};
  buttons[2] = {BT_C,  BT_D_CMD};
  buttons[3] = {BT_D,  BT_D_CMD};
  buttons[4] = {FX_L,  FX_L_CMD};
  buttons[5] = {FX_R,  FX_R_CMD};
  buttons[6] = {BT_ST, BT_ST_CMD};

  // set pins to read, and output high
  // when the pins read low, we know that the buttons have been grounded (aka, pushed down)
  
  // Set pins to INPUT mode with PULLUP to high, we want them to read key presses with a LOW read from the device
  for (int i = 0; i < 7; i++)
    pinMode(buttons[i].pin, INPUT_PULLUP);
  
  // Set encoder button to INPUT mode, we want them to read data from the device
  pinMode(enc_btn_L, INPUT);
  pinMode(enc_btn_R, INPUT);

  // Begin keyboard
  Keyboard.begin();
  Keyboard.releaseAll();
}

void loop()
{
  // Read Information from Encoders
  knob1 = enc1.read();
  update_knobs1();
  
  knob2 = enc2.read();
  update_knobs2();

  // Read Button Presses
  // If button is HIGH, then it is NOT pressed
  // If button is LOW, then it IS pressed
  for (int i = 0; i < 7; i++) {
    if (digitalRead(buttons[i].pin))
      Keyboard.release(buttons[i].key);
    else
      Keyboard.press(buttons[i].key);
  }

  if (digitalRead(enc_btn_L) == LOW)
  {
    Keyboard.press(ENC_BTNL_CMD);
  }
  else
  {
    Keyboard.release(ENC_BTNL_CMD);
  }

  if (digitalRead(enc_btn_R) == LOW)
  {
    Keyboard.press(ENC_BTNR_CMD);
  }
  else
  {
    Keyboard.release(ENC_BTNR_CMD);
  }
}

// Check Encoder Knobs for updates. Detents expose changes in polarity.
void update_knobs1()
{
  if (knob1 != old_knob1)
  {
    // if there's a difference in encoder movement from last pass, press button
    timesincelastrot1 = millis();
    if (knob1 < old_knob1)
    {
      Keyboard.press(ENC_LR_CMD);
    }
    else
    {
      Keyboard.press(ENC_LL_CMD);
    }

    // if overflown, then write back to the encoder
    if ((old_knob1 == knob1) != knob1)
    {
      enc1.write(old_knob1);
    }
  }
  else
  {
    Keyboard.release(ENC_LL_CMD);
    Keyboard.release(ENC_LR_CMD);
  }
}

void update_knobs2()
{
  if (knob2 != old_knob2)
  {
    // if there's a difference in encoder movement from last pass, press button
    timesincelastrot2 = millis();
    if (knob2 < old_knob2)
    {
      Keyboard.press(ENC_RL_CMD);
    }
    else
    {
      Keyboard.press(ENC_RR_CMD);
    }

    // if overflown, then write back to the encoder
    if ((old_knob2 = knob2) != knob2)
    {
      enc2.write(old_knob2);
    }
  }
  else
  {
    Keyboard.release(ENC_RR_CMD);
    Keyboard.release(ENC_RL_CMD);
  }
}
