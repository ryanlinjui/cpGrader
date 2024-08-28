use std::fs::{self, DirEntry};
use std::io;
use std::path::{Path, PathBuf};
use zip::ZipArchive;
use std::ffi::OsStr;

fn extract(src_folder: &str, dest_folder: &str) -> io::Result<Vec<String>> {
    let target_key = vec!["*akefile", "hw0*0*.c", "README*", "bonus*", "*.pdf"];
    let filter_dir = vec!["__MACOSX", ".vscode"];
    let exclude_folder_name = vec!["NTU", "NTUST"];

    let mut student_list = Vec::new();

    let root = std::env::current_dir()?;
    let src_folder_path = root.join(src_folder);
    let dest_folder_path = root.join(dest_folder);

    // Create dest_folder
    if dest_folder_path.exists() {
        fs::remove_dir_all(&dest_folder_path)?;
    }
    fs::create_dir(&dest_folder_path)?;

    // Check the src_folder
    for entry in fs::read_dir(src_folder_path)? {
        let entry = entry?;
        let path = entry.path();
        if path.is_dir() && path.file_name().unwrap_or_default().to_str().unwrap_or_default().contains("assignsubmission_file") {
            let student_id = path.file_name().unwrap_or_default().to_str().unwrap_or_default().split(" ").next().unwrap_or_default().to_string();
            if exclude_folder_name.iter().any(|&name| student_id.contains(name)) {
                continue;
            }

            // Extract the zip file
            for entry in fs::read_dir(&path)? {
                let entry = entry?;
                let path = entry.path();
                if path.extension() == Some(OsStr::new("zip")) {
                    let file = fs::File::open(&path)?;
                    let mut archive = ZipArchive::new(file)?;
                    for i in 0..archive.len() {
                        let mut file = archive.by_index(i)?;
                        let outpath = dest_folder_path.join(file.sanitized_name());

                        if (&*file.name()).ends_with('/') {
                            fs::create_dir_all(&outpath)?;
                        } else {
                            if let Some(p) = outpath.parent() {
                                if !p.exists() {
                                    fs::create_dir_all(&p)?;
                                }
                            }
                            let mut outfile = fs::File::create(&outpath)?;
                            io::copy(&mut file, &mut outfile)?;
                        }
                    }
                    student_list.push(student_id);
                    break; // Only process the first zip file
                }
            }
        }
    }

    Ok(student_list)
}