from cpGrader import Grader

grader = Grader()

def main():
    grader.run(
        moodle_submission_dir="submissions",
        disable_verify=True
    )

if __name__ == "__main__":
    main()