#pragma once
#include "util/common.h"
#include "util/range.h"
#include "system/proto/node.pb.h"
#include "data/proto/data.pb.h"
namespace PS {

// assign *node* with proper rank_id, key_range, etc..
class NodeAssigner {
 public:
  NodeAssigner(int num_servers, Range<Key> key_range = Range<Key>::all()) {
    num_servers_ = num_servers;
    key_range_ = key_range;
  }
  ~NodeAssigner() { }

  void assign(Node* node) {
    Range<Key> kr = key_range_;
    int rank = 0;
    if (node->role() == Node::SERVER) {
      kr = key_range_.evenDivide(num_servers_, server_rank_);
      rank = server_rank_ ++;
    } else if (node->role() == Node::SERVER) {
      rank = worker_rank_ ++;
    }
    node->set_rank(rank);
    kr.to(node->mutable_key());
  }

  void remove(const Node& node) {
    // TODO
  }
 protected:
  int num_servers_ = 0;
  int server_rank_ = 0;
  int worker_rank_ = 0;
  Range<Key> key_range_;
};

// divide *data* into *num* parts.
class DataAssigner {
 public:
  DataAssigner() { }
  DataAssigner(const DataConfig& data, int num) { set(data, num); }
  ~DataAssigner() { }

  void set(const DataConfig& data, int num);
  bool next(DataConfig *data);

 private:
  std::vector<DataConfig> parts_;
  int cur_i = 0;
};

} // namespace PS