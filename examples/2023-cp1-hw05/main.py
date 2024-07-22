from cpGrader import Grader

grader = Grader()

def main():
    grader.run(
        moodle_submission_dir="/home/cp-hw/cp1-hw05",
        disable_verify=True
    )

if __name__ == "__main__":
    main()