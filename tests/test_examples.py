import csv
import sys
import pytest
import subprocess

from cpGrader.config import DEFAULT_GRADING_REPORT_PATH

def read_grade_report(file_path: str) -> list:
    return [row for row in csv.reader(open(file_path, mode="r", encoding="utf-8"))]

STUDENT_ID_LIST = [
    "41247001S",
    "41247012S",
    "41247024S",
    "41247032S",
    "41247039S",
    "41247057S",
]

EXAMPLES_LIST = [
    "2023-cp1-hw01",
    "2023-cp1-hw02",
    "2023-cp1-hw03",
    "2023-cp1-hw04",
    "2023-cp1-hw05",
    "2023-cp1-mid",
    "2023-cp1-fin",
    "2024-cp2-hw01",
    "2024-cp2-hw02",
    "2024-cp2-hw03",
    "2024-cp2-hw04",
    "2024-cp2-mid",
    "2024-cp2-fin"
]

@pytest.mark.parametrize("examples_folder", EXAMPLES_LIST)
def test_run_all_examples(examples_folder: str):
    subprocess.run(
        f"cd ./examples/{examples_folder} && {sys.executable} main.py",
        shell=True
    )
    report = read_grade_report(f"./examples/{examples_folder}/{DEFAULT_GRADING_REPORT_PATH}")
    
    # Check if the report has the correct number of rows
    assert len(report) == len(STUDENT_ID_LIST)

    # Check if the report has all the student IDs
    for student_id in STUDENT_ID_LIST:
        assert any(student_id in row for row in report)