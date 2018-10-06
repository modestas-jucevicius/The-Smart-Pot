#![feature(plugin)]
#![plugin(rocket_codegen)]
#![feature(decl_macro)]

extern crate rocket;
extern crate serde;
extern crate serde_json;
#[macro_use]
extern crate  serde_derive;


mod arduino_interf;
mod server;
use self::arduino_interf::*;

fn main() {
    let obj = Objects::SENSOR;
    let dev = Arduino::new("COM1".to_string());
    println!("Object::SENSOR = {}", obj.as_str());
}