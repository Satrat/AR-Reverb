#include <float.h>
#include <assert.h>
#include "meshEdit.h"
#include "mutablePriorityQueue.h"

namespace CMU462 {

VertexIter HalfedgeMesh::splitEdge(EdgeIter e0) {

  // TODO: (meshEdit)
  // This method should split the given edge and return an iterator to the
  // newly inserted vertex. The halfedge of this vertex should point along
  // the edge that was split, rather than the new edges.

      // first handle boundary case
      if( e0->isBoundary() )
      {
         // get current elements
         HalfedgeIter h5 = e0->halfedge(); if( h5->isBoundary() ) h5 = h5->twin();
         HalfedgeIter h4 = h5->next();
         HalfedgeIter h1 = h4->next();
         HalfedgeIter hb = h5->twin();
         VertexIter v1 = h1->vertex();
         VertexIter v3 = h5->vertex();
         VertexIter v4 = h4->vertex();
         EdgeIter e1 = h1->edge();
         EdgeIter e4 = h4->edge();
         FaceIter f1 = h1->face();
         FaceIter fb = hb->face();

         // get previous and next half edge along the boundary loop
         HalfedgeIter hbp = hb;
         do
         {
            hbp = hbp->twin()->next();
         }
         while( hbp->twin()->next() != hb );
         hbp = hbp->twin();
         HalfedgeIter hbn = hb->next();

         // allocate new elements
         VertexIter v5 = newVertex();
         EdgeIter e5 = newEdge();
         EdgeIter e7 = newEdge();
         EdgeIter e8 = newEdge();
         FaceIter f3 = newFace();
         FaceIter f6 = newFace();
         HalfedgeIter h8  = newHalfedge();
         HalfedgeIter h9  = newHalfedge();
         HalfedgeIter h11 = newHalfedge();
         HalfedgeIter h13 = newHalfedge();
         HalfedgeIter hb1 = newHalfedge();
         HalfedgeIter hb2 = newHalfedge();

         // set new vertex location
         v5->position = ( v3->position + v4->position ) / 2.;

         // connect new elements
         h1->setNeighbors( h11, h1->twin(), v1, e1, f3 );
         h4->setNeighbors( h9,  h4->twin(), v4, e4, f6 );
         h8->setNeighbors( h4,  hb1,      v5, e8, f6 );
         h9->setNeighbors( h8,  h13,      v1, e5, f6 );
         h11->setNeighbors( h13, hb2,     v3, e7, f3 );
         h13->setNeighbors( h1,  h9,      v5, e5, f3 );
         hb1->setNeighbors( hb2, h8,  v4, e8, fb );
         hb2->setNeighbors( hbn, h11, v5, e7, fb );
         hbp->next() = hb1;
         v1->halfedge() = h1;
         v3->halfedge() = h11;
         v4->halfedge() = h4;
         v5->halfedge() = h13;
         e1->halfedge() = h1;
         e4->halfedge() = h4;
         e5->halfedge() = h9;
         e7->halfedge() = h11;
         e8->halfedge() = h8;
         f3->halfedge() = h1;
         f6->halfedge() = h4;
         fb->halfedge() = hb1;

         // deallocate old elements
         deleteEdge( e0 );
         deleteFace( f1 );
         deleteHalfedge( h5 );
         deleteHalfedge( hb );

         return v5;
      }
      else
      {
         // get current elements
         HalfedgeIter h5 = e0->halfedge();
         HalfedgeIter h6 = h5->twin();
         HalfedgeIter h4 = h5->next();
         HalfedgeIter h1 = h4->next();
         HalfedgeIter h3 = h6->next();
         HalfedgeIter h2 = h3->next();
         VertexIter v1 = h1->vertex();
         VertexIter v2 = h2->vertex();
         VertexIter v3 = h3->vertex();
         VertexIter v4 = h4->vertex();
         EdgeIter e1 = h1->edge();
         EdgeIter e2 = h2->edge();
         EdgeIter e3 = h3->edge();
         EdgeIter e4 = h4->edge();
         FaceIter f1 = h1->face();
         FaceIter f2 = h2->face();

         // allocate new elements
         VertexIter v5 = newVertex();
         EdgeIter e5 = newEdge();
         EdgeIter e6 = newEdge();
         EdgeIter e7 = newEdge();
         EdgeIter e8 = newEdge();
         FaceIter f3 = newFace();
         FaceIter f4 = newFace();
         FaceIter f5 = newFace();
         FaceIter f6 = newFace();
         HalfedgeIter h7  = newHalfedge();
         HalfedgeIter h8  = newHalfedge();
         HalfedgeIter h9  = newHalfedge();
         HalfedgeIter h10 = newHalfedge();
         HalfedgeIter h11 = newHalfedge();
         HalfedgeIter h12 = newHalfedge();
         HalfedgeIter h13 = newHalfedge();
         HalfedgeIter h14 = newHalfedge();

         // set new vertex location
         v5->position = ( v3->position + v4->position ) / 2.;

         // connect new elements
         h1->setNeighbors( h11, h1->twin(), v1, e1, f3 );
         h2->setNeighbors( h12, h2->twin(), v2, e2, f4 );
         h3->setNeighbors( h10, h3->twin(), v3, e3, f5 );
         h4->setNeighbors( h9,  h4->twin(), v4, e4, f6 );
         h7->setNeighbors( h3,  h11,      v5, e7, f5 );
         h8->setNeighbors( h4,  h12,      v5, e8, f6 );
         h9->setNeighbors( h8,  h13,      v1, e5, f6 );
         h10->setNeighbors( h7,  h14,      v2, e6, f5 );
         h11->setNeighbors( h13, h7,       v3, e7, f3 );
         h12->setNeighbors( h14, h8,       v4, e8, f4 );
         h13->setNeighbors( h1,  h9,       v5, e5, f3 );
         h14->setNeighbors( h2,  h10,      v5, e6, f4 );
         v1->halfedge() = h1;
         v2->halfedge() = h2;
         v3->halfedge() = h3;
         v4->halfedge() = h4;
         v5->halfedge() = h7;
         e1->halfedge() = h1;
         e2->halfedge() = h2;
         e3->halfedge() = h3;
         e4->halfedge() = h4;
         e5->halfedge() = h9;
         e6->halfedge() = h10;
         e7->halfedge() = h11;
         e8->halfedge() = h12;
         f3->halfedge() = h1;
         f4->halfedge() = h2;
         f5->halfedge() = h3;
         f6->halfedge() = h4;

         // deallocate old elements
         deleteEdge( e0 );
         deleteFace( f1 );
         deleteFace( f2 );
         deleteHalfedge( h5 );
         deleteHalfedge( h6 );

         return v5;
      }

}

VertexIter HalfedgeMesh::collapseEdge(EdgeIter e) {

  // TODO: (meshEdit)
  // This method should collapse the given edge and return an iterator to
  // the new vertex created by the collapse.

      HalfedgeIter he; // dummy iterator

      // handle boundary case first
      if( e->isBoundary() )
      {
         // get pointers to the original geometry
         HalfedgeIter h0 = e->halfedge(); if( h0->isBoundary() ) h0 = h0->twin();
         HalfedgeIter h1 = h0->next();
         HalfedgeIter h2 = h1->next();
         HalfedgeIter h1f = h1->twin();
         HalfedgeIter h2f = h2->twin();
         HalfedgeIter hb = h0->twin();
         HalfedgeIter hbn = hb->next();
         HalfedgeIter hbp = hb; do { hbp = hbp->twin()->next(); } while( hbp->twin()->next() != hb ); hbp = hbp->twin();
         VertexIter v0 = h2->vertex();
         VertexIter v1 = h0->vertex();
         VertexIter v2 = h1->vertex();
         EdgeIter e1 = h1->edge();
         EdgeIter e2 = h2->edge();
         FaceIter f = h0->face();
         FaceIter fb = hb->face();

         // create new mesh elements
         VertexIter v = newVertex();
         EdgeIter e0 = newEdge();

         // link together elements
         // vertices
         v0->halfedge() = h1f;
         v->halfedge() = hbn;
         v->position = ( v1->position + v2->position ) / 2.;

         // edges
         e0->halfedge() = h1f;
         // faces
         fb->halfedge() = hbn;
         // halfedges
         he = v1->halfedge(); do { he->vertex() = v; he = he->twin()->next(); } while( he != v1->halfedge() );
         he = v2->halfedge(); do { he->vertex() = v; he = he->twin()->next(); } while( he != v2->halfedge() );
         h1f->twin() = h2f;
         h2f->twin() = h1f;
         h1f->edge() = h2f->edge() = e0;
         hbp->next() = hbn;

         // remove old elements
         deleteVertex( v1 );
         deleteVertex( v2 );
         deleteEdge( e );
         deleteEdge( e1 );
         deleteEdge( e2 );
         deleteFace( f );
         deleteHalfedge( h0 );
         deleteHalfedge( h1 );
         deleteHalfedge( h2 );
         deleteHalfedge( hb );

         return v;
      }
      else
      {
         // get fixed pointers to the original geometry
         HalfedgeIter h0 = e->halfedge();
         HalfedgeIter h1 = h0->twin();
         HalfedgeIter h00 = h0->next(), h01 = h00->next();
         HalfedgeIter h10 = h1->next(), h11 = h10->next();
         HalfedgeIter h00f = h00->twin();
         HalfedgeIter h01f = h01->twin();
         HalfedgeIter h10f = h10->twin();
         HalfedgeIter h11f = h11->twin();
         VertexIter v0 = h0->vertex();
         VertexIter v1 = h1->vertex();
         VertexIter v01 = h01->vertex();
         VertexIter v11 = h11->vertex();
         EdgeIter e00 = h00->edge();
         EdgeIter e01 = h01->edge();
         EdgeIter e10 = h10->edge();
         EdgeIter e11 = h11->edge();
         FaceIter f0 = h0->face();
         FaceIter f1 = h1->face();
         bool v0b = v0->isBoundary();
         bool v1b = v1->isBoundary();

         // check that the intersection of the 1-ring neighborhoods of the two
         // edge endpoints contains only the two vertices opposite the edge
         set<VertexCIter> neighbors;
         size_t n = 0;
         he = v0->halfedge(); do { n++; neighbors.insert( he->twin()->vertex() ); he = he->twin()->next(); } while( he != v0->halfedge() );
         he = v1->halfedge(); do { n++; neighbors.insert( he->twin()->vertex() ); he = he->twin()->next(); } while( he != v1->halfedge() );
         if( n - neighbors.size() != 2 )
         {
            return v0;
         }

         // create new mesh elements
         EdgeIter e0 = newEdge();
         EdgeIter e1 = newEdge();
         VertexIter v = newVertex();

         // link old elements to new ones
         // vertices
         v->halfedge() = h01f;
         // edges
         e0->halfedge() = h01f;
         e1->halfedge() = h11f;
         // faces
         // (nothing to do)
         // halfedges
         he = v0->halfedge(); do { he->vertex() = v; he = he->twin()->next(); } while( he != v0->halfedge() );
         he = v1->halfedge(); do { he->vertex() = v; he = he->twin()->next(); } while( he != v1->halfedge() );
         v01->halfedge() = h00f;
         v11->halfedge() = h10f;
         h00f->edge() = e0;
         h01f->edge() = e0;
         h10f->edge() = e1;
         h11f->edge() = e1;
         h00f->twin() = h01f;
         h01f->twin() = h00f;
         h10f->twin() = h11f;
         h11f->twin() = h10f;

         // if both vertices were on the bounday, put new vertex
         // at average of neighbors; otherwise, put it at the boundary
         if( v0b )
         {
            v->position = v0->position;
         }
         else if( v1b )
         {
            v->position = v1->position;
         }
         else
         {
            v->position = (v0->position+v1->position)/2.;
         }

         // remove old mesh elements
         deleteVertex( v0 );
         deleteVertex( v1 );
         deleteEdge( e );
         deleteEdge( e00 );
         deleteEdge( e01 );
         deleteEdge( e10 );
         deleteEdge( e11 );
         deleteFace( f0 );
         deleteFace( f1 );
         deleteHalfedge( h0 );
         deleteHalfedge( h00 );
         deleteHalfedge( h01 );
         deleteHalfedge( h1 );
         deleteHalfedge( h10 );
         deleteHalfedge( h11 );

         return v;
      }

}

FaceIter HalfedgeMesh::eraseVertex(VertexIter v)
{
   // Collect the faces, edges, and halfedges
   // around v that will ultimately be deleted
   vector<FaceIter> deadFaces;
   vector<EdgeIter> deadEdges;
   vector<HalfedgeIter> deadHalfedges;
   HalfedgeIter h = v->halfedge();
   do
   {
      deadFaces.push_back( h->face() );
      deadEdges.push_back( h->edge() );
      deadHalfedges.push_back( h );
      deadHalfedges.push_back( h->twin() );

      h = h->twin()->next();
   }
   while( h != v->halfedge() );

   // Create a new face, that will effectively replace this whole vertex neighborhood
   FaceIter f = newFace();
   f->halfedge() = h->next();

   // Walk again around the vertex, this time doing three things:
   //    1. linking the "outer" halfedges together in a loop
   //    2. pointing these halfedges to the new face f
   //    3. pointing the vertices of these halfedges back at these halfdges (in case they point to a halfedge that will be deleted)
   do
   {
      HalfedgeIter i = h->next();

      i->vertex()->halfedge() = i;

      while( i->next()->next() != h )
      {
         i->face() = f;
         i = i->next();
      }
      i->next() = i->next()->twin()->next();

      h = h->twin()->next();
   }
   while( h != v->halfedge() );

   // Finally, delete all the elements that are no longer part of the mesh
   for( int i = 0; i < deadFaces.size(); i++ ) deleteFace( deadFaces[i] );
   for( int i = 0; i < deadEdges.size(); i++ ) deleteEdge( deadEdges[i] );
   for( int i = 0; i < deadHalfedges.size(); i++ ) deleteHalfedge( deadHalfedges[i] );

   // Return the new face
   return f;
}

FaceIter HalfedgeMesh::eraseEdge( EdgeIter e )
{
  // TODO: (meshEdit)
  // This method should erase the given edge and return an iterator to the
  // merged face.
   
   // skip boundary edges
   if( e->isBoundary() ) return e->halfedge()->face();

   HalfedgeIter h0 = e->halfedge(); // will be deleted
   HalfedgeIter h1 = h0->twin(); // will be deleted

   VertexIter v0 = h0->vertex();
   VertexIter v1 = h1->vertex();

   FaceIter f0 = h0->face();
   FaceIter f1 = h1->face(); // will be deleted

   // Find the halfedges H0, H1 preceding h0, h1
   HalfedgeIter H0 = h0; do { H0 = H0->next(); } while( H0->next() != h0 );
   HalfedgeIter H1 = h1; do { H1 = H1->next(); } while( H1->next() != h1 );

   H0->next() = h1->next();
   H1->next() = h0->next();

   v0->halfedge() = h1->next();
   v1->halfedge() = h0->next();

   HalfedgeIter h = h0->next();
   do
   {
      h->face() = f0;
      h = h->next();
   }
   while( h != h0->next() );

   f0->halfedge() = h0->next();

   deleteEdge( e );
   deleteHalfedge( h0 );
   deleteHalfedge( h1 );
   deleteFace( f1 );

   return f0;
}

EdgeIter HalfedgeMesh::flipEdge(EdgeIter e0) {

  // TODO: (meshEdit)
  // This method should flip the given edge and return an iterator to the
  // flipped edge.

      // skip boundary edges
      if( e0->isBoundary() ) return e0;

      // get current elements
      HalfedgeIter h1 = e0->halfedge();
      HalfedgeIter h2 = h1->next();
      HalfedgeIter h3 = h2->next();
      HalfedgeIter h4 = h1->twin();
      HalfedgeIter h5 = h4->next();
      HalfedgeIter h6 = h5->next();
      VertexIter v0 = h4->vertex();
      VertexIter v1 = h1->vertex();
      VertexIter v2 = h3->vertex();
      VertexIter v3 = h6->vertex();
      FaceIter f0 = h1->face();
      FaceIter f1 = h4->face();

      // recconnect current elements
      h1->setNeighbors( h6, h4, v2, e0, f0 );
      h2->setNeighbors( h1, h2->twin(), v0, h2->edge(), f0 );
      h3->setNeighbors( h5, h3->twin(), v2, h3->edge(), f1 );
      h4->setNeighbors( h3, h1, v3, e0, f1 );
      h5->setNeighbors( h4, h5->twin(), v1, h5->edge(), f1 );
      h6->setNeighbors( h2, h6->twin(), v3, h6->edge(), f0 );
      v0->halfedge() = h2;
      v1->halfedge() = h5;
      v2->halfedge() = h1;
      v3->halfedge() = h4;
      e0->halfedge() = h1;
      f0->halfedge() = h1;
      f1->halfedge() = h4;

      return e0;

}

void HalfedgeMesh::subdivideQuad( bool useCatmullClark )
{
   // Unlike the local mesh operations (like bevel or edge flip), we will perform
   // subdivision by splitting *all* faces into quads "simultaneously."  Rather
   // than operating directly on the halfedge data structure (which as you've seen
   // is quite difficult to maintain!) we are going to do something a bit nicer:
   //
   //    1. Create a raw list of vertex positions and faces (rather than a full-
   //       blown halfedge mesh).
   //
   //    2. Build a new halfedge mesh from these lists, replacing the old one.
   //
   // Sometimes rebuilding a data structure from scratch is simpler (and even more
   // efficient) than incrementally modifying the existing one.  These steps are
   // detailed below.

   // TODO Step I: Compute the vertex positions for the subdivided mesh.  Here we're
   // going to do something a little bit strange: since we will have one vertex in
   // the subdivided mesh for each vertex, edge, and face in the original mesh, we
   // can nicely store the new vertex *positions* as attributes on vertices, edges,
   // and faces of the original mesh.  These positions can then be conveniently
   // copied into the new, subdivided mesh.
   // [See subroutines for actual "TODO"s]
   if( useCatmullClark )
   {
      computeCatmullClarkPositions();
   }
   else
   {
      computeLinearSubdivisionPositions();
   }
   
   // TODO Step II: Assign a unique index (starting at 0) to each vertex, edge, and
   // face in the original mesh.  These indices will be the indices of the vertices
   // in the new (subdivided mesh).  They do not have to be assigned in any particular
   // order, so long as no index is shared by more than one mesh element, and the
   // total number of indices is equal to V+E+F, i.e., the total number of vertices
   // plus edges plus faces in the original mesh.  Basically we just need a one-to-one
   // mapping between original mesh elements and subdivided mesh vertices.
   // [See subroutine for actual "TODO"s]
   assignSubdivisionIndices();

   // TODO Step III: Build a list of quads in the new (subdivided) mesh, as tuples of
   // the element indices defined above.  In other words, each new quad should be of
   // the form (i,j,k,l), where i,j,k and l are four of the indices stored on our
   // original mesh elements.  Note that it is essential to get the orientation right
   // here: (i,j,k,l) is not the same as (l,k,j,i).  Indices of new faces should
   // circulate in the same direction as old faces (think about the right-hand rule).
   // [See subroutines for actual "TODO"s]
   vector< vector<Index> > subDFaces;
   vector< Vector3D > subDVertices;
   buildSubdivisionFaceList( subDFaces );
   buildSubdivisionVertexList( subDVertices );

   // TODO Step IV: Pass the list of vertices and quads to a routine that clears the
   // internal data for this halfedge mesh, and builds new halfedge data from scratch,
   // using the two lists.
   rebuild( subDFaces, subDVertices );
}

/**
 * Compute new vertex positions for a mesh that splits each polygon
 * into quads (by inserting a vertex at the face midpoint and each
 * of the edge midpoints).  The new vertex positions will be stored
 * in the members Vertex::newPosition, Edge::newPosition, and
 * Face::newPosition.  The values of the positions are based on
 * simple linear interpolation, e.g., the edge midpoints and face
 * centroids.
 */
void HalfedgeMesh::computeLinearSubdivisionPositions()
{
   // TODO For each vertex, assign Vertex::newPosition to
   // its original position, Vertex::position.
   for( VertexIter v = vertices.begin(); v != vertices.end(); v++ )
   {
      v->newPosition = v->position;
   }

   // TODO For each edge, assign the midpoint of the two original
   // positions to Edge::newPosition.
   for( EdgeIter e = edges.begin(); e != edges.end(); e++ )
   {
      Vector3D pi = e->halfedge()->vertex()->position;
      Vector3D pj = e->halfedge()->twin()->vertex()->position;
      e->newPosition = ( pi + pj ) / 2.;
   }

   // TODO For each face, assign the centroid (i.e., arithmetic mean)
   // of the original vertex positions to Face::newPosition.  Note
   // that in general, NOT all faces will be triangles!
   for( FaceIter f = faces.begin(); f != faces.end(); f++ )
   {
      Vector3D sum( 0., 0., 0. );
      double degree = 0.;

      HalfedgeIter h = f->halfedge();
      do
      {
         sum += h->vertex()->position;
         degree += 1.;

         h = h->next();
      }
      while( h != f->halfedge() );

      f->newPosition = sum / degree;
   }
}

/**
 * Compute new vertex positions for a mesh that splits each polygon
 * into quads (by inserting a vertex at the face midpoint and each
 * of the edge midpoints).  The new vertex positions will be stored
 * in the members Vertex::newPosition, Edge::newPosition, and
 * Face::newPosition.  The values of the positions are based on
 * the Catmull-Clark rules for subdivision.
 */
void HalfedgeMesh::computeCatmullClarkPositions()
{
   // TODO The implementation for this routine should be
   // a lot like HalfedgeMesh::computeLinearSubdivisionPositions(),
   // except that the calculation of the positions themsevles is
   // slightly more involved, using the Catmull-Clark subdivision
   // rules. XXX Need to carefully specify rules...

   // TODO face
   for( FaceIter f = faces.begin(); f != faces.end(); f++ )
   {
      Vector3D sum( 0., 0., 0. );
      double degree = 0.;

      HalfedgeIter h = f->halfedge();
      do
      {
         sum += h->vertex()->position;
         degree += 1.;

         h = h->next();
      }
      while( h != f->halfedge() );

      f->newPosition = sum / degree;
   }

   // TODO edges
   for( EdgeIter e = edges.begin(); e != edges.end(); e++ )
   {
      Vector3D pi = e->halfedge()->vertex()->position;
      Vector3D pj = e->halfedge()->twin()->vertex()->position;
      Vector3D ci = e->halfedge()->face()->newPosition;
      Vector3D cj = e->halfedge()->twin()->face()->newPosition;
      e->newPosition = ( pi + pj + ci +  cj ) / 4.;
   }
   
   // TODO vertices
   for( VertexIter v = vertices.begin(); v != vertices.end(); v++ )
   {
      Vector3D Q( 0., 0., 0. );
      Vector3D R( 0., 0., 0. );
      Vector3D S = v->position;

      double n = 0.;
      HalfedgeIter h = v->halfedge();
      do
      {
         Q += h->face()->newPosition;
         R += h->edge()->newPosition;
         n += 1.;
            
         h = h->twin()->next();
      }
      while( h != v->halfedge() );
      
      Q /= n;
      R /= n;

      v->newPosition = (Q/n) + (2.*R/n) + (S*(n-3.)/n);
   }
}

/**
 * Assign a unique integer index to each vertex, edge, and face in
 * the mesh, starting at 0 and incrementing by 1 for each element.
 * These indices will be used as the vertex indices for a mesh
 * subdivided using Catmull-Clark (or linear) subdivision.
 */
void HalfedgeMesh::assignSubdivisionIndices()
{
   // TODO Start a counter at zero; if you like, you can use the
   // "Index" type (defined in halfedgeMesh.h)
   Index index = 0;

   // TODO Iterate over vertices, assigning values to Vertex::index
   for( VertexIter v = vertices.begin(); v != vertices.end(); v++ )
   {
      v->index = index;
      index++;
   }
   
   // TODO Iterate over edges, assigning values to Edge::index
   for( EdgeIter e = edges.begin(); e != edges.end(); e++ )
   {
      e->index = index;
      index++;
   }
   
   // TODO Iterate over faces, assigning values to Face::index
   for( FaceIter f = faces.begin(); f != faces.end(); f++ )
   {
      f->index = index;
      index++;
   }
}

/**
 * Build a flat list containing all the vertex positions for a
 * Catmull-Clark (or linear) subdivison of this mesh.  The order of
 * vertex positions in this list must be identical to the order
 * of indices assigned to Vertex::newPosition, Edge::newPosition,
 * and Face::newPosition.
 */
void HalfedgeMesh::buildSubdivisionVertexList( vector<Vector3D>& subDVertices )
{
   // TODO Resize the vertex list so that it can hold all the vertices.
   int nV = vertices.size();
   int nE = edges.size();
   int nF = faces.size();
   subDVertices.resize( nV + nE + nF );
   
   // TODO Iterate over vertices, assigning Vertex::newPosition to the appropriate
   // location in the new vertex list.
   for( VertexIter v = vertices.begin(); v != vertices.end(); v++ )
   {
      subDVertices[ v->index ] = v->newPosition;
   }
   
   // TODO Iterate over edges, assigning Edge::newPosition to the appropriate
   // location in the new vertex list.
   for( EdgeIter e = edges.begin(); e != edges.end(); e++ )
   {
      subDVertices[ e->index ] = e->newPosition;
   }
   
   // TODO Iterate over faces, assigning Face::newPosition to the appropriate
   // location in the new vertex list.
   for( FaceIter f = faces.begin(); f != faces.end(); f++ )
   {
      subDVertices[ f->index ] = f->newPosition;
   }
}

/**
 * Build a flat list containing all the quads in a Catmull-Clark
 * (or linear) subdivision of this mesh.  Each quad is specified
 * by a vector of four indices (i,j,k,l), which come from the
 * members Vertex::index, Edge::index, and Face::index.  Note that
 * the ordering of these indices is important because it determines
 * the orientation of the new quads; it is also important to avoid
 * "bowties."  For instance, (l,k,j,i) has the opposite orientation
 * of (i,j,k,l), and if (i,j,k,l) is a proper quad, then (i,k,j,l)
 * will look like a bowtie.
 */
void HalfedgeMesh::buildSubdivisionFaceList( vector< vector<Index> >& subDFaces )
{
   // TODO This routine is perhaps the most tricky step in the construction of
   // a subdivision mesh (second, perhaps, to computing the actual Catmull-Clark
   // vertex positions).  Basically what you want to do is iterate over faces,
   // then for each for each face, append N quads to the list (where N is the
   // degree of the face).  For this routine, it may be more convenient to simply
   // append quads to the end of the list (rather than allocating it ahead of
   // time), though YMMV.  You can of course iterate around a face by starting
   // with its first halfedge and following the "next" pointer until you get
   // back to the beginning.  The tricky part is making sure you grab the right
   // indices in the right order---remember that there are indices on vertices,
   // edges, AND faces of the original mesh.  All of these should get used.  Also
   // remember that you must have FOUR indices per face, since you are making a
   // QUAD mesh!

   // TODO iterate over faces
   for( Face f : faces )
   {
      Index i = f.index;

      // TODO loop around face
      HalfedgeIter h = f.halfedge();
      do
      {
         Index j = h->edge()->index;
         Index k = h->next()->vertex()->index;
         Index l = h->next()->edge()->index;

         // TODO build lists of four indices for each sub-quad
         vector<Index> quad( 4 );
         quad[0] = i;
         quad[1] = j;
         quad[2] = k;
         quad[3] = l;
            
         // TODO append each list of four indices to face list
         subDFaces.push_back( quad );

         h = h->next();
      }
      while( h != f.halfedge() );
   }
}

bool HalfedgeMesh::zero_length_edge(HalfedgeIter h) {
  Vector3D u = h->vertex()->position;
  Vector3D v = h->twin()->vertex()->position;
  Vector3D w = u-v;
  if (w.norm2() < 0.0001) return true;
  else return false;
}

bool HalfedgeMesh::zero_length_edge(VertexIter u, VertexIter v) {
  Vector3D _u = u->position;
  Vector3D _v = v->position;
  Vector3D w = _u-_v;
  if (w.norm2() < 0.0001) return true;
  else return false;
}

void HalfedgeMesh::_bevel_fc_reposition_with_dist(vector<Vector3D>& orig, vector<HalfedgeIter>& hs, double shift, double inset )
{
   int N = hs.size();

   for( int i = 0; i < N; i++ )
   {
      Vector3D p0 = orig[(i+0)%N];
      Vector3D p1 = orig[(i+1)%N];
      Vector3D p2 = orig[(i+2)%N];

      Vector3D u = ( p0-p1 ).unit();
      Vector3D v = ( p2-p1 ).unit();
      Vector3D n = cross( u, v ).unit();

      Vector3D w = shift*n - inset*( u+v ).unit();

      hs[(i+2)%N]->vertex()->position += w;
   }


  // if (hs.size() == 0) return;
  // for (int i=0; i<hs.size(); i++) {
  //   if (i == 0) {
  //     if (zero_length_edge(hs[hs.size()-1]->vertex(), hs[i]->vertex()) 
  //       && inset < 0) { continue; }
  //   } else {
  //     if (zero_length_edge(hs[i]->vertex(), hs[i-1]->vertex()) 
  //       && inset < 0) { continue; }
  //   }
  // }
  // for (auto h : hs) {
  //   VertexIter v0 = h->vertex();
  //   VertexIter v1 = h->twin()->vertex();
  //   Vector3D v0candidate = v0->position+changeInDir(orig, v1->position, inset);
  //   Vector3D orig_vec = v1->position - orig;
  //   Vector3D cand_vec = v1->position - v0candidate;
  //   if (orig_vec.norm() - cand_vec.norm() < 0.001) {
  //     v0candidate = orig;
  //   }
  //   // TODO max size limit
  //   v0->position = v0candidate;
  // }
}

void HalfedgeMesh::_bevel_vtx_reposition_with_dist(Vector3D orig, vector<HalfedgeIter>& hs, double inset) {
  if (hs.size() == 0) return;
  for (int i=0; i<hs.size(); i++) {
    // check for upperbound
    if (zero_length_edge(hs[i]) && inset > 0) { continue; }
    // check for lowerboud
    if (i == 0) {
      if (zero_length_edge(hs[hs.size()-1]->vertex(), hs[i]->vertex()) 
        && inset < 0) { continue; }
    } else {
      if (zero_length_edge(hs[i]->vertex(), hs[i-1]->vertex()) 
        && inset < 0) { continue; }
    }
  }
  for (auto h : hs) { // recall: h points away from original vertex
    VertexIter v0 = h->vertex();
    VertexIter v1 = h->twin()->vertex();
    Vector3D v0candidate = v0->position+changeInDir(v0->position, v1->position, inset);
    // Vector3D orig_vec = v1->position - v0->position;
    Vector3D orig_vec = v1->position - orig;
    Vector3D cand_vec = v1->position - v0candidate;
    if (orig_vec.norm() - cand_vec.norm() < 0.001) {
      v0candidate = orig;
    }
    if (orig_vec.x * cand_vec.x < 0
      || orig_vec.y * cand_vec.y < 0
      || orig_vec.z * cand_vec.z < 0) {
      v0candidate = v1->position - (0.001 * orig_vec);
    }
    v0->position = v0candidate;
  }
}

void HalfedgeMesh::_bevel_edge_reposition_with_dist(vector<Vector3D>& origs, 
                                                    vector<HalfedgeIter>& hs, 
                                                    double inset) {
  if (hs.size() == 0) return;
  for (int i=0; i<hs.size(); i++) {
    // check for upperbound
    if (zero_length_edge(hs[i]) && inset > 0) { continue; }
    // check for lowerboud
    if (i == 0) {
      if (zero_length_edge(hs[hs.size()-1]->vertex(), hs[i]->vertex()) 
        && inset < 0) { continue; }
    } else {
      if (zero_length_edge(hs[i]->vertex(), hs[i-1]->vertex()) 
        && inset < 0) { continue; }
    }
  }
  for (int i=0; i<hs.size(); i++) {
    VertexIter v0 = hs[i]->vertex();
    VertexIter v1 = hs[i]->twin()->vertex();
    Vector3D orig = origs[i];

    Vector3D v0candidate = v0->position+changeInDir(orig, v1->position, inset);

    Vector3D orig_vec = v1->position - orig;
    Vector3D cand_vec = v1->position - v0candidate;
    if (orig_vec.norm() - cand_vec.norm() < 0.001) {
      v0candidate = orig;
    }
    if (orig_vec.x * cand_vec.x < 0
      || orig_vec.y * cand_vec.y < 0
      || orig_vec.z * cand_vec.z < 0) {
      v0candidate = v1->position - (0.001 * orig_vec);
    }
    v0->position = v0candidate;
  }
}

Vector3D HalfedgeMesh::changeInDir(Vector3D vstart, Vector3D vend, double len) {
  Vector3D v = vend-vstart;
  v.normalize();
  return len*v;
}

FaceIter HalfedgeMesh::bevelVertex(VertexIter v) {
  int deg = 0;
  HalfedgeIter _h = v->halfedge();
  do {
    _h = _h->twin()->next();
    deg++;
  } while (_h != v->halfedge());
  
  FaceIter f = newFace();

  HalfedgeIter h = v->halfedge();
  HalfedgeIter hnext, h2prev;
  HalfedgeIter hstart;
  VertexIter vstart;

  for (int i=0; i<deg; i++) {
    // set local and loop vars
    HalfedgeIter h0 = h->twin();
    FaceIter f0 = h0->face();
    hnext = h->twin()->next();

    // create new elements
    VertexIter v0;
    if (i != deg-1) v0 = newVertex();
    else v0 = v;
    HalfedgeIter h1 = newHalfedge();
    HalfedgeIter h2 = newHalfedge();
    EdgeIter e = newEdge();

    // modify and set pointers
    e->halfedge() = h1;
    v0->halfedge() = h1;
    // v0->position = bevelNewVertexPos(v, h0->vertex(), len);
    v0->position = v->position;
    f0->halfedge() = h1;

    h->vertex() = v0;
    h->edge() = h->edge();
    h->face() = h->face();
    h->twin() = h->twin();
    h->next() = h->next();

    h0->vertex() = h0->vertex();
    h0->edge() = h0->edge();
    h0->face() = h0->face();
    h0->twin() = h0->twin();
    h0->next() = h1;

    h1->vertex() = v0;
    h1->edge() = e;
    h1->face() = f0;
    h1->twin() = h2;
    h1->next() = hnext;

    if (i == 0) h2->vertex() = h2->vertex();
    else h2prev->vertex() = v0;
    h2->edge() = e;
    h2->face() = f;
    h2->twin() = h1;
    if (i == 0) h2->next() = h2->next();
    else h2->next() = h2prev;

    if (i == 0) {
      vstart = v0;
      hstart = h2;
    }

    if (i == deg-1) {
      h2->vertex() = vstart;
      hstart->next() = h2;
    }

    h2prev = h2;
    h = hnext;
  }

  f->halfedge() = h2prev;
  return f;
}

FaceIter HalfedgeMesh::bevelEdge(EdgeIter e) {
  HalfedgeIter h0 = e->halfedge();
  HalfedgeIter h1 = h0->twin();
  VertexIter v0 = h0->vertex();
  VertexIter v1 = h1->vertex();
  FaceIter f0 = h0->face();
  FaceIter f1 = h1->face();

  HalfedgeIter v1_start = h0->next();
  HalfedgeIter v0_start = h1->next();
  int v0_deg = 1; // v0_deg is in fact v0's degree - 1
  int v1_deg = 1; // same

  HalfedgeIter v1_end = v1_start->twin();
  while (v1_end->next() != h1) {
    v1_end = v1_end->next()->twin();
    v1_deg++;
  }

  HalfedgeIter v0_end = v0_start->twin();
  while (v0_end->next() != h0) {
    v0_end = v0_end->next()->twin();
    v0_deg++;
  }

  FaceIter f = newFace();

  HalfedgeIter h2 = v1_start;
  HalfedgeIter _h2, h2next, _h2_prev, h3_prev;
  VertexIter v2_prev;
  for (int i=0; i<v1_deg+v0_deg; i++) {
    if (i == v1_deg) {
      assert(v0_start == h2->next());
      h2 = v0_start;
    }
    _h2 = h2->twin();
    h2next = h2->twin()->next();
    HalfedgeIter h3, _h3;
    if (i == 0) {
      v1->halfedge() = h2;
    } else {
      EdgeIter e0 = newEdge();
      h3 = newHalfedge();
      _h3 = newHalfedge();
      VertexIter v2;
      if (i == v1_deg+v0_deg-1) {
        v2 = v0;
      } else {
        v2 = newVertex();
        if (i < v1_deg) v2->position = v1->position;
        else v2->position = v0->position;
      }

      _h2_prev->next() = _h3;

      e0->halfedge() = h3;
      v2->halfedge() = h2;

      _h3->twin() = h3;
      _h3->next() = h2;
      _h3->vertex() = v2_prev;
      _h3->edge() = e0;
      _h3->face() = h2->face();

      h3->twin() = _h3;
      h3->next() = h3_prev;
      h3->vertex() = v2;
      h3->edge() = e0;
      h3->face() = f;

      h2->vertex() = v2;
      h2->face()->halfedge() = _h3;
    }
    _h2_prev = _h2;
    v2_prev = h2->vertex();
    if (i == 0) {
      h3_prev = h1;
      h1->face() = f;
    } else {
      h3_prev = h3;
    }
    h2 = h2next;
  }
  _h2_prev->next() = h0;

  h0->vertex() = v2_prev;

  h1->next() = h3_prev;
  h1->face() = f;

  v0->halfedge() = h0;

  f->halfedge() = h1;

  return f;
}

FaceIter HalfedgeMesh::bevelFace(FaceIter f) {

  HalfedgeIter h = f->halfedge();
  HalfedgeIter hprev = h;
  do {
    hprev = h;
    h = h->next();
  } while (h != f->halfedge());
  
  FaceIter cf = f;
  VertexIter v_start;
  HalfedgeIter h0_start, h1_start, h1_prev, _h1_prev, hnext;
  FaceIter f0_prev;
  int deg = f->degree();

  for (int i=0; i<deg; i++) {
    hnext = h->next();
    EdgeIter e0 = newEdge();
    EdgeIter e1 = newEdge();
    FaceIter f0 = newFace();
    VertexIter v = newVertex();
    HalfedgeIter h0 = newHalfedge();
    HalfedgeIter _h0 = newHalfedge();
    HalfedgeIter h1 = newHalfedge();
    HalfedgeIter _h1 = newHalfedge();

    v->position = h->vertex()->position;

    f0->halfedge() = _h0; // need to save f0 for next round
    e0->halfedge() = h0;
    e1->halfedge() = h1;
    v->halfedge() = _h0;

    h0->twin() = _h0;
    h0->next() = h0->next(); // need to save h0 for last round
    h0->vertex() = h->vertex();
    h0->edge() = e0;
    h0->face() = h0->face();

    _h0->twin() = h0;
    _h0->next() = h;
    _h0->vertex() = v;
    _h0->edge() = e0;
    _h0->face() = f0;

    h1->twin() = _h1;
    h1->next() = h1->next(); // need to save h1 for next round and last round
    h1->vertex() = v;
    h1->edge() = e1;
    h1->face() = cf; // need to finally set cf->edge()

    _h1->twin() = h1;
    _h1->next() = _h0;
    _h1->vertex() = _h1->vertex(); // need to save _h1 for next round
    _h1->edge() = e1;
    _h1->face() = f0;

    h->face() = f0;

    if (i == 0) {
      v_start = v;
      h0_start = h0;
      h1_start = h1;
    } else {
      h0->next() = _h1_prev;
      h0->face() = f0_prev;
      h1_prev->next() = h1;
      _h1_prev->vertex() = v;
      hprev->next() = h0;
    }

    if (i == deg-1) {
      h0_start->next() = _h1;
      h0_start->face() = f0;
      h1->next() = h1_start;
      _h1->vertex() = v_start;
      h->next() = h0_start;
      cf->halfedge() = h1;
    }

    f0_prev = f0;
    h1_prev = h1;
    _h1_prev = _h1;
    hprev = h;
    h = hnext;
  }

  return cf;
}

void HalfedgeMesh::splitPolygons(vector<FaceIter>& fcs) {
  for (auto f : fcs) splitPolygon(f);
}

void HalfedgeMesh::splitPolygon(FaceIter f) {
  if (f->degree() == 3) return;
  HalfedgeIter h = f->halfedge();
  VertexIter v = h->vertex();
  
  HalfedgeIter h0 = h;
  HalfedgeIter h1 = h->next();
  HalfedgeIter h2 = h1->next();
  HalfedgeIter h4 = h;
  while (h4->next() != f->halfedge()) {
    h4 = h4->next();
  } // h4 = h->prev(), so to speak
  VertexIter v0 = h->vertex();
  VertexIter v1 = h1->vertex();
  VertexIter v2 = h2->vertex();
  FaceIter f0 = f;

  FaceIter f1 = newFace();
  EdgeIter e3 = newEdge();
  HalfedgeIter h3 = newHalfedge();
  HalfedgeIter _h3 = newHalfedge();

  h0->twin() = h0->twin();
  h0->next() = h0->next();
  h0->vertex() = h0->vertex();
  h0->edge() = h0->edge();
  h0->face() = f1;

  h1->twin() = h1->twin();
  h1->next() = h3;
  h1->vertex() = h1->vertex();
  h1->edge() = h1->edge();
  h1->face() = f1;

  h3->twin() = _h3;
  h3->next() = h0;
  h3->vertex() = v2;
  h3->edge() = e3;
  h3->face() = f1;

  _h3->twin() = h3;
  _h3->next() = h2;
  _h3->vertex() = v0;
  _h3->edge() = e3;
  _h3->face() = f0;

  h4->twin() = h4->twin();
  h4->next() = _h3;
  h4->vertex() = h4->vertex();
  h4->edge() = h4->edge();
  h4->face() = f0;

  e3->halfedge() = h3;
  f0->halfedge() = _h3;
  f1->halfedge() = h3;
  splitPolygon(f0);
}

EdgeRecord::EdgeRecord(EdgeIter& _edge) : edge(_edge) {

  // TODO: (meshEdit)
  // Compute the combined quadric from the edge endpoints.
  // -> Build the 3x3 linear system whose solution minimizes the quadric error
  //    associated with these two endpoints.
  // -> Use this system to solve for the optimal position, and store it in
  //    EdgeRecord::optimalPoint.
  // -> Also store the cost associated with collapsing this edg in
  //    EdgeRecord::Cost.

}

void MeshResampler::upsample(HalfedgeMesh& mesh)
// This routine should increase the number of triangles in the mesh using Loop subdivision.
{

  // TODO: (meshEdit)
  // Compute new positions for all the vertices in the input mesh, using
  // the Loop subdivision rule, and store them in Vertex::newPosition.
  // -> At this point, we also want to mark each vertex as being a vertex of the
  //    original mesh.
  // -> Next, compute the updated vertex positions associated with edges, and
  //    store it in Edge::newPosition.
  // -> Next, we're going to split every edge in the mesh, in any order.  For
  //    future reference, we're also going to store some information about which
  //    subdivided edges come from splitting an edge in the original mesh, and
  //    which edges are new, by setting the flat Edge::isNew. Note that in this
  //    loop, we only want to iterate over edges of the original mesh.
  //    Otherwise, we'll end up splitting edges that we just split (and the
  //    loop will never end!)
  // -> Now flip any new edge that connects an old and new vertex.
  // -> Finally, copy the new vertex positions into final Vertex::position.

      // Each vertex and edge of the original surface can be associated with a vertex in the new (subdivided) surface.
      // Therefore, our strategy for computing the subdivided vertex locations is to *first* compute the new positions
      // using the connectity of the original (coarse) mesh; navigating this mesh will be much easier than navigating
      // the new subdivided (fine) mesh, which has more elements to traverse.  We will then assign vertex positions in
      // the new mesh based on the values we computed for the original mesh.

      // Compute updated positions for all the vertices in the original mesh, using the Loop subdivision rule.
      for( VertexIter v = mesh.verticesBegin(); v != mesh.verticesEnd(); v++ )
      {
         // We also want to mark this vertex as being a vertex of the original mesh.
         v->isNew = false;

         // Compute the Loop subdivision coefficient beta for this vertex.
         double n = (double) v->degree();
         double beta = ( n>3.? (3./(8.*n)) : 3./16. );

         // Iterate over neighbors of this vertex, accumulating the new position.
         HalfedgeIter h = v->halfedge();
         Vector3D p = (1.-n*beta) * v->position; // Add contribution of center vertex.
         do
         {
            p += beta * h->next()->vertex()->position; // Add contribution of neighbor vertex.

            h = h->twin()->next(); // Advance to the next neighbor.
         }
         while( h != v->halfedge() ); // End iteration over vertex neighbors.

         // Store the updated vertex position for this vertex.
         v->newPosition = p;
      }

      // Next, compute the updated vertex positions associated with edges.
      for( EdgeIter e = mesh.edgesBegin(); e != mesh.edgesEnd(); e++ )
      {
         // Grab the four vertex positions in the immediate neighborhood of this edge.
         Vector3D a0 = e->halfedge()->vertex()->position;
         Vector3D a1 = e->halfedge()->twin()->vertex()->position;
         Vector3D b0 = e->halfedge()->next()->next()->vertex()->position;
         Vector3D b1 = e->halfedge()->twin()->next()->next()->vertex()->position;

         // Compute the vertex position that will be associated with this edge's midpoint,
         // using the Loop subdivision weights.
         e->newPosition = (3./8.)*( a0 + a1 ) + (1./8.)*( b0 + b1 );
      }

      // Next, we're going to split every edge in the mesh, in any order.  For future
      // reference, we're also going to store some information about which subdivided
      // edges come from splitting an edge in the original mesh, and which edges are new.
      // In this loop, we only want to iterate over edges of the original mesh---otherwise,
      // we'll end up splitting edges that we just split (and the loop will never end!)
      int nEdges = mesh.nEdges();
      EdgeIter e = mesh.edgesBegin();
      for( int i = 0; i < nEdges; i++ ) // Loop over original mesh edges.
      {
         // Since the edge split will invalidate the current edge,
         // we need to grab a pointer to the next edge in the list NOW.
         EdgeIter nextEdge = e; nextEdge++;

         // Split the edge, and get a pointer to the newly inserted midpoint
         // vertex.  Note that HalfedgeMesh::splitEdge() should set the
         // halfedge pointer of this newly inserted vertex such that it points
         // to one of the edges in the original mesh.
         VertexIter m = mesh.splitEdge( e );

         // Mark this vertex as being new (i.e., not part of the original mesh).
         m->isNew = true;

         // Copy the position we computed for this edge to the new edge midpoint.
         m->newPosition = e->newPosition;

         // Flag the four edges incident on this vertex as either being old
         // (part of the original mesh) or new (a new edge coming from the split).
         bool isNew = false;
         HalfedgeIter h = m->halfedge();
         do
         {
            h->edge()->isNew = isNew;
            h = h->twin()->next();
            isNew = !isNew;
         }
         while( h != m->halfedge() );

         // Advance to the next edge, using the pointer we stored before.
         e = nextEdge;
      }

      // Finally, flip any new edge that connects an old and new vertex.
      for( EdgeIter e = mesh.edgesBegin(); e != mesh.edgesEnd(); e++ )
      {
         if( e->isNew )
         {
            // Get the endpoints of this edge.
            VertexIter v0 = e->halfedge()->vertex();
            VertexIter v1 = e->halfedge()->twin()->vertex();

            // If exactly one endpoint is new, flip this edge.
            if( ( v0->isNew && !v1->isNew ) ||
                ( v1->isNew && !v0->isNew ) )
            {
               mesh.flipEdge( e );
            }
         }
      }

      // Copy the updated vertex positions to the subdivided mesh.
      for( VertexIter v = mesh.verticesBegin(); v != mesh.verticesEnd(); v++ )
      {
         v->position = v->newPosition;
      }
}

void MeshResampler::downsample(HalfedgeMesh& mesh)
{

  // TODO: (meshEdit)
  // Compute initial quadrics for each face by simply writing the plane equation
  // for the face in homogeneous coordinates. These quadrics should be stored
  // in Face::quadric
  // -> Compute an initial quadric for each vertex as the sum of the quadrics
  //    associated with the incident faces, storing it in Vertex::quadric
  // -> Build a priority queue of edges according to their quadric error cost,
  //    i.e., by building an EdgeRecord for each edge and sticking it in the
  //    queue.
  // -> Until we reach the target edge budget, collapse the best edge. Remember
  //    to remove from the queue any edge that touches the collapsing edge
  //    BEFORE it gets collapsed, and add back into the queue any edge touching
  //    the collapsed vertex AFTER it's been collapsed. Also remember to assign
  //    a quadric to the collapsed vertex, and to pop the collapsed edge off the
  //    top of the queue.

      // Our goal is to reduce the number of triangles by a factor of four,
      // which roughly corresponds to a doubling of the edge length (mirroring
      // the halving of edge length we get with Loop subdivision).
      const int targetFaceCount = mesh.nFaces() / 4;

      // Compute initial quadrics for each face by simply writing the plane
      // equation for the face in homogeneous coordinates.
      for( FaceIter f = mesh.facesBegin(); f != mesh.facesEnd(); f++ )
      {
         Vector3D N = f->normal();
         Vector3D p = f->halfedge()->vertex()->position;
         Vector4D u( N.x, N.y, N.z, -dot(N,p) );
         f->quadric = outer( u, u );
      }

      // Compute an initial quadric for each vertex as the sum of the quadrics
      // associated with the incident faces.
      for( VertexIter v = mesh.verticesBegin(); v != mesh.verticesEnd(); v++ )
      {
         Vector3D p = v->position;
         Vector4D q = Vector4D( p.x, p.y, p.z, 1. );

         v->quadric.zero();

         HalfedgeIter h = v->halfedge();
         do
         {
            v->quadric += h->face()->quadric;
            h = h->twin()->next();
         }
         while( h != v->halfedge() );
      }

      // Build a priority queue of edges according to their quadric error cost.
      MutablePriorityQueue<EdgeRecord> edgeQueue;
      for( EdgeIter e = mesh.edgesBegin(); e != mesh.edgesEnd(); e++ )
      {
         e->record = EdgeRecord( e );
         edgeQueue.insert( e->record );
      }

      // Until we reach the target edge budget, collapse the best edge.
      while( mesh.nFaces() > targetFaceCount )
      {
         HalfedgeIter h;

         EdgeRecord r = edgeQueue.top();
         EdgeIter e = r.edge;

         VertexIter v0 = e->halfedge()->vertex();
         VertexIter v1 = e->halfedge()->twin()->vertex();

         h = v0->halfedge();
         do
         {
            EdgeIter ei = h->edge();
            if( ei != e )
            {
               edgeQueue.remove( ei->record );
            }
            h = h->twin()->next();
         }
         while( h != v0->halfedge() );

         h = v1->halfedge();
         do
         {
            EdgeIter ei = h->edge();
            if( ei != e )
            {
               edgeQueue.remove( ei->record );
            }
            h = h->twin()->next();
         }
         while( h != v1->halfedge() );

         Matrix4x4 K0 = v0->quadric;
         Matrix4x4 K1 = v1->quadric;

         VertexIter v = mesh.collapseEdge( e );
         v->position = r.optimalPoint;
         v->quadric = K0 + K1;

         h = v->halfedge();
         do
         {
            EdgeIter e = h->edge();
            e->record = EdgeRecord( e );
            edgeQueue.insert( e->record );

            h = h->twin()->next();
         }
         while( h != v->halfedge() );

         edgeQueue.pop();
      }
}

void MeshResampler::resample(HalfedgeMesh& mesh) {

  // TODO: (meshEdit)
  // Compute the mean edge length.
  // Repeat the four main steps for 5 or 6 iterations
  // -> Split edges much longer than the target length (being careful about
  //    how the loop is written!)
  // -> Collapse edges much shorter than the target length.  Here we need to
  //    be EXTRA careful about advancing the loop, because many edges may have
  //    been destroyed by a collapse (which ones?)
  // -> Now flip each edge if it improves vertex degree
  // -> Finally, apply some tangential smoothing to the vertex positions

      const int nIterations = 5;
      const int nSmoothingIterations = 20;
      EdgeIter e;

      // Compute the mean edge length; this will be the target length for remeshing.
      double meanEdgeLength = 0.;
      for( e = mesh.edgesBegin(); e != mesh.edgesEnd(); e++ )
      {
         meanEdgeLength += e->length();
      }
      meanEdgeLength /= (double) mesh.nEdges();
      meanEdgeLength *= .9;
      const double maxLength = meanEdgeLength * 4./3.;
      const double minLength = meanEdgeLength * 4./5.;

      for( int iteration = 0; iteration < nIterations; iteration++ )
      {
         cerr << "iteration: " << iteration << endl;

         cerr << "splitting" << endl;
         // We first try to get more uniform edge lengths by splitting edges longer
         // than the threshold and collapsing edges shorter than the threshold.
         e = mesh.edgesBegin();
         while( e != mesh.edgesEnd() )
         {
            double length = e->length();

            // After splitting or collapsing, the current edge may no longer
            // exist; therefore, we need to grab a pointer to the next edge NOW.
            EdgeIter nextEdge = e;
            nextEdge++;

            if( length > maxLength ) // Split edges that are longer than the target length.
            {
               mesh.splitEdge( e );
            }

            e = nextEdge;
         }

         cerr << "collapsing" << endl;
         // We first try to get more uniform edge lengths by splitting edges longer
         // than the threshold and collapsing edges shorter than the threshold.
         e = mesh.edgesBegin();
         while( e != mesh.edgesEnd() )
         {
            double length = e->length();

            // After splitting or collapsing, the current edge may no longer
            // exist; therefore, we need to grab a pointer to the next edge NOW.
            EdgeIter nextEdge = e;
            nextEdge++;

            if( length < minLength ) // Collapse edges that are shorter than the target length.
            {
               // Collapsing an edge may invalidate not only this edge, but
               // also any of the four edges in its immediate neighborhood.
               // Therefore, we will advance the next edge to the first edge
               // that is NOT one of these edges.
               EdgeIter e1 = e->halfedge()->next()->edge();
               EdgeIter e2 = e->halfedge()->next()->next()->edge();
               EdgeIter e3 = e->halfedge()->twin()->next()->edge();
               EdgeIter e4 = e->halfedge()->twin()->next()->next()->edge();
               while( nextEdge == e  ||
                      nextEdge == e1 ||
                      nextEdge == e2 ||
                      nextEdge == e3 ||
                      nextEdge == e4 )
               {
                  nextEdge++;
               }

               // Now we can safely collapse the edge.
               mesh.collapseEdge( e );
            }

            e = nextEdge;
         }

         cerr << "flipping" << endl;
         // Next, we flip edges in an effort to get more uniform vertex valence.
         e = mesh.edgesBegin();
         while( e != mesh.edgesEnd() )
         {
            // After flipping, the current edge may no longer exist; therefore,
            // we need to grab a pointer to the next edge NOW.  (In principle,
            // one could implement edge flip such that the flipped edge is
            // not destroyed; however, we will not make that assumption here
            // for the sake of safety/robustness to future changes in the method
            // that performs the flipping.)
            EdgeIter nextEdge = e;
            nextEdge++;

            int a0 = e->halfedge()->vertex()->degree();
            int a1 = e->halfedge()->twin()->vertex()->degree();
            int b0 = e->halfedge()->next()->next()->vertex()->degree();
            int b1 = e->halfedge()->twin()->next()->next()->vertex()->degree();
            if( a0 != 3 && a1 != 3 && b0 != 3 && b1 != 3 )
            {
               int oldDefect = abs( a0-6 ) + abs( a1-6 ) + abs( b0-6 ) + abs( b1-6 );
               int newDefect = abs( (a0-1)-6 ) + abs( (a1-1)-6 ) + abs( (b0+1)-6 ) + abs( (b1+1)-6 );

               if( newDefect < oldDefect && a0-1 > 3 && a1-1 > 3 )
               {
                  mesh.flipEdge( e );
               }
            }

            e = nextEdge;
         }

         cerr << "smoothing" << endl;
         // Finally, we apply some tangential smoothing, to improve the shape
         // of the triangles.
         for( int i = 0; i < nSmoothingIterations; i++ )
         {
            // First, we compute the centroids of each vertex (i.e., the average
            // of its neighbors' positions).
            for( VertexIter v = mesh.verticesBegin(); v != mesh.verticesEnd(); v++ )
            {
               v->newPosition = v->position;
            }

            // Next, we move the vertex in a tangential direction toward its centroid.
            for( VertexIter v = mesh.verticesBegin(); v != mesh.verticesEnd(); v++ )
            {
               Vector3D N = v->normal();
               Vector3D p = v->position;
               Vector3D c = v->neighborhoodCentroid();

               Vector3D u = .2*(c-p);
               u -= dot( u, N )*N;

               v->newPosition += u;
            }

            for( VertexIter v = mesh.verticesBegin(); v != mesh.verticesEnd(); v++ )
            {
               v->position = v->newPosition;
            }
         }
      }
}

} // namespace CMU462
