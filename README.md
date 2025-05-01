# NekoCat Engine 系統架構說明
NekoCat Engine 是一款採用SDL2框架由C寫成的視覺小說引擎，只要輕鬆幾行指令，就可以快速建立專業高級的視覺小說遊戲！

## 系統架構
```
.
├── Assets/
│   ├── music
│   ├── background
│   ├── item
│   ├── sound
│   └── ...
├── Core/
├── assets.nekocat	所有其他素材的相關資料
├── character.nekocat	所有角色的設定，例如name、color…等
├── item.nekocat	所有物品的設定
├── script.nekocat	遊戲的劇本
├── player.sav	        玩家存檔
└── system.conf	有關系統和介面的相關全域設置，例如文字顯示速度、音量控制…等
```
`Nekocat Engine`由以下檔案組成:
| 檔案名稱          | 說明                                                          |
| ----------------- | ------------------------------------------------------------- |
| assets.nekocat    | 所有其他素材的相關資料                                          |
| character.nekocat | 所有角色的設定，例如`name`、`color`...等                        |
| item.nekocat      | 所有物品的設定                                                |
| script.nekocat    | 遊戲的劇本                                                   |
| player.sav        | 玩家存擋                                                              |
| system.conf       | 有關系統和介面的相關全域設置，例如文字顯示速度、音量控制...等 |

## assets.nekocat

在`Nekocat Engine`中，角色和物品相關的素材必須分別定義於`character.nekocat`和`item.nekocat`內，其餘的所有素材必須在`assets.nekocat`中宣告。

在`assets.nekocat`宣告的素材，檔案必須放置在專案資料夾中`Assets`裡對應`Action ID`的資料夾，舉例來說：


```toml=
# 注意：是小寫background
[background]
mountain = "mountain.png"

[Sound]
riverFlow = "river_water_flowing.mp3"

[Music]
ntnu = "ntnu.mp3"

```
其中的檔案結構應類似如下：
```
.
├── Assets/
│   ├── background/
│   │   └── mountain.png
│   ├── sound/
│   │   └── river_water_flowing.mp3
│   └── ...
├─ ...
```
## character.nekocat
`character.nekocat`定義了遊戲裡所有角色的資料，每個角色擁有屬於自己的一張`table`，每個`table`有對應的屬性，相關的屬性請參考以下。

### 屬性
| 名稱                     | 型態            | 可選 | 說明                                                    |
| ------------------------ | --------------- | ---- | ------------------------------------------------------- |
| `description`            | `string`        | 否   | 角色的個性、背景故事等，是用來丟給LLM互動的重要人格資料 |
| `name`                   | `string`        | 否   | 角色名字                                                |
| `color`                  | `string`        | 是   | 角色名字顏色，預設為黑色                                |
| `directory`              | `string`        | 否   | 角色素材在Assets目錄下的自訂子資料夾名稱                |
| `sprites_list`           | `Array<string>` | 否   | 角色所有立繪名稱                                        |
| `sprites_image_list`     | `Array<string>` | 否   | 角色立繪名稱各自對應到的檔案名稱                        |
| `expressions_list`       | `Array<string>` | 否   | 角色所有頭部縮圖名稱                                    |
| `expressions_image_list` | `Array<string>` | 否   | 角色頭部縮圖名稱各自對應到的檔案名稱                    |


