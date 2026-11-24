# NekoCat Engine

NekoCat Engine 是一款以 C 語言搭配 SDL2 寫成的視覺小說引擎。劇本、角色、素材全部用 TOML 格式的設定檔描述，不需要改任何程式碼，就能做出一款有對話、分支選項、道具背包與存檔的視覺小說遊戲。

> 專案內附的範例遊戲為《歐姆巴的奇幻之旅》，可直接編譯遊玩，也可以當作撰寫劇本的參考。

## 目錄

- [快速開始](#快速開始)
- [操作方式](#操作方式)
- [專案結構](#專案結構)
- [設定檔說明](#設定檔說明)
  - [system.conf](#systemconf)
  - [assets.nekocat](#assetsnekocat)
  - [character.nekocat](#characternekocat)
  - [item.nekocat](#itemnekocat)
  - [player.sav](#playersav)
  - [script.nekocat](#scriptnekocat)
- [Script Actions 指令參考](#script-actions-指令參考)
- [授權](#授權)

## 快速開始

需要 SDL2 以及 SDL2_image、SDL2_ttf、SDL2_mixer 的開發套件。以 Ubuntu / Debian 為例：

```bash
sudo apt install build-essential libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-mixer-dev
```

```bash
make
```

```bash
./main
```

- `make` 只會在原始碼有變動時重新編譯；編譯失敗會直接顯示錯誤，不會默默執行舊的執行檔。
- `make clean` 可刪除編譯產物。
- 遊戲進度存在 `player.sav`，想從頭開始可以還原它：

```bash
git checkout -- player.sav
```

## 操作方式

| 操作       | 功能                 |
| ---------- | -------------------- |
| 滑鼠左鍵   | 推進對話、點選選項   |
| `E` 鍵     | 開啟／關閉道具背包   |
| 畫面存檔鈕 | 將目前進度寫入 `player.sav` |

## 專案結構

```
.
├── Assets/               所有素材
│   ├── background/       背景圖
│   ├── music/            背景音樂（BGM）
│   ├── sound/            音效
│   ├── item/             道具圖示
│   ├── font/             字型
│   ├── image/            介面圖片（按鈕、對話框等）
│   └── <角色資料夾>/      各角色的頭像與立繪
├── Core/                 引擎核心（對話、背包、開場畫面…）
├── Library/              第三方函式庫（TOML 解析器、cJSON）
├── master.c              引擎主程式
├── Makefile
├── system.conf           系統全域設定
├── assets.nekocat        背景、音樂、音效素材宣告
├── character.nekocat     角色設定
├── item.nekocat          道具設定
├── script.nekocat        遊戲劇本
└── player.sav            玩家存檔
```

## 設定檔說明

所有 `.nekocat`、`.conf`、`.sav` 檔案都是 [TOML](https://toml.io/) 格式。

| 檔案                | 說明                                           |
| ------------------- | ---------------------------------------------- |
| `system.conf`       | 系統與介面的全域設定                           |
| `assets.nekocat`    | 背景、音樂、音效的素材宣告                     |
| `character.nekocat` | 所有角色的設定，例如 `name`、`color`…等         |
| `item.nekocat`      | 所有道具的設定                                 |
| `player.sav`        | 玩家存檔：變數、持有道具與目前進度             |
| `script.nekocat`    | 遊戲劇本                                       |

### system.conf

系統全域設定。目前支援的欄位：

```toml
[Global]
gamename = "歐姆巴的奇幻之旅"
```

> 文字速度、音量等設定尚在規劃中。

### assets.nekocat

角色與道具的素材分別定義在 `character.nekocat` 與 `item.nekocat`，**其餘素材（背景、音樂、音效）都必須在 `assets.nekocat` 中宣告**，格式為 `素材 ID = "檔名"`。

| 區塊           | 檔案存放位置          | 使用的指令                  |
| -------------- | --------------------- | --------------------------- |
| `[background]` | `Assets/background/`  | [`Background`](#background) |
| `[Music]`      | `Assets/music/`       | [`Music`](#music)           |
| `[Sound]`      | `Assets/sound/`       | [`Sound`](#sound)           |

> 注意大小寫：區塊名稱是小寫的 `background`，但 `Music`、`Sound` 首字大寫；資料夾名稱則一律小寫。

```toml
[background]
mountain = "mountain.png"

[Music]
ntnu = "ntnu.mp3"

[Sound]
riverFlow = "river_water_flowing.mp3"
```

對應的檔案結構：

```
Assets/
├── background/
│   └── mountain.png
├── music/
│   └── ntnu.mp3
└── sound/
    └── river_water_flowing.mp3
```

### character.nekocat

定義遊戲中所有角色。檔案最上方的 `char_list` 列出所有角色 ID，接著每個角色各有一個以角色 ID 命名的 table。

#### 屬性

| 名稱                     | 型態            | 必填 | 說明                                                         |
| ------------------------ | --------------- | ---- | ------------------------------------------------------------ |
| `name`                   | `string`        | 是   | 角色顯示名稱                                                 |
| `description`            | `string`        | 是   | 角色個性、背景故事等，作為 LLM 互動用的人格資料              |
| `color`                  | `string`        | 否   | 名字顏色（如 `"#F5D80A"`），預設為黑色                       |
| `directory`              | `string`        | 是   | 角色素材在 `Assets/` 下的資料夾名稱                          |
| `expressions_list`       | `Array<string>` | 是   | 頭像（expression）名稱，顯示在對話框旁                       |
| `expressions_image_list` | `Array<string>` | 是   | 每個頭像名稱對應的檔名，順序需與 `expressions_list` 相同      |
| `sprites_list`           | `Array<string>` | 否   | 立繪（sprite）名稱，顯示在畫面中央                           |
| `sprites_image_list`     | `Array<string>` | 否   | 每個立繪名稱對應的檔名，順序需與 `sprites_list` 相同          |

**expression 與 sprite 的差別：**

![expression 與 sprite 示意](https://hackmd.io/_uploads/rJrK12gzR.jpg)

#### 素材放置位置

- 頭像（expression）：`Assets/<directory>/`
- 立繪（sprite）：`Assets/<directory>/character/`

#### 範例

```toml
char_list = ["ohm", "lunei"]

[ohm]
name = "歐姆巴"
description = "殺伐果斷，利己主義，帶著點傲嬌的反差"
color = "#F5D80A"
directory = "ohm"

expressions_list = ["normal"]
expressions_image_list = ["ohm_normal_head.png"]

[lunei]
name = "嚕捏"
description = "能力是不死，初期個性偏溫和"
color = "#F5D80A"
directory = "lunei"

sprites_list = ["happy", "sad"]
sprites_image_list = ["lunei_happy.png", "lunei_sad.png"]

expressions_list = ["normal"]
expressions_image_list = ["lunei_normal_head.png"]
```

```
Assets/
├── ohm/
│   └── ohm_normal_head.png
└── lunei/
    ├── lunei_normal_head.png
    └── character/
        ├── lunei_happy.png
        └── lunei_sad.png
```

### item.nekocat

定義遊戲中所有道具，每個道具一個 table，table 名稱即為道具 ID。

| 名稱          | 型態     | 說明                                   |
| ------------- | -------- | -------------------------------------- |
| `name`        | `string` | 道具名稱                               |
| `description` | `string` | 道具說明，會顯示在背包中               |
| `icon_path`   | `string` | 道具圖示檔名，檔案放在 `Assets/item/`  |

```toml
[love-letter]
name = "土匪寫給未婚妻的情書"
description = "寶貝，等我打完這場仗後就回家鄉跟你結婚"
icon_path = "love-letter.png"
```

### player.sav

玩家存檔，記錄遊戲中所有變數、持有道具與目前進度。

> ⚠️ 劇本中用到的**變數與道具都必須先在這裡宣告並給初始值**，且變數與道具的 ID 不可重複。

```toml
# 所有劇情變數
[Variable]
HP = 1200
ohmLove = 100

# 玩家持有的道具與數量（數量為 0 也必須先宣告）
[Items]
flashlight = 1
cake = 0

# 目前進度（由引擎存檔時自動寫入，通常不需要手動編輯）
[Other]
script = [
    { action = "Background", background = "mountain"},
    { action = "Music", music_id = "ntnu"},
    { action = "SetCharacter", number = 1, command_list = ["lunei"], mood_list = ["happy"]},
    { action = "Jump", label_id = "Start", col_idx = 3},
]
```

`[Other]` 會在讀檔時被執行，用來還原背景、音樂、角色，最後 `Jump` 回存檔時的劇本位置。

### script.nekocat

劇本檔案，由數個 table（段落）組成，每個 table 內有一個 `script` 陣列，依序存放要執行的[指令](#script-actions-指令參考)。

- 必須包含 `[Start]` 段落，遊戲會從這裡開始執行。
- `[Start]` 的第一個指令必須是 `Background`。

```toml
[Start]
script = [
    { action = "Background", background = "mountain"},
    { action = "Music", music_id = "ntnu"},
    { action = "Dialog", command = "ohm", mood = "normal", text = "Hello world!"},
    { action = "Dialog", command = "no name", text = "（旁白）"},
    { action = "Option", optionCount = 2, optionContent = ["往左走", "往右走"], optionJump = ["left", "right"]},
]

[left]
script = [
    { action = "Dialog", command = "ohm", mood = "normal", text = "左邊什麼都沒有。"},
    { action = "Jump", label_id = "Ending"},
]

[right]
script = [
    { action = "Give item", item_id = "flashlight", count = "+1"},
    { action = "Jump", label_id = "Ending"},
]

[Ending]
script = [
    { action = "Dialog", command = "no name", text = "全劇終"},
]
```

## Script Actions 指令參考

| 指令                                     | 功能                         |
| ---------------------------------------- | ---------------------------- |
| [`Dialog`](#dialog)                      | 顯示對話                     |
| [`Background`](#background)              | 更換背景                     |
| [`SetCharacter`](#setcharacter)          | 顯示角色立繪                 |
| [`Music`](#music)                        | 播放／停止背景音樂           |
| [`Sound`](#sound)                        | 播放音效                     |
| [`Option`](#option)                      | 顯示選項按鈕並分支           |
| [`Jump`](#jump)                          | 跳到其他段落（可加條件）     |
| [`Modify variable`](#modify-variable)    | 修改變數                     |
| [`Give item`](#give-item)                | 給予／移除道具               |

### Dialog

顯示一句對話，並可控制對話框旁的角色頭像（expression）。

| 參數      | 型態     | 說明                                                         |
| --------- | -------- | ------------------------------------------------------------ |
| `command` | `string` | 發言角色的 ID；填 `"no name"` 則不顯示名字（旁白）           |
| `mood`    | `string` | 對話框旁要顯示的頭像，需為該角色 `expressions_list` 中的名稱 |
| `text`    | `string` | 對話內容                                                     |

```toml
{ action = "Dialog", command = "ohm", mood = "normal", text = "Hello world!"},
{ action = "Dialog", command = "no name", text = "這是一段旁白。"},
```

> `mood` 控制的是對話框旁的頭像，不是畫面中央的立繪；立繪請用 [`SetCharacter`](#setcharacter)。

### Background

更換背景圖片。

| 參數         | 型態     | 說明                                                  |
| ------------ | -------- | ----------------------------------------------------- |
| `background` | `string` | 背景 ID，須先在 `assets.nekocat` 的 `[background]` 宣告 |

```toml
{ action = "Background", background = "mountain"},
```

### SetCharacter

在畫面中央顯示角色立繪，會取代目前畫面上的所有立繪。

| 參數           | 型態            | 說明                                                         |
| -------------- | --------------- | ------------------------------------------------------------ |
| `number`       | `int`           | 要顯示的角色數量（最多 3 位）；設為 `0` 則清除所有立繪       |
| `command_list` | `Array<string>` | 要顯示的角色 ID                                              |
| `mood_list`    | `Array<string>` | 每位角色要用的立繪，需為該角色 `sprites_list` 中的名稱       |

```toml
{ action = "SetCharacter", number = 2, command_list = ["lunei", "12"], mood_list = ["happy", "sad"]},
{ action = "SetCharacter", number = 0},
```

### Music

播放背景音樂，會不斷循環直到被替換或停止。

| 參數       | 型態     | 說明                                                                 |
| ---------- | -------- | -------------------------------------------------------------------- |
| `music_id` | `string` | 音樂 ID，須先在 `assets.nekocat` 的 `[Music]` 宣告；填 `"close"` 則停止音樂 |

```toml
{ action = "Music", music_id = "ntnu"},
{ action = "Music", music_id = "close"},
```

### Sound

播放一次音效。

| 參數       | 型態     | 說明                                                |
| ---------- | -------- | --------------------------------------------------- |
| `sound_id` | `string` | 音效 ID，須先在 `assets.nekocat` 的 `[Sound]` 宣告 |

```toml
{ action = "Sound", sound_id = "door"},
```

### Option

在畫面上顯示選項按鈕，玩家點選後跳到對應段落。

| 參數            | 型態            | 說明                                 |
| --------------- | --------------- | ------------------------------------ |
| `optionCount`   | `int`           | 選項數量（最多 3 個）                |
| `optionContent` | `Array<string>` | 每個選項顯示的文字                   |
| `optionJump`    | `Array<string>` | 每個選項被選擇後要跳到的段落 ID      |

```toml
[Start]
script = [
    { action = "Option", optionCount = 3, optionContent = ["我想吃壽司", "我想睡覺", "我想耍廢"], optionJump = ["eat", "sleep", "rest"]},
]

[eat]
script = [
    { action = "Dialog", command = "ohm", mood = "normal", text = "壽司好吃"},
]

[sleep]
script = [
    { action = "Dialog", command = "ohm", mood = "normal", text = "睡覺好爽"},
]

[rest]
script = [
    { action = "Dialog", command = "ohm", mood = "normal", text = "耍廢中"},
]
```

### Jump

跳到另一個段落。可以無條件跳轉，也可以依變數值決定是否跳轉。

| 參數       | 型態            | 必填 | 說明                                                         |
| ---------- | --------------- | ---- | ------------------------------------------------------------ |
| `label_id` | `string`        | 是   | 目標段落 ID                                                  |
| `variable` | `Array<string>` | 否   | 要判斷的變數名稱（須在 `player.sav` 的 `[Variable]` 宣告）   |
| `mode`     | `Array<string>` | 否   | 比較方式：`">"`、`"="`、`"<"`                                |
| `gap`      | `Array<int>`    | 否   | 比較的數值                                                   |
| `col_idx`  | `int`           | 否   | 從目標段落的第幾個指令開始執行（從 0 起算），預設為 0         |

- 沒有 `variable` 時為**無條件跳轉**。
- 有 `variable` 時，會逐一檢查 `變數 mode gap`，**全部條件成立（AND）才跳轉**，否則繼續執行下一個指令。

```toml
# 無條件跳轉
{ action = "Jump", label_id = "Ending"},

# 條件跳轉：love > 100 且 hp < 10 才跳到 true_end
{ action = "Jump", label_id = "true_end", variable = ["love", "hp"], mode = [">", "<"], gap = [100, 10]},
{ action = "Jump", label_id = "bad_end"},
```

> 舊版的 `Logical option` 指令已淘汰，請改用條件式 `Jump`。

### Modify variable

修改 `player.sav` 中 `[Variable]` 的變數。

| 參數            | 型態     | 說明           |
| --------------- | -------- | -------------- |
| `variable_name` | `string` | 要修改的變數   |
| `value`         | `string` | 要設定的值     |

`value` 規則：

- 有正負號（如 `"+5"`、`"-1"`）：在原本的值上加減。
- 沒有正負號（如 `"3"`、`"0"`）：直接指定為該值。
- 必須是**字串**，要用雙引號包起來。

```toml
{ action = "Modify variable", variable_name = "hp", value = "-1"},
{ action = "Modify variable", variable_name = "mp", value = "0"},
```

### Give item

增減玩家持有的道具，道具須先在 `item.nekocat` 定義、並在 `player.sav` 的 `[Items]` 宣告。

| 參數      | 型態     | 說明                         |
| --------- | -------- | ---------------------------- |
| `item_id` | `string` | 道具 ID                      |
| `count`   | `string` | 數量，規則同 `Modify variable` 的 `value` |

```toml
{ action = "Give item", item_id = "flashlight", count = "+1"},
{ action = "Give item", item_id = "cake", count = "0"},
```

## 開發中功能

以下功能已有設計或部分程式碼，但**尚未接入引擎**，目前寫在劇本裡不會有作用：

- `Clear`：清除對話框文字。
- `LLM`：與角色進行大型語言模型開放式對話（相關程式碼位於 `Core/InteractLLM.c`，角色的 `description` 即為其人格資料）。

## 授權

本專案採用 [MIT License](LICENSE)。
