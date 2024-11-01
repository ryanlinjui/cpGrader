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
use clap::{arg, command, value_parser, Arg, ArgAction, ArgGroup, Command};

fn main() -> Result<(), Box<dyn std::error::Error>> {
    let target_dir = "./example"; // 替換成你的目標資料夾路徑
    let mut students = extract_students(target_dir)?;

    println!("請輸入該次作業名稱：");
    let mut homework_name = String::new();
    io::stdin().read_line(&mut homework_name)?;
    let homework_name = homework_name.trim().to_string();

    for student in &students {
        println!("{:?}", student);
    }

    for student in &mut students {
        grader(student, &homework_name)?;
        prompt_for_grade(student, &homework_name)?;
        cleanup_student_folder()?;
    }

    Ok(())
}
// fn main(){
//     let match_result = command!()
//     .subcommand(
//         Command::new("grade")
//             .about("Automatically grade students by dependcies")
//     )
//     .subcommand(
//         Command::new("score")
//             .about("Input students score for one problem")
//             .arg(
//                 Arg::new("problem")
//                     .short('p')
//                     .long("problem")
//                     .required(true)
//                     .help("This argument define which problem to score")
//                     //.conflicts_with("lastname")
//             )
//             .arg(
//                 Arg::new("score")

//                     .short('s')
//                     .long("score")
//                     //.aliases(["lname", "lastname"])
//                     .required(true)
//                     .help("The score of homework")
//             )
//     )
//     .subcommand(
//         Command::new("build")
//         .about("Clean, unzip, make, copy depends to grader dir")
//             .arg(
//                 Arg::new("index")
//                     .short('i')
//                     .long("index")
//                     //.required("true")
//                     .help("This argument define which person to build")
//                     .conflicts_with("id")
//             )
//             .arg(
//                 Arg::new("id")

//                     //.short('id')
//                     .long("id")
//                     .aliases(["id", "ID"])
//                     //.required(true)
//                     .help("Or you can use student id")
//             )
//     )
//     .arg(
//         Arg::new("clean")
//             .short('c')
//             .long("clean")
//             //.required(true)
//             .help("Clean the grader dir")
//             .conflicts_with_all(["log","exit"])
//             .action(ArgAction::SetTrue)
//     )
//     .arg(
//         Arg::new("log")
//             .short('l')
//             .long("log")
//             //.required(true)
//             .help("print the log")
//             .conflicts_with_all(["clean","exit"])
//             .action(ArgAction::SetTrue)
//     )
//     .arg(
//         Arg::new("exit")
//             .short('e')
//             .long("exit")
//             //.required(true)
//             .help("exit the program and create csv of grade")
//             .action(ArgAction::SetTrue)
//             //.conflicts_with_all("")
//     )
//     .get_matches();
//     if let Some(matches) = match_result.subcommand_matches("grade") {
//         println!("Grading students");
//     }else if let Some(matches) = match_result.subcommand_matches("score") {
//         let problem: &String = matches.get_one("problem").unwrap();
//         let score: &String = matches.get_one("score").unwrap();
//         println!("Scoring problem {} with score {}", problem, score);
//     } else if let Some(matches) = match_result.subcommand_matches("build") {
//         if let Some(index) = matches.get_one::<String>("index") {
//             println!("Building for index {}", index);
//         } else if let Some(id) = matches.get_one::<String>("id") {
//             println!("Building for ID {}", id);
//         }
//     } else if match_result.contains_id("clean") {
//         println!("Cleaning the grader dir");
//         cleanup_student_folder();
//     } else if match_result.contains_id("log") {
//         println!("Printing the log");
//     } else if match_result.contains_id("exit") {
//         println!("Exiting the program and creating CSV of grade");
//     }
// }
