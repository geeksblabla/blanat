// Compile with: rustc ./solution.rs -O -C debuginfo=0

use std::fs::File;
use std::io::{self, Result, Write};
use std::sync::{Arc,RwLock};
use std::thread;
use std::str;

#[cfg(unix)]
use std::os::unix::io::AsRawFd;


/* ------------------BEGIN OF UTILITIES-------------------- */

/* -------------------------------------------- */
/*         BEGIN OF FXHASHMAP SOURCE            */
/* from https://github.com/BurntSushi/byteorder */
/*    from https://github.com/cbreeden/fxhash   */
/* -------------------------------------------- */
#[allow(warnings)]
mod fxhash {
    use std::collections::HashMap;
    use std::hash::{BuildHasherDefault, Hasher};
    use std::ops::BitXor;
    use std::convert::TryInto;

    #[cfg(target_endian = "little")]
    pub type NativeEndian = LittleEndian;

    #[cfg(target_endian = "big")]
    pub type NativeEndian = BigEndian;

    #[derive(Clone, Copy, Debug, Eq, Hash, Ord, PartialEq, PartialOrd)]
    pub enum LittleEndian {}

    impl LittleEndian {
        #[inline]
        fn read_u16(buf: &[u8]) -> u16 {
            u16::from_le_bytes(buf[..2].try_into().unwrap())
        }
    
        #[inline]
        fn read_u32(buf: &[u8]) -> u32 {
            u32::from_le_bytes(buf[..4].try_into().unwrap())
        }
    
        #[inline]
        fn read_u64(buf: &[u8]) -> u64 {
            u64::from_le_bytes(buf[..8].try_into().unwrap())
        }
    }    

    #[derive(Clone, Copy, Debug, Eq, Hash, Ord, PartialEq, PartialOrd)]
    pub enum BigEndian {}

    impl BigEndian {
        #[inline]
        fn read_u16(buf: &[u8]) -> u16 {
            u16::from_be_bytes(buf[..2].try_into().unwrap())
        }
    
        #[inline]
        fn read_u32(buf: &[u8]) -> u32 {
            u32::from_be_bytes(buf[..4].try_into().unwrap())
        }
    
        #[inline]
        fn read_u64(buf: &[u8]) -> u64 {
            u64::from_be_bytes(buf[..8].try_into().unwrap())
        }
    }    


    pub type FxBuildHasher = BuildHasherDefault<FxHasher>;
    pub type FxHashMap<K, V> = HashMap<K, V, FxBuildHasher>;

    const ROTATE: u32 = 5;
    const SEED64: u64 = 0x51_7c_c1_b7_27_22_0a_95;
    const SEED32: u32 = 0x9e_37_79_b9;

    #[cfg(target_pointer_width = "32")]
    const SEED: usize = SEED32 as usize;
    #[cfg(target_pointer_width = "64")]
    const SEED: usize = SEED64 as usize;
    
    trait HashWord {
        fn hash_word(&mut self, word: Self);
    }
    
    macro_rules! impl_hash_word {
        ($($ty:ty = $key:ident),* $(,)*) => (
            $(
                impl HashWord for $ty {
                    #[inline]
                    fn hash_word(&mut self, word: Self) {
                        *self = self.rotate_left(ROTATE).bitxor(word).wrapping_mul($key);
                    }
                }
            )*
        )
    }
    
    impl_hash_word!(usize = SEED, u32 = SEED32, u64 = SEED64);
    
    #[inline]
    #[cfg(target_pointer_width = "32")]
    fn write(hash: usize, bytes: &[u8]) -> usize {
        write32(hash as u32, bytes) as usize
    }
    #[inline]
    #[cfg(target_pointer_width = "64")]
    fn write(hash: usize, bytes: &[u8]) -> usize {
        write64(hash as u64, bytes) as usize
    }
    
    #[inline]
    fn write32(mut hash: u32, mut bytes: &[u8]) -> u32 {
        while bytes.len() >= 4 {
            hash.hash_word(NativeEndian::read_u32(bytes));
            bytes = &bytes[4..];
        }
    
        if bytes.len() >= 2 {
            hash.hash_word(u32::from(NativeEndian::read_u16(bytes)));
            bytes = &bytes[2..];
        }
    
        if let Some(&byte) = bytes.first() {
            hash.hash_word(u32::from(byte));
        }
    
        hash
    }
    
    #[inline]
    fn write64(mut hash: u64, mut bytes: &[u8]) -> u64 {
        while bytes.len() >= 8 {
            hash.hash_word(NativeEndian::read_u64(bytes));
            bytes = &bytes[8..];
        }
    
        if bytes.len() >= 4 {
            hash.hash_word(u64::from(NativeEndian::read_u32(bytes)));
            bytes = &bytes[4..];
        }
    
        if bytes.len() >= 2 {
            hash.hash_word(u64::from(NativeEndian::read_u16(bytes)));
            bytes = &bytes[2..];
        }
    
        if let Some(&byte) = bytes.first() {
            hash.hash_word(u64::from(byte));
        }
    
        hash
    }
    
    #[derive(Debug, Clone)]
    pub struct FxHasher {
        hash: usize,
    }
    
    impl Default for FxHasher {
        #[inline]
        fn default() -> FxHasher {
            FxHasher { hash: 0 }
        }
    }
    
    impl Hasher for FxHasher {
        #[inline]
        fn write(&mut self, bytes: &[u8]) {
            self.hash = write(self.hash, bytes);
        }
    
