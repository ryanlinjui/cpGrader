use std::fs::{self, File};
use std::io::{self, BufReader};
use std::path::Path;
use zip::read::ZipArchive;
use std::process::Command;
use crate::Student;

pub fn unzip_student_file(student: &Student, output_dir: &str) -> Result<(), Box<dyn std::error::Error>> {
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

pub fn run_make(student: &Student, homework_name: &str) -> Result<(), Box<dyn std::error::Error>> {
    let student_output_dir = format!("./grader/{}/{}_{}", student.id, student.id, homework_name);
    let output = Command::new("make")
        .current_dir(&student_output_dir)
        .output()?;

    // let stdout = String::from_utf8_lossy(&output.stdout);
    // let stderr = String::from_utf8_lossy(&output.stderr);

    //let compile_log = format!("stdout:\n{}\nstderr:\n{}", stdout, stderr);
    //log_compile(student, &compile_log)?;

    if output.status.success() {
        println!("學生 {} 的 make 成功。", student.id);
    } else {
        println!("學生 {} 的 make 失敗。", student.id);
        return Err(Box::from("make 失敗"));
    }

    Ok(())
}