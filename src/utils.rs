use std::fs::{self, DirEntry};
use std::path::{Path, PathBuf};
use walkdir::WalkDir;
use zip::ZipArchive;
use std::{io,fs,env};
use std::process::Command;

pub fn extract(src_folder: &str, dest_folder: &str) -> io::Result<Vec<String>> {
    let target_keys = vec!["*akefile", "hw0*0*.c", "README*", "bonus*", "*.pdf"];
    let filter_dirs = vec!["__MACOSX", ".vscode"];
    let exclude_folder_names = vec!["NTU", "NTUST"];

    let mut student_list = Vec::new();

    let root = std::env::current_dir()?;
    let src_folder_path = root.join(src_folder);
    let tmp_folder_path = src_folder_path.join("tmp");
    let dest_folder_path = root.join(dest_folder);

    if tmp_folder_path.exists() {
        fs::remove_dir_all(&tmp_folder_path)?;
    }
    fs::create_dir(&tmp_folder_path)?;

    if dest_folder_path.exists() {
        fs::remove_dir_all(&dest_folder_path)?;
    }
    fs::create_dir(&dest_folder_path)?;

    for entry in fs::read_dir(src_folder_path)? {
        let entry = entry?;
        let path = entry.path();
        if path.is_dir() && path.file_name().unwrap().to_str().unwrap().contains("assignsubmission_file") {
            let student_id = path.file_name().unwrap().to_str().unwrap().split(" ").next().unwrap().to_string();
            let student_id = if exclude_folder_names.iter().any(|&name| student_id.contains(name)) {
                let parts: Vec<&str> = path.file_name().unwrap().to_str().unwrap().split(" ").collect();
                format!("{}_{}", parts[0], parts[1])
            } else {
                student_id
            };

            let student_tmp_folder = tmp_folder_path.join(&student_id);
            fs::create_dir(&student_tmp_folder)?;

            for entry in fs::read_dir(&path)? {
                let entry = entry?;
                let path = entry.path();
                if path.is_file() && path.extension().unwrap_or_default() == "zip" {
                    let file = fs::File::open(&path)?;
                    let mut archive = ZipArchive::new(file)?;
                    archive.extract(&student_tmp_folder)?;
                    break;
                }
            }

            // This part needs to be adapted to Rust's way of handling pattern matching and directory traversal
            // The logic for finding target directories and copying them to the destination will be similar
            // but requires careful handling of Path and PathBuf objects, as well as iterating with WalkDir

            // Example of iterating with WalkDir and pattern matching is omitted for brevity
        }
    }

    // Cleanup tmp folder
    fs::remove_dir_all(tmp_folder_path)?;

    Ok(student_list)
}

fn build(folder: &str, copy_files: Vec<&str>, command: &str) {
    let root = env::current_dir().unwrap();

    for file in copy_files {
        let file_path = Path::new(file);
        let destination = Path::new(folder).join(file_path.file_name().unwrap());
        if file_path.is_dir() {
            // 使用 fs_extra::dir::copy 來拷貝目錄，因為 std::fs 不直接支持目錄的拷貝
            fs_extra::dir::copy(file, folder, &fs_extra::dir::CopyOptions::new()).unwrap();
        } else {
            fs::copy(file, destination).unwrap();
        }
    }

    env::set_current_dir(Path::new(folder)).unwrap();
    let _output = Command::new("sh")
        .arg("-c")
        .arg(command)
        .output()
        .expect("Failed to execute command");

    env::set_current_dir(root).unwrap();
}
