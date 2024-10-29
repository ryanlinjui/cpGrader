use std::fs;
use std::fs::File;
use std::io::{self, BufReader, Write};
use std::path::Path;
use regex::Regex;
use zip::read::ZipArchive;

#[derive(Debug)]
struct Student {
    index: usize,
    id: String,
    name: String,
    zip_file: Option<String>,
    folder_path: String,
}

fn main() -> Result<(), Box<dyn std::error::Error>> {
    let target_dir = "./example"; // 替換成你的目標資料夾路徑
    let students = extract_students(target_dir)?;

    for student in &students {
        println!("{:?}", student);
    }

    println!("請選擇一個學生的索引：");
    let mut input = String::new();
    io::stdin().read_line(&mut input)?;
    let index: usize = input.trim().parse()?;

    if let Some(student) = students.iter().find(|&s| s.index == index) {
        if let Some(zip_file) = &student.zip_file {
            unzip_student_file(student, "./grader")?;
        } else {
            println!("學生 {} 沒有 zip 檔案。", student.id);
        }
    } else {
        println!("無效的索引。");
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