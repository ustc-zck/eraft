//! Implementation of persistent binary search tree

use corundum::*;
use std::fmt::{Display, Error, Formatter};

type P = default::Allocator;
type Pbox<T> = corundum::Pbox<T, P>;
type Ptr = Option<Pbox<BTreeNode>>;

const KV_MAX_SIZE: usize = 1024 * 10; // 10KB

struct FixStr {
    chars: [u8; KV_MAX_SIZE],
}

impl FixStr {
    pub fn to_string(&self) -> String {
        let s = String::from_utf8(self.chars.to_vec()).unwrap();
        s.to_string()
    }
}

impl From<String> for FixStr {
    fn from(value: String) -> Self {
        let value = value.as_bytes();
        let mut chars = [0u8; KV_MAX_SIZE];
        unsafe {
            std::ptr::copy_nonoverlapping(
                &value[0],
                &mut chars[0] as *mut u8,
                value.len().min(KV_MAX_SIZE),
            );
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
            dst = &node.slots[if String::from(key).gt(&node.key.to_string()) {
                1
            } else {
                0
            }];
        }
        let _ = Pbox::initialize(dst, BTreeNode::new(key, value));
    }

    pub fn find(&self, key: &str) -> Option<String> {
        let mut curr = &self.root;
        while let Some(node) = curr {
            let node_key = node.key.to_string().trim_matches(char::from(0)).to_string();
            if node_key.eq(&key) {
                return Some(
                    node.value
                        .to_string()
                        .trim_matches(char::from(0))
                        .to_string(),
                );
            } else {
                curr = &node.slots[if String::from(key).gt(&node_key) {
                    1
                } else {
                    0
                }];
            }
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
