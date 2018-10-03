use serde_json::Error;
use super::super::arduino_interf::*;
use rocket::State;
use std::sync::RwLock;



#[derive(Serialize, Deserialize)]
struct ForJson {
    device : String,
    param  : String,
    data   : u32
}

fn status_to_json(device: Devices, arduino: &mut Arduino) -> Option<String> {
    let com_str = CommandString{command: Commands::GET, object: Objects::SENSOR,
                                device: device.clone(), setwhat: SetWhat::NULL, value: 0};
    match device {
        Devices::H2OSENSOR => arduino.get_data(&com_str),
        Devices::PPMSENSOR => arduino.get_data(&com_str),
        _                  => None,
    }

}

#[get("/H2O_SENSOR")]
fn getSensor(arduino: State<Arduino>) -> String {
    unsafe {
        match status_to_json(Devices::H2OSENSOR, &mut arduino) {
            Some(resp) => resp,
            None => r#"{'error' : "failed to get data"}"#,
        }
    }
}


