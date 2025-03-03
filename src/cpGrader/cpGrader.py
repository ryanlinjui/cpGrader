import os
import fnmatch
import logging
import objprint
from typing import (
    List,
    Callable,
    Union,
    Optional
)

import toml

from .testcase import Case
from .utils import (
    extract,
    build,
    grade,
    ExecuteException
)
from .config import (
    DEFAULT_WORKSPACE_DIR,
    DEFAULT_TESTCASE_SRC,
    DEFAULT_GRADING_REPORT_PATH,
    DEFAULT_EXTRACTED_FILES_DIR,
    DEFAULT_CONFIG_FILE,
    DEFAULT_CASE_NAME,
    ALL_MATCH_CASE
)

class Grader:
    def __init__(self, config_file: Optional[str] = DEFAULT_CONFIG_FILE):
        self.grade_report: List[List[str]] = []
        
        # Initialize Test Cases Configuration
        self.case_list: List[Case] = []
        self.support_files: List[str] = []
        if config_file is not None:
            self.__read_config(config_file)

    def __read_config(self, config_file: str) -> None:
        config = toml.load(config_file)

        # Global Test Cases Configuration
        if "global" not in config: config["global"] = {}
        self.support_files = config["global"].get("support", [])
        global_case_file = config["global"].get("file", None)
        global_correct_file = config["global"].get("correct", None)
        global_pts = config["global"].get("pts", 0)
        global_command = config["global"].get("command", None)

        if "case" in config: # Test Cases Configuration
            for _case in config["case"]:
                name = _case.get("name", None)
                case_file = _case.get("file", None)
                command = _case.get("command", global_command)
                pts = _case.get("pts", 0)
                correct_file = _case.get("correct", global_correct_file)

                self.case_list.append(
                    Case(
                        name=name,
                        case_file=case_file,
                        command=command,
                        pts=pts,
                        correct_file=correct_file,
                        verify_func=None
                    )
                )

        else: # Default Test Cases Configuration
            self.case_list.append(
                Case(
                    name=DEFAULT_CASE_NAME,
                    case_file=global_case_file,
                    command=global_command,
                    pts=global_pts,
                    correct_file=global_correct_file,
                    verify_func=None
                )
            )

    def setcase(self, match_case: Union[str, List[str]] = ALL_MATCH_CASE) -> Callable:
        def wrapper(func: Callable) -> None:
            match_case_list = match_case if isinstance(match_case, list) else [match_case]
            for _case in self.case_list:
                if any(fnmatch.fnmatchcase(_case.name, name) for name in match_case_list):
                    _case.verify_func = func
        return wrapper
    
    def run(
        self,
        moodle_submission_dir: str,
        specific_students: list = [],
        save_path: str = DEFAULT_GRADING_REPORT_PATH,
        disable_extract: bool = False,
        disable_build: bool = False,
        disable_execute: bool = False,
        disable_verify: bool = False
    ) -> None:
        logging.debug(f"Object: {objprint.objstr(self)}")

        student_id_list: List[str] = []
        student_extract_dir = DEFAULT_EXTRACTED_FILES_DIR
        
        # Extract Student Files and Get Student ID List
        if disable_extract:
            student_id_list = [f for f in sorted(os.listdir(DEFAULT_EXTRACTED_FILES_DIR)) if os.path.isdir(os.path.join(DEFAULT_EXTRACTED_FILES_DIR, f))]
        else:
            student_id_list = extract(moodle_submission_dir, student_extract_dir)

        for student_id in student_id_list:
            if specific_students != [] and not any(s in student_id for s in specific_students):
                continue
            
            # Initialize Student Grade Report
            logging.info(f"Grading: {student_id}")
            self.grade_report.append([student_id, [], []])
            student_dir = os.path.join(student_extract_dir, student_id)

            # Build Student File
            logging.info(f"Building {student_id}: {not disable_build}")
            if disable_build: 
                continue
            
            build(folder=student_dir, copy_file=self.support_files)
            
            # Execute and Verify Student File
            for _case in self.case_list:
                exception_signal = None

                logging.info(f"Executing {student_id}: {not disable_execute}, case: {_case.name}")
                if not disable_execute:
                    exception_signal = _case.execute(student_dir=student_dir)
                
                logging.info(f"Verifying: {student_id}: {not disable_verify}, case: {_case.name}, exception: {exception_signal}")
                if exception_signal == ExecuteException.NoFile: 
                    break
                elif disable_verify or exception_signal != None: 
                    continue
                
                _case.verify()
            
            logging.info(f"Grading Complete: {student_id}, grade report: {self.grade_report[-1]}")

        logging.info(f"Generating Grade Report: {save_path}")
        grade(grade_report=self.grade_report, save_path=save_path)