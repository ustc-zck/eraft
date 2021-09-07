
#ifndef YCSB_C_Eraft_DB_H_
#define YCSB_C_Eraft_DB_H_


#include <Kv/config.h>
#include <Kv/raft_client.h>
#include <eraftio/helloworld.grpc.pb.h>
#include <eraftio/kvrpcpb.grpc.pb.h>
#include <grpcpp/grpcpp.h>
#include "core/db.h"
#include <iostream>
#include <string>
#include "core/properties.h"

using std::cout;
using std::endl;

namespace ycsbc {

class EraftDB : public DB {
 public:
  EraftDB(string addr){
          std::shared_ptr<Config> conf =
      std::make_shared<Config>(addr, "#", 0);
      raftClient_ = std::make_shared<RaftClient>(conf);
      addr_ = addr;
  }

  int Read(const std::string &table, const std::string &key,
           const std::vector<std::string> *fields,
           std::vector<KVPair> &result);

           
  int Insert(const std::string &table, const std::string &key,
             std::vector<KVPair> &values);
             
  int Delete(const std::string &table, const std::string &key);

  int Scan(const std::string &table, const std::string &key,
           int len, const std::vector<std::string> *fields,
           std::vector<std::vector<KVPair>> &result) {
    throw "Scan: function not implemented!";
  }

  int Update(const std::string &table, const std::string &key,
             std::vector<KVPair> &values){
      throw "Update: function not implemented!";
  }


 private:
  std::shared_ptr<RaftClient> raftClient_;
  std::string addr_;
};

} // ycsbc

#endif 

