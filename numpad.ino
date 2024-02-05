#include <BleKeyboard.h>
#include <Keypad.h>

#define ENABLE_LOGS 1 // 开启或关闭日志

#if ENABLE_LOGS
#define LOG(x)  Serial.println(x)
#define LOG_HEX(x)  Serial.println(x, HEX)
#else
#define LOG(x)
#endif

// 键盘设置
const byte ROWS = 5; // 五行
const byte COLS = 4; // 四列

const uint8_t KEY_NUM_LOCK = 0xDB;

char keys[ROWS][COLS] = {
  {KEY_NUM_LOCK,  KEY_NUM_SLASH,  KEY_NUM_ASTERISK,  KEY_NUM_MINUS},
  {KEY_NUM_7,     KEY_NUM_8,      KEY_NUM_9,         KEY_NUM_PLUS},
  {KEY_NUM_4,     KEY_NUM_5,      KEY_NUM_6,         NO_KEY},
  {KEY_NUM_1,     KEY_NUM_2,      KEY_NUM_3,         KEY_NUM_ENTER},
  {KEY_NUM_0,     NO_KEY,         KEY_NUM_PERIOD,    NO_KEY}
};

byte rowPins[ROWS] = {6, 7, 8, 9, 10};
byte colPins[COLS] = {0, 1, 2, 3};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
BleKeyboard bleKeyboard("ESP32-Keypad", "Espressif", 100);

void setup() {
  Serial.begin(9600);
  bleKeyboard.begin();
  LOG("Bluetooth Keyboard Started");
}

void loop() {
  if (bleKeyboard.isConnected()) {
    char key = keypad.getKey();
    
    if (key) {
      LOG(String("Pressed: "));
      LOG_HEX(key);
      bleKeyboard.press(key); // 发送按键
      delay(100); // 简单的防抖动
      bleKeyboard.release(key); // 释放按键
    }
  } else {
    LOG("Bluetooth not connected");
    delay(500); // 当蓝牙未连接时降低循环速度
  }
}