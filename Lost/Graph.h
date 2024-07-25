#pragma once
#include <vector>
#include <iostream>
#include <map>

// [!] TODO : Figure this out, holy fuck ;-;


class Graph
{
public:

public:
	Graph();
	~Graph();

	inline bool hasNode(uint32_t id) const { return adjacencyMap.count(id); };
	
	uint32_t nextAvailableNodeID() const;
	std::vector<uint32_t> nextAvailableNodeIDs(int count) const;
	uint32_t addNode(std::vector<uint32_t>& connections);
	uint32_t addNodeByConnections(uint32_t nodeID, std::vector<uint32_t>& connections);
	std::vector<Graph> removeNode(uint32_t id); // Returns if the removal of the node split the graph

	std::map<uint32_t, std::vector<uint32_t>> adjacencyMap;
private:
	uint32_t totalNodes = 0;

	void m_AddEdge(std::map<uint32_t, std::vector<uint32_t>>& adj, int u, int v);
};