        #[inline]
        fn write_u8(&mut self, i: u8) {
            self.hash.hash_word(i as usize);
        }
    
        #[inline]
        fn write_u16(&mut self, i: u16) {
            self.hash.hash_word(i as usize);
        }
    
        #[inline]
        fn write_u32(&mut self, i: u32) {
            self.hash.hash_word(i as usize);
        }
    
        #[inline]
        #[cfg(target_pointer_width = "32")]
        fn write_u64(&mut self, i: u64) {
            self.hash.hash_word(i as usize);
            self.hash.hash_word((i >> 32) as usize);
        }
    
        #[inline]
        #[cfg(target_pointer_width = "64")]
        fn write_u64(&mut self, i: u64) {
            self.hash.hash_word(i as usize);
        }
    
        #[inline]
        fn write_usize(&mut self, i: usize) {
            self.hash.hash_word(i);
        }
    
        #[inline]
        fn finish(&self) -> u64 {
            self.hash as u64
        }
    }    
}
type FxHashMap<K, V> = crate::fxhash::FxHashMap<K, V>;
/* -------------------------------------- */
/*      END OF FXHASHMAP SOURCE           */
/* -------------------------------------- */


/* -------------------------------------------- */
/*     BEGIN OF MMAP CODE SOURCE FOR WINDOWS    */
/* from https://github.com/retep998/winapi-rs   */
/* from https://github.com/danburkert/memmap-rs */
/* -------------------------------------------- */
#[allow(warnings)]
const PAGE_READONLY: u32 = 0x02;
#[allow(warnings)]
const FILE_MAP_READ: u32 = 0x0004;
//const SECTION_MAP_READ: u32 = 0x0004;

#[cfg(windows)]#[allow(warnings)]
mod windows_mmap {

    use super::*;
    use std::os::windows::io::{AsRawHandle,RawHandle};
    use std::{mem, ptr};
    use std::os::raw::c_void;

    pub type DWORD = u32;

    pub type SIZE_T = usize;

    pub type HANDLE = *mut c_void;

    pub type BOOL = i32;

    pub type LPVOID = *mut c_void;

    pub type LPCWSTR = *const u16;

    pub type LPCVOID = *const c_void;

    pub type WORD = u16;

    pub type DWORD_PTR = usize;

    pub type PDWORD = *mut DWORD;



    #[repr(C)]
    struct SYSTEM_INFO {
        wProcessorArchitecture: u16,
        dwPageSize: u32,
        lpMinimumApplicationAddress: *mut c_void,
        lpMaximumApplicationAddress: *mut c_void,
        dwActiveProcessorMask: usize,
        dwNumberOfProcessors: u32,
        dwProcessorType: u32,
        dwAllocationGranularity: u32,
        wProcessorLevel: u16,
        wProcessorRevision: u16,
    }

    struct SECURITY_ATTRIBUTES {
        nLength: u32,
        lpSecurityDescriptor: LPVOID,
        bInheritHandle: BOOL,
    }
    pub type LPSECURITY_ATTRIBUTES = *mut SECURITY_ATTRIBUTES;

    pub type LPSYSTEM_INFO = *mut SYSTEM_INFO;

    extern "system" {
        pub fn GetSystemInfo(lpSystemInfo: *mut SYSTEM_INFO);

        pub fn CloseHandle(
            hObject: HANDLE,
        ) -> BOOL;

        pub fn CreateFileMappingW(
            hFile: HANDLE,
            lpFileMappingAttributes: LPSECURITY_ATTRIBUTES,
            flProtect: DWORD,
            dwMaximumSizeHigh: DWORD,
            dwMaximumSizeLow: DWORD,
            lpName: LPCWSTR,
        ) -> HANDLE;

        pub fn MapViewOfFile(
            hFileMappingObject: HANDLE,
            dwDesiredAccess: DWORD,
            dwFileOffsetHigh: DWORD,
            dwFileOffsetLow: DWORD,
            dwNumberOfBytesToMap: SIZE_T,
        ) -> LPVOID;

        pub fn UnmapViewOfFile(
            lpBaseAddress: LPCVOID,
        ) -> BOOL;
        pub fn VirtualProtect(
            lpAddress: LPVOID,
            dwSize: SIZE_T,
            flNewProtect: DWORD,
            lpflOldProtect: PDWORD,
        ) -> BOOL;

    }

