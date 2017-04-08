/*
* Graph.hpp
*/
#ifndef GRAPH_H_
#define GRAPH_H_

#pragma once

#include <vector>
#include <queue>
#include <list>
#include <limits>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <unordered_map>
#include <cassert>
#include "node.hpp"


using namespace std;

const double SEARCH_RADIUS = 8.25;
const int NOT_VISITED = 0;
const int BEING_VISITED = 1;
const int DONE_VISITED = 2;
const int INT_INFINITY = INT_MAX;
const int WALK_SPEED = 1;
const int BUS_SPEED = 10;
const int METRO_SPEED = 20;
const int PIXEL_TO_METER = 8;



class Node;
class Edge;
class TransportLine;

typedef unordered_map<int, Node *> hashNodes;
typedef unordered_map<int, Edge *> hashEdges;



typedef struct {
	int path;
	int dist;
	char wayToGetThere;
} PathTo;



/* ================================================================================================
* Class Graph
* ================================================================================================
*/

class Graph {
	hashNodes nodeMap;
	hashEdges edgeMap;
	void dfs(Node *v, vector<int> &res) const;
	//exercicio 5
	int numCycles;
	void dfsVisit(Node *v);
	void dfsVisit();
	int highestEdgeId;
	vector<TransportLine*> transportationLines;

	//void getPathTo(Node *origin, list<int> &res);

public:
	Graph();
	~Graph();

	hashNodes* copyNodes();
	void copyEdges(hashNodes originalNodes);

	void setNodeMap(hashNodes *map);
	void setEdgeMap(hashEdges map);
	void setHighestEdgeId(int id);
	void setTransportationLines(vector<TransportLine*> tlVector);
	bool addNode(const int &in, Point coords);
	void addTransportationLine(TransportLine *t1,unordered_map<int, pair<int, int>> &edgeOD);
	bool addEdge(int id,const int &sourc, const int &dest);
	bool removeNode(const int &in);
	bool removeEdge(const int &sourc, const int &dest);
	
	hashNodes getNodeMap() const;
	Edge* getEdgeById(int id);

	Node* getNode(const int &v) const;

	vector<Node*> getSources() const;
	vector<PathTo> getPath(const int &origin, const int &dest);
	vector<Node *> getNodePath(const int &origin, const int &dest);
	vector<Node *> getCloseNodes(int max_dist, Node * n_source);
	vector<Edge *> getCloseEdges(const vector<Node*>& closeNodes, Node * n_source);

	void unweightedShortestPath(const int &v);

	void bellmanFordShortestPath(const int &v);

	bool checkWalkPercentage(const int &origin, const int &dest, float percentage);

	

	void dijkstraShortestDistance(const int & s);
	void dijkstraShortestDistance(const int & s, const int & d);
	void dijkstraBestTime(const int & s);
	void dijkstraBestTimeWithWaitingTime(const int &s);
	void dijkstraBestTimeWithFavoriteTransport(const int & s, char favorite);
	void dijkstraBestTimeWithFavoriteTransportAndWaitingTime(const int &s, char favorite);
	void dijkstraLessTransportsUsed(const int &s); //(Renato) Este � para sair, verificar s� se isto ajuda alguma coisa a implementar o menor custo

	void addEdgesFoot(vector<Edge*> & edges, vector<Edge *> & onFoot);
	bool alreadyExists(vector<Edge*> & edges, Edge * e);
	bool isChangingTransport(unordered_set<string> &edgeLines, unordered_set<string> vPathLines);
	

	void preprocessGraphForWaitingTimes();
	Graph * copy();

};


#endif /* GRAPH_H_ */
