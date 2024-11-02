// grader.rs
use crate::Student;
use crate::unzip_student_file; // Adjust the path if necessary
use crate::run_make; // Adjust the path if necessary
//use crate::log_errors; // Adjust the path if necessary
use std::io;
use crate::cleanup_student_folder; // Add this line to import the function


// pub fn grade_student(student: &mut Student, homework_name: &str) -> Result<(), Box<dyn std::error::Error>> {
//     grader(student, &homework_name)?;
//     prompt_for_grade(student, &homework_name)?;
//     cleanup_student_folder()?;
//     Ok(())
// }

// pub fn load_first_ungraded_student(students: &mut Vec<Student>) -> Option<&mut Student> {
//     students.iter_mut().find(|student| !student.is_graded)
// }

pub fn grader(student: &mut Student, homework_name: &str) -> Result<(), Box<dyn std::error::Error>> {
    if student.is_graded {
        println!("學生 {} 已經被評分過。", student.id);
        //return Ok(());
    }
    println!("處理學生：{} - {}", student.index, student.name);
    if let Some(zip_file) = &student.zip_file {
        if let Err(e) = unzip_student_file(student, "./grader") {
            student.errors.push(format!("解壓縮錯誤: {}", e));
        } else if let Err(e) = run_make(student, homework_name) {
            student.errors.push(format!("make 錯誤: {}", e));
        }
    } else {
        student.errors.push(format!("學生 {} 沒有 zip 檔案。", student.id));
    }

    // if !student.errors.is_empty() {
    //     //log_errors(student)?;
    // }

    Ok(())
}
pub fn prompt_for_grade(student: &mut Student, homework_name: &str) -> Result<(), Box<dyn std::error::Error>> {
    println!("請輸入學生 {} 的 {} 成績：", student.id, homework_name);
    let mut grade = String::new();
    io::stdin().read_line(&mut grade)?;
    let grade = grade.trim().to_string();
    student.grades.insert(homework_name.to_string(), grade.clone());
    println!("學生 {} 的 {} 成績為：{}", student.id, homework_name, grade);
    Ok(())
}
pub fn score_student(student: &mut Student, problem: &str, score: &str) -> Result<(), Box<dyn std::error::Error>> {
    student.grades.insert(problem.to_string(), score.to_string());
    student.is_graded = true;
    Ok(())
}