    #[allow(dead_code)]
    pub struct MmapInner {
    file: Option<File>,
    pub ptr: *mut c_void,
    pub len: usize,
    copy: bool,
    }
    impl MmapInner {
    pub fn new(
        file: &File,
        protect: DWORD,
        access: DWORD,
        offset: u64,
        len: usize,
        copy: bool,
    ) -> io::Result<MmapInner> {
        let alignment = offset % allocation_granularity() as u64;
        let aligned_offset = offset - alignment as u64;
        let aligned_len = len + alignment as usize;

        unsafe {
            let handle = CreateFileMappingW(
                file.as_raw_handle(),
                ptr::null_mut(),
                protect,
                0,
                0,
                ptr::null(),
            );
            if handle == ptr::null_mut() {
                return Err(io::Error::last_os_error());
            }

            let ptr = MapViewOfFile(
                handle,
                access,
                (aligned_offset >> 16 >> 16) as DWORD,
                (aligned_offset & 0xffffffff) as DWORD,
                aligned_len as SIZE_T,
            );
            CloseHandle(handle);

            if ptr == ptr::null_mut() {
                Err(io::Error::last_os_error())
            } else {
                Ok(MmapInner {
                    file: Some(file.try_clone()?),
                    ptr: ptr.offset(alignment as isize),
                    len: len as usize,
                    copy: copy,
                })
            }
        }
    }
    pub fn map(len: usize, file: &File, offset: u64) -> io::Result<MmapInner> {
        let write = protection_supported(file.as_raw_handle(), 0x04);
        let exec = protection_supported(file.as_raw_handle(), 0x20);
        let mut access = FILE_MAP_READ;
        let protection = match (write, exec) {
            (true, true) => {
                access |= 0x0002 | 0x0020;
                0x40
            }
            (true, false) => {
                access |= 0x0002;
                0x04
            }
            (false, true) => {
                access |= 0x0020;
                0x20
            }
            (false, false) => PAGE_READONLY,
        };

        let mut inner = MmapInner::new(file, protection, access, offset, len, false)?;
        if write || exec {
            inner.make_read_only()?;
        }
        Ok(inner)
    }

    #[inline]
    pub fn len(&self) -> usize {
        self.len
    }
    #[inline]
    pub fn ptr(&self) -> *const u8 {
        self.ptr as *const u8
    }

    #[inline]
    pub fn mut_ptr(&mut self) -> *mut u8 {
        self.ptr as *mut u8
    }

    pub fn make_read_only(&mut self) -> io::Result<()> {
        self.virtual_protect(PAGE_READONLY)
    }

    pub fn virtual_protect(&mut self, protect: DWORD) -> io::Result<()> {
        unsafe {
            let alignment = self.ptr as usize % allocation_granularity();
            let ptr = self.ptr.offset(-(alignment as isize));
            let aligned_len = self.len as SIZE_T + alignment as SIZE_T;

            let mut old = 0;
            let result = VirtualProtect(ptr, aligned_len, protect, &mut old);

            if result != 0 {
                Ok(())
            } else {
                Err(io::Error::last_os_error())
            }
        }
    }

}
    pub fn protection_supported(handle: RawHandle, protection: DWORD) -> bool {
        unsafe {
            let handle = CreateFileMappingW(handle, ptr::null_mut(), protection, 0, 0, ptr::null());
            if handle == ptr::null_mut() {
                return false;
            }
            CloseHandle(handle);
            true
        }
    }
        
    pub fn allocation_granularity() -> usize {
    unsafe {
        let mut info = mem::zeroed();
        GetSystemInfo(&mut info);
        return info.dwAllocationGranularity as usize;
    }
    }

    impl Drop for MmapInner {
        fn drop(&mut self) {
        let alignment = self.ptr as usize % allocation_granularity();
        unsafe {
            let ptr = self.ptr.offset(-(alignment as isize));
            assert!(
                UnmapViewOfFile(ptr) != 0,
                "unable to unmap mmap: {}",
                io::Error::last_os_error()
            );
        }
    }
    }
}
/* -------------------------------------- */
/* END OF MMAP CODE SOURCE FOR WINDOWS  */
/* -------------------------------------- */

/* -------------------------------------------- */
/*      BEGIN OF MMAP CODE SOURCE FOR UNIX      */
/*   from https://github.com/rust-lang/libc     */
/* from https://github.com/danburkert/memmap-rs */
/* -------------------------------------------- */
#[cfg(unix)]
mod unix_mmap {
    use super::*;

    use std::os::unix::io::{RawFd};
    use std::{ptr};


    pub const MAP_FAILED: *mut ::std::ffi::c_void = !0 as *mut ::std::ffi::c_void;

        
    extern "C" {
        pub fn mmap(
            addr: *mut ::std::ffi::c_void,
            len: usize,
            prot: i32,
            flags: i32,
            fd: i32,
            offset: i64,
        ) -> *mut ::std::ffi::c_void;

        pub fn munmap(addr: *mut ::std::ffi::c_void, len: usize) -> i32;
        pub fn sysconf(name: i32) -> i64;
    }

    pub struct MmapInner {
        pub ptr: *mut std::ffi::c_void,
        pub len: usize,
    }
    
    impl MmapInner {
        /// Creates a new `MmapInner`.
        ///
        /// This is a thin wrapper around the `mmap` sytem call.
        pub fn new(
            len: usize,
            prot: i32,
            flags: i32,
            file: RawFd,
            offset: u64,
        ) -> io::Result<MmapInner> {
            let alignment = offset % page_size() as u64;
            let aligned_offset = offset - alignment;
            let aligned_len = len + alignment as usize;
            if aligned_len == 0 {
                // Normally the OS would catch this, but it segfaults under QEMU.
                return Err(io::Error::new(
                    io::ErrorKind::InvalidInput,
                    "memory map must have a non-zero length",
                ));
            }
    
            unsafe {
                let ptr = mmap(
                    ptr::null_mut(),
                    aligned_len as usize,
                    prot,
                    flags,
                    file,
                    aligned_offset as i64,
                );
    
                if ptr == MAP_FAILED {
                    Err(io::Error::last_os_error())
                } else {
                    Ok(MmapInner {
                        ptr: ptr.offset(alignment as isize),
                        len: len,
                    })
                }
            }
            
        }
        pub fn map(len: usize, file: &File, offset: u64) -> io::Result<MmapInner> {
            MmapInner::new(
                len,
                1,
                1,
                file.as_raw_fd(),
                offset,
            )
        }
        #[inline]
        pub fn len(&self) -> usize {
            self.len
        }
        #[inline]
        pub fn ptr(&self) -> *const u8 {
            self.ptr as *const u8
        }
    
