/*
 * 檔案名稱：Righteous Branch.ino (義枝)
 * 版本：2.0.1
 * 日期：2025年6月
 * 
 * 功能說明：
 * ===========
 * 本程式控制具有六個伺服馬達的機械裝置，模擬有機生命體的各種動作。
 * 通過數學函數和漸變效果創造出流暢、自然且不可預測的運動模式，
 * 使機械結構展現出類似生物的行為特徵。
 * 
 * 硬體需求：
 * ===========
 * - ESP8266 LOLIN D1 R1 開發板
 * - 6個伺服馬達，建議型號 SG90 或 MG996R（視負載需求）
 * - 適當的電源供應（推薦 5V 3A 以上）
 * 
 * 針腳連接：
 * ===========
 * - D2: 頭部伺服 (Head) - 控制頭部旋轉
 * - D3: 頸部伺服 (Neck) - 控制頸部傾斜
 * - D4: 肘部伺服 (Elbow) - 控制肘部彎曲
 * - D5: 上臂伺服 (Upper) - 控制上臂旋轉
 * - D6: 前臂伺服 (Forearm) - 控制前臂旋轉 
 * - D7: 基座伺服 (Base) - 控制基座旋轉
 * 
 * 運動模式說明：
 * ===========
 * 0. 閒置呼吸 (Idle Breathing)：
 *    模擬生物輕微的呼吸動作，表現靜止狀態下的微小生命跡象。
 *    
 * 1. 環顧四周 (Look Around)：
 *    頭部和基座緩慢轉動，呈現如同生物觀察環境的行為。
 *    
 * 2. 伸展 (Stretching)：
 *    模擬生物伸展肢體的動作，所有關節有協調地拉伸。
 *    
 * 3. 伸手夠取 (Reaching)：
 *    前傾並伸展前臂，像是嘗試接觸或夠取物體。
 *    
 * 4. 閃避 (Dodging)：
 *    模擬迅速避開威脅的動作，關節有協調的反向運動。
 *    
 * 5. 抽搐 (Twitching)：
 *    產生輕微且快速的顫動，模擬神經反應或警覺狀態。
 *    
 * 6. 增強伸展 (Enhanced Stretching)：
 *    更大幅度的伸展動作，各關節有序延遲運動，創造波浪感。
 * 
 * 技術實現：
 * ===========
 * - 時間變量(t)：持續增加的計數器，用於產生連續運動
 * - 隨機模式切換：每10-15秒自動切換運動模式
 * - 數學函數：
 *   - 正弦波(sin)：創造週期性、有機的波動效果
 *   - 緩動函數(easeInOutSine)：實現加速減速的平滑運動
 *   - 線性插值(lerp)：平滑過渡不同角度，增加慣性感
 * - 相位差：通過不同相位的運動函數，使各關節之間有連續性的波浪效果
 * - 角度限制：使用constrain函數確保所有伺服馬達在安全角度範圍內運行
 * 
 * 安全設計：
 * ===========
 * - 每個關節都設有安全角度範圍，防止機械結構損壞
 * - 平滑插值減少突然運動，降低對伺服馬達的沖擊
 * - 延遲20ms控制更新頻率，確保系統穩定性
 */

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
