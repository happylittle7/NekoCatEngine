# Script Actions

## Dialogs
**Action ID:** `Dialog`
用來顯示對話，包含控制對話框旁的expression
### 屬性參數
| 參數名稱  | 型態     | 說明                                  |
| --------- | -------- | ------------------------------------- |
| `command` | `string` | 發言的角色，若為`no name`則不顯示名字 |
| `mood`          |  `string`        |    旁邊角色要顯示`expression`的`expression id`                                   |
| `text`    | `string` | 要顯示在對話框中的文字                |

### Example

```toml=
[Start]
script = [
    { action = "Dialog", command = "ohm", mood = "normal", text = "Hello world!"},
    #注意：以下控制的是對話框旁的expression顯示，不是螢幕中間的立繪
    { action = "Dialog", command = "no name", text = "Hello world!"},
    
    { action = "Jump", label_id = "Ending"},
]

[Ending]
script = [

    { action = "Dialog", command = "no name", text = "Hello world!"},
    
]
```



## Clear
**Action ID:** `Clear`
清除文字框

## Play Music (BGM)
**Action ID:** `Music`
播放背景音樂(BGM)，會一直重複播放

### 屬性參數
| 參數名稱   | 型態     | 說明                                             |
| ---------- | -------- | ------------------------------------------------ |
| `music_id` | `string` | 要用來播放的音樂，必須先在assets.nekocat中`[Music]`宣告過，另外，如果music_id是close的話則終止音樂 |

### Example
```toml=
[Start]
script = [
    { action = "Music", music_id = "ntnu.mp3"},
]
```
## Play Sound Effects
**Action ID:** `Sound`
播放背景音樂(BGM)，會一直重複播放

### 屬性參數
| 參數名稱   | 型態     | 說明                                             |
| ---------- | -------- | ------------------------------------------------ |
| `sound_id` | `string` | 要用來播放的音樂，必須先在assets.nekocat中`[Sound]`宣告過 |

### Example
```toml=
[Start]
script = [
    { action = "Sound", sound_id = "door.mp3"},
]
```
## Change Background
**Action ID:** `Background`
更換背景圖片

### 屬性參數
| 型態            | 說明     |                                                  |
| --------------- | -------- | ------------------------------------------------ |
| `background` | `string` | 要更換的背景，必須先在assets.nekocat中`[background]`宣告過 |

### Example
```toml=
[Start]
script = [
    { action = "Background", background = "mountain"},
]
```

## Set Character
**Action ID:** `SetCharacter`
顯示角色立繪

### 屬性參數


### Example
```toml=
[Start]
script = [
    { action = "SetCharacter", number = 2, command_list = ["ohm","ohmf"], mood_list = ["Normal","sad"]},
]
```

## Option
**Action ID:** `Option`
在畫面中顯示按鈕，供玩家點擊觸發對應的分支

### 屬性參數
| 參數名稱       | 型態     | 說明                       |
| -------------- | -------- | -------------------------- |
| `optionCount` | `int`    | 選項數量                   |
| `optionContent`  | `string` | 每個選項要顯示的文字       |
| `optionJump`  | `string` | 選擇該選項後要jump到的段落 |

### Example
```toml=
[Start]
script = [
    { action = "Option", optionCount = 3, optionContent = ["我想吃壽司","我想睡覺","我想耍廢"], optionJump = ["eat","sleep","rest"]},
]

[eat]
script = [
    { action = "Dialog", command = "ohm", text = "壽司好吃"}
]

[sleep]
script = [
    { action = "Dialog", command = "ohm", text = "睡覺好爽"}
]
    
[rest]
script = [
    { action = "Dialog", command = "ohm", text = "耍廢中"}
]
```
## Logical option (已淘汰Duplicated)
**Action ID:** `Logical option`
邏輯判斷玩家的各種變數，當compare_variable的條件全部同時（and關係）達成，則jump到對應的段落

### 屬性參數
| 參數名稱           | 型態     | 說明                                     |
| ------------------ | -------- | ---------------------------------------- |
| `choose_count`     | `int`    | 選項數量                                 |
| `condition_mode`   | `string` | 進行比較的模式（符號）                   |
| `compare_variable` | `string` | 要用來比較的變數名稱（會放在符號的左邊） |
| `compare_value`    | `int`    | 要用來比較的數值（會放在符號右邊）       |
| `success_jump`     | `string` | 前面比較條件全部達成要跳到的地方         |
| `fail_jump`        | `string` | 未達成前面全部條件要跳到的地方 **（注意！：可以為空，當為`fail_jump`為空時，沒達成條件就繼續往下執行下一個指令）**           |

### Example
```toml=
[Start]
script = [
    { action = "Logical operation", choose_count = 3, condition_mode = ["<","=",">"], compare_variable = ["love_point","temperature","hungry_point"], compare_value = [100,27,10],success_jump="true_end", fail_jump="bad_end"}
]

```

## Give item
**Action ID:** `Give item`
給予玩家道具，道具必須在item.nekocat裡面事先定義過

### 屬性參數
|    參數名稱        |型態       |        說明               |
| --------- | -------- | ------------ |
| `item_id` | `string` | 要給的道具id |
| `count`   | `string`    | 給的道具數量（可以是負數） |

在`count`中如果數字前面有加正負號，表示對該變數進行加或減，若沒有正負號或者是 $0$，則表示將該變數指定為該數值。

注意！`count`是`string`型態，必須長像範例那樣用雙引號包起數字(`"1"`)。

### Example
```toml=
[Start]
script = [
    { action = "Give item", item_id = "flashlight", count = "1"},
]
```

## Modify variable
**Action ID:** `Modify variable`
修改玩家身上的變數

### 屬性參數
|    參數名稱        |型態       |        說明               |
| --------------- | -------- | --------------------- |
| `variable_name` | `string` | 要新增/修改的變數名稱 |
| `value`         | `string` | 要設定的值            |

在`value`中如果數字前面有加正負號，表示對該變數進行加或減，若沒有正負號或者是 $0$，則表示將該變數指定為該數值。

注意！`value`是`string`型態，必須長像範例那樣用雙引號包起數字(`"1"`)。

### Example
```toml=
[Start]
script = [
    { action = "Modify variable", variable_name = "hp", value = "-1"},
    { action = "Modify variable", variable_name = "mp", value = "0"},
]
```


## LLM Mode
**Action ID:** `LLM`
進入大型語言模型開放對話模式

### 屬性參數
| 參數名稱       | 型態     | 說明                                     |
| -------------- | -------- | ---------------------------------------- |
| `prompt`       | `string` | 要傳入的提示詞(prompt)                   |
| `history`      | `string` | 對話的歷史紀錄，如果沒有的話請傳`"NULL"` |
| `character_id` | `string` | 要進行LLM模式的角色                     |

回傳：對話歷史cJSON物件

### Example
```toml=
[Start]
script = [
    { action = "LLM", prompt = "", value = "-1"},

]
```
