extern "C" {
    fn i_i(value: i32) -> i32;
    fn i_()->i32;
    fn v_i(p1: i32);
    fn I_I(value: i64) -> i64;
    fn I_()->i64;
    fn v_I(p1: i64);
    fn v_f(value: f32);
    fn v_F(value: f64);
    fn v_ii(p1: i32, p2: i32);
    fn i_iii(p1: i32, p2: i32, p3: i32);
}

#[no_mangle]
unsafe fn call() {
    let res = i_i(42);
    v_i(res);
    I_I(42);
    v_f(42.0);
    v_F(42.0);
    v_ii(0, 1);
    i_iii(3, 2, 1);
}

#[no_mangle]
unsafe fn test(param: i32) -> i32 {
    param
}

