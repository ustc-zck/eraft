#!/bin/bash

set -xe

SRCPATH="/root/eraft"

build_dir="$SRCPATH/build_"
mkdir -p $build_dir && cd $build_dir
cmake "$SRCPATH" \
    -DENABLE_TESTS=on
make -j 2  # if nproc > 4, can't build

if [ ! -d "$SRCPATH/output" ]; then
  mkdir $SRCPATH/output
  mkdir $SRCPATH/output/logs
fi

cp $build_dir/RaftCore/test/RaftTests $SRCPATH/output
#cp $build_dir/Logger/test/logger_tests $SRCPATH/output

$build_dir/RaftCore/test/RaftTests
#$build_dir/Logger/test/logger_tests
