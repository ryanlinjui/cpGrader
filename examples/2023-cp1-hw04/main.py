import re
import signal

from sympy import sympify

from cpGrader import Grader

class VerfiyTimeoutError(Exception):
    pass

def timeout_handler(signum, frame):
    raise VerfiyTimeoutError()

signal.signal(signal.SIGALRM, timeout_handler)

grader = Grader()

def sympy_format(poly_str: str) -> str:
    return re.compile(r"(\d+)x").sub(r"\1*x",  poly_str.replace("^", "**").replace("{number}x", "{number}*x"))

@grader.setcase()
def verify(case_name: str, student_output: str, correct_output: str):    
    student = student_output.split("\n")[4:]
    correct = correct_output.split("\n")[4:]

    if (case_name == "case5" and len(student) <= 3) == False:
        try:
            f_x = sympify(sympy_format(student[0].split("f(x):")[-1]))
            g_x = sympify(sympy_format(student[1].split("g(x):")[-1]))
            fg_m = sympify(sympy_format(student[2].split("(f(x)g(x))':")[-1]))
            fg_d = sympify(sympy_format(f"({student[3].split('f(x)')[-1]})/({sympy_format(student[5].split('g(x)')[-1])})"))
        
        except:
            assert False, "Error: Incorrect format"
        
        try:
            signal.alarm(5)
            
            correct_f_x = correct[0].split("f(x):")[-1]
            assert f_x.equals(correct_f_x), f"f(x) mismatch"

            correct_g_x = correct[1].split("g(x):")[-1]
            assert g_x.equals(correct_g_x), f"g(x) mismatch"

            correct_fg_m = correct[2].split("(f(x)g(x))':")[-1]
            assert fg_m.equals(correct_fg_m), f"(f(x)g(x))' mismatch"
            
            correct_fg_d = correct[3].split("f(x)/g(x))':")[-1]
            assert fg_d.equals(correct_fg_d), f"(f(x)/g(x))' mismatch"

        except VerfiyTimeoutError:
            assert False, "Verfiy Function Unkown Error: timeout for 5 seconds"
        
        finally:
            signal.alarm(0)

def main():
    grader.run(
        moodle_submission_dir="./submissions"
    )

if __name__ == "__main__":
    main()