from cpGrader import Grader

grader = Grader()

@grader.setcase()
def verify(case_name: str, student_output: str, correct_output: str):
    student_number = student_output.split("\n")[1].replace(" ", "")
    correct_number = correct_output.split("\n")[1].replace(" ", "")
    assert student_number == correct_number, f"number mismatch: student={student_number}, correct={correct_number}"

def main():
    grader.run(
        moodle_submission_dir="./submissions"
    )

if __name__ == "__main__":
    main()