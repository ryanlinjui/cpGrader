import os
import csv
import time
import fnmatch
import shutil
import subprocess
import zipfile
from typing import (
    List,
    Tuple,
    Union
)

import pexpect

def extract(src_folder:str, dest_folder:str) -> list:
    target_key = ["*akefile", "hw0*0*.c", "README*", "bonus*", "*.pdf"]
    filter_dir = ["__MACOSX", ".vscode"]
    exclude_folder_name = ["NTU", "NTUST"]
    student_list = []

    root = os.getcwd()
    os.chdir(src_folder)

    # Create tmp folder
    if os.path.exists("tmp"):
        shutil.rmtree("tmp")
    os.mkdir("tmp")

    # Create dest_folder
    dest_folder = os.path.join(root, dest_folder)
    if os.path.exists(dest_folder):
        shutil.rmtree(dest_folder)
    os.mkdir(dest_folder)

    # Check the src_folder
    for current_dir in sorted(os.listdir()):

        # Check if the current_dir is a moodle submission directory
        if os.path.isdir(current_dir) == False or ("assignsubmission_file" in current_dir) == False:
            continue
        
        # Process the directory name to get the student_id
        student_id = current_dir.split(" ")[0]
        if any(name in student_id for name in exclude_folder_name):
            student_id = current_dir.split(" ")[0] + "_" + current_dir.split(" ")[1]

        # Extract the zip file
        os.mkdir(os.path.join("tmp", student_id))
        os.chdir(current_dir)
        for file in os.listdir():
            if file.endswith(".zip"):
                with zipfile.ZipFile(os.path.join(file), "r") as zip_ref:
                    zip_ref.extractall(os.path.join("..", "tmp", student_id))
                break

        # Dive and find src_folder that have target_key list, and the result must be not filter_dir
        os.chdir(os.path.join("..", "tmp"))
        target_dir = os.path.join(student_id)
        os.chdir(target_dir)

        for r, dirs, files in os.walk("."):
            if any(fnmatch.filter(files, key) for key in target_key) and not any(filt in r for filt in filter_dir):
                target_dir = os.path.abspath(r)
                break
        
        # In case: Found the target_dir. Student ID and the Directory is valid.
        if "/" in target_dir: 
            shutil.copytree(target_dir, os.path.join(root, src_folder, dest_folder, student_id))
            student_list.append(student_id)
        
        os.chdir(os.path.join(root, src_folder))

    shutil.rmtree("tmp")
    os.chdir(root)

    return student_list

def build(folder:str, copy_file:list=[], command:str="make") -> None:
    root = os.getcwd()
    for file in copy_file:
        try:
            if os.path.isdir(file):
                shutil.copytree(file, os.path.join(folder, os.path.basename(file)))
            else:
                shutil.copy(file, folder)

        except shutil.SameFileError:
            pass

    os.chdir(folder)
    with open(os.devnull, "w") as f:
        try:
            subprocess.call(command, shell=True, stdout=f, stderr=f, timeout=3)
        except subprocess.TimeoutExpired:
            pass
    os.chdir(root)

class ExecuteException(Exception):
    def __init__(self, message: str, output: str = ""):
        super().__init__(message)
        self.output = output

    class ProgramError(Exception):
        COMMENT = "program error"
        
        def __init__(self, message: str, output: str = ""):
            super().__init__(message)
            self.comment = self.COMMENT
            self.output = output

    class NoFile(Exception):
        COMMENT = "no file"

        def __init__(self, message: str, output: str = ""):
            super().__init__(message)
            self.comment = self.COMMENT
            self.output = output

def execute(folder:str, command:str, stdin_list:list=[]) -> str:
    root = os.getcwd()
    os.chdir(folder)
    
    output = ""
    errno_output = ""
    TIMEOUT = 5

    try:
        p = pexpect.spawn(f"/bin/sh -c '{command}; echo ERRNO:\<$?\>'; exit;", encoding="utf-8", timeout=TIMEOUT)
        for line in stdin_list:
            time.sleep(0.1) # input buffer time
            p.sendline(line)
        p.expect(pexpect.EOF)

        index = p.before.find("ERRNO:<")
        output = p.before[:index].replace("\r", "")
        errno_output = p.before[index:].replace("\r", "")

    except pexpect.TIMEOUT:
        error_msg = f"In folder {folder} with command {command}, timeout {TIMEOUT} seconds"
        raise ExecuteException.ProgramError(error_msg, output)

    except KeyboardInterrupt as e:
        p.terminate(force=True)
        error_msg = f"In folder {folder} with command {command}, keyboard interrupt"
        raise ExecuteException.ProgramError(error_msg, output)
    
    except Exception as e:
        raise e

    finally:
        p.close()
        os.chdir(root)

    errno_lines = errno_output.split("\n")
    permit_errno = [0, 1, 11, 255]

    if not any(f"ERRNO:<{errno}>" in line for line in errno_lines for errno in permit_errno):
        if any("ERRNO:<127>" in line for line in errno_lines):
            error_msg = f"In folder {folder} with command {command}, no such file or directory"
            raise ExecuteException.NoFile(error_msg, output)
        else:
            error_msg = f"In folder {folder} with command {command}, program error"
            raise ExecuteException.ProgramError(error_msg, output)
    
    return output

def grade(grade_report: List[Tuple[str, List[Union[int, str]], List[str]]], save_path: str) -> None:
    for student in grade_report:
        student[1] = sum(student[1])
        student[2] = "\n".join(student[2])

    with open(save_path, "w+", newline="") as f:
        writer = csv.writer(f)
        writer.writerows(grade_report)