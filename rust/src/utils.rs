use std::os::raw::c_char;
use std::cmp;

use libc;

#[inline]
pub fn strncpy(dst: *mut c_char, source: &str, n: libc::size_t) {
    let n = cmp::min(source.len(), n - 1);
    unsafe {
        libc::strncpy(dst, source.as_ptr() as *const c_char, n);
        *dst.offset(n as isize) = '\0' as c_char;
    }
}