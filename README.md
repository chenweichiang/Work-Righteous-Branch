# 義枝 (Righteous Branch) - 互動機械藝術裝置

[![版本](https://img.shields.io/badge/版本-2.0.1-blue.svg)](https://github.com/chenweichiang/Work-Righteous-Branch/releases/tag/v2.0.1)

<img src="https://raw.githubusercontent.com/chenweichiang/Work-Righteous-Branch/main/images/righteous-branch.jpg" alt="義枝裝置實景" width="800">

這個 Arduino 項目控制一個模擬有機生命運動的多關節機械裝置，通過六個伺服馬達實現類生物行為的運動模式。


## 功能概述

「義枝」是一個互動式機械裝置，透過模仿生物運動創造出有機且自然的動態藝術體驗。本系統使用 ESP8266 LOLIN D1 R1 開發板控制六個伺服馬達，實現了七種不同的運動模式：

1. **閒置呼吸 (Idle Breathing)**：模擬生物輕微的呼吸動作，表現出靜止狀態下的微小生命跡象
2. **環顧四周 (Look Around)**：裝置會緩慢轉動頭部和基座，呈現如同生物觀察環境的行為
3. **伸展 (Stretching)**：模擬生物伸展肢體的動作，各關節協調運動
4. **伸手夠取 (Reaching)**：前傾並伸展前臂，像是嘗試接觸或夠取物體
5. **閃避 (Dodging)**：模擬迅速避開威脅的動作，關節有協調的反向運動
6. **抽搐 (Twitching)**：產生輕微且快速的顫動，模擬神經反應
7. **增強伸展 (Enhanced Stretching)**：更大幅度的伸展動作，各關節有序延遲運動

系統會在這些模式間自動隨機切換，每種模式持續約 10-15 秒不等，創造出持續變化的生命化運動體驗。

## 技術細節

- **數學模型**：使用正弦波函數、緩動函數和線性插值實現流暢自然的運動
- **運動特性**：
  - 使用相位差創造波浪式運動鏈
  - 加入隨機性增強自然感
  - 通過插值實現慣性運動感
- **安全設計**：程式內置角度限制，確保伺服馬達運動在安全範圍內

## 硬體需求

- ESP8266 LOLIN D1 R1 開發板
- 6 個伺服馬達 (推薦使用 SG90 或 MG996R，視負載而定)
- 適合的電源供應 (建議 5V 3A 以上)
- 機械架構 (支架、連接件等)
- 電線和連接器

## 針腳連接

| 伺服馬達 | 連接針腳 | 功能                |
|----------|----------|-------------------|
| servoHead    | D2       | 控制頭部旋轉        |
| servoNeck    | D3       | 控制頸部傾斜        |
| servoElbow   | D4       | 控制肘部彎曲        |
| servoUpper   | D5       | 控制上臂旋轉        |
| servoForearm | D6       | 控制前臂旋轉        |
| servoBase    | D7       | 控制基座旋轉        |

## ESP8266 LOLIN D1 R1 安裝指南

### 1. 安裝 Arduino IDE

1. 從 [Arduino 官方網站](https://www.arduino.cc/en/software) 下載並安裝最新版本的 Arduino IDE

### 2. 安裝 ESP8266 開發板支援

1. 打開 Arduino IDE
2. 前往 `檔案 > 偏好設定`
3. 在「額外的開發板管理員網址」欄位中添加以下網址：
   ```
   https://arduino.esp8266.com/stable/package_esp8266com_index.json
   ```
4. 點擊「確定」
5. 前往 `工具 > 開發板 > 開發板管理員`
6. 在搜尋欄位中輸入「ESP8266」
7. 找到「ESP8266 by ESP8266 Community」並安裝
8. 安裝完成後關閉開發板管理員

### 3. 選擇正確的開發板設定

1. 前往 `工具 > 開發板`
2. 選擇「LOLIN(WEMOS) D1 R1」
3. 設定以下參數：
   - 上傳速度：115200
   - CPU 頻率：80MHz
   - Flash 大小：視您的板子而定，通常為 4MB (FS:2MB OTA:~1019KB)
   - Debug port：Disabled
   - Debug Level：None
   - IwIP Variant：v2 Lower Memory
   - Reset Method：nodemcu
   - Crystal Frequency：26MHz
   - Flash Mode：DIO
   - Flash Frequency：40MHz
   - Port：選擇您的 ESP8266 開發板所連接的 COM 端口

### 4. 安裝必要的程式庫

1. 前往 `工具 > 管理程式庫`
2. 搜尋並安裝「Servo」程式庫

### 5. 連接硬體

1. 使用 USB 線將 ESP8266 LOLIN D1 R1 連接到電腦
2. 確保開發板被正確識別（在 `工具 > 端口` 中應該出現）
3. 根據前述針腳連接表連接伺服馬達
4. 確保提供足夠的電源給伺服馬達（可能需要外部電源）

### 6. 上傳程式

1. 打開 `Righteous Branch.ino` 檔案
2. 點擊上傳按鈕 (箭頭圖標)
3. 等待編譯和上傳完成
4. 上傳完成後，裝置應開始運行

## 故障排除

- **伺服馬達不動或抖動**：檢查電源供應是否足夠，或者調整程式中的角度限制
- **上傳失敗**：
  - 確保選擇了正確的板子和端口
  - 嘗試按住 ESP8266 板上的 FLASH 按鈕，然後按一下 RST 按鈕
  - 檢查 USB 線是否能夠傳輸數據（有些僅支援充電）
- **動作不協調**：調整程式中的插值參數或運動範圍

## 調整與客製化

可通過修改程式碼中的以下參數來客製化裝置行為：

- `modeDuration`：調整每種模式的持續時間
- `headMid`, `neckMid` 等：調整各關節的中心位置
- `headRange`, `neckRange` 等：調整各關節的運動範圍
- 各 case 中的參數：調整特定模式中的運動特性
- `lerp` 函數中的第三個參數：調整運動的平滑程度

## 授權資訊

本項目採用 [請選擇合適的授權] 授權。

## 作者

[Chiang, Chenwei]
https://interaction.tw/