        #[inline]
        pub fn mut_ptr(&mut self) -> *mut u8 {
            self.ptr as *mut u8
        }
    }

    impl Drop for MmapInner {
        fn drop(&mut self) {
            let alignment = self.ptr as usize % page_size();
            unsafe {
                assert!(
                    munmap(
                        self.ptr.offset(-(alignment as isize)),
                        (self.len + alignment) as usize
                    ) == 0,
                    "unable to unmap mmap: {}",
                    io::Error::last_os_error()
                );
            }
            
        }
    }
    
    unsafe impl Sync for MmapInner {}
    unsafe impl Send for MmapInner {}

    fn page_size() -> usize {
        #[cfg(target_os = "linux")]
        const _SC_PAGESIZE: i32 = 30;
        #[cfg(target_os = "macos")]
        const _SC_PAGESIZE: i32 = 29;
    
        unsafe { sysconf(_SC_PAGESIZE) as usize }
    }
}
/* -------------------------------------- */
/*   END OF MMAP CODE SOURCE FOR LINUX    */
/* -------------------------------------- */

/* -------------------------------------- */
/*         BEGIN OF MMAP WRAPPER          */
/* -------------------------------------- */

use std::io::{Error, ErrorKind};
#[derive(Clone, Debug, Default)]
pub struct MmapOptions {
    offset: u64,
    len: Option<usize>,
    stack: bool,
}

impl MmapOptions {
    pub fn new() -> MmapOptions {
        MmapOptions::default()
    }

    pub fn offset(&mut self, offset: u64) -> &mut Self {
        self.offset = offset;
        self
    }

    pub fn len(&mut self, len: usize) -> &mut Self {
        self.len = Some(len);
        self
    }

    /// Returns the configured length, or the length of the provided file.
    fn get_len(&self, file: &File) -> Result<usize> {
        self.len.map(Ok).unwrap_or_else(|| {
            let len = file.metadata()?.len() - self.offset;
            if len > (usize::MAX as u64) {
                return Err(Error::new(
                    ErrorKind::InvalidData,
                    "memory map length overflows usize",
                ));
            }
            Ok(len as usize)
        })
    }

    pub fn stack(&mut self) -> &mut Self {
        self.stack = true;
        self
    }
    #[cfg(windows)]
    pub unsafe fn map(&self, file: &File) -> Result<Mmap> {
        windows_mmap::MmapInner::map(self.get_len(file)?, file, self.offset).map(|inner| Mmap { inner: inner })

    }
    #[cfg(unix)]
    pub unsafe fn map(&self, file: &File) -> Result<Mmap> {
        unix_mmap::MmapInner::map(self.get_len(file)?, file, self.offset).map(|inner| Mmap { inner: inner })

    }

}
pub struct Mmap {
    #[cfg(windows)]
    inner: windows_mmap::MmapInner,
    #[cfg(unix)]
    inner: unix_mmap::MmapInner,
}

impl Mmap {
    pub unsafe fn map(file: &File) -> Result<Mmap> {
        MmapOptions::new().map(file)
    }
    pub fn len(&self) -> usize {
        self.inner.len()
    }
    pub fn ptr(&self) -> *const u8 {
        self.inner.ptr()
    }
        // Add a method to get the length of the memory-mapped file
    pub fn mut_ptr(&mut self) -> *mut u8 {
        self.inner.mut_ptr()
    }
    // Safe method to access a slice of the mapped memory region.
    pub fn as_slice(&self) -> &[u8] {
        unsafe {
            // SAFETY: This is safe under the assumption that the memory region
            // remains valid for the lifetime of the Mmap object, and the Mmap object
            // is not concurrently mutated (which it shouldn't be, as per its design).
            std::slice::from_raw_parts(self.inner.ptr(), self.inner.len())
        }
    }
}
mod mmap_utils {
    use super::*;
    use std::slice;

    pub struct SafeMmap {
        ptr: *mut u8,
        len: usize,
    }

    unsafe impl Send for SafeMmap {}
    unsafe impl Sync for SafeMmap {}

    impl SafeMmap {
        pub fn new(mmap: &Mmap) -> Self {
            Self {
                ptr: mmap.ptr() as *mut u8, // Assuming `ptr` returns a mutable raw pointer
                len: mmap.len(),
            }
        }

        // Provides safe, concurrent access to a slice of the memory-mapped area
        pub fn as_slice(&self, start: usize, end: usize) -> Result<&[u8]> {
            if start > end || end > self.len {
                return Err(io::Error::new(
                    io::ErrorKind::InvalidInput,
                    "Invalid slice range",
                ));
            }
            // Safety: This is safe because we ensure the slice is within the bounds of the mapped memory
            // and the SafeMmap struct guarantees that the memory will not be unmapped while in use.
            unsafe {
                Ok(slice::from_raw_parts(self.ptr.add(start), end - start))
            }
        }
    }
}
/* -------------------------------------- */
/*          END OF MMAP WRAPPER           */
/* -------------------------------------- */

/* ------------------END OF UTILITIES-------------------- */




