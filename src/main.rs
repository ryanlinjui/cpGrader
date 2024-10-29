use std::fs;
use std::fs::File;
use std::io::{self, BufReader, Write};
use std::path::Path;
use std::process::{Command, Stdio};
use regex::Regex;
use zip::read::ZipArchive;
use std::collections::HashMap;

#[derive(Debug, Clone)]
struct Student {
    index: usize,
    id: String,
    name: String,
    zip_file: Option<String>,
    folder_path: String,
    errors: Vec<String>,
    grades: HashMap<String, String>,
}

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
        cleanup_student_folder(student, &homework_name)?;
    }

    Ok(())
}

fn grader(student: &mut Student, homework_name: &str) -> Result<(), Box<dyn std::error::Error>> {
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

fn prompt_for_grade(student: &mut Student, homework_name: &str) -> Result<(), Box<dyn std::error::Error>> {
    println!("請輸入學生 {} 的 {} 成績：", student.id, homework_name);
    let mut grade = String::new();
    io::stdin().read_line(&mut grade)?;
    let grade = grade.trim().to_string();
    student.grades.insert(homework_name.to_string(), grade.clone());
    println!("學生 {} 的 {} 成績為：{}", student.id, homework_name, grade);
    Ok(())
}

fn cleanup_student_folder(student: &Student, homework_name: &str) -> Result<(), Box<dyn std::error::Error>> {
    let student_output_dir = format!("./grader/{}/{}_{}", student.id, student.id, homework_name);
    fs::remove_dir_all(&student_output_dir)?;
    println!("已刪除學生 {} 的資料夾。", student.id);
    Ok(())
}

fn log_errors(student: &Student) -> Result<(), Box<dyn std::error::Error>> {
    let log_file_path = format!("./grader/{}_errors.log", student.id);
    let mut log_file = File::create(log_file_path)?;
    for error in &student.errors {
        writeln!(log_file, "{}", error)?;
    }
    Ok(())
}

fn log_compile(student: &Student, output: &str) -> Result<(), Box<dyn std::error::Error>> {
    let log_file_path = format!("./grader/{}_compile.log", student.id);
    let mut log_file = File::create(log_file_path)?;
    writeln!(log_file, "{}", output)?;
    Ok(())
}

fn run_make(student: &Student, homework_name: &str) -> Result<(), Box<dyn std::error::Error>> {
    let student_output_dir = format!("./grader/{}/{}_{}", student.id, student.id, homework_name);
    let output = Command::new("make")
        .current_dir(&student_output_dir)
        .output()?;

    let stdout = String::from_utf8_lossy(&output.stdout);
    let stderr = String::from_utf8_lossy(&output.stderr);

    let compile_log = format!("stdout:\n{}\nstderr:\n{}", stdout, stderr);
    log_compile(student, &compile_log)?;

    if output.status.success() {
        println!("學生 {} 的 make 成功。", student.id);
    } else {
        println!("學生 {} 的 make 失敗。", student.id);
        return Err(Box::from("make 失敗"));
    }

    Ok(())
}

fn extract_students(target_dir: &str) -> Result<Vec<Student>, Box<dyn std::error::Error>> {
    let re = Regex::new(r"(\d{8}[A-Z])\s+(\S+)_\d+_assignsubmission_file_")?;
    let mut students = Vec::new();
    let entries = fs::read_dir(target_dir)?;

    for (index, entry) in entries.enumerate() {
        let entry = entry?;
        let path = entry.path();
        if !path.is_dir() {
            continue;
        }

        let folder_name = match path.file_name().and_then(|n| n.to_str()) {
            Some(name) => name,
            None => continue,
        };

        let caps = match re.captures(folder_name) {
            Some(caps) => caps,
            None => continue,
        };

        let mut zip_file = None;
        let student_entries = fs::read_dir(&path)?;
        for student_entry in student_entries {
            let student_entry = student_entry?;
            let student_path = student_entry.path();
            if student_path.extension().and_then(|ext| ext.to_str()) == Some("zip") {
                if let Some(file_name) = student_path.file_name().and_then(|n| n.to_str()) {
                    zip_file = Some(file_name.to_string());
                    break;
                }
            }
        }

        let student = Student {
            index,
            id: caps[1].to_string(),
            name: caps[2].to_string(),
            zip_file,
            folder_path: path.to_str().unwrap().to_string(),
            errors: Vec::new(),
            grades: HashMap::new(),
        };
        students.push(student);
    }

    Ok(students)
}

fn unzip_student_file(student: &Student, output_dir: &str) -> Result<(), Box<dyn std::error::Error>> {
    let zip_file_path = format!("{}/{}", student.folder_path, student.zip_file.as_ref().unwrap());
    let file = File::open(&zip_file_path)?;
    let mut archive = ZipArchive::new(BufReader::new(file))?;

    let student_output_dir = format!("{}/{}", output_dir, student.id);
    fs::create_dir_all(&student_output_dir)?;

    for i in 0..archive.len() {
        let mut file = archive.by_index(i)?;
        let outpath = match file.enclosed_name() {
            Some(path) => student_output_dir.clone() + "/" + path.to_str().unwrap(),
            None => continue,
        };

        if file.name().ends_with('/') {
            fs::create_dir_all(&outpath)?;
        } else {
            if let Some(p) = Path::new(&outpath).parent() {
                if !p.exists() {
                    fs::create_dir_all(&p)?;
                }
            }
            let mut outfile = File::create(&outpath)?;
            io::copy(&mut file, &mut outfile)?;
        }
    }

    Ok(())
}