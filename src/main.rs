
use clap::{arg, command, value_parser, Arg, ArgAction, ArgGroup, Command};

fn main() {
    let match_result = command!()
    .subcommand(
        Command::new("register-person")
            .arg(
                Arg::new("firstname")
                    //.group("person-register")
                    .short('f')
                    .long("first-name")
                    .aliases(["fname", "firstname"])
                    .required(true)
                    .help("This argument takes the persons first name")
                    //.conflicts_with("lastname")
            )
            .arg(
                Arg::new("lastname")
                    //.group("person-register")
                    .short('l')
                    .long("last-name")
                    .aliases(["lname", "lastname"])
                    .required(true)
                    .help("This argument takes the persons last name")
            )
    )
    .subcommand(
        Command::new("register-pet")
            .arg(
                Arg::new("pet-name")
                    .long("pet-name")
                    .short('p')
                    .required(true)
            )   
    )
    .about("This app register people names") 
    .arg(
        Arg::new("fluffy")
        .long("fluffy")
        .help("Is the person wearing a fluffy coat or not")
    )
    .get_matches();


    //println!("pet name: {}", match_result.get_one::<String>("pet-name").unwrap_or(&"no pet name".to_string()))
    let pet_args = match_result.subcommand_matches("register-pet");
    println!("Does pet name exist? {}", pet_args.unwrap().contains_id("pet-name"));

}