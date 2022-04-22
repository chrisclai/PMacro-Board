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
#include <Mouse.h>
#include <Keyboard.h>

/////////////////////////////////////////////////////////////////
// CHANGE THIS PART FOR CUSTOM COMMANDS //
char BT_A_CMD = 'd'; // Leftmost 1U Key Command
char BT_B_CMD = 'f'; // Middle left 1U Key Command
char BT_C_CMD = 'j'; // Middle right 1U Key Command
char BT_D_CMD = 'k'; // Rightmost 1U Key Command

char FX_L_CMD = 'c';   // Bottom Left 2U Key Command
char FX_R_CMD = 'm';   // Bottom Right 2U Key Command

char BT_ST_CMD = 'y';  // Start Button Key Command

char ENC_LL_CMD = 'w'; // Left Encoder Left Turn Command
char ENC_LR_CMD = 'e'; // Left Encoder Right Turn Command
char ENC_RL_CMD = 'o'; // Right Encoder Left Turn Command
char ENC_RR_CMD = 'p'; // Right Encoder Right Turn Command

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

// Setup Function: Runs one time when the device powers on
void setup()
{
  // set pins to read, and output high
  // when the pins read low, we know that the buttons have been grounded (aka, pushed down)
  
  // Set pins to INPUT mode, we want them to read data from the device
  pinMode(BT_A, INPUT);
  pinMode(BT_B, INPUT);
  pinMode(BT_C, INPUT);
  pinMode(BT_D, INPUT);
  pinMode(FX_L, INPUT);
  pinMode(FX_R, INPUT);
  pinMode(BT_ST, INPUT);

  // Set encoder button to INPUT mode, we want them to read data from the device
  pinMode(enc_btn_L, INPUT);
  pinMode(enc_btn_R, INPUT);

  // Preset keys to HIGH, when keys are pressed will read LOW
  digitalWrite(BT_A, HIGH);
  digitalWrite(BT_B, HIGH);
  digitalWrite(BT_C, HIGH);
  digitalWrite(BT_D, HIGH);
  digitalWrite(FX_L, HIGH);
  digitalWrite(FX_R, HIGH);
  digitalWrite(BT_ST, HIGH);

  // Begin mouse and keyboard
  Mouse.begin();
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
  if (digitalRead(BT_A) == LOW)
  {
    Keyboard.press(BT_A_CMD);
  }
  else
  {
    Keyboard.release(BT_A_CMD);
  }

  if (digitalRead(BT_B) == LOW)
  {
    Keyboard.press(BT_B_CMD);
  }
  else
  {
    Keyboard.release(BT_B_CMD);
  }

  if (digitalRead(BT_C) == LOW)
  {
    Keyboard.press(BT_C_CMD);
  }
  else
  {
    Keyboard.release(BT_C_CMD);
  }

  if (digitalRead(BT_D) == LOW)
  {
    Keyboard.press(BT_D_CMD);
  }
  else
  {
    Keyboard.release(BT_D_CMD);
  }

  if (digitalRead(FX_L) == LOW)
  {
    Keyboard.press(FX_L_CMD);
  }
  else
  {
    Keyboard.release(FX_L_CMD);
  }

  if (digitalRead(FX_R) == LOW)
  {
    Keyboard.press(FX_R_CMD);
  }
  else
  {
    Keyboard.release(FX_R_CMD);
  }

  if (digitalRead(BT_ST) == LOW)
  {
    Keyboard.press(BT_ST_CMD);
  }
  else
  {
    Keyboard.release(BT_ST_CMD);
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
