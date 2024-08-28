struct folders {
    pub src: String,
    pub dest: String,
}


impl folders {
    pub fn new(src: &str, dest: &str) -> folders {
        folders {
            src: src.to_string(),
            dest: dest.to_string(),
        }
    }
}

pub fn exract(src: &str, dest: &str) -> vec<str> {
    target_key = vec!["*akefile", "hw0*0*.c", "README*", "bonus*", "*.pdf"];
    filter_dir = vec!["__MACOSX",".vscode",".git"];
    exclude_folder_name = ["NTU", "NTUST"];

    student_list = vec![];
    
    
    
    folders {
        src: src.to_string(),
        dest: dest.to_string(),
    }
 
}