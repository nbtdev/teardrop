/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKGP_ABSTRACT_MESH_H
#define HKGP_ABSTRACT_MESH_H

#include <Common/Internal/GeometryProcessing/hkGeometryProcessing.h>
#include <Common/Internal/GeometryProcessing/Topology/hkgpTopology.h>

/// Abstract mesh definitions
struct hkgpAbstractMeshDefinitions
{
	/// Uid
	typedef hkgpTopology::Uid	Uid;	

	/// List : simple double linked list storage
	template <typename T,typename BASE,typename ALLOCATOR=hkGeometryProcessing::PoolAllocator<T,32> >
	struct List
	{
		/// Item
		struct Item : BASE
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_GEOMETRY,Item);
			HK_FORCE_INLINE T*			next() const	{ return(m_next); }
			HK_FORCE_INLINE T*&			next()			{ return(m_next); }
			HK_FORCE_INLINE T*			prev() const	{ return(m_prev); }
			HK_FORCE_INLINE T*&			prev()			{ return(m_prev); }
		public:
			Item() : BASE() {}
			Item&	operator=(const Item&) { return(*this); }
		protected:
			Item(const Item& other) : BASE(other) {}
		private:
			T*						m_next;		///
			T*						m_prev;		///
		};
								List() : m_used(0),m_numUsed(0)	{}
		inline					~List();
		inline void				reset();
		inline	T*				allocate(const T& value=T());
		inline void				release(T* item);
		inline void				moveUp(T* item);
		inline void				moveDown(T* item);
		HK_FORCE_INLINE T*		getFirst() const	{ return(m_used); }
		HK_FORCE_INLINE T*		getLast() const		{ T* t=getFirst();if(t) { while(t->next()) t=t->next(); } return(t); }
		HK_FORCE_INLINE int		getSize() const		{ return(m_numUsed); }		
	private:
		List(const List&) {}
		void operator=(const List&) {}
	private:
		ALLOCATOR				m_allocator;///
		T*						m_used;		///
		int						m_numUsed;	///
		hkUint32				m_uid;		///
	};

	/// Vertex
	template <typename VERTEX,typename BASE>
	struct Vertex : List<VERTEX,BASE>::Item
	{
		typedef typename List<VERTEX,BASE>::Item tBase;
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_GEOMETRY,Vertex);
		static HK_FORCE_INLINE bool isOrdered(const VERTEX* a,const VERTEX* b)
		{
			/* Slower but deterministic		*/ 
			const int c((*a).compare(*b));
			HK_ASSERT2(0x571567,c!=0,"Cannot find ordering");
			if(c<0) return(true);
			if(c>0) return(false);			
			return(true);
		}
	};

	/// Triangle
	template <typename TRIANGLE,typename BASE,typename VERTEX>
	struct Triangle : List<TRIANGLE,BASE>::Item
	{
		typedef typename List<TRIANGLE,BASE>::Item	tBase;
		typedef const VERTEX&						vParm;
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_GEOMETRY,Triangle);
		#if defined(HK_DEBUG)
		HK_FORCE_INLINE				Triangle()								{ m_links[0]=m_links[1]=m_links[2]=0;m_vertices[0]=m_vertices[1]=m_vertices[2]=0; }
		HK_FORCE_INLINE				~Triangle()								{	HK_ASSERT2(0x75542,!hasReferences(),"Triangle still referenced");
																				invalidate(); }
		HK_FORCE_INLINE void		invalidate()							{	m_links[0]=m_links[1]=m_links[2]=hkgpTopology::invalidUid();
																				m_vertices[0]=m_vertices[1]=m_vertices[2]=*(VERTEX*)&m_links[0]; }
		#endif
		HK_FORCE_INLINE void		setVertices(vParm a,vParm b,vParm c)	{ m_vertices[0]=a;m_vertices[1]=b;m_vertices[2]=c; }
		HK_FORCE_INLINE bool		hasReferences() const					{ return(0!=((link(0)|link(1)|link(2))>>2)); }
		HK_FORCE_INLINE VERTEX		vertex(int i) const						{ return(m_vertices[i]); }
		HK_FORCE_INLINE VERTEX&		vertex(int i)							{ return(m_vertices[i]); }
		HK_FORCE_INLINE Uid			link(int i) const						{ return(m_links[i]); }
		HK_FORCE_INLINE Uid&		link(int i)								{ return(m_links[i]); }
	private:
		VERTEX					m_vertices[3];	///
		Uid						m_links[3];		///
	};

	/// Tetrahedron
	template <typename TETREHEDRON,typename BASE,typename TRIANGLE>
	struct Tetrahedron : List<TETREHEDRON,BASE>::Item
	{
		typedef typename List<TETREHEDRON,BASE>::Item	tBase;		
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_GEOMETRY,Tetrahedron);

		HK_FORCE_INLINE	TRIANGLE	base() const							{ return m_base; }
		HK_FORCE_INLINE	TRIANGLE&	base()									{ return m_base; }
	private:
		TRIANGLE				m_base;			///
	};
	
	/// Topology check errors
	enum Error
	{
		VALID,				///
		ORPHAN_VERTEX,		///
		ZOMBIE_VERTEX,		///
		ZOMBIE_TRIANGLE,	///
		VOID_TRIANGLE,		///
		INVALID_BIND,		///
		SELF_EDGE,			///
		NULL_EDGE,			///
	};
};
	