const CITIES_BYTES: [&[u8]; 101] = [
    b"\x43\x61\x73\x61\x62\x6c\x61\x6e\x63\x61",
    b"\x52\x61\x62\x61\x74",
    b"\x4d\x61\x72\x72\x61\x6b\x65\x63\x68",
    b"\x46\x65\x73",
    b"\x54\x61\x6e\x67\x69\x65\x72",
    b"\x41\x67\x61\x64\x69\x72",
    b"\x4d\x65\x6b\x6e\x65\x73",
    b"\x4f\x75\x6a\x64\x61",
    b"\x4b\x65\x6e\x69\x74\x72\x61",
    b"\x54\x65\x74\x6f\x75\x61\x6e",
    b"\x53\x61\x66\x69",
    b"\x45\x6c\x5f\x4a\x61\x64\x69\x64\x61",
    b"\x42\x65\x6e\x69\x5f\x4d\x65\x6c\x6c\x61\x6c",
    b"\x45\x72\x72\x61\x63\x68\x69\x64\x69\x61",
    b"\x54\x61\x7a\x61",
    b"\x45\x73\x73\x61\x6f\x75\x69\x72\x61",
    b"\x4b\x68\x6f\x75\x72\x69\x62\x67\x61",
    b"\x47\x75\x65\x6c\x6d\x69\x6d",
    b"\x4a\x6f\x72\x66\x5f\x45\x6c\x5f\x4d\x65\x6c\x68\x61",
    b"\x4c\x61\x61\x79\x6f\x75\x6e\x65",
    b"\x4b\x73\x61\x72\x5f\x45\x6c\x5f\x4b\x65\x62\x69\x72",
    b"\x53\x61\x6c\x65",
    b"\x42\x69\x72\x5f\x4c\x65\x68\x6c\x6f\x75",
    b"\x41\x72\x66\x6f\x75\x64",
    b"\x54\x65\x6d\x61\x72\x61",
    b"\x4d\x6f\x68\x61\x6d\x6d\x65\x64\x69\x61",
    b"\x53\x65\x74\x74\x61\x74",
    b"\x42\xc3\xa9\x6e\x69\x5f\x4d\x65\x6c\x6c\x61\x6c",
    b"\x4e\x61\x64\x6f\x72",
    b"\x4b\x61\x6c\x61\x61\x74\x5f\x4d\x47\x6f\x75\x6e\x61",
    b"\x43\x68\x69\x63\x68\x61\x6f\x75\x61",
    b"\x43\x68\x65\x66\x63\x68\x61\x6f\x75\x65\x6e",
    b"\x41\x6c\x5f\x48\x6f\x63\x65\x69\x6d\x61",
    b"\x54\x61\x6f\x75\x72\x69\x72\x74",
    b"\x54\x61\x72\x6f\x75\x64\x61\x6e\x74",
    b"\x47\x75\x65\x6c\x74\x61\x5f\x5a\x65\x6d\x6d\x75\x72",
    b"\x44\x61\x6b\x68\x6c\x61",
    b"\x4c\x61\xc3\xa2\x79\x6f\x75\x6e\x65",
    b"\x54\x69\x7a\x6e\x69\x74",
    b"\x54\x69\x6e\x67\x68\x69\x72",
    b"\x49\x66\x72\x61\x6e\x65",
    b"\x41\x7a\x72\x6f\x75",
    b"\x42\x61\x62\x5f\x54\x61\x7a\x61",
    b"\x42\x65\x72\x72\x65\x63\x68\x69\x64",
    b"\x53\x69\x64\x69\x5f\x53\x6c\x69\x6d\x61\x6e\x65",
    b"\x53\x6f\x75\x6b\x5f\x4c\x61\x72\x62\x61\x61",
    b"\x54\x69\x66\x6c\x65\x74",
    b"\x53\x69\x64\x69\x5f\x42\x65\x6e\x6e\x6f\x75\x72",
    b"\x4c\x61\x72\x61\x63\x68\x65",
    b"\x54\x61\x6e\x2d\x54\x61\x6e",
    b"\x53\x69\x64\x69\x5f\x49\x66\x6e\x69",
    b"\x47\x6f\x75\x6c\x6d\x69\x6d\x61",
    b"\x4d\x69\x64\x65\x6c\x74",
    b"\x46\x69\x67\x75\x69\x67",
    b"\x41\x7a\x69\x6c\x61\x6c",
    b"\x4a\x65\x72\x61\x64\x61",
    b"\x59\x6f\x75\x73\x73\x6f\x75\x66\x69\x61",
    b"\x4b\x73\x61\x72\x5f\x65\x73\x5f\x53\x65\x67\x68\x69\x72",
    b"\x54\x69\x63\x68\x6b\x61",
    b"\x41\x69\x74\x5f\x4d\x65\x6c\x6c\x6f\x75\x6c",
    b"\x4c\x61\x79\x6f\x75\x6e\x65",
    b"\x42\x65\x6e\x5f\x67\x75\x65\x72\x69\x72",
    b"\x4f\x75\x61\x72\x7a\x61\x7a\x61\x74\x65",
    b"\x49\x6e\x65\x7a\x67\x61\x6e\x65",
    b"\x4f\x75\x6a\x64\x61\x5f\x41\x6e\x67\x61\x64",
    b"\x53\x65\x66\x72\x6f\x75",
    b"\x41\x6f\x75\x72\x69\x72",
    b"\x4f\x75\x6c\x61\x64\x5f\x54\x65\x69\x6d\x61",
    b"\x54\x69\x63\x68\x6c\x61",
    b"\x42\x6e\x69\x5f\x48\x61\x64\x69\x66\x61",
    b"\x46\x71\x75\x69\x68\x5f\x42\x65\x6e\x5f\x53\x61\x6c\x61\x68",
    b"\x47\x75\x65\x72\x63\x69\x66",
    b"\x42\x6f\x75\x61\x72\x66\x61",
    b"\x44\x65\x6d\x6e\x61\x74\x65",
    b"\x41\x68\x66\x69\x72",
    b"\x42\x65\x72\x6b\x61\x6e\x65",
    b"\x41\x6b\x68\x66\x65\x6e\x69\x72",
    b"\x42\x6f\x75\x6c\x65\x6d\x61\x6e\x65",
    b"\x4b\x68\x65\x6e\x69\x66\x72\x61",
    b"\x42\x69\x72\x5f\x41\x6e\x7a\x65\x72\x61\x6e\x65",
    b"\x41\x73\x73\x61",
    b"\x53\x6d\x61\x72\x61",
    b"\x42\x6f\x75\x6a\x64\x6f\x75\x72",
    b"\x54\x61\x72\x66\x61\x79\x61",
    b"\x4f\x75\x61\x7a\x7a\x61\x6e\x65",
    b"\x5a\x61\x67\x6f\x72\x61",
    b"\x68\x61\x64\x5f\x73\x6f\x75\x61\x6c\x65\x6d",
    b"\x53\x61\x69\x64\x69\x61",
    b"\x42\x61\x62\x5f\x42\x65\x72\x72\x65\x64",
    b"\x4d\x69\x64\x61\x72",
    b"\x4d\x6f\x75\x6c\x61\x79\x5f\x42\x6f\x75\x73\x73\x65\x6c\x68\x61\x6d",
    b"\x4b\x68\x65\x6d\x69\x73\x73\x65\x74",
    b"\x47\x75\x65\x72\x67\x75\x65\x72\x61\x74",
    b"\x41\x73\x69\x6c\x61\x68",
    b"\x53\x69\x64\x69\x5f\x42\x6f\x75\x7a\x69\x64",
    b"\x54\x61\x66\x72\x61\x6f\x75\x74",
    b"\x49\x6d\x7a\x6f\x75\x72\x65\x6e",
    b"\x5a\x65\x6d\x61\x6d\x72\x61",
    b"\x53\x69\x64\x69\x5f\x4b\x61\x63\x65\x6d",
    b"\x44\x72\x61\x72\x67\x61",
    b"\x53\x6b\x68\x69\x72\x61\x74\x65",
];
const PRODUCTS_BYTES: [&[u8]; 94] = [
    b"\x41\x70\x70\x6c\x65",
    b"\x42\x61\x6e\x61\x6e\x61",
    b"\x4f\x72\x61\x6e\x67\x65",
    b"\x53\x74\x72\x61\x77\x62\x65\x72\x72\x79",
    b"\x47\x72\x61\x70\x65\x73",
    b"\x57\x61\x74\x65\x72\x6d\x65\x6c\x6f\x6e",
    b"\x50\x69\x6e\x65\x61\x70\x70\x6c\x65",
    b"\x4d\x61\x6e\x67\x6f",
    b"\x4b\x69\x77\x69",
    b"\x50\x65\x61\x63\x68",
    b"\x50\x6c\x75\x6d",
    b"\x43\x68\x65\x72\x72\x79",
    b"\x50\x65\x61\x72",
    b"\x42\x6c\x75\x65\x62\x65\x72\x72\x79",
    b"\x52\x61\x73\x70\x62\x65\x72\x72\x79",
    b"\x42\x6c\x61\x63\x6b\x62\x65\x72\x72\x79",
    b"\x43\x61\x6e\x74\x61\x6c\x6f\x75\x70\x65",
    b"\x48\x6f\x6e\x65\x79\x64\x65\x77",
    b"\x43\x6f\x63\x6f\x6e\x75\x74",
    b"\x50\x6f\x6d\x65\x67\x72\x61\x6e\x61\x74\x65",
    b"\x4c\x65\x6d\x6f\x6e",
    b"\x4c\x69\x6d\x65",
    b"\x47\x72\x61\x70\x65\x66\x72\x75\x69\x74",
    b"\x41\x76\x6f\x63\x61\x64\x6f",
    b"\x50\x61\x70\x61\x79\x61",
    b"\x47\x75\x61\x76\x61",
    b"\x46\x69\x67",
    b"\x50\x61\x73\x73\x69\x6f\x6e\x5f\x46\x72\x75\x69\x74",
    b"\x41\x70\x72\x69\x63\x6f\x74",
    b"\x4e\x65\x63\x74\x61\x72\x69\x6e\x65",
    b"\x43\x75\x63\x75\x6d\x62\x65\x72",
    b"\x43\x61\x72\x72\x6f\x74",
    b"\x42\x72\x6f\x63\x63\x6f\x6c\x69",
    b"\x53\x70\x69\x6e\x61\x63\x68",
    b"\x4b\x61\x6c\x65",
    b"\x4c\x65\x74\x74\x75\x63\x65",
    b"\x54\x6f\x6d\x61\x74\x6f",
    b"\x42\x65\x6c\x6c\x5f\x50\x65\x70\x70\x65\x72",
    b"\x5a\x75\x63\x63\x68\x69\x6e\x69",
    b"\x45\x67\x67\x70\x6c\x61\x6e\x74",
    b"\x43\x61\x62\x62\x61\x67\x65",
    b"\x43\x61\x75\x6c\x69\x66\x6c\x6f\x77\x65\x72",
    b"\x42\x72\x75\x73\x73\x65\x6c\x73\x5f\x53\x70\x72\x6f\x75\x74\x73",
    b"\x52\x61\x64\x69\x73\x68",
    b"\x42\x65\x65\x74",
    b"\x41\x73\x70\x61\x72\x61\x67\x75\x73",
    b"\x41\x72\x74\x69\x63\x68\x6f\x6b\x65",
    b"\x47\x72\x65\x65\x6e\x5f\x42\x65\x61\x6e\x73",
    b"\x50\x65\x61\x73",
    b"\x43\x65\x6c\x65\x72\x79",
    b"\x4f\x6e\x69\x6f\x6e",
    b"\x47\x61\x72\x6c\x69\x63",
    b"\x50\x6f\x74\x61\x74\x6f",
    b"\x53\x77\x65\x65\x74\x5f\x50\x6f\x74\x61\x74\x6f",
    b"\x59\x61\x6d",
    b"\x42\x75\x74\x74\x65\x72\x6e\x75\x74\x5f\x53\x71\x75\x61\x73\x68",
    b"\x41\x63\x6f\x72\x6e\x5f\x53\x71\x75\x61\x73\x68",
    b"\x50\x75\x6d\x70\x6b\x69\x6e",
    b"\x43\x72\x61\x6e\x62\x65\x72\x72\x79",
    b"\x47\x6f\x6a\x69\x5f\x42\x65\x72\x72\x79",
    b"\x43\x75\x72\x72\x61\x6e\x74",
    b"\x44\x61\x74\x65",
    b"\x43\x6c\x65\x6d\x65\x6e\x74\x69\x6e\x65",
    b"\x52\x68\x75\x62\x61\x72\x62",
    b"\x43\x68\x61\x72\x64",
    b"\x43\x6f\x6c\x6c\x61\x72\x64\x5f\x47\x72\x65\x65\x6e\x73",
    b"\x50\x61\x72\x73\x6c\x65\x79",
    b"\x43\x69\x6c\x61\x6e\x74\x72\x6f",
    b"\x4d\x69\x6e\x74",
    b"\x42\x61\x73\x69\x6c",
    b"\x54\x68\x79\x6d\x65",
    b"\x52\x6f\x73\x65\x6d\x61\x72\x79",
    b"\x53\x61\x67\x65",
    b"\x44\x69\x6c\x6c",
    b"\x4f\x72\x65\x67\x61\x6e\x6f",
    b"\x4a\x61\x63\x6b\x66\x72\x75\x69\x74",
    b"\x53\x74\x61\x72\x66\x72\x75\x69\x74",
    b"\x50\x65\x72\x73\x69\x6d\x6d\x6f\x6e",
    b"\x47\x69\x6e\x67\x65\x72",
    b"\x54\x75\x72\x6e\x69\x70",
    b"\x4a\x69\x63\x61\x6d\x61",
    b"\x4b\x6f\x68\x6c\x72\x61\x62\x69",
    b"\x57\x61\x74\x65\x72\x63\x72\x65\x73\x73",
    b"\x4f\x6b\x72\x61",
    b"\x50\x6c\x61\x6e\x74\x61\x69\x6e",
    b"\x43\x61\x63\x74\x75\x73\x5f\x50\x65\x61\x72",
    b"\x4b\x69\x77\x61\x6e\x6f",
    b"\x53\x71\x75\x61\x73\x68\x5f\x42\x6c\x6f\x73\x73\x6f\x6d",
    b"\x44\x72\x61\x67\x6f\x6e\x5f\x46\x72\x75\x69\x74",
    b"\x50\x61\x72\x73\x6e\x69\x70",
    b"\x52\x75\x74\x61\x62\x61\x67\x61",
    b"\x53\x61\x6c\x73\x69\x66\x79",
    b"\x42\x6f\x6b\x5f\x43\x68\x6f\x79",
    b"\x45\x6e\x64\x69\x76\x65",
];

