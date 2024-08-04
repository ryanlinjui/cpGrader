import re

from cpGrader import Grader

grader = Grader()

@grader.setcase()
def verify(case_name: str, student_output: str, correct_output: str):
    submit = student_output.split("\n")
    correct = correct_output.split("\n")
    
    assert len(submit) == len(correct), "Line count mismatch"
    for i in range(len(submit)):
        assert submit[i] == correct[i], f"Line {i+1} mismatch"

def main():
    grader.run(
        moodle_submission_dir="./submissions"
    )

if __name__ == "__main__":
    main()