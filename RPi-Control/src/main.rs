mod arduino_interf;
use self::arduino_interf::*;

fn main() {
    let obj = Objects::SENSOR;
    let dev = Arduino::new("COM1".to_string());
    println!("Object::SENSOR = {}", obj.as_str());
}