from cpGrader import Grader

grader = Grader(
    config_file=None
)

def main():
    grader.run(
        moodle_submission_dir="/home/cp-hw/cp2-hw04",
        disable_build=True
    )

if __name__ == "__main__":
    main()