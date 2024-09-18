use csv::ReaderBuilder;
use serde::Deserialize;
use std::collections::HashMap;
use std::error::Error;
use std::fs::{self, File};
use std::io::{Read, Write};
use std::path::{Path, PathBuf};
use std::fs::read_to_string;
use zip::read::ZipArchive;

#[derive(Debug, Deserialize)]
struct Student {
    index: String,
    id: String,
    name: String,
    class: String,
    total_grade: String,
    final_total_grade: Option<String>,
    hw01: String,
    hw02: String,
    hw03: String,
    hw04: String,
    hw05: String,
    mid: String,
    final_exam: String,
    bonus: String,
}

#[derive(Debug, Deserialize)]
struct Config {
    target_directory: String,
    assignment: String,
    date: String,
    scores: HashMap<String, u32>,
}

fn read_csv(file_path: &str) -> Result<HashMap<String, Student>, Box<dyn Error>> {
    let file = File::open(file_path)?;
    let mut rdr = ReaderBuilder::new()
        .has_headers(false) // 不啟用標題行的自動解析
        .from_reader(file);

    // 手動讀取並跳過標題行
    let mut records = rdr.records();
    if let Some(header) = records.next() {
        header?; // 讀取並丟棄標題行
    }

    let mut students = HashMap::new();
    for result in records {
        let record = result?;
        let student: Student = record.deserialize(None)?;
        students.insert(student.id.clone(), student);
    }

    Ok(students)
}

fn process_subdirectories(base_path: &str, students: &HashMap<String, Student>) -> Result<HashMap<String, String>, Box<dyn Error>> {
    let submissions_path = Path::new(base_path).join("submissions");
    println!("Submissions path: {:?}", submissions_path);
    let mut submissions = HashMap::new();

    for (id ,_student) in students {
        let mut found = false;

        for entry in fs::read_dir(&submissions_path)? {
            let entry = entry?;
            let path = entry.path();

            if !path.is_dir() {
                continue;
            }

            let dir_name = match path.file_name().and_then(|n| n.to_str()) {
                Some(name) => name,
                None => continue,
            };

            let parts: Vec<&str> = dir_name.split_whitespace().collect();
            if parts.len() < 1 {
                continue;
            }

            let student_id = parts[0];
            if student_id != id {
                continue;
            }

            for file_entry in fs::read_dir(&path)? {
                let file_entry = file_entry?;
                let file_path = file_entry.path();
                if file_path.is_file() && file_path.extension().and_then(|ext| ext.to_str()) == Some("zip") {
                    submissions.insert(id.clone(), file_path.to_string_lossy().to_string());
                    found = true;
                    break;
                }
            }

            if found {
                break;
            }
        }

        if !found {
            submissions.insert(id.clone(), "no submit".to_string());
        }
    }

    Ok(submissions)
}

fn extract_submissions(base_path: &str, submissions: &HashMap<String, String>) -> Result<(), Box<dyn Error>> {
    let extraction_path = Path::new(base_path).join("extraction");

    // 檢查並建立 extraction 目錄
    if !extraction_path.exists() {
        fs::create_dir(&extraction_path)?;
    }

    for (id, file_path) in submissions {
        if file_path == "no submit" {
            continue;
        }

        let student_extraction_path = extraction_path.join(id);

        // 檢查並建立學生的 extraction 目錄
        if !student_extraction_path.exists() {
            fs::create_dir(&student_extraction_path)?;
        }

        let file = File::open(file_path)?;
        let mut archive = ZipArchive::new(file)?;

        for i in 0..archive.len() {
            let mut file = archive.by_index(i)?;
            let outpath = student_extraction_path.join(file.name());

            if file.name().ends_with('/') {
                fs::create_dir_all(&outpath)?;
            } else {
                if let Some(p) = outpath.parent() {
                    if !p.exists() {
                        fs::create_dir_all(&p)?;
                    }
                }
                let mut outfile = File::create(&outpath)?;
                std::io::copy(&mut file, &mut outfile)?;
            }
        }
    }

    Ok(())
}

fn read_config(file_path: &str) -> Result<Config, Box<dyn Error>> {
    let config_content = read_to_string(file_path)?;
    let config: Config = toml::from_str(&config_content)?;
    Ok(config)
}

fn main() {
    let config = match read_config("config.toml") {
        Ok(config) => config,
        Err(err) => {
            eprintln!("Error reading config file: {}", err);
            return;
        }
    };

    let students = match read_csv("grade.csv") {
        Ok(students) => students,
        Err(err) => {
            eprintln!("Error reading CSV file: {}", err);
            return;
        }
    };

    // 打印讀取到的學生資料
    for (id, student) in &students {
        println!("學號: {}, 姓名: {}", id, student.name);
    }

    let submissions = match process_subdirectories(&config.target_directory, &students) {
        Ok(submissions) => submissions,
        Err(err) => {
            eprintln!("Error processing subdirectories: {}", err);
            return;
        }
    };

    // 打印每個學生的提交狀態
    for (id, file) in &submissions {
        println!("學號: {}, 檔案: {}", id, file);
    }

    // 解壓縮每個有提交作業的學生的 ZIP 檔案
    if let Err(err) = extract_submissions(&config.target_directory, &submissions) {
        eprintln!("Error extracting submissions: {}", err);
    }

    // 打印作業和分數配置
    println!("本次作業: {}", config.assignment);
    println!("日期: {}", config.date);
    for (task, score) in &config.scores {
        println!("題目: {}, 分數: {}", task, score);
    }
}