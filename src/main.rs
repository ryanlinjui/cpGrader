mod load;
mod build;
mod grader;
mod clean;
mod log;
mod student;

use std::io;
use load::extract_students;
use build::{unzip_student_file, run_make};
use grader::{grader, prompt_for_grade};
use clean::cleanup_student_folder;
//use log::log_compile;
use student::Student;

// fn main() -> Result<(), Box<dyn std::error::Error>> {
//     let target_dir = "./example"; // 替換成你的目標資料夾路徑
//     let mut students = extract_students(target_dir)?;

//     println!("請輸入該次作業名稱：");
//     let mut homework_name = String::new();
//     io::stdin().read_line(&mut homework_name)?;
//     let homework_name = homework_name.trim().to_string();

//     for student in &students {
//         println!("{:?}", student);
//     }

//     for student in &mut students {
//         grader(student, &homework_name)?;
//         prompt_for_grade(student, &homework_name)?;
//         cleanup_student_folder(student, &homework_name)?;
//     }

//     Ok(())
// }