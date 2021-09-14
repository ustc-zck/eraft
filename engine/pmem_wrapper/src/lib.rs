#![feature(test)]

extern crate test;

pub mod kv;

#[cfg(test)]
mod tests {
    use corundum::default::*;
    use corundum::open_flags::*;
    use test::Bencher;

    #[bench]
    fn bench_put_key(b: &mut Bencher) {
        let root = Allocator::open::<crate::kv::KvStore<i32>>("testfile", O_CFNE | O_1GB).unwrap();
        b.iter(move || {
            for i in 0..100 {
                let key = format!("key{}", i);
                root.put(&*key, i);
            }
        });
    }
}
