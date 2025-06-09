#include <Servo.h>
#define PI 3.1415926

Servo servoHead, servoNeck, servoElbow, servoUpper, servoForearm, servoBase;

float t = 0;
int mode = 0;
unsigned long lastChange = 0;
int modeDuration = 10000 + random(5000); // 每 10-15 秒切換一次

float currHead = 90, currNeck = 90, currElbow = 90;
float currUpper = 90, currForearm = 90, currBase = 90;

// 插值
float lerp(float a, float b, float f) { return a + (b - a) * f; }
// easing
float easeInOutSine(float x) { return -(cos(PI * x) - 1.0) / 2.0; }

void setup() {
  Serial.begin(115200);
  servoHead.attach(D2);
  servoNeck.attach(D3);
  servoElbow.attach(D4);
  servoUpper.attach(D5);
  servoForearm.attach(D6);
  servoBase.attach(D7);
}

void loop() {
  if (millis() - lastChange > modeDuration) {
    mode = random(0, 7);  // 0~5
    lastChange = millis();
    modeDuration = 10000 + random(5000); // reset duration
  }

  t += 0.02;

  // 安全角度參數
  float headMid = 90, headRange = 40;
  float neckMid = 90, neckRange = 40;
  float elbowMid = 90, elbowRange = 90;
  float upperMid = 90, upperRange = 80;
  float forearmMid = 90, forearmRange = 70;
  float baseMid = 90, baseRange = 60;

  // 呼吸波
  float breath = sin(t * 0.3);

  // 生物波動推進
  float pHead     = easeInOutSine(fmod(t + 0.0, 1.0));
  float pNeck     = easeInOutSine(fmod(t + 0.1, 1.0));
  float pElbow    = easeInOutSine(fmod(t + 0.2, 1.0));
  float pUpper    = easeInOutSine(fmod(t + 0.3, 1.0));
  float pForearm  = easeInOutSine(fmod(t + 0.4, 1.0));
  float pBase     = easeInOutSine(fmod(t + 0.5, 1.0));

  // 初始化目標角度
  float targetHead = headMid, targetNeck = neckMid;
  float targetElbow = elbowMid, targetUpper = upperMid;
  float targetForearm = forearmMid, targetBase = baseMid;

  // 動作模式切換（每個 case 可依生物行為細緻調）
  switch (mode) {
    case 0: // idle breathing
      targetHead = headMid + 5 * pHead + 3 * breath;
      targetNeck = neckMid + 5 * pNeck + 2 * breath;
      targetElbow = elbowMid + 8 * pElbow + 1 * breath;
      targetUpper = upperMid + 6 * pUpper;
      targetForearm = forearmMid + 6 * pForearm;
      targetBase = baseMid + 5 * breath;
      break;

    case 1: // look around
      targetHead = headMid + headRange * sin(t * 2.0);
      targetNeck = neckMid + neckRange * sin(t * 1.5);
      targetElbow = elbowMid + 10 * pElbow;
      targetUpper = upperMid + 8 * breath;
      targetForearm = forearmMid + 5;
      targetBase = baseMid + baseRange * sin(t * 0.5);
      break;

    case 2: // stretching
      targetHead = headMid + 10 * breath;
      targetNeck = neckMid + 8 * pNeck;
      targetElbow = elbowMid + 70 * pElbow;
      targetUpper = upperMid + 60 * pUpper;
      targetForearm = forearmMid + 50 * pForearm;
      targetBase = baseMid + 10 * pBase;
      break;

    case 3: // reaching
      targetHead = headMid + 5 * sin(t * 1.0);
      targetNeck = neckMid + 10 * sin(t * 0.9);
      targetElbow = elbowMid + 25 * pElbow;
      targetUpper = upperMid + 60 * pUpper;
      targetForearm = forearmMid + 60 * sin(t * 1.3);
      targetBase = baseMid + 10 * sin(t * 0.5);
      break;

    case 4: // dodging
      targetHead = headMid - 15 * sin(t * 2.3);
      targetNeck = neckMid - 10 * sin(t * 2.1);
      targetElbow = elbowMid - 30 * sin(t * 1.8);
      targetUpper = upperMid - 40 * sin(t * 1.5);
      targetForearm = forearmMid - 30 * sin(t * 1.3);
      targetBase = baseMid + 30 * sin(t * 1.2);
      break;

    case 5: // twitching
      targetHead = headMid + 3 * sin(t * 4.5);
      targetNeck = neckMid + 3 * sin(t * 5.0);
      targetElbow = elbowMid + 20 * sin(t * 6.0);
      targetUpper = upperMid + 25 * sin(t * 6.5);
      targetForearm = forearmMid + 15 * sin(t * 5.8);
      targetBase = baseMid + 5 * sin(t * 3.5);
      break;
    
    case 6: // enhanced stretching
      targetHead     = headMid + 10 * easeInOutSine(fmod(t, 1.0));       // 上仰
      targetNeck     = neckMid - 5 * easeInOutSine(fmod(t + 0.2, 1.0));  // 後縮
      targetElbow    = elbowMid + 80 * easeInOutSine(fmod(t + 0.3, 1.0));
      targetUpper    = upperMid + 70 * easeInOutSine(fmod(t + 0.4, 1.0));
      targetForearm  = forearmMid + 60 * easeInOutSine(fmod(t + 0.5, 1.0));
      targetBase     = baseMid - 10 * easeInOutSine(fmod(t + 0.6, 1.0)); // 後傾
      break;

  }

  // 平滑角度（慣性感）
  currHead     = constrain(lerp(currHead, targetHead, 0.1), 10, 170);
  currNeck     = constrain(lerp(currNeck, targetNeck, 0.1), 10, 170);
  currElbow    = constrain(lerp(currElbow, targetElbow, 0.08), 0, 180);
  currUpper    = constrain(lerp(currUpper, targetUpper, 0.08), 10, 170);
  currForearm  = constrain(lerp(currForearm, targetForearm, 0.08), 20, 160);
  currBase     = constrain(lerp(currBase, targetBase, 0.06), 30, 150);

  // 寫入馬達
  servoHead.write((int)currHead);
  servoNeck.write((int)currNeck);
  servoElbow.write((int)currElbow);
  servoUpper.write((int)currUpper);
  servoForearm.write((int)currForearm);
  servoBase.write((int)currBase);

  delay(20);
}
