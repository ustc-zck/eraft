#include <Kv/node.h>
#include <Kv/utils.h>
#include <Kv/bootstrap.h>
#include <Logger/Logger.h>

#include <eraftio/raft_serverpb.pb.h>

namespace kvserver
{

extern bool DoBootstrapStore(std::shared_ptr<Engines> engines, uint64_t clusterID, uint64_t storeID);

extern uint64_t gCounter;

Node::Node(std::shared_ptr<RaftStore> system, std::shared_ptr<Config> cfg)
{
    this->clusterID_ = 1;
    metapb::Store store;
    store.set_address(cfg->storeAddr_);
    this->store_ = std::make_shared<metapb::Store>(store);
    this->cfg_ = cfg;
    this->system_ = system;
}

Node::~Node()
{

}

bool Node::Start(std::shared_ptr<Engines> engines, std::shared_ptr<Transport> trans)
{
    uint64_t storeID;
    if(!this->CheckStore(*engines, &storeID))
    {
        Logger::GetInstance()->ERRORS("store id: " + std::to_string(storeID) + " not found");
    }
    if(storeID == Assistant::GetInstance()->kInvalidID)
    {
        this->BootstrapStore(*engines, &storeID);
    }
    this->store_->set_id(storeID);
    auto checkRes = this->CheckOrPrepareBoostrapCluster(engines, storeID);
    if(!checkRes.second)
    {
        Logger::GetInstance()->ERRORS("check or prepare boostrap cluster error");
        return false;
    }
    bool newCluster = (checkRes.first != nullptr);
    if(newCluster)
    {
        // try to boostrap cluster
        if(!this->BoostrapCluster(engines, checkRes.first, &newCluster))
        {
            Logger::GetInstance()->ERRORS("boostrap cluster error");
            return false;
        }
    }
    // // TODO: put scheduler store
    if(!this->StartNode(engines, trans))
    {
        Logger::GetInstance()->ERRORS("start node error");
        return false;
    }
    return true;
}

bool Node::CheckStore(Engines& engs, uint64_t* storeId)
{
    raft_serverpb::StoreIdent ident;
    if(!Assistant::GetInstance()->GetMeta(engs.kvDB_, Assistant::GetInstance()->VecToString(Assistant::GetInstance()->StoreIdentKey), &ident).ok())
    {
        // hey store ident meta key error
        *storeId = 0;
    }
    if(ident.cluster_id() != this->clusterID_)
    {
        *storeId = 0;
        //TODO: log cluster id mismatch
        return false;
    }
    if(ident.store_id() == Assistant::GetInstance()->kInvalidID)
    {
        *storeId = 0;
        return false;
    }
    *storeId = ident.store_id();
    return true;
}

uint64_t Node::AllocID()
{
    Logger::GetInstance()->INFO("start alloc id");
    return BootHelper().GetInstance()->MockSchAllocID();
}

std::pair<std::shared_ptr<metapb::Region> , bool> Node::CheckOrPrepareBoostrapCluster(std::shared_ptr<Engines> engines, uint64_t storeID)
{
    raft_serverpb::RegionLocalState* state = new raft_serverpb::RegionLocalState();
    if(Assistant::GetInstance()->GetMeta(engines->kvDB_, Assistant::GetInstance()->VecToString(Assistant::GetInstance()->PrepareBootstrapKey), state).ok())
    {
        return std::make_pair<std::shared_ptr<metapb::Region> , bool>(std::make_shared<metapb::Region>(state->region()), true);
    }
    // if(!this->CheckClusterBoostrapped())
    // {
    //     return std::make_pair<std::shared_ptr<metapb::Region> , bool>(nullptr, false);
    // }
    // else
    // {
    //     return std::make_pair<std::shared_ptr<metapb::Region> , bool>(nullptr, true);
    // }
    return this->PrepareBootstrapCluster(engines, storeID);
}

bool Node::CheckClusterBoostrapped()
{
    // call sch to check cluster boostrapped
    return false;
}

std::pair<std::shared_ptr<metapb::Region> , bool> Node::PrepareBootstrapCluster(std::shared_ptr<Engines> engines,  uint64_t storeID)
{
    auto regionID = BootHelper().GetInstance()->MockSchAllocID();
    auto peerID = BootHelper().GetInstance()->MockSchAllocID();
    Logger::GetInstance()->INFO("bootstrap node with regionID: " + std::to_string(regionID) + 
    "  storeID: " + std::to_string(storeID) + " peerID: " + std::to_string(peerID) + " clusterID: " + std::to_string(this->clusterID_));
    return BootHelper().GetInstance()->PrepareBootstrap(engines, storeID, regionID, peerID);
}

bool Node::BoostrapCluster(std::shared_ptr<Engines> engines, std::shared_ptr<metapb::Region> firstRegion, bool* isNewCluster)
{
    auto regionID = firstRegion->id();
    
    // TODO: send boostrap to scheduler
    return true;
}


bool Node::BootstrapStore(Engines& engs, uint64_t* storeId)
{
    auto storeID = this->AllocID();
    Logger::GetInstance()->INFO("boostrap store with storeID: " + std::to_string(storeID));
    if(!BootHelper::GetInstance()->DoBootstrapStore(std::make_shared<Engines>(engs), this->clusterID_, storeID))
    {
        Logger::GetInstance()->ERRORS("do bootstrap store error");
        return false;
    }
    *storeId = storeID;
    return true;
}

bool Node::StartNode(std::shared_ptr<Engines> engines, std::shared_ptr<Transport> trans)
{
    return this->system_->Start(this->store_, this->cfg_, engines, trans);
}

bool Node::StopNode(uint64_t storeID)
{
    this->system_->ShutDown();
}

void Node::Stop()
{
    this->StopNode(this->store_->id());
}

uint64_t Node::GetStoreID()
{
    return this->store_->id();
}

} // namespace kvserver
