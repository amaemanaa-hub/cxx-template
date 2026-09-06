#[cxx::bridge]
mod ffi {
    extern "Rust" {
        fn get_int() -> i32; 
    }
}

const X: i32 = 50;  

pub fn get_int() -> i32 { 
    X  
}