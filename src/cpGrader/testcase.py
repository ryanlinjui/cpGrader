import os
import shutil
import logging
from sys import executable
from inspect import currentframe
from typing import (
    List,
    Callable,
    Union,
    Optional
)

from .utils import (
    build,
    execute,
    ExecuteException
)
from .config import (
    MAX_OUTPUT_SIZE,
    DEFAULT_CASE_NAME
)

class Case:
    def __init__(
        self,
        name: str,
        case_file: Optional[str],
        command: Optional[str],
        pts: Union[int, float],
        correct_file: Optional[str],
        verify_func: Optional[Callable]
    ):
        self.name: str = name
        self.case_file: Optional[str] = case_file
        self.command: Optional[str] = command
        self.pts: Union[int, float] = pts
        self.correct_file: Optional[str] = correct_file
        self.verify_func: Optional[Callable] = verify_func
        
        self.case_data: List[str] = []
        self.student_output: str = ""
        self.correct_output: str = ""

    def __correct_setup(self) -> None:
        # No correct file
        if self.correct_file == None:
            logging.debug(f"No correct file")
            return

        # Plain Text correct file
        if self.correct_file.endswith(".txt"):
            logging.debug(f"Read correct output")
            self.correct_output = open(self.correct_file, "r").read()
            return
        
        # Correct Program processing
        logging.debug(f"Correct program processing, case: {self.name}")
        support_files = currentframe().f_back.f_back.f_locals["self"].support_files
        correct_dir = os.path.dirname(self.correct_file)
        correct_file = os.path.basename(self.correct_file)

        if correct_file.endswith(".c"):  # C correct program
            logging.debug(f"C correct program runnning")
            logging.debug(f"Buidling: {correct_dir}, support_files: {support_files}")
            build(folder=correct_dir, copy_file=support_files)

            command = f"./{correct_file}".replace(".c", "")
            logging.debug(f"Execute: {correct_dir}, command: {command}")
            self.correct_output = execute(
                folder=correct_dir,
                command=command,
                stdin_list=self.case_data
            )
            logging.debug(f"C correct program finished")
            
        elif correct_file.endswith(".py"): # Python correct program
            logging.debug(f"Python correct program running")
            command = f"{executable} {correct_file}"
            logging.debug(f"Excute: {correct_dir}, command: {command}")
            self.correct_output = execute(
                folder=correct_dir,
                command=command,
                stdin_list=self.case_data
            )
            logging.debug(f"Python correct program finished")
        
        output_filepath = os.path.join(correct_dir, f"{self.name}.txt")
        logging.debug(f"Write correct output: {output_filepath}")
        with open(output_filepath, "w+") as f:
            f.write(self.correct_output)

    def execute(self, student_dir: str) -> Optional[ExecuteException]:
        # setup case data and build/execute correct file
        if self.case_file != None:
            self.case_data = [line.strip() for line in open(self.case_file, "r").readlines()]
        
        logging.debug(f"Correct Setup: {student_dir}, {self.name}")
        self.__correct_setup()

        output = ""
        comment = ""
        exception_signal = None

        logging.debug(f"Execute Student Setup: {student_dir}, {self.name}")
        try:
            logging.debug(f"Execute: {student_dir}, command: {self.command}")
            output = execute(
                folder=student_dir,
                command=self.command,
                stdin_list=self.case_data
            )

        except ExecuteException.NoFile as e:
            logging.error(f"Failed - {str(e)}")
            output = e.output
            comment = e.comment
            exception_signal = ExecuteException.NoFile
            
        except ExecuteException.ProgramError as e:
            logging.error(f"{self.name}: Failed - {str(e)}")
            output = e.output
            comment = e.comment
            if self.name != DEFAULT_CASE_NAME:
                comment = f"{self.name}: {comment}"
            exception_signal = ExecuteException.ProgramError
        
        except Exception as e:
            logging.error(f"Exception: {str(e)}")

        if len(output) > MAX_OUTPUT_SIZE:
            logging.warning(f"In {student_dir}, {self.name}: outputs are truncated to {MAX_OUTPUT_SIZE} characters")
            output = output[:MAX_OUTPUT_SIZE]

        output_filepath = os.path.join(student_dir, f"{self.name}.txt")
        logging.debug(f"Write student output: {output_filepath}")
        with open(output_filepath, "w+") as f:
            f.write(output)
        self.student_output = output

        # Append to grade report
        logging.debug(f"Exception Signal: {exception_signal}")
        if exception_signal:
            (lambda x: (x[1].append(0), x[2].append(comment), x))(currentframe().f_back.f_locals["self"].grade_report[-1])
        
        return exception_signal

    def verify(self) -> None:
        if self.verify_func == None:
            logging.debug(f"No verify function")
            return

        pts = 0
        comment = ""

        try:
            logging.debug(f"Verify: {self.name}")
            self.verify_func(self.name, self.student_output, self.correct_output)
            pts = self.pts
            comment = f"{self.name}: AC"
            logging.info(f"{self.name}: AC")

        except AssertionError as e:
            comment = f"{self.name}: WA, {str(e)}"
            logging.error(f"{self.name}: WA, {str(e)}")
        
        # Append to grade report
        logging.debug(f"Append to grade report")
        (lambda x: (x[1].append(pts), x[2].append(comment), x))(currentframe().f_back.f_locals["self"].grade_report[-1])