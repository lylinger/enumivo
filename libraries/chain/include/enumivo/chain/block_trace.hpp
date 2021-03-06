/**
 *  @file
 *  @copyright defined in enumivo/LICENSE.txt
 */
#pragma once
#include <enumivo/chain/block.hpp>
#include <enumivo/chain/transaction_trace.hpp>

namespace enumivo { namespace chain {

   struct shard_trace {
      digest_type                   shard_action_root;
      digest_type                   shard_transaction_root;
      vector<transaction_trace>     transaction_traces;
      uint64_t                      cpu_usage;

      flat_set<shard_lock>          read_locks;
      flat_set<shard_lock>          write_locks;

      void append( transaction_trace&& res ) {
         transaction_traces.emplace_back(move(res));
      }

      void append( const transaction_trace& res ) {
         transaction_traces.emplace_back(res);
      }

      void finalize_shard();
   };

   struct cycle_trace {
      vector<shard_trace>           shard_traces;
   };

   struct region_trace {
      vector<cycle_trace>           cycle_traces;
   };

   struct block_trace {
      explicit block_trace(const signed_block& s)
              :block(s)
      {}

      const signed_block&     block;
      vector<region_trace>    region_traces;
      vector<transaction>     implicit_transactions;
      digest_type             calculate_action_merkle_root()const;
      digest_type             calculate_transaction_merkle_root()const;
      uint64_t                calculate_cpu_usage() const;
   };


} } // enumivo::chain

FC_REFLECT( enumivo::chain::shard_trace, (shard_action_root)(shard_transaction_root)(transaction_traces)(cpu_usage)(read_locks)(write_locks))
FC_REFLECT( enumivo::chain::cycle_trace, (shard_traces))
FC_REFLECT( enumivo::chain::region_trace, (cycle_traces))
FC_REFLECT( enumivo::chain::block_trace, (region_traces))
