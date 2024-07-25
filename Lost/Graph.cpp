#include "Graph.h"

Graph::Graph()
{
}

Graph::~Graph()
{
}

uint32_t Graph::nextAvailableNodeID() const
{
	// Find a free integer within the map
	size_t freeIndex = 0;
	while (freeIndex < SIZE_MAX)
	{
		if (adjacencyMap.count(freeIndex) == 0)
			return freeIndex;
		freeIndex++;
	}

	return -1;
}

std::vector<uint32_t> Graph::nextAvailableNodeIDs(int count) const
{
	std::vector<uint32_t> availableNodeIDs;

	size_t freeIndex = 0;
	while (freeIndex < SIZE_MAX && availableNodeIDs.size() < count)
	{
		if (adjacencyMap.count(freeIndex) == 0)
		{
			availableNodeIDs.push_back(freeIndex);
		}
		freeIndex++;
	}

	return availableNodeIDs;
}

uint32_t Graph::addNode(std::vector<uint32_t>& connections)
{
	// Find a free integer within the map
	size_t freeIndex = 0;
	while (freeIndex < SIZE_MAX)
	{
		if (adjacencyMap.count(freeIndex) == 0)
			break;
		freeIndex++;
	}

	// Append an empty int list to the adjacency map
	adjacencyMap[freeIndex] = std::vector<uint32_t>{};

	for (uint32_t& c : connections)
		m_AddEdge(adjacencyMap, c, freeIndex);

	totalNodes++;
	return freeIndex;
}

uint32_t Graph::addNodeByConnections(uint32_t nodeID, std::vector<uint32_t>& connections)
{
	for (uint32_t& c : connections)
		adjacencyMap[c].push_back(nodeID);
	totalNodes = adjacencyMap.size();
	return nodeID;
}

std::vector<Graph> Graph::removeNode(uint32_t id)
{
	// Only returns the extra graphs
	std::vector<Graph> outGraphs = {};
	int totalGraphs = 0;

	// Get the node to test if the map is split from
	int testNode = -1;
	int testedNodes = 0;
	if (adjacencyMap[id].size() > 0)
	{
		testNode = adjacencyMap[id][0];
		testedNodes++;
	}

	// Loop through node's connections and remove any reference to it
	for (int i = 0; i < adjacencyMap[id].size(); i++)
	{
		int toClearReference = adjacencyMap[id][i];
		for (int j = 0; j < adjacencyMap[toClearReference].size(); j++)
		{
			if (adjacencyMap[toClearReference][j] == id)
				adjacencyMap[toClearReference].erase(adjacencyMap[toClearReference].begin() + j);
		}
	}

	std::vector<uint32_t> removedConnections = adjacencyMap[id];
	adjacencyMap.erase(id);
	totalNodes--;

	if (testNode != -1)
	{
		// Create visited array
		std::map<int, bool> visited;
		for (auto& [key, val] : adjacencyMap)
			visited[key] = false;

		// Initialize search with testNode
		int passedNodes = 0;
		std::vector<int> freeNodes = { testNode };

		// Loop until it has gone over every node in the graph
		while (passedNodes < adjacencyMap.size() || freeNodes.size() > 0)
		{

			while (freeNodes.size() > 0)
			{
				visited[freeNodes[0]] = true;

				for (int i = 0; i < adjacencyMap[freeNodes[0]].size(); i++)
				{
					// Check if visited node
					if (!visited[adjacencyMap[freeNodes[0]][i]])
					{
						freeNodes.push_back(adjacencyMap[freeNodes[0]][i]);
						visited[adjacencyMap[freeNodes[0]][i]] = true;
					}
				}

				// If there is more than 1 graph, move the nodes to the other graph
				if (totalGraphs > 0)
				{
					outGraphs[totalGraphs - 1].adjacencyMap[freeNodes[0]] = adjacencyMap[freeNodes[0]];

					// Don't need to remove references, since this code is only ran on the seperate graph
					// and all nodes it can link to, will also be removed
					adjacencyMap.erase(freeNodes[0]);
					totalNodes--;

					outGraphs[totalGraphs - 1].totalNodes++;
				}

				// Remove from beginning of queue
				freeNodes.erase(freeNodes.begin());
				passedNodes++;
			}

			// Check if it's tested all connected nodes
			if (testedNodes < removedConnections.size())
			{
				// Here we know there is more than one group
				totalGraphs++;
				outGraphs.push_back(Graph{});

				// Loop through all testable nodes
				for (int checkID = testedNodes; checkID < removedConnections.size(); checkID++)
				{
					// Check if node has been visited
					if (!visited[int(removedConnections[testedNodes])])
					{
						// If not, set freeNodes to testedNodes
						freeNodes = { int(removedConnections[testedNodes]) };
						//passedNodes++;
						testedNodes++;
						break;
					}

					// Unreachable if it finds a node that wasn't visited
					testedNodes++;
				}
			}
		}
	}

	return outGraphs;
}

// Utility function to add an edge
void Graph::m_AddEdge(std::map<uint32_t, std::vector<uint32_t>>& adj, int u, int v)
{
	adj[u].push_back(v);
	adj[v].push_back(u);
}