// Define data structure
struct ProcessResult {
    cities: [f64; CITIES_BYTES.len()], // Total price per city
    products: [[f64; PRODUCTS_BYTES.len()]; CITIES_BYTES.len()], // Minimum price per product per city
}

impl Default for ProcessResult {
    fn default() -> Self {
        ProcessResult {
            cities: [0.0; CITIES_BYTES.len()],
            products: [[0.0; PRODUCTS_BYTES.len()]; CITIES_BYTES.len()],
        }
    }
}


// Process a chunk of the file based on the given offset and length
fn process_file_chunk(
    segment: &[u8],
    city_map: &FxHashMap<&[u8], usize>,
    product_map: &FxHashMap<&[u8], usize>,
) -> Result<ProcessResult> {

    let mut result = ProcessResult::default();

    for line in segment.split(|&x| x == b'\n').filter_map(|slice| Some(slice)) {
        if let Some(first_comma) = line.iter().position(|&x| x == b',') {
            let city_part = &line[..first_comma];
            if let Some(second_comma) = line[first_comma + 1..].iter().position(|&x| x == b',') {
                let product_part = &line[first_comma + 1..first_comma + 1 + second_comma];
                let price_part = &line[first_comma + 1 + second_comma + 1..];

                let city_index = *city_map.get(city_part).unwrap_or(&0);
                let product_index = *product_map.get(product_part).unwrap_or(&0);

                let price = std::str::from_utf8(price_part).ok().and_then(|s| s.parse().ok()).unwrap_or(0.0);

                result.cities[city_index] += price as f64;
                let current_price = result.products[city_index][product_index];
                if current_price == 0.0 || price < current_price {
                    result.products[city_index][product_index] = price;
                }
            }
        }
    }

    Ok(result)
}

