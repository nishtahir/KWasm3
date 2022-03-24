extern "C" {
    fn i32callback(value: i32);
    fn i64callback(value: i64);
    fn f32callback(value: f32);
    fn f64callback(value: f64);
    fn function2(p1: i32, p2: i32);
    fn function3(p1: i32, p2: i32, p3: i32);
    fn i_iii(p1: i32, p2: i32, p3: i32) -> i32;
}

#[no_mangle]
unsafe fn call() {
    i32callback(42);
    i64callback(42);
    f32callback(42.0);
    f64callback(42.0);
    function2(0, 1);
    function3(3, 2, 1);
    i_iii(4, 5, 6);
}

// extern "C" {
//     #[link_name = "pinMode"]
//     fn pin_mode(pin: i32, mode: i32);

//     #[link_name = "digitalWrite"]
//     fn digital_write(pin: i32, state: i32);

//     fn delay(millis: i32);
// }

// #[no_mangle]
// unsafe fn _start() {
//     pin_mode(LED, OUTPUT);
//     digital_write(LED, HIGH);
//     delay(1000);
//     digital_write(LED, LOW);
// }

// const LED: i32 = 0x02;
// const OUTPUT: i32 = 0x01;
// const LOW: i32 = 0x00;
// const HIGH: i32 = 0x01;

// unsafe fn _setup() {
//     pin_mode(LED, OUTPUT);
// }

// unsafe fn _loop() {
//     digital_write(LED, LOW);
//     delay(1000);
//     digital_write(LED, HIGH);
//     delay(1000);
// }
