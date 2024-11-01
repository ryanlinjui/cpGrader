
use clap::{arg, command, value_parser, Arg, ArgAction, ArgGroup, Command};


fn main(){
    let match_result = command!()
    .subcommand(
        Command::new("score")
            .about("Input students score for one problem")
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
    .arg(
        Arg::new("clean")
            .short('c')
            .long("clean")
            //.required(true)
            .help("Clean the grader dir")
            .conflicts_with_all(["log","exit"])
    )
    .arg(
        Arg::new("log")
            .short('l')
            .long("log")
            //.required(true)
            .help("print the log")
            .conflicts_with_all(["clean","exit"])
    )
    .arg(
        Arg::new("exit")
            .short('e')
            .long("exit")
            //.required(true)
            .help("exit the program and create csv of grade")
            //.conflicts_with_all("")
    )
    .get_matches();

    if let Some(matches) = match_result.subcommand_matches("score") {
        let problem: &String = matches.get_one("problem").unwrap();
        let score: &String = matches.get_one("score").unwrap();
        println!("Scoring problem {} with score {}", problem, score);
    } else if let Some(matches) = match_result.subcommand_matches("build") {
        if let Some(index) = matches.get_one::<String>("index") {
            println!("Building for index {}", index);
        } else if let Some(id) = matches.get_one::<String>("id") {
            println!("Building for ID {}", id);
        }
    } else if match_result.contains_id("clean") {
        println!("Cleaning the grader dir");
    } else if match_result.contains_id("log") {
        println!("Printing the log");
    } else if match_result.contains_id("exit") {
        println!("Exiting the program and creating CSV of grade");
    }
}
