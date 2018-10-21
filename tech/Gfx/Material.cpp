/******************************************************************************
Copyright (c) 2018 Teardrop Games

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
******************************************************************************/


#include "Material.h"

#include "Gfx/Connection.h"
#include "Gfx/MaterialOutput.h"
#include "Util/Hash.h"

#include <algorithm>
#include <set>

using namespace Teardrop;
using namespace Gfx;

TD_CLASS_IMPL(Material);

Material::Material()
    : mHash(0)
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
	return true;
}

void Material::addConnection(Connection* conn)
{
	mConnections.insert(Connections::value_type(conn->output(), conn));

    // force a rehash next time someone asks (typically will force a new
    // shader creation)
    mHash = 0;
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
        for (typename NodeList::iterator it = mNodes.begin(); it != mNodes.end(); ++it) {
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
        for (typename EdgeList::iterator e = mEdges.begin(); e != mEdges.end(); ++e) {
			if (e->mFrom == node) {
				visit(e->mTo, sortedNodes, visited);
			}
		}

		sortedNodes.push_back(node);
	}
};

void Material::sortExpressions()
{
    // force a rehash after this
    mHash = 0;

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
	if (expressionCount()) 
        return mSortedExpressions.data();

	return nullptr;
}

size_t Material::expressionCount()
{
    return mSortedExpressions.size();
}

size_t Material::connectionCount()
{
    return mConnections.size();
}

size_t Material::connections(Connection** connections, size_t nConnections)
{
	if (connections == 0 || nConnections == 0)
        return mConnections.size();

	// otherwise, fill in the passed array
    size_t nConn = std::max(nConnections, mConnections.size());
	Connections::iterator it = mConnections.begin();
    for (size_t i=0; i<nConn; ++i) {
		connections[i] = it->second;
	}

	return nConn;
}

uint64_t Material::hash()
{
    if (mHash) {
        return mHash;
    }

    // otherwise, we need to crawl the DAG and hash anything that is not a dynamic input of
    // some sort (sampler inputs, things that come from uniform buffers, etc. -- anything that
    // is not defined statically by the material itself)
    MaterialExpression** expressions = sortedExpressions();
    if (expressions == nullptr) {
        sortExpressions();
        expressions = sortedExpressions();
    }

    if (expressions == nullptr) {
        return 0;
    }

    size_t exprCount = expressionCount();
    for (size_t i=0; i<exprCount; ++i) {
        MaterialExpression* expr = expressions[i];

        // each expression knows how to hash itself, and this method takes a seed value
        mHash = expr->hash(mHash);
    }

    return mHash;
}
