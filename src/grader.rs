// grader.rs
use crate::Student;

pub fn grader(student: &mut Student, homework_name: &str) -> Result<(), Box<dyn std::error::Error>> {
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

    if !student.errors.is_empty() {
        log_errors(student)?;
    }

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