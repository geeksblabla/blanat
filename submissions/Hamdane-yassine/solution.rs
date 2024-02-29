// Compile with: rustc -O solution.rs -o solution

use std::fs::File;
use std::io::{self, BufRead, BufReader, Seek, SeekFrom, Result, Write};
use std::sync::{Arc,RwLock};
use std::thread;
use std::str;


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

/* ------------------END OF UTILITIES-------------------- */



// all cities and products in bytes format
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
    products: [[f32; PRODUCTS_BYTES.len()]; CITIES_BYTES.len()], // Minimum price per product per city
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
    file_path: &str,
    start_offset: u64,
    len: usize,
    city_map: &FxHashMap<&[u8], usize>,
    product_map: &FxHashMap<&[u8], usize>
) -> Result<ProcessResult> {
    let mut file = File::open(file_path)?;
    let file_len = file.metadata()?.len(); // Retrieve the file's total length

    // Check if the starting offset exceeds the file size
    if start_offset >= file_len {
        return Err(io::Error::new(io::ErrorKind::InvalidInput, "start_offset exceeds file size"));
    }

    file.seek(SeekFrom::Start(start_offset))?; // Move to the starting offset

    let mut reader = BufReader::with_capacity(16  * 1024 * 1024, file);
    let mut buffer: Vec<u8> = Vec::new();
    
    // Read until the newline to ensure starting at the beginning of a line
    if start_offset != 0 { 
        reader.read_until(b'\n', &mut buffer)?;
    }

    let mut actual_len = len as u64; // Convert len to u64 for comparison

    // Ensure not to read beyond the file's remaining content based on the start_offset
    if start_offset + actual_len > file_len {
        actual_len = file_len - start_offset;
    }

    let mut result = ProcessResult::default();
    let mut processed_bytes: u64 = 0; // Keep track of how many bytes have been processed

    while processed_bytes < actual_len {
        buffer.clear(); // Clear the buffer for the next line
        let bytes_read = reader.read_until(b'\n', &mut buffer)?;
        if bytes_read == 0 { // End of file
            break;
        }

        processed_bytes += bytes_read as u64; // Update the count of processed bytes

        // Avoid processing the line if it exceeds the specified length
        if processed_bytes > actual_len {
            break;
        }

        if let Some(&b'\n') = buffer.last() {
            buffer.pop(); // Remove trailing newline character for consistency with split behavior
        }

        // Now, process the line similar to the original function
        if let Some(first_comma) = buffer.iter().position(|&x| x == b',') {
            let city_part = &buffer[..first_comma];
            if let Some(second_comma) = buffer[first_comma + 1..].iter().position(|&x| x == b',') {
                let product_part = &buffer[first_comma + 1..first_comma + 1 + second_comma];
                let price_part = &buffer[first_comma + 1 + second_comma + 1..];

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
    let mut products_min_prices = vec![vec![f32::MAX; PRODUCTS_BYTES.len()]; CITIES_BYTES.len()];
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
        .filter(|&(_, &price)| price < f32::MAX)
        .map(|(index, &price)| (str::from_utf8(PRODUCTS_BYTES[index]).unwrap_or("Unknown Product"), price))
        .collect::<Vec<(&str, f32)>>();

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

// Function to find offsets for dividing a file into chunks for parallel processing
fn find_chunk_offsets(file_path: &str, num_chunks: usize) -> Result<Vec<u64>> {
    let file = File::open(file_path)?;
    let file_size = file.metadata()?.len();
    let chunk_size = file_size / num_chunks as u64;
    let mut offsets = vec![0];
    let mut reader = BufReader::new(file);

    for _ in 0..num_chunks - 1 {
        let seek_pos = offsets.last().unwrap() + chunk_size;
        reader.seek(SeekFrom::Start(seek_pos))?;
        
        let mut buf = Vec::new();
        reader.read_until(b'\n', &mut buf)?;
        offsets.push(seek_pos + buf.len() as u64);
    }

    Ok(offsets)
}

// Main function to orchestrate the parallel processing of a file
fn main() -> Result<()> {
    // Creates city and product lookup maps
    let (city_map, product_map) = create_lookup_maps();
    // Wraps the maps with Arc for thread-safe reference counting
    let city_map = Arc::new(city_map);
    let product_map = Arc::new(product_map);

    let file_path = "input.txt";
    
    // Determines the number of threads based on available parallelism
    let num_threads = std::thread::available_parallelism().unwrap().get() * 2;
    // Finds offsets to divide the file into chunks for each thread
    let offsets = find_chunk_offsets(file_path, num_threads)?;

    // Shared vector to store results from each thread
    let results = Arc::new(RwLock::new(Vec::new()));
    let mut handles = vec![];


    // Spawns threads to process each file chunk in parallel
    for i in 0..num_threads {

        let city_map_clone = Arc::clone(&city_map);
        let product_map_clone = Arc::clone(&product_map);
        
        let start_offset = offsets[i];
        let end_offset = if i < num_threads - 1 { offsets[i + 1] } else { std::u64::MAX };

        let thread_results = Arc::clone(&results);
        let file_path = file_path.to_string();

        let handle = thread::spawn(move || {
            
            // Calculates length of the chunk to process
            let len = if end_offset == std::u64::MAX {
                std::u64::MAX - start_offset
            } else {
                end_offset - start_offset
            } as usize;
            
            // Processes the file chunk and stores the result
            let result = process_file_chunk(&file_path, start_offset, len, &city_map_clone, &product_map_clone).unwrap();
            thread_results.write().unwrap().push(result);
        });
        
        handles.push(handle);
    }

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
