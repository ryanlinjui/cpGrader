use crate::Student;
use std::fs;

pub fn cleanup_student_folder(student: &Student, homework_name: &str) -> Result<(), Box<dyn std::error::Error>> {
    let student_output_dir = format!("./grader/{}/{}_{}", student.id, student.id, homework_name);
    fs::remove_dir_all(&student_output_dir)?;
    println!("已刪除學生 {} 的資料夾。", student.id);
    Ok(())
}