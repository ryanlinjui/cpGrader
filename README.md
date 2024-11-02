
## Finish

- Unzip
- Make
- Log
- Clean module
- load

## Todo

- csv

### Commands

- `--score <hw0303> <int>`: 打分
- `--clean`: Clean
- `--build`: Clean, unzip, make, log, copy solution
- `--log`: Log
- `--exit`

### Toml

- HW0幾
- 改第幾提

### Grading

- `--grade`: 自動批改
    1. 純輸出 run+diff
    2. 有測資 固定IO
        - 有多少筆
        - `testcase0X` 裏面有 `in/` 跟 `out/`
            - `in` ...
            - `out` ...
    3. 沒有固定IO (other)
    4. 互動題
    5. 帶參數 (可選參數)

### Diff

- 要如何輸出是個問題
- 該題總分加總起來為20

### Output

- `--output`: Output a CSV
- Log to zip
- If ok, score
- If error, error message