讀檔用法已放在 GitHub 上: [https://github.com/happylittle7/NekoCatEngine/tree/toml](https://github.com/happylittle7/NekoCatEngine/tree/toml)

#### P.S:角色 expression 和 sprite 說明：  
![example](https://hackmd.io/_uploads/rJrK12gzR.jpg)

## item.nekocat
這個檔案存放所有遊戲中會用到的道具
```toml=
[Flashlight]
name = "手電筒"
description = "一個普通的手電筒"

[lolipop]
name = "哥哥的棒棒糖"
description = "來自哥哥用心準備的棒棒糖"
icon_path = "flashlight.jpg"


```

## script.nekocat
`script.nekocat`為劇情腳本的檔案，檔案內可包含數個`table`，每個`table`內必須有一個`script`陣列，裡面儲存相關劇情的指令，可使用的指令請參考 [Script Actions](https://hackmd.io/@happylittle7/rk6DKmMzC)。

`script.nekocat`內必須至少包含`[Start]`區塊，Nekocat Engine會從`[Start]`處開始執行指令。  
注意！在`Start`區塊中的第一個action必須是Background。


## player.sav
此為玩家存檔檔案，裡面記錄了所有遊戲中用到的變數和對應的值。
注意：要在`script.nekocat`裡使用的變數和`items`必須事先在這邊宣告，且`item`和`variable`中的id名字不可相同。
```toml=
#此區塊儲存所有變數
[Variable]
HP = 1200
ohmLove = ㄅㄢ

#目前玩家擁有的道具清單
[Items]
flashlight = 1;
cake = 0; #須邀先初始化

#玩家目前執行到的進度
[Other]
current_table_id = "Start"
{ action = "Background", background = "background_1"}
{ action = "Music", music_id = "BGM_1"}
{ action = "SetCharacter", number = 2, command_list = ["ohm","ohmf"], mood_list = ["Normal","sad"]}

```

## system.conf
此為有關遊戲系統的設定黨，包含了如音量、文字速度等相關設定，詳細內容待定。

## Example
檔案架構：
```
├── assets/
│   ├── ohm/
│   │   ├── ohm_happy.png
│   │   └── ohm_sad.png
│   ├── ohmf/
│   │   ├── ohmf_happy.png
│   │   └── ohmf_sad.png
│   └── ohmm/
│       ├── ohmm_happy.png
│       └── ohmm_sad.png
├── character.nekocat
├── script.nekocat
├── system.conf
├── ...
```
character.nekocat:
```toml=
char_list = ["ohm","ohmf","ohmm"]

[ohm]
name = "歐姆巴"
description = "性格怪異，平常喜歡在家睡覺，講話開頭會加欸幹兩個字，充滿滿滿的台味。"
color = "#F5D80A"
directory = "ohm"

sprites_list = ["happy","sad"]
sprites_image_list = ["ohm_happy.png","ohm_happy.png"]

expressions_list = ["happy","sad"]
expressions_image_list = ["ohm_happy.png","ohm_sad.png"]

[ohmf]
name = "歐姆爸"
color = "#F5D80A"
directory = "ohmf"

sprites_list = ["happy","sad"]
sprites_image_list = ["ohmf_happy.png","ohmf_happy.png"]

expressions_list = ["happy","sad"]
expressions_image_list = ["ohmf_happy.png","ohmf_sad.png"]

[ohmm]
name = "歐姆媽"
color = "#F5D80A"
directory = "ohmm"

sprites_list = ["happy","sad"]
sprites_image_list = ["ohmm_happy.png","ohmm_happy.png"]

expressions_list = ["happy","sad"]
expression_image_list = ["ohmm_happy.png","ohmm_sad.png"]


```
script.nekocat:
```toml=
[Start]
script = [
    # 建議第一行就先指定背景，否則效果很差
    { action = "SetBackground", background = "mountain"},
    { action = "Dialog", command = "ohm", text = "Hello world!"},
    
    { action = "Dialog", command = "ohm", text = "Hello world!"},
    { action = "Dialog", command = "no name", text = "Hello world!"},
    { action = "Jump", label_id = "Ending"}, 
    #會有兩種jump一個是要看變數值決定，另一個是直接跳
]

[Ending]
script = [

    { action = "Dialog", command = "no name", text = "Hello world!"},
    { action = "Option", optionCount = 3, optionContent = ["Button1","Button2","Button3"], optionJump = ["label_id_1","label_id_2","label_id_3"]}
]
```

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

