    //mod arduino_interf_impl;
    extern crate serialport;

    #[cfg(unix)]
    use std::str;
    use std::io::Read;
    use std::env;
    use serialport::posix::TTYPort;
    use std::time::Duration;


    #[cfg(unix)]
    const DEFAULT_TTY: &str = "/dev/ttyUSB0";
    #[cfg(windows)]
    const DEFAULT_TTY: &str = "COM1";






    fn cast_u32_to_bool(to_cast: &u32) -> bool {
        if *to_cast > 0 {
            true
        } else {
            false
        }
    }

    pub struct OptionStruct {
        pub h2o_sensor_pin: u8,
        pub water_pump_pin: u8,
        pub h2o_sensor_threshold: u8,

        pub water_pump_using_time: bool,
        pub water_pump_time_on: u32,
        pub water_pump_time_off: u32,

        pub ppm_sensor_pin: u8,
        pub food_pump_pin: u8,
        pub ppm_sensor_threshold: u8,

        pub food_pump_using_time: bool,
        pub food_pump_time_on: u32,
        pub food_pump_time_off: u32,
        pub setup_name: &'static str
    }



    pub enum Commands {
        SET,
        GET
    }

    impl Commands {
        pub fn as_str(&self) -> &'static str {
            match *self {
                Commands::GET => "GET ",
                Commands::SET => "SET "
            }
        }
    }

    pub enum Objects {
        DEVICE,
        SENSOR,
        //ALL
    }

    impl Objects {
        pub fn as_str(&self) -> &'static str {
            match *self {
                Objects::DEVICE => "DEVICE ",
                Objects::SENSOR => "SENSOR ",
                //Objects::ALL    => "ALL",
                _               => "",
            }
        }
    }

    #[derive(Clone)]
    pub enum Devices {
        WATERPUMP,
        FOODPUMP,
        H2OSENSOR,
        PPMSENSOR
    }

    impl Devices {
        fn as_str(&self) -> &'static str {
            match *self {
                Devices::WATERPUMP => "H2OP",
                Devices::FOODPUMP  => "FOODP",
                Devices::H2OSENSOR => "H2OS",
                Devices::PPMSENSOR => "PPMS",
                //_                  => "",
            }

        }
    }

    pub enum SetWhat {
        THRESHOLD,
        TIMEON,
        TIMEOFF,
        USINGTIME,
        NULL

    }
    impl SetWhat {
        fn as_str(&self) -> &'static str {
            match *self {
                SetWhat::THRESHOLD => "THRESHOLD",
                SetWhat::TIMEON    => "ONTIME",
                SetWhat::TIMEOFF   => "OFFTIME",
                SetWhat::USINGTIME => "USETIME",
                _                  => "",
            }
        }
    }
    const settings: serialport::SerialPortSettings =  serialport::SerialPortSettings {
        baud_rate: 9600,
        Data_bits: serialport::DataBits::Eight,
        flow_control: serialport::FlowControl::None,
        parity: serialport::Parity::None,
        stop_bits: serialport::StopBits::One,
        timeout: Duration::from_millis(1),
    };


    pub struct CommandString {
        pub command     : Commands,
        pub object      : Objects ,
        pub device      : Devices ,
        pub setwhat     : SetWhat,
        pub value       : u32
    }

    pub enum Errors {
        ErrNoError,
        ErrNoCommand,
        ErrUnknownObj,
        ErrNotSetup,
        ErrSetup,
        ErrNoSens,
        ErrNoDev,
        ErrUseDev,
        ErrNoAttr,
        ErrParam
    }

    pub struct Arduino
    {
        port: serialport::SerialPort,
        options: OptionStruct,
        errors: Errors,
        is_setup: bool,
        connected: bool
    }

    impl Arduino {
        pub fn validate_opts(opts_to_check: &OptionStruct) -> bool
        {
            if opts_to_check.h2o_sensor_threshold > 100 {
                return false;
            } else if opts_to_check.ppm_sensor_threshold > 100 {
                return false;
            } else {
                true
            }
        }

        pub fn validate_command(command_to_check: &CommandString) -> bool
        {
            match command_to_check.command {

               Commands::GET  =>   match command_to_check.object {
                    Objects::DEVICE => return false,
                    _ => match command_to_check.device {
                        Devices::FOODPUMP => return false,
                        Devices::WATERPUMP => return false,
                        _ => true,
                    },
                }


             Commands::SET =>   match command_to_check.object {
                    Objects::SENSOR => return false,
                    Objects::DEVICE => match command_to_check.device {
                        Devices::PPMSENSOR => return false,
                        Devices::H2OSENSOR => return false,
                        _ => match command_to_check.setwhat {
                            SetWhat::THRESHOLD => if command_to_check.value > 100 { false} else { true },
                            SetWhat::USINGTIME => if command_to_check.value > 1   { false} else { true },
                            _                  => return true,

                        },
                    },
                }
            }

        }
        pub fn init_opts(&mut self, options: OptionStruct) -> bool
        {
            if Arduino::validate_opts(&options) == true {
                self.options = options;
                self.is_setup = true;
                return true;
            } else {
                false
            }
        }

        pub fn new(path_or_name: String) -> Result<Arduino, &'static str> {
            #[cfg(windows)]
            let try_port = serialport::open_with_settings(&path_or_name);
            #[cfg(unix)]

            let try_port = serialport::open_with_settings(Path::new(path_or_name));
            let mut port = match try_port {
                Ok(good) => good,
                Err(err) => return Err("Failed to open port"),
            };



            let options = OptionStruct {
                h2o_sensor_pin: 10,
                water_pump_pin: 12,
                h2o_sensor_threshold: 0,

                water_pump_using_time: false,
                water_pump_time_on: 0,
                water_pump_time_off: 0,

                ppm_sensor_pin: 11,
                food_pump_pin: 13,
                ppm_sensor_threshold: 0,

                food_pump_using_time: false,
                food_pump_time_on: 0,
                food_pump_time_off: 0,
                setup_name: "None"
            };
            let errors = Errors::ErrNoError;

            Ok(Arduino { port, options, errors, is_setup: false, connected: false })
        }

        fn connect_to_serial(&mut self) -> Option<bool> {
            let mut buf: [u8; 2] = [0, 0];
            loop {
                match self.port.read_exact(&mut buf) {
                    Ok(ch) => {
                        if buf[0] as char == 'A' {
                            #[allow(unused_must_use)]
                            match self.port.write(String::from("A").as_bytes()) {
                                Ok(a)     => (),
                                Err(_err) => return None,
                            }
                            match self.port.read_exact(&mut buf) {
                                Ok(n) => {
                                    if str::from_utf8(&buf).unwrap() == "B\r" {
                                        self.connected = true;
                                        return Some(true);
                                    };
                                }
                                Err(err) => return None,
                            }
                        }
                    }
                    Err(err) => return None,
                }
            }
        }

        fn send_all_options(&mut self) -> Option<bool> {
            let mut buf: [u8; 128] = [0; 128];
            if self.connected == false || self.is_setup == false {
                return None
            }
            match self.port.read(&mut buf) {
                Ok(a)     => (),
                Err(_err) => return None,
            }
            if str::from_utf8(&buf).unwrap() == "SENDOPTS\r" {
                let str_to_print = format!("{} {} {} {} {} {} {} {} {} {} {} {} {}\r",
                                         self.options.h2o_sensor_pin, self.options.water_pump_pin,
                                         self.options.h2o_sensor_threshold, self.options.water_pump_using_time as u8,
                                         self.options.water_pump_time_on, self.options.water_pump_time_off,
                                         self.options.ppm_sensor_pin, self.options.food_pump_pin,
                                         self.options.ppm_sensor_threshold, self.options.food_pump_using_time,
                                         self.options.food_pump_time_on, self.options.food_pump_time_off,
                                         self.options.setup_name);
                match self.port.write_all(str_to_print.as_bytes()) {
                    Ok(a) => Some(true),
                    Err(why) => None,
                };
            }

            Some(false)
        }

        fn send_setting(&mut self, command: &CommandString) -> Option<bool>
        {
            if Arduino::validate_command(&command) == false {
                return None;
            }


            match command.device {
                Devices::WATERPUMP => {
                    match command.setwhat {
                        SetWhat::USINGTIME => {
                            self.options.water_pump_using_time = cast_u32_to_bool(&command.value);
                            ()
                        },
                        SetWhat::THRESHOLD => {
                            self.options.h2o_sensor_threshold = command.value as u8;
                            ()
                        },
                        SetWhat::TIMEON => {
                            self.options.water_pump_time_on = command.value;
                            ()
                        },
                        SetWhat::TIMEOFF => {
                            self.options.water_pump_time_off = command.value;
                            ()
                        },
                        SetWhat::NULL => return None,
                    };
                },
                Devices::FOODPUMP => {
                    match command.setwhat {
                        SetWhat::USINGTIME => {
                            self.options.food_pump_using_time =  cast_u32_to_bool(&command.value);
                            ()
                        },
                        SetWhat::THRESHOLD => {
                            self.options.ppm_sensor_threshold = command.value as u8;
                            ()
                        },
                        SetWhat::TIMEON => {
                            self.options.food_pump_time_on = command.value;
                            ()
                        },
                        SetWhat::TIMEOFF => {
                            self.options.food_pump_time_off = command.value;
                            ()
                        },
                        SetWhat::NULL => return None,
                    };
                },
                _ => return None,
            };

            let com_str = format!("SET {} {} {} {}\r", command.object.as_str(), command.device.as_str(),
                                                              command.setwhat.as_str() ,command.value );
            match self.port.write_all(com_str.as_bytes()) {
                Ok(a)    => Some(true),
                Err(_err) => None,
            }
        }

        pub fn get_data(&mut self, command: &CommandString) -> Option<String>
        {

            if Arduino::validate_command(&command) == false {
                return None;
            };
            if self.is_setup == false {
                return None
            };

            let mut buf : [u8; 128] = [0;128];
            if self.connected == false {
                return None
            }
            let com_str = format!("GET {} {}\r", command.object.as_str(), command.device.as_str());
            match self.port.write_all(com_str.as_bytes())
                {
                    Ok(a)     => (),
                    Err(_err) => return None,
                };
            match self.port.read(&mut buf) {
                Ok(a)     => (),
                Err(_err) => return None,
            };
            let mut response = match String::from_utf8(buf.to_vec()) {
                Ok(a)  => Some(a),
                Err(_err) => None,
            };
            response
        }

    }