/// Abstract mesh implementation
template <typename EDGE,typename VERTEX,typename TRIANGLE,typename ALLOCATOR=hkContainerHeapAllocator>
class hkgpAbstractMesh : public hkReferencedObject
{
public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_GEOMETRY);
	/// Types
	typedef ALLOCATOR							Allocator;
	typedef TRIANGLE							Triangle;
	typedef VERTEX								Vertex;
	typedef EDGE								Edge;
	typedef hkgpAbstractMesh					AbstractMesh;

	/// Vertex iterator
	struct VertexIterator
	{			
	private:
		friend class hkgpAbstractMesh;
		VertexIterator() {}
		VertexIterator(Vertex* first) : m_vertex(first) {}
		Vertex*	m_vertex;			
	public:
		VertexIterator&		operator++()			{ if(m_vertex) m_vertex=m_vertex->next(); return(*this); }
		VertexIterator		operator++(int)			{ const VertexIterator i=this;operator++();return(i); }
		Vertex*				operator()() const		{ return(m_vertex); }
		operator const VertexIterator*() const		{ return(m_vertex?this:0); }
	};

	/// Triangle iterator
	struct TriangleIterator
	{
	private:
		friend class hkgpAbstractMesh;
		TriangleIterator() {}
		TriangleIterator(Triangle* first) : m_triangle(first) {}
		Triangle*	m_triangle;			
	public:
		TriangleIterator&	operator++()			{ if(m_triangle) m_triangle=m_triangle->next(); return(*this); }
		TriangleIterator	operator++(int)			{ const TriangleIterator i=this;operator++();return(i); }
		Triangle*			operator()() const		{ return(m_triangle); }
		operator const TriangleIterator*() const	{ return(m_triangle?this:0); }
	};

	/// Edge iterator
	struct EdgeIterator
	{
	private:
		friend class hkgpAbstractMesh;
		EdgeIterator() {}
		EdgeIterator(Edge first) : m_edge(first)	{ if(m_edge.isValid() && !m_edge.isMaster()) next(); }
		Edge		m_edge;

	public:
		EdgeIterator&		operator++()			{ if(m_edge.isValid()) next(); return(*this); }
		EdgeIterator		operator++(int)			{ const EdgeIterator i=this;next();return(i); }
		Edge				operator()() const		{ return(m_edge); }
		operator const EdgeIterator*() const		{ return(m_edge.isValid()?this:0); }
	private:
		inline void			next();
	};
	//
	// Methods
	//
	
	/// Detructor
	virtual inline								~hkgpAbstractMesh();

	/// Reset and delete mesh features.
	virtual inline	void						reset();

	/// Check if the mesh does not containt any topological errors.
	inline bool									isValid() const;

	/// Check for topological errors and return the first one encountered if any.
	inline hkgpAbstractMeshDefinitions::Error	checkTopology(bool checkForOrphanVertices=true) const;

	/// Check if the mesh containt a reference to a given vertex.
	inline bool									hasReferenceTo(const VERTEX* vertex) const;

	/// Check if the mesh is a closed manifold.
	inline bool									isClosedManifold() const;

	/// Build a Reeb-Graph for this and a given scalar function.
	inline bool									buildReebGraph() const;

	/// Bind an edge.
	inline bool									bindEdge(EDGE edge) const;

	/// Find an edge.
	inline EDGE									findEdgeOrdered(const VERTEX* a,const VERTEX* b) const;

	/// Find an edge.
	inline EDGE									findEdgeUnordered(const VERTEX* a,const VERTEX* b) const;

	/// Find a triangle matching any combination of Vertices a,b,c.
	inline TRIANGLE*							findTriangleUnordered(const VERTEX* a,const VERTEX* b,const VERTEX* c) const;

	/// Get a vertex iterator.
	HK_FORCE_INLINE VertexIterator				getVertexIterator() const	{ return(VertexIterator(m_vertices.getFirst())); }

	/// Get a triangle iterator.
	HK_FORCE_INLINE TriangleIterator			getTriangleIterator() const { return(TriangleIterator(m_triangles.getFirst())); }

	/// Get an edge iterator.
	HK_FORCE_INLINE EdgeIterator				getEdgeIterator() const		{ return(EdgeIterator(EDGE(m_triangles.getFirst(),0))); }

	//
	// Typedefs
	//

	typedef hkGeometryProcessing::PoolAllocator<VERTEX,32,ALLOCATOR>									VertexPoolAllocator;
	typedef hkGeometryProcessing::PoolAllocator<TRIANGLE,32,ALLOCATOR>									TrianglePoolAllocator;
	
	typedef hkgpAbstractMeshDefinitions::List<VERTEX,typename VERTEX::tBase,VertexPoolAllocator>		VertexList;
	typedef hkgpAbstractMeshDefinitions::List<TRIANGLE,typename VERTEX::tBase,TrianglePoolAllocator>	TriangleList;

	//
	//	Fields
	//
	
	VertexList		m_vertices;		///
	TriangleList	m_triangles;	///
};