// Function to process result and outputs the collected results
fn process_and_output_results(results: Vec<ProcessResult>) -> Result<()> {
    let mut city_totals = vec![0.0; CITIES_BYTES.len()];
    let mut products_min_prices = vec![vec![f64::MAX; PRODUCTS_BYTES.len()]; CITIES_BYTES.len()];
    let mut ofile = File::create("output.txt")?;

    // Update to use lengths of CITIES_BYTES and PRODUCTS_BYTES
    for result in results.iter() {
        for (city_index, city_total) in result.cities.iter().enumerate() {
            city_totals[city_index] += city_total;
        }
    }

    let (min_city_index, _) = city_totals.iter().enumerate()
        .min_by(|(_, a), (_, b)| a.partial_cmp(b).unwrap())
        .unwrap();

    // Process only the products for the cheapest city
    for result in results {
        for (product_index, &price) in result.products[min_city_index].iter().enumerate() {
            if price != 0.0 && price < products_min_prices[min_city_index][product_index] {
                products_min_prices[min_city_index][product_index] = price;
            }
        }
    }

    // Convert city name from bytes to string for output
    let min_city_name = str::from_utf8(CITIES_BYTES[min_city_index]).unwrap_or("Unknown City");
    writeln!(ofile, "{} {:.2}", min_city_name, city_totals[min_city_index])?;

    // Prepare and sort products for output
    let products_prices = products_min_prices[min_city_index]
        .iter()
        .enumerate()
        .filter(|&(_, &price)| price < f64::MAX)
        .map(|(index, &price)| (str::from_utf8(PRODUCTS_BYTES[index]).unwrap_or("Unknown Product"), price))
        .collect::<Vec<(&str, f64)>>();

    let mut sorted_products = products_prices;
    sorted_products.sort_by(|a, b| a.1.partial_cmp(&b.1).unwrap_or(std::cmp::Ordering::Equal).then_with(|| a.0.cmp(&b.0)));

    // Output the top 5 products
    for (product, price) in sorted_products.iter().take(5) {
        writeln!(ofile, "{} {:.2}", product, *price)?;
    }

    Ok(())
}

