//
//  basic_db.cc
//  YCSB-C
//
//  Created by Jinglei Ren on 12/17/14.
//  Copyright (c) 2014 Jinglei Ren <jinglei@ren.systems>.
//

#include "db/db_factory.h"

#include <string>
#include "db/basic_db.h"
#include "db/eraft_db.h"


using namespace std;
using ycsbc::DB;
using ycsbc::DBFactory;
using ycsbc::EraftDB;

DB* DBFactory::CreateDB(utils::Properties &props) {
  if (props["dbname"] == "basic") {
    return new BasicDB;
  } else if (props["dbname"] == "eraft") {
    std::string addr = props["addr"];
    return new EraftDB(addr);
  } else return NULL;
}

