import csv
import sys
import subprocess

from cpGrader.config import DEFAULT_GRADING_REPORT_PATH
from cpGrader.utils import ExecuteException

def read_grade_report(file_path: str) -> list:
    return [row for row in csv.reader(open(file_path, mode="r", encoding="utf-8"))]

def get_example_grade_report(example_dir: str) -> list:
    subprocess.run(f"cd {example_dir} && {sys.executable} main.py", shell=True)
    return read_grade_report(f"{example_dir}/{DEFAULT_GRADING_REPORT_PATH}")

def compare_grade_reports(example: list, correct: list) -> None:
    assert len(example) == len(correct)
    
    example.sort(key=lambda x: x[0])
    correct.sort(key=lambda x: x[0])
    for i in range(len(example)):
        if any(
            c in example[i][-1] or c in correct[i][-1] for c in
            [cls.COMMENT for cls in vars(ExecuteException).values() if hasattr(cls, "COMMENT")]
        ):
            assert example[i][-1] == correct[i][-1]
        else:
            assert example[i][:-2] == correct[i][:-2]

def run(example_dir: str, correct_grade_report: str):
    example = get_example_grade_report(example_dir)
    correct = read_grade_report(correct_grade_report)
    compare_grade_reports(example, correct)

# ===================================
# NTNU 2023 Computer Programming I
# ===================================
def test_2023_cp1_hw01():
    run(
        example_dir="./examples/2023-cp1-hw01",
        correct_grade_report="./tests/assets/grade_reports/2023-cp1-hw01-report.csv"
    )

def test_2023_cp1_hw02():
    run(
        example_dir="./examples/2023-cp1-hw02",
        correct_grade_report="./tests/assets/grade_reports/2023-cp1-hw02-report.csv"
    )

def test_2023_cp1_hw03():
    run(
        example_dir="./examples/2023-cp1-hw03",
        correct_grade_report="./tests/assets/grade_reports/2023-cp1-hw03-report.csv"
    )

def test_2023_cp1_hw04():
    run(
        example_dir="./examples/2023-cp1-hw04",
        correct_grade_report="./tests/assets/grade_reports/2023-cp1-hw04-report.csv"
    )

def test_2023_cp1_hw05():
    run(
        example_dir="./examples/2023-cp1-hw05",
        correct_grade_report="./tests/assets/grade_reports/2023-cp1-hw05-report.csv"
    )

def test_2023_cp1_midterm():
    run(
        example_dir="./examples/2023-cp1-mid",
        correct_grade_report="./tests/assets/grade_reports/2023-cp1-mid-report.csv"
    )

def test_2023_cp1_final():
    run(
        example_dir="./examples/2023-cp1-fin",
        correct_grade_report="./tests/assets/grade_reports/2023-cp1-fin-report.csv"
    )

# ===================================
# NTNU 2024 Computer Programming II
# ===================================
def test_2024_cp2_hw01():
    run(
        example_dir="./examples/2024-cp2-hw01",
        correct_grade_report="./tests/assets/grade_reports/2024-cp2-hw01-report.csv"
    )

def test_2024_cp2_hw02():
    run(
        example_dir="./examples/2024-cp2-hw02",
        correct_grade_report="./tests/assets/grade_reports/2024-cp2-hw02-report.csv"
    )

def test_2024_cp2_hw03():
    run(
        example_dir="./examples/2024-cp2-hw03",
        correct_grade_report="./tests/assets/grade_reports/2024-cp2-hw03-report.csv"
    )

def test_2024_cp2_hw04():
    run(
        example_dir="./examples/2024-cp2-hw04",
        correct_grade_report="./tests/assets/grade_reports/2024-cp2-hw04-report.csv"
    )

def test_2024_cp2_midterm():
    run(
        example_dir="./examples/2024-cp2-mid",
        correct_grade_report="./tests/assets/grade_reports/2024-cp2-mid-report.csv"
    )

def test_2024_cp2_final():
    run(
        example_dir="./examples/2024-cp2-fin",
        correct_grade_report="./tests/assets/grade_reports/2024-cp2-fin-report.csv"
    )