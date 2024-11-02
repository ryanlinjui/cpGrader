mod load;
mod build;
mod grader;
mod clean;
mod log;
mod student;

use std::io;
use load::{extract_students, store, to_csv};
use build::{unzip_student_file, run_make};
use grader::{grader, prompt_for_grade, score_student, grade_student};
use clean::cleanup_student_folder;
use serde_json::value::Index;
//use log::log_compile;
use student::Student;
use load::load;
use clap::{arg, command, value_parser, Arg, ArgAction, ArgGroup, Command};
use std::fs;

fn main() -> Result<(), Box<dyn std::error::Error>> {
    let target_dir = "./example"; // 替換成你的目標資料夾路徑
    let mut students = load(target_dir)?;

    let config_content = fs::read_to_string("config.toml")?;
    let config: toml::Value = toml::from_str(&config_content)?;
    let homework_name = config["global"]["homework"].as_str().unwrap().to_string();
    for student in &students {
        println!("{:?}", student);
    }

    let match_result = command!()
    .subcommand(
        Command::new("grade")
            .about("Automatically grade students by dependcies")
            .arg(
                Arg::new("index")
                    .short('i')
                    .long("index")
                    .required(true)
                    .help("This argument define which person to build")
                    .conflicts_with("id")
            )
            .arg(
                Arg::new("id")

                    //.short('id')
                    .long("id")
                    .aliases(["id", "ID"])
                    .required(true)
                    .help("Or you can use student id")
            )
            .arg(
                Arg::new("problem")
                    .short('p')
                    .long("problem")
                    .required(true)
                    .help("This argument define which problem to grade")   
            )
            .arg(
                Arg::new("testcase")
                    .short('t')
                    .long("testcase")
                    .required(true)
                    .help("This argument define which testcase to grade")
            )
    )
    .subcommand(
        Command::new("score")
            .about("Input students score for one problem")
            .arg(
                Arg::new("index")
                    .short('i')
                    .long("index")
                    .required(true)
                    .help("This argument define which person to build")
                    .conflicts_with("id")
            )
            .arg(
                Arg::new("id")

                    //.short('id')
                    .long("id")
                    .aliases(["id", "ID"])
                    .required(true)
                    .help("Or you can use student id")
            )
            .arg(
                Arg::new("problem")
                    .short('p')
                    .long("problem")
                    .required(true)
                    .help("This argument define which problem to score")
                    //.conflicts_with("lastname")
            )
            .arg(
                Arg::new("score")
                    .short('s')
                    .long("score")
                    //.aliases(["lname", "lastname"])
                    .required(true)
                    .help("The score of homework")
            )
    )
    .subcommand(
        Command::new("build")
        .about("Clean, unzip, make, copy depends to grader dir")
            .arg(
                Arg::new("index")
                    .short('i')
                    .long("index")
                    //.required("true")
                    .help("This argument define which person to build")
                    .conflicts_with("id")
            )
            .arg(
                Arg::new("id")

                    //.short('id')
                    .long("id")
                    .aliases(["id", "ID"])
                    //.required(true)
                    .help("Or you can use student id")
            )
    )
    .subcommand(
        Command::new("clean")
            .about("Clean the grader dir")
    )
    .subcommand(
        Command::new("log")
            .about("Print the log")
    )
    .subcommand(
        Command::new("exit")
            .about("Exit the program and create CSV of grade")
    )
    .get_matches();
    if let Some(matches) = match_result.subcommand_matches("grade") {
        println!("Grading students(in development)");
        let problem = matches.get_one::<String>("problem").unwrap();
        let testcase = matches.get_one::<String>("testcase").unwrap();
        

        Ok(())
    } else if let Some(matches) = match_result.subcommand_matches("score") {
        if let Some(index) = matches.get_one::<String>("index") {
            let index: usize = index.parse()?;
            if index < students.len() {
            let student = &mut students[index];
            let problem = matches.get_one::<String>("problem").unwrap();
            let score = matches.get_one::<String>("score").unwrap();
            println!("Scoring student at index {}", index);
            score_student(student, problem, score)?;
            
            } else {
            println!("Index out of bounds");
            }
        } else if let Some(id) = matches.get_one::<String>("id") {
            if let Some(student) = students.iter_mut().find(|s| s.id == *id) {
                println!("Scoring student with ID {}", id);
                let problem = matches.get_one::<String>("problem").unwrap();
                let score = matches.get_one::<String>("score").unwrap();
                score_student(student, problem, score)?;
            } else {
                println!("Student ID not found");
            }
        } else {
            println!("No valid index or ID provided");
        }
        store(&students)?;
        Ok(())
    } else if let Some(matches) = match_result.subcommand_matches("build") {
        let student = if let Some(index) = matches.get_one::<String>("index") {
            println!("Building for index {}", index);
            let index: usize = index.parse()?;
            &mut students[index]
        } else if let Some(id) = matches.get_one::<String>("id") {
            println!("Building for ID(in dev) {}", id);
            let index = students.iter().position(|s| s.id == *id).unwrap();
            &mut students[index]
        } else {
            println!("Building for first ungraded student");
            let index = students.iter().position(|s| !s.is_graded).unwrap();
            &mut students[index]
        };
        grader(student, &homework_name)?;
        store(&students)?;
        Ok(())
    } else if let Some(matches) = match_result.subcommand_matches("clean"){
        println!("Cleaning the grader dir");
        cleanup_student_folder()?;
        store(&students)?;
        Ok(())

    } else if let Some(matches) = match_result.subcommand_matches("log") {
        println!("Printing the log");
        for student in &students {
            println!("{:?}", student);
        }
        store(&students)?;
        Ok(())
    } else if let Some(matches) = match_result.subcommand_matches("exit") {
        println!("Exiting the program and creating CSV of grade");
        to_csv(&students, "./grades.csv")?;
        Ok(())
    } else {
        Ok(())
    }
}
