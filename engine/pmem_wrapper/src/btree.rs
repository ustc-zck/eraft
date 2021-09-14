//! Implementation of persistent binary search tree

use corundum::open_flags::*;
use corundum::*;
use std::env;
use std::fmt::{Display, Error, Formatter};

type P = default::Allocator;
//type P = Heap;
type Pbox<T> = corundum::Pbox<T, P>;
type Ptr = Option<Pbox<BTreeNode>>;

struct FixStr {
    chars: [u8; 32],
}

impl FixStr {
  pub fn ToString(&self) -> String {
    let s = String::from_utf8(self.chars.to_vec()).unwrap();
    s
  }
}

impl From<String> for FixStr {
    fn from(value: String) -> Self {
        let value = value.as_bytes();
        let mut chars = [0u8; 32];
        unsafe {
            std::ptr::copy_nonoverlapping(&value[0], &mut chars[0] as *mut u8, value.len().min(32));
        }
        FixStr { chars }
    }
}

impl Display for FixStr {
    fn fmt(&self, f: &mut Formatter<'_>) -> Result<(), Error> {
        let s = String::from_utf8(self.chars.to_vec()).unwrap();
        write!(f, "{}", s)
    }
}

struct BTreeNode {
    key: FixStr,
    slots: [Ptr; 2],
    value: FixStr,
}

impl BTreeNode {
    pub fn new(key: &str, value: &str) -> Self {
        Self {
            key: FixStr::from(key.to_string()),
            slots: [None, None],
            value: FixStr::from(value.to_string()),
        }
    }
}

impl Display for BTreeNode {
    fn fmt(&self, f: &mut Formatter<'_>) -> Result<(), Error> {
        write!(f, "{} {}", self.key, self.value)
    }
}

#[derive(Root)]
pub struct BTree {
    root: Ptr,
}

impl BTree {
    pub fn insert(&self, key: &str, value: &str) {
        let mut dst = &self.root;
        while let Some(node) = dst {
            dst = &node.slots[if key.as_bytes() > node.key.ToString().as_bytes() { 1 } else { 0 }];
        }
        let _ = Pbox::initialize(dst, BTreeNode::new(key, value));
    }

    pub fn find(&self, key: &str) -> Option<String> {
        let mut curr = &self.root;
        while let Some(node) = curr {
            // if node.key->eq(key) {
            //     return Some(node.value.to_string());
            // } else {
            //     curr = &node.slots[if key > node.key { 1 } else { 0 }];
            // }
        }
        None
    }

    fn foreach<F: Copy + FnOnce(&BTreeNode) -> ()>(node: &Ptr, f: F) {
        if let Some(node) = node {
            Self::foreach(&node.slots[0], f);
            f(node);
            Self::foreach(&node.slots[1], f);
        }
    }

    pub fn print(&self) {
        Self::foreach(&self.root, |p| {
            println!("{}", p);
        });
    }
}
