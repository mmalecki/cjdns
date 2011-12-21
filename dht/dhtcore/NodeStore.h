#ifndef NODE_STORE_H
#define NODE_STORE_H

#include <stdint.h>
#include <stdbool.h>

#include "dht/Address.h"
#include "dht/dhtcore/Node.h"
#include "memory/MemAllocator.h"

struct NodeStore;

/**
 * Create a new NodeStore.
 *
 * @param myAddress the address for this DHT node.
 * @param capacity the number of nodes which this store can hold.
 * @param allocator the allocator to allocate storage space for this NodeStore.
 */
struct NodeStore* NodeStore_new(struct Address* myAddress,
                                const uint32_t capacity,
                                const struct MemAllocator* allocator);

/**
 * Find a node in the store.
 *
 * @param store a store to get the node from.
 * @param addr the identifier for the node to lookup.
 * @return A pointer to the node if one is found, otherwise NULL.
 */
struct Node* NodeStore_getNode(const struct NodeStore* store, struct Address* addr);

/**
 * Put a node into the store.
 *
 * @param store a node store to insert into.
 * @param addr the address of the new node.
 * @param reachDiff how much to adjust the reach in a new node, if the node already exists,
 *                  this will alter the reach by this amount, if changing the reach by this
 *                  amount causes the reach to become negative or nolonger fit in a uint32
 *                  type, it will be set to 0 or UINT32_MAX, respectively.
 *                  Undefined behavior will result if this input exceeds UINT32_MAX.
 */
void NodeStore_addNode(struct NodeStore* store,
                       struct Address* addr,
                       const int64_t reachDiff);

/**
 * Get the best nodes for servicing a lookup.
 * These are returned in reverse order, from farthest to closest.
 *
 * @param store the store to get the nodes from.
 * @param targetAddress the address to get closest nodes for.
 * @param count the number of nodes to return.
 * @param allowNodesFartherThanUs if true then return nodes which are farther than the target then we are.
 *                                this is required for searches but unallowable for answering queries.
 * @param allocator the memory allocator to use for getting the memory to store the output.
 */
struct NodeList* NodeStore_getClosestNodes(struct NodeStore* store,
                                           struct Address* targetAddress,
                                           const uint32_t count,
                                           const bool allowNodesFartherThanUs,
                                           const struct MemAllocator* allocator);

/**
 * Change the reach of a node in the NodeStore.
 * Just changing the reach number will have no effect unless it is "committed"
 * by calling NodeStore_updateReach().
 *
 * @param node the node to update.
 * @param store the store where that node is contained.
 */
void NodeStore_updateReach(const struct Node* const node,
                           const struct NodeStore* const store);

uint32_t NodeStore_size(const struct NodeStore* const store);

uint64_t NodeStore_decreaseReach(const uint32_t decreaseReachBy,
                                 const struct NodeStore* const store);

struct Node* NodeStore_getNodeByNetworkAddr(uint8_t networkAddress[Address_NETWORK_ADDR_SIZE],
                                            struct NodeStore* store);
#endif