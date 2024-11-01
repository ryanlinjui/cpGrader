
// // Import the Student type
// use crate::student::Student;
// use std::fs::File;

// pub fn log_errors(student: &Student) -> Result<(), Box<dyn std::error::Error>> {
//     let log_file_path = format!("./grader/{}_errors.log", student.id);
//     let mut log_file = File::create(log_file_path)?;
//     for error in &student.errors {
//         writeln!(log_file, "{}", error)?;
//     }
//     Ok(())
// }

// pub fn log_compile(student: &Student, output: &str) -> Result<(), Box<dyn std::error::Error>> {
//     let log_file_path = format!("./grader/{}_compile.log", student.id);
//     let mut log_file = File::create(log_file_path)?;
//     writeln!(log_file, "{}", output)?;
//     Ok(())
// }