// Function to create lookup maps for city and product identifiers
fn create_lookup_maps() -> (FxHashMap<&'static [u8], usize>, FxHashMap<&'static [u8], usize>) {
    let city_map: FxHashMap<_, _> = CITIES_BYTES.iter().enumerate().map(|(i, &city)| (city, i)).collect();
    let product_map: FxHashMap<_, _> = PRODUCTS_BYTES.iter().enumerate().map(|(i, &product)| (product, i)).collect();

    (city_map, product_map)
}



fn main() -> Result<()> {
    // Creates city and product lookup maps
    let (city_map, product_map) = create_lookup_maps();
    
    // Wraps the maps with Arc for thread-safe reference counting
    let city_map = Arc::new(city_map);
    let product_map = Arc::new(product_map);
    

    const NUMBER_THREADS: usize = 17;

    // Shared vector to store results from each thread
    let results = Arc::new(RwLock::new(Vec::new()));

    let file = File::open("input.txt")?;

    // Creating memory mapping
    let mmap = unsafe { MmapOptions::new().map(&file)? };
    
    let safe_mmap = Arc::new(mmap_utils::SafeMmap::new(&mmap));

    let total_size = file.metadata()?.len() as usize;
    
    let segment_size = total_size / NUMBER_THREADS;

    // Parallel processing
    let handles: Vec<_> = (0..NUMBER_THREADS).map(|i: usize| {
        let safe_mmap_clone = Arc::clone(&safe_mmap);
        
        let thread_results = Arc::clone(&results);
        
        let city_map_clone = Arc::clone(&city_map);
        let product_map_clone = Arc::clone(&product_map);

        thread::spawn(move || {

            let mut start = i * segment_size;
            let mut end = if i == NUMBER_THREADS - 1 {
                total_size
            } else {
                start + segment_size
            };

            // Adjust `start` to ensure it's the beginning of a line, except for the first chunk
            if i != 0 {
                while start < total_size && safe_mmap_clone.as_slice(start - 1, start).expect("Invalid slice range")[0] != b'\n' {
                    start += 1;
                }
            }

            // Adjust `end` to ensure it ends at the complete line, except for the last chunk
            if i != NUMBER_THREADS - 1 {
                while end < total_size && safe_mmap_clone.as_slice(end - 1, end).expect("Invalid slice range")[0] != b'\n' {
                    end += 1;
                }
            }
            let chunk = safe_mmap_clone.as_slice(start, end).expect("Invalid slice range");

            // Process the segment slice...
            let result = process_file_chunk(chunk, &city_map_clone, &product_map_clone).unwrap();
            thread_results.write().unwrap().push(result);

        })
    }).collect();


    
     // Waits for all threads to complete
    for handle in handles {
        handle.join().unwrap();
    }

    // Collects results from all threads
    let results_vec = results.write().unwrap().drain(..).collect();

    // Processes and outputs the collected results
    let _ = process_and_output_results(results_vec);

    Ok(())
}
