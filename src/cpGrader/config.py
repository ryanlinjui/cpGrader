import toml
import logging

logging.basicConfig(
    level=logging.DEBUG,
    filename="runtime.log",
    filemode="w",
    format="%(asctime)s %(levelname)s: %(message)s"
)

DEFAULT_WORKSPACE_DIR = "."
DEFAULT_TESTCASE_SRC = "testcase"
DEFAULT_GRADING_REPORT_PATH = "report.csv"
DEFAULT_EXTRACTED_FILES_DIR = "files"
DEFAULT_CONFIG_FILE = "config.toml"
DEFAULT_CASE_NAME = "DefaultCase"

# 5 MB with 1 byte = 1 English character
MAX_OUTPUT_SIZE = 5 * 1024 * 1024

# wildcard for all cases
ALL_MATCH_CASE = "*"