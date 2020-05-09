#include <Arduino.h>
#include <OneButton.h>
#include <BleKeyboard.h>
#include "configs.h"

BleKeyboard bleKeyboard("Streamdeck DIY", "Alvaro Viebrantz", 50);

OneButton button1(16, true);
OneButton button2(4, true);
OneButton button3(13, true);
OneButton button4(17, true);
OneButton button5(14, true);
OneButton button6(12, true);

OneButton buttons[NUM_BUTTON] = {button1, button2, button3, button4, button5, button6};
int buttonIndexes[NUM_BUTTON] = {0, 1, 2, 3, 4, 5};

//KeyboardConfig keyboardConfig = testKeyboardConfig;
KeyboardConfig keyboardConfig = obsConfig;

void onClick(void *p);

void setup()
{
  while (!Serial)
  {
  }

  Serial.begin(9600);

  for (int i = 0; i < NUM_BUTTON; i++)
  {
    buttons[i].attachClick(onClick, &buttonIndexes[i]);
  }

  bleKeyboard.begin();
}

void onClick(void *p)
{
  int index = *((int *)p);
  Serial.print("Button ");
  Serial.print(index + 1);
  Serial.println(" Pressed");

  if (bleKeyboard.isConnected())
  {
    //bleKeyboard.print("Hello world");

    KeyConfig keyConfig = keyboardConfig.keysConfig[index];
    switch (keyConfig.actionType)
    {
    case MEDIA:
    {
      bleKeyboard.write(keyConfig.keys);
      break;
    }
    case TEXT:
    {
      bleKeyboard.printf("%s", keyConfig.keys);
      break;
    }
    case KEYSTROKE:
    {
      bleKeyboard.printf("%c", keyConfig.key);
      break;
    }
    case KEY_COMBINATION:
    {
      int len = strlen((const char *)keyConfig.keys);
      for (int i = 0; i < len; i++)
      {
        bleKeyboard.press(keyConfig.keys[i]);
      }
      bleKeyboard.press(keyConfig.key);
      delay(100);
      bleKeyboard.releaseAll();
      break;
    }
    default:
      Serial.println("Unknow type");
      break;
    }
  }
}

void loop()
{
  for (int i = 0; i < NUM_BUTTON; i++)
  {
    buttons[i].tick();
  }
  delay(10);
}