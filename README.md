# cpGrader: A Python Package to Grade Programming Assignments Automatically

cpGrader is designed to be used with the course ["NTNU CSIE Computer Programming I/II"](https://sites.google.com/gapps.ntnu.edu.tw/neokent/teaching) by instructor [neokent (紀博文 Po-Wen Chi)](https://sites.google.com/gapps.ntnu.edu.tw/neokent/about-me).

## Installation

```bash
pip install cpGrader
```

## How does it work?

![assets](https://github.com/ryanlinjui/cpGrader/assets/57468611/da57e04c-316b-46ce-a400-0cad8c00ff17)

### Stage (You can enable/disable the stage)
- `Extract`: Get all the students's source file from moodle submission folder.
- `Build`: Copy the support files to the student's folder and Compile the student's program.
- `Execute`: Run the student's program by the testcase and Generate output file by each testcase.
- `Verify`: Compare the student's output with the correct plaintext output or the correct program's output.

### Step
1. Download all the student submissions from [Moodle](https://moodle3.ntnu.edu.tw).
2. Think about the testcases and strategies that how to grade the student's assignment.
3. Write a config file.
4. Design the verify function.
5. Run your grader.

## Getting Started

### TOML Config File

- `global`: the global settings (Field below is just for global).
    - `support [list]`: the files copied to the student's folder.
- `case`: the testcase settings (Each case can be independent).
    - `name [string]`: name of testcase.
    - `file [string]`: filepath of testcase file.
    - `pts [float | int]`: grading points.
    - `correct [string]`: the correct file which is program file (`.c`, `.py`) or plain text file (`.txt`, `.out`).
    - `command [string]`: the command to run student's program.

### Basic Example

#### Config File
```toml
[global]
support = ["input.bmp", "assignment.h"]
correct = "./correct.c"
command = "./assignment"

[[case]]
name = "case1"
file = "./testcase/1.in"
pts = 5

[[case]]
name = "case2"
file = "./testcase/2.in"
pts = 10
```

#### Python Code
```python
from cpGrader import Grader

grader = Grader()

@grader.setcase()
def verify(case_name: str, student_output: str, correct_output: str):
    assert student_output == correct_output

grader.run(
    moodle_submission_dir="./cp-hw"
)
```
