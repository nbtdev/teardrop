/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/


#include "Material.h"
#include "MaterialOutput.h"
#include "FragmentShader.h"
#include "ShaderManager.h"
#include "Connection.h"
#include <set>

using namespace Teardrop;
using namespace Gfx;

TD_CLASS_IMPL(Material);

Material::Material()
	: mShader(0)
{
}

Material::~Material()
{
}

bool Material::initialize()
{
	return true;
}

bool Material::destroy()
{
	ShaderManager::instance().release(mShader);
	mShader = 0;

	return true;
}

FragmentShader* Material::shader()
{
	if (!mShader) {
		mShader = ShaderManager::instance().createOrFindInstanceOf(this);
		mShader->initialize();
	}

	return mShader;
}

void Material::addConnection(Connection* conn)
{
	mConnections.insert(Connections::value_type(conn->output(), conn));
}

void Material::apply()
{
	shader()->apply();
}

template<typename NodeType>
class DAG 
{
public:
	typedef NodeType* Node;
	typedef std::list<Node> NodeList;
	typedef std::vector<Node> NodeArray;

	struct Edge {
		Node mFrom;
		Node mTo;
	};

	typedef std::list<Edge> EdgeList;

	DAG() {}
	~DAG() {}

	void addNode(Node node) {
		mNodes.push_back(node);
	}

	void addEdge(Edge edge) {
		mEdges.push_back(edge);
	}

	void topologicalSort(NodeArray& sortedNodes) {
		sortedNodes.clear();
		sortedNodes.reserve(mNodes.size());

		std::set<Node> visited;
		for (NodeList::iterator it = mNodes.begin(); it != mNodes.end(); ++it) {
			visit(*it, sortedNodes, visited);
		}
	}

private:
	NodeList mNodes;
	EdgeList mEdges;

	void visit(Node node, NodeArray& sortedNodes, std::set<Node>& visited) {
		if (visited.find(node) != visited.end())
			return;

		visited.insert(node);

		// find the nodes on which we depend; that is, edges that have 'node'
		// as 'from' will have our dependencies as 'to'
		for (EdgeList::iterator e = mEdges.begin(); e != mEdges.end(); ++e) {
			if (e->mFrom == node) {
				visit(e->mTo, sortedNodes, visited);
			}
		}

		sortedNodes.push_back(node);
	}
};

void Material::sortExpressions()
{
	std::set<MaterialExpression*> added;
	DAG<MaterialExpression> dag;

	for (Connections::iterator it = mConnections.begin(); it != mConnections.end(); ++it) {
		MaterialExpression* from = it->second->getFromExpression();
		if (added.find(from) == added.end()) {
			dag.addNode(from);
			added.insert(from);
		}

		MaterialExpression* to = it->second->getToExpression();
		if (added.find(to) == added.end()) {
			dag.addNode(to);
			added.insert(to);
		}

		// NOTE: "from" and "to" in the expressions denote data flow, not dependency specification,
		// so in terms of dependency, they are reversed
		DAG<MaterialExpression>::Edge edge;
		edge.mFrom = to;
		edge.mTo = from;
		dag.addEdge(edge);
	}

	dag.topologicalSort(mSortedExpressions);
}

MaterialExpression** Material::sortedExpressions()
{
	return &mSortedExpressions[0];
}

int Material::expressionCount()
{
	return int(mSortedExpressions.size());
}

int Material::connections(Connection** connections, int nConnections)
{
	if (connections == 0 || nConnections == 0)
		return int(mConnections.size());

	// otherwise, fill in the passed array
	int nConn = std::max(nConnections, int(mConnections.size()));
	Connections::iterator it = mConnections.begin();
	for (int i=0; i<nConn; ++i) {
		connections[i] = it->second;
	}

	return nConn;
}