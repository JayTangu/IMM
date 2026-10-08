# WAV 音訊檔頭格式規範 (WAV Header 44 Bytes Format)

WAV（Waveform Audio File Format）是以微軟與 IBM 制定的 RIFF（Resource Interchange File Format）架構為基礎的音訊檔案格式。標準未壓縮 PCM 格式的 WAV 檔頭固定佔用前 44 個位元組（00h ～ 2Bh）[cite: 2]。

---

## 一、WAV 檔頭 44 Bytes 結構對照表

| 位址 (Offset) | 長度 (Bytes) | 欄位名稱 (Field Name) | 端序 (Endianness) | 考卷十六進位範例 (Hex) | 欄位意義與數值說明 |
| :--- | :---: | :--- | :---: | :--- | :--- |
| **00h - 03h** | 4 | **ChunkID** | Big-Endian | `52 49 46 46` | 固定 ASCII 字串 `"RIFF"`，宣告檔案採用 RIFF 封裝格式[cite: 2]。 |
| **04h - 07h** | 4 | **ChunkSize** | Little-Endian | `50 69 1A 00` | 整個檔案的大小扣除前 8 Bytes（即 `36 + Subchunk2Size`）[cite: 2]。 |
| **08h - 0Bh** | 4 | **Format** | Big-Endian | `57 41 56 45` | 固定 ASCII 字串 `"WAVE"`，宣告此檔案為波形音訊[cite: 2]。 |
| **0Ch - 0Fh** | 4 | **Subchunk1ID (Z)** | Big-Endian | `66 6D 74 20` | 固定 ASCII 字串 `"fmt "`（注意末尾包含一個空格，ASCII 碼為 `20h`）[cite: 2]。 |
| **10h - 13h** | 4 | **Subchunk1Size** | Little-Endian | `10 00 00 00` | 格式區塊長度，未壓縮 PCM 格式固定為 16 Bytes（即十六進位 `10h`）[cite: 2]。 |
| **14h - 15h** | 2 | **AudioFormat** | Little-Endian | `01 00` | 音訊編碼格式，數值 `1` 代表線性 PCM（未壓縮音訊）[cite: 2]。 |
| **16h - 17h** | 2 | **NumChannels** | Little-Endian | `01 00` | 聲道數（`1` = 單聲道 Mono，`2` = 雙聲道 Stereo）[cite: 2]。 |
| **18h - 1Bh** | 4 | **SampleRate** | Little-Endian | `44 AC 00 00` | 取樣頻率（Hz），`0x0000AC44` 換算十進位為 44,100 Hz[cite: 2]。 |
| **1Ch - 1Fh** | 4 | **ByteRate** | Little-Endian | `44 AC 00 00` | 每秒傳輸位元組數（計算公式：`SampleRate × BlockAlign`）[cite: 2]。 |
| **20h - 21h** | 2 | **Bpsample** | Little-Endian | `01 00` | 每次取樣的總位元組數（計算公式：`NumChannels × BitsPerSample / 8`）[cite: 2]。 |
| **22h - 23h** | 2 | **BitsPerSample (Y)** | Little-Endian | `08 00` | 每個取樣點的量化深度（Bit Depth，例如 8-bit、16-bit）[cite: 2]。 |
| **24h - 27h** | 4 | **data** | Big-Endian | `64 61 74 61` | 固定 ASCII 字串 `"data"`，宣告音訊波形資料本體開始[cite: 2]。 |
| **28h - 2Bh** | 4 | **dataSize** | Little-Endian | `5C EE 55 00` | 音訊資料區塊本體的總位元組長度[cite: 2]。 |

---

## 二、關鍵計算公式與欄位關係

* **每次取樣位元組數（BlockAlign / BpSample）**[cite: 2]：
  $$\text{BlockAlign} = \text{NumChannels} \times \frac{\text{BitsPerSample}}{8}$$

* **每秒位元組傳輸率（ByteRate / Bpsec）**[cite: 2]：
  $$\text{ByteRate} = \text{SampleRate} \times \text{BlockAlign}$$

* **小端序（Little-Endian）讀取原則**：
  * 數值欄位（如 SampleRate、BlockAlign、BitsPerSample）在記憶體中採低位元組在前、高位元組在後排列[cite: 2]。
  * 文字識別碼（如 `RIFF`、`WAVE`、`fmt `、`data`）則為 Big-Endian，按字元順序由左至右直接讀取[cite: 2]。