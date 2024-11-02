// load.rs
use std::fs;
//use std::intrinsics::simd::simd_or;
use std::path::Path;
use regex::Regex;
use std::collections::HashMap;
use crate::Student;
use serde_json::Value;
use std::fs::File;
use std::io::Write;
//use csv::Writer;

pub fn load(target_dir: &str) -> Result<Vec<Student>, Box<dyn std::error::Error>> {
    let status_file = format!("./status/status.json");
    if Path::new(&status_file).exists() {
        let file_content = fs::read_to_string(&status_file).map_err(|e| Box::new(e) as Box<dyn std::error::Error>)?;
        let students: Vec<Student> = serde_json::from_str(&file_content).map_err(|e| Box::new(e) as Box<dyn std::error::Error>)?;
        return Ok(students);
    }
    // 提取學生資料
    let students = extract_students(target_dir)?;

    // 序列化學生資料
    let serialized_students = serde_json::to_string(&students).map_err(|e| Box::new(e) as Box<dyn std::error::Error>)?;
    

    //sort students by id

    // 將序列化的資料寫入 status 檔案
    let mut file = File::create(status_file).map_err(|e| Box::new(e) as Box<dyn std::error::Error>)?;
    file.write_all(serialized_students.as_bytes()).map_err(|e| Box::new(e) as Box<dyn std::error::Error>)?;

    Ok(students)
}
pub fn store(students: &Vec<Student>) -> Result<(), Box<dyn std::error::Error>> {
    let status_file = format!("./status/status.json");
    let serialized_students = serde_json::to_string(students).map_err(|e| Box::new(e) as Box<dyn std::error::Error>)?;
    let mut file = File::create(status_file).map_err(|e| Box::new(e) as Box<dyn std::error::Error>)?;
    file.write_all(serialized_students.as_bytes()).map_err(|e| Box::new(e) as Box<dyn std::error::Error>)?;
    Ok(())
}

pub fn extract_students(target_dir: &str) -> Result<Vec<Student>, Box<dyn std::error::Error>> {
    let re = Regex::new(r"(\d{8}[a-zA-Z])\s+(\S+)_\d+_assignsubmission_file_")?;
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
            id: caps[1].to_uppercase(),
            name: caps[2].to_string(),
            zip_file,
            folder_path: path.to_str().unwrap().to_string(),
            errors: Vec::new(),
            grades: HashMap::new(),
            is_graded: false,
        };
        students.push(student);
    }
    // Sort students by id
    students.sort_by(|a, b| a.id.cmp(&b.id));

    // Change index by id
    for (new_index, student) in students.iter_mut().enumerate() {
        student.index = new_index;
    }

    Ok(students)
}

pub fn to_csv(students: &Vec<Student>, output_file: &str) -> Result<(), Box<dyn std::error::Error>> {
    let mut wtr = csv::Writer::from_path(output_file)?;

    // Write the header
    wtr.write_record(&["Index", "ID", "Name", "Zip File", "Folder Path", "Errors", "Grades", "Is Graded"])?;

    // Write student data
    for student in students {
        wtr.write_record(&[
            student.index.to_string(),
            student.id.clone(),
            student.name.clone(),
            student.zip_file.clone().unwrap_or_default(),
            student.folder_path.clone(),
            format!("{:?}", student.errors),
            format!("{:?}", student.grades),
            student.is_graded.to_string(),
        ])?;
    }

    wtr.flush()?;
    Ok(())
}