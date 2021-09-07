

#include "db/eraft_db.h"

#include <cstring>

using namespace std;

namespace ycsbc {

int EraftDB::Read(const string &table, const string &key,
         const vector<string> *fields,
         vector<KVPair> &result) {
  kvrpcpb::RawGetRequest request;
  request.mutable_context()->set_region_id(1);
  request.set_cf("test");
  request.set_key(key);
  std::string value = raftClient_->GetRaw(addr_, request);
  if(value != ""){
    return DB::kOK;
  }else{
    return DB::kErrorNoData;
  }
}

int EraftDB::Insert(const std::string &table, const std::string &key,
             std::vector<KVPair> &values){
  kvrpcpb::RawPutRequest request;
  request.mutable_context()->set_region_id(1);
  request.set_cf("test");
  request.set_key(key);
  request.set_value(values[0].second);
  request.set_type(1);
  if(raftClient_->PutRaw(addr_, request)){
    return DB::kOK;
  }else{
    return DB::kErrorNoData;
  }

}

int EraftDB::Delete(const std::string &table, const std::string &key){
  kvrpcpb::RawPutRequest request;
  request.mutable_context()->set_region_id(1);
  request.set_cf("test");
  request.set_key(key);
  request.set_value("#");
  request.set_type(2);
  if(raftClient_->PutRaw(addr_, request)){
    return DB::kOK;
  }else{
    return DB::kErrorNoData;
  }
}


} // namespace ycsbc