/// Abstract tetrahedral mesh implementation
template <typename EDGE,typename VERTEX,typename TRIANGLE,typename TETRAHEDRON,typename ALLOCATOR=hkContainerHeapAllocator>
class hkgpAbstractTetraMesh : public hkgpAbstractMesh<EDGE,VERTEX,TRIANGLE,ALLOCATOR>
{
public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_GEOMETRY);
	/// Types
	typedef TETRAHEDRON											Tetrahedron;
	typedef hkgpAbstractMesh<EDGE,VERTEX,TRIANGLE,ALLOCATOR>	Parent;
	typedef hkgpAbstractTetraMesh								AbstractTetraMesh;

	/// Tetrahedra iterator
	struct TetrahedraIterator
	{			
	private:
		friend class hkgpAbstractTetraMesh;
		TetrahedraIterator() {}
		TetrahedraIterator(Tetrahedron* first) : m_tetrahedron(first) {}
		Tetrahedron*	m_tetrahedron;
	public:
		TetrahedraIterator&		operator++()			{ if(m_tetrahedron) m_tetrahedron=m_tetrahedron->next(); return(*this); }
		TetrahedraIterator		operator++(int)			{ const TetrahedraIterator i=this;operator++();return(i); }
		Tetrahedron*			operator()() const		{ return(m_tetrahedron); }
		operator const TetrahedraIterator*() const		{ return(m_tetrahedron?this:0); }
	};

	//
	// Methods
	//
	
	/// Reset and delete mesh features.
	inline	void								reset();
	
	/// Get a tetrahedra iterator.
	HK_FORCE_INLINE TetrahedraIterator			getTetrahedraIterator() const	{ return(TetrahedraIterator(m_tetrahedra.getFirst())); }
	
	//
	// Typedefs
	//

	typedef hkGeometryProcessing::PoolAllocator<TETRAHEDRON,32,ALLOCATOR>										TetrahedronPoolAllocator;
	
	typedef hkgpAbstractMeshDefinitions::List<TETRAHEDRON,typename TETRAHEDRON::tBase,TetrahedronPoolAllocator>	TetrahedronList;

	//
	//	Fields
	//

	TetrahedronList	m_tetrahedra;	///
};

#include <Common/Internal/GeometryProcessing/AbstractMesh/hkgpAbstractMesh.inl>

#endif // HKGP_ABSTRACT_MESH_H


/*
* Havok SDK - NO SOURCE PC DOWNLOAD, BUILD(#20091222)
* 
* Confidential Information of Havok.  (C) Copyright 1999-2009
* Telekinesys Research Limited t/a Havok. All Rights Reserved. The Havok
* Logo, and the Havok buzzsaw logo are trademarks of Havok.  Title, ownership
* rights, and intellectual property rights in the Havok software remain in
* Havok and/or its suppliers.
* 
* Use of this software for evaluation purposes is subject to and indicates
* acceptance of the End User licence Agreement for this product. A copy of
* the license is included with this software and is also available at www.havok.com/tryhavok.
* 
*/
