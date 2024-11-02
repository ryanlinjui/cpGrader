use crate::Student;
use std::fs;

pub fn cleanup_student_folder() -> Result<(), Box<dyn std::error::Error>> {
    let student_output_dir = format!("./grader/");
    fs::remove_dir_all(&student_output_dir)?;
    println!("已刪除學生的資料夾。");
    Ok(())
}