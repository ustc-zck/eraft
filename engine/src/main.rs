//! simplekv.rs -- a rust implementation of [simplekv example] from PMDK,
//! libpmemobj++ of simple kv which uses vector to hold values, string as a key
//! and array to hold buckets.
//!
//! [simplekv example]: https://github.com/pmem/libpmemobj-cpp/tree/master/examples/simplekv
#![feature(type_name_of_val)]

use corundum::default::*;
use corundum::open_flags::*;
use pmem_wrapper::*;

type P = Allocator;

pub fn main() {
    let root = P::open::<btree::BTree>("testfile1", O_CFNE | O_1GB).unwrap();

    use std::env;
    use std::vec::Vec as StdVec;

    let args: StdVec<String> = env::args().collect();

    if args.len() < 3 {
        println!(
            "usage: {} file-name [get key|put key value] | [burst get|put|putget count]",
            args[0]
        );
        return;
    }

    if args[2] == String::from("get") && args.len() == 4 {
        let res = root.find(args[3].as_str());
        match res {
            Some(res) => {
                println!("{}", res);
            }
            None => {
                println!("Not Found Value")
            }
        }
    } else if args[2] == String::from("put") && args.len() == 5 {
        root.insert(args[3].as_str(), args[4].as_str());
    }
    if args[2] == String::from("burst")
        && (args[3] == String::from("put") || args[3] == String::from("putget"))
        && args.len() == 5
    {
        // Not Impl
    }
    if args[2] == String::from("burst")
        && (args[3] == String::from("get") || args[3] == String::from("putget"))
        && args.len() == 5
    {
        // Not Impl
    }
}
