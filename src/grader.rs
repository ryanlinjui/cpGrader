// grader.rs
use crate::Student;
use crate::unzip_student_file; // Adjust the path if necessary
use crate::run_make; // Adjust the path if necessary
//use crate::log_errors; // Adjust the path if necessary
use std::io;
use crate::cleanup_student_folder; // Add this line to import the function
use std::process::Command;
use std::io::Write;
use std::fs;


pub fn grade_student(student: &mut Student, homework_name: &str, testcase_name: &str)-> Result<(), Box<dyn std::error::Error>> {
    // Iterate over all test cases

    let config_content = fs::read_to_string("config.toml")?;
    let config: toml::Value = toml::from_str(&config_content)?;
    let testcase_num: i32 = config[testcase_name]["testcase"].as_str().unwrap().parse().unwrap();
    
    let mut total_score = 0;
    for i in 1..=testcase_num { // Assuming there are 10 test cases
        println!("running testcase: {}", i);

        // // Construct the command to run the student's homework
        // let student_id = &student.id;
        // let hw_path = format!("./grader/{}/{}_HW01/hw010{}", student_id, student_id, i);
        // let input_path = format!("./testcase/testcase05/in/{}.in", i);
        // let output_path = format!("./testcase/testcase05/out/{}.out", i);

        // // Run the student's homework with the input file
        // let output = Command::new(hw_path)
        //     .arg("<")
        //     .arg(&input_path)
        //     .output()
        //     .expect("Failed to execute process");

        // // Print the output
        // println!("{}", String::from_utf8_lossy(&output.stdout));

        // // Prompt for the score
        // println!("請輸入分數：");
        // let mut score = String::new();
        // io::stdin().read_line(&mut score)?;
        // let score: i32 = score.trim().parse().expect("Invalid score input");

        // // Add the score to the total score
        // total_score += score;
        //     Ok(())
        // }
    
        // // Print the total score
        // println!("總分：{}", total_score);
    
        // // Update the student's score
        // student.grades.insert(homework_name.to_string(), total_score.to_string());
        // student.is_graded = true;
    
    }

    // Print the total score
    println!("total_score：{}", total_score);

    // Update the student's score
    student.grades.insert(homework_name.to_string(), total_score.to_string());
    student.is_graded = true;
    Ok(())
}

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