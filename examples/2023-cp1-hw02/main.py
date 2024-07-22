import re

from cpGrader import (
    Grader
)

grader = Grader()

@grader.setcase(match_case="case[167]")
def verify1(case_name: str, student_output: str, correct_output: str):
    ERROR_THRESHOLD = 5.0
    student = student_output[student_output.find("Output"):].split("\n")[1:-1]
    student_num_list = []
    for line in student[1:-1]:
        re_result = re.findall(r"\d+\.\d+|\d+", line)
        if len(re_result) > 0:
            student_num_list.append(
                [float(value) if '.' in value else int(value) for value in re_result]
            )

    correct = correct_output[correct_output.find("Output"):].split("\n")[1:-1]
    correct_num_list = []
    for line in correct[1:-1]:
        re_result = re.findall(r"\d+\.\d+|\d+", line)
        if len(re_result) > 0:
            correct_num_list.append(
                [float(value) if '.' in value else int(value) for value in re_result]
            )

    assert len(student_num_list) == len(correct_num_list), f"output list length mismatch: student:{len(student_num_list)}, correct:{len(correct_num_list)}"

    for n1 in range(len(correct_num_list)):
        for n2 in range(len(correct_num_list[n1])):
            element1 = correct_num_list[n1][n2]
            element2 = student_num_list[n1][n2]
            absolute_difference = abs(element1 - element2)

            if element1 == 0:
                if element1 == element2: 
                    continue
                assert element1 == element2, f"number mismatch, student:{student_num_list[n1]}, correct:{correct_num_list[n1]}"

            percent_error = (absolute_difference / element1) * 100
            assert percent_error <= ERROR_THRESHOLD or absolute_difference <= ERROR_THRESHOLD, f"percent should be less than {ERROR_THRESHOLD}%, student:{student_num_list[n1]}, correct:{correct_num_list[n1]}, percent:{percent_error}"

@grader.setcase(match_case="case[2345]")
def verify2(case_name: str, student_output: str, correct_output: str):
    if len(student_output.split("\n")) > 7:
        assert re.findall(r"\d+\.\d+|\d+", '\n'.join(student_output.split('\n')[7:])) == [], f"no invalid detected"

def main():
    grader.run(
        moodle_submission_dir="/home/cp-hw/cp1-hw02"
    )

if __name__ == "__main__":
    main()