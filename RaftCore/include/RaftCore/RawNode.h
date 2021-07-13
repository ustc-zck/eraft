// Copyright 2015 The etcd Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.


// @file RawNode.h
// @author Colin
// This module declares the eraft::RawNode class.
// 
// Inspired by etcd golang version.

#ifndef ERAFT_RAWNODE_H
#define ERAFT_RAWNODE_H

#include <stdint.h>
#include <eraftio/eraftpb.pb.h>
#include <RaftCore/Raft.h>

namespace eraft
{

class DReady
{

public:

    std::shared_ptr<ESoftState> softSt;
    
    eraftpb::HardState hardSt;

    DReady() {}

    ~DReady() {}


    // Entries specifies entries to be saved to stable storage BEFORE
    // Messages are sent.
    std::vector<eraftpb::Entry> entries;

    eraftpb::Snapshot snapshot;

    std::vector<eraftpb::Entry> committedEntries;

    std::vector<eraftpb::Message> messages;

};

class RawNode {

public:

    std::shared_ptr<RaftContext> raft;

    RawNode(Config& config);

    // Tick advances the internal logical clock by a single tick.
    void Tick();

    // Campaign causes this RawNode to transition to candidate state.
    void Campaign();

    // Propose proposes data be appended to the raft log.
    void Propose(std::vector<uint8_t>* data);

    // ProposeConfChange proposes a config change.
    void ProposeConfChange(eraftpb::ConfChange cc);

    // ApplyConfChange applies a config change to the local node.
    eraftpb::ConfState ApplyConfChange(eraftpb::ConfChange cc);

    // Step advances the state machine using the given message.
    void Step(eraftpb::Message m);

    // EReady returns the current point-in-time state of this RawNode.
    DReady EReady();

    // HasReady called when RawNode user need to check if any Ready pending.
    bool HasReady();

    // Advance notifies the RawNode that the application has applied and saved progress in the
    // last Ready results.
    void Advance(DReady rd);

    // GetProgress return the the Progress of this node and its peers, if this
    // node is leader.
    std::map<uint64_t, Progress> GetProgress();

    // TransferLeader tries to transfer leadership to the given transferee.
    void TransferLeader(uint64_t transferee);

private:

    std::shared_ptr<ESoftState> prevSoftSt;

    std::shared_ptr<eraftpb::HardState> prevHardSt;
};

} // namespace eraft


#endif