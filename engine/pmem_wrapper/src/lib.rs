#![feature(test)]

extern crate test;

pub mod btree;
pub mod kv;

#[cfg(test)]
mod tests {
    use corundum::default::*;
    use corundum::open_flags::*;
    use test::Bencher;

    #[bench]
    fn bench_put_key_kv(b: &mut Bencher) {
        let root = Allocator::open::<crate::kv::KvStore<i32>>("testfile", O_CFNE | O_1GB).unwrap();
        b.iter(move || {
            for i in 0..10 {
                let key = format!("{}", i);
                root.put(&*key, i);
            }
        });
    }

    #[bench]
    fn bench_insert_btree(b: &mut Bencher) {
        let root = Allocator::open::<crate::btree::BTree>("testfile1", O_CFNE | O_1GB).unwrap();
        b.iter(move || {
            for i in 0..10 {
                let test_case = format!("{}", i);
                root.insert(test_case.as_str(), test_case.as_str());
                let res = root.find(test_case.as_str());
                assert_eq!(test_case, res.unwrap());
            }
        });
    }
}
