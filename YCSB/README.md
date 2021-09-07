## YCSB Benchmark

- 测试方法如下

```
#find leader node
addr="172.19.0.12:20161"
cd /eraft/build_/YCSB
./ycsb -db eraft -threads 2 -addr $addr -P /eraft/YCSB/workloads/workloada.spec
```

#### 待完善

- Insert操作时间统计：目前的统计时间是Insert操作提交给raft group的时间，并不是insert操作被Commit的时间。
- benchmark启动前预插入到数据库中的数据的设置待编写。
- benchmark运行完成后删除数据库中数据待编写。


#### 待核查

- 已经被Commit的insert操作数据是否真的插入到rocksdb中待核查。


