import re

from cpGrader import Grader

grader = Grader()

@grader.setcase()
def verify(case_name: str, student_output: str, correct_output: str):
    if case_name == "case4" or case_name == "case5":
        submit_query = next((s for s in student_output.split("\n") if "Query Result count:" in s), "")
        correct_query = next(s for s in correct_output.split("\n") if "Query Result count:" in s)
        assert re.findall(r"-?\d+", submit_query) == re.findall(r"-?\d+", correct_query), "Query Result count is not correct."
    else:
        assert student_output == correct_output, "Output is not correct."

def main():
    grader.run(
        moodle_submission_dir="./submissions"
    )

if __name__ == "__main__":
    main()