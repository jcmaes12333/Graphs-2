//----------------------------------------------------------------------
// NAME: J. Maes
// FILE: adjacency_matrix.cpp
// DATE: Spring 2021
// DESC: Adjacency matrix implementation of the graph interface
//----------------------------------------------------------------------



#ifndef ADJACENCY_MATRIX_CPP
#define ADJACENCY_MATRIX_CPP

#include "adjacency_matrix.h"
#include <iostream>

AdjacencyMatrix::AdjacencyMatrix(int vertex_count)
{
  int size = vertex_count * vertex_count;
  adj_matrix = new int*[size];
  number_of_vertices = vertex_count;
  number_of_edges = 0;
  for (int i = 0; i < size; i++)
    adj_matrix[i] = nullptr;//initialize matrix
}

AdjacencyMatrix::~AdjacencyMatrix()
{
  make_empty();	
}

AdjacencyMatrix::AdjacencyMatrix(const AdjacencyMatrix& rhs)
 : number_of_vertices(0)
{
  *this = rhs;//call assignment operator
}

// assignment operator
AdjacencyMatrix& AdjacencyMatrix::operator=(const AdjacencyMatrix& rhs)
{
  if (this == &rhs)
	  return *this;
  if (number_of_vertices != 0)
    make_empty();
  number_of_edges = 0;
  number_of_vertices = rhs.vertex_count();
  int size = number_of_vertices * number_of_vertices;
  int v1 = 0;
  adj_matrix = new int*[size];
  for (int i = 0; i < size; i++)
  	adj_matrix[i] = nullptr;
  for (int i = 0; i < number_of_vertices; i++)//nested for loop to iterate through matrix
  {
    int edge = 0;
    for (int j = 0; j < number_of_vertices; j++)
    {		
      int mIndex = index(i,j);//find index
      if (rhs.get_edge(i,j,edge))//if there is an edge
      {
	adj_matrix[mIndex] = new int; 
        *adj_matrix[mIndex] = edge;//copy edge
	number_of_edges++;//update edges
      }
    }
  }  
}

  // add or update an edge between existing vertices in the graph
void AdjacencyMatrix::set_edge(int v1, int edge_label, int v2)
{
  if (v1 < number_of_vertices && v1 >= 0 && v2 < number_of_vertices && v2>=0)
  {
    int mIndex = index(v1,v2);//find index
    if (adj_matrix[mIndex] == nullptr)
    {
      number_of_edges++;
      adj_matrix[mIndex] = new int;//make space if we need
    }
    *adj_matrix[mIndex] = edge_label;//update label regardless
  }
}

  // check if the given edge is in the graph
bool AdjacencyMatrix::has_edge(int v1, int v2) const
{
  if (v1 < number_of_vertices && v1 >= 0 && v2 < number_of_vertices && v2>=0)
  {
    int mIndex = index(v1,v2);
    if (adj_matrix[mIndex])//if this index of the matrix is not a nullptr
      return true;
  }
  return false;
}

bool AdjacencyMatrix::get_edge(int v1, int v2, int& edge) const
{
  if (v1 < number_of_vertices && v1 >= 0 && v2 < number_of_vertices && v2>=0)
  {
    int mIndex = index(v1,v2);
    if (adj_matrix[mIndex] != nullptr)
    {
      edge = *adj_matrix[mIndex];//return edge
      return true;
    }
  }
  return false;
}

void AdjacencyMatrix::remove_edge(int v1, int v2)
{
  int i = index(v1, v2);
  if (adj_matrix[i])
  {
    delete adj_matrix[i];
    adj_matrix[i] = NULL;
    number_of_edges--;
  }
}
// get all vertices on an outgoing edge from given vertex
void AdjacencyMatrix::connected_to(int v1, std::list<int>& vertices) const
{
  if (v1 >= 0 && v1 < number_of_vertices)
  {
    int firstIndex = index(v1,0);
    for (int i = firstIndex; i < number_of_vertices + firstIndex; i++)//checks outgoing edges
    {
      if (adj_matrix[i] != nullptr)
        vertices.push_front(i - firstIndex);
    }
  }
}

// get all vertices on an incoming edge to given vertex
void AdjacencyMatrix::connected_from(int v2, std::list<int>& vertices) const
{
  if (v2 >= 0 && v2 < number_of_vertices)
  {
    for (int i = 0; i < number_of_vertices; i++)//checks incoming edges
    {
      int mIndex = index(i,v2);
      if (adj_matrix[mIndex] != nullptr)
      {
        vertices.push_front(i);
      }
    }
  }
}

  // get all vertices adjacent to a vertex, that is, connected to or connected
  // from the vertex (may return duplicate vertices)
void AdjacencyMatrix::adjacent(int v, std::list<int>& vertices) const
{
  if (v >= 0 && v < number_of_vertices)
  {
    for (int i = 0; i < number_of_vertices; i++)
    {
      int mIndex = index(i, v);
      if (adj_matrix[mIndex] != nullptr)//if there is an edge between the two vertices
        vertices.push_front(i);//add to list
    }
    for (int i = 0; i < number_of_vertices; i++)
    {
      if (i != v)//keeps us from adding duplicate edges
      {
        int mIndex = index(v,i);
        if (adj_matrix[mIndex] != nullptr)
          vertices.push_front(i);
      }
    }
  }
}

  // get number of nodes in the graph
int AdjacencyMatrix::vertex_count() const
{
  return number_of_vertices;
}

  // get number of edges in the graph
int AdjacencyMatrix::edge_count() const
{
  return number_of_edges;
}

int AdjacencyMatrix::index(int x, int y) const
{
  return (x * number_of_vertices) + y; //index formula
}

void AdjacencyMatrix::make_empty()
{
  int size = number_of_vertices * number_of_vertices;
  for (int i = 0; i < size; i++)
  {
	 
	 if (adj_matrix[i] != nullptr)
		 delete (adj_matrix[i]);
		 
  }	  
  delete[] adj_matrix;
}
#endif
