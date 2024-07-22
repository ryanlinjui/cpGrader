from cpGrader import (
    Grader
)

grader = Grader()

@grader.setcase()
def verify(case_name: str, student_output: str, correct_output: str):
    submit = student_output
    correct = correct_output

    if case_name == "case5":
        assert "usage" in submit.lower(), f"Usage message is not showing well."
    elif case_name == "case6":
        assert  "invalid" in submit.lower() \
                or "error" in submit.lower() \
                or "unknown" in submit.lower() \
                or "not" in submit.lower(), f"Invalid message is not showing well."
        
    else:
        submit = [part.strip() for part in student_output.split("\n")]
        correct = [part.strip() for part in correct_output.split("\n")]
        
        for i in range(len(correct)):
            assert submit[i].replace(" ", "") == correct[i].replace(" ", ""), f"Output is not correct."

def main():
    grader.run(
        moodle_submission_dir="/home/cp-hw/cp2-hw03"
    )

if __name__ == "__main__":
    main()