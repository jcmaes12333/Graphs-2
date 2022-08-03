//NAME: JC Maes
// FILE: adjacency_list.cpp
// DATE: Spring 2021
// DESC: Adjacency list implementation of the graph interface


#ifndef ADJACENCY_LIST_CPP
#define ADJACENCY_LIST_CPP

#include "adjacency_list.h"
#include <iostream>

AdjacencyList::AdjacencyList(int vertex_count)
  : number_of_vertices(vertex_count)
{
  adj_list_out = new Node*[vertex_count];
  adj_list_in = new Node*[vertex_count];
  for (int i = 0; i < vertex_count; i++)
  { 
    adj_list_out[i] = nullptr;
    adj_list_in[i] = nullptr;//initialize matrices      
  }
  number_of_edges = 0;	
}

AdjacencyList::~AdjacencyList()
{
  if (number_of_vertices != 0)
    make_empty();	
}

AdjacencyList::AdjacencyList(const AdjacencyList& rhs)
  :  number_of_vertices(0)
{
  *this = rhs;	
}

AdjacencyList& AdjacencyList:: operator=(const AdjacencyList& rhs)//assignment operator
{
  make_empty();
  int edge;
  number_of_vertices = rhs.vertex_count();
  number_of_edges = rhs.edge_count();
  adj_list_in = new Node*[number_of_vertices];
  adj_list_out = new Node*[number_of_vertices];
  Node* tmp = nullptr;
  Node* prev = nullptr;
  for (int i = 0; i < vertex_count(); i++)
  {
    adj_list_out[i] = nullptr;
    adj_list_in[i] = nullptr; //initialize matrices     
  }
  for (int i = 0; i < rhs.vertex_count(); i++)
  {
    for (int j = 0; j < rhs.vertex_count(); j++)
    {
      if (rhs.get_edge(i, j, edge))//if there is an edge, copy out and in edges from list
      {
        tmp = adj_list_out[i];
	while (tmp != NULL)
	{
  	  prev = tmp;
	  tmp = tmp->next;
	}
	tmp = new Node;
	tmp->vertex = j;
	tmp->edge = edge;
	tmp->next = nullptr;
	if (adj_list_out[i] == NULL)
	  adj_list_out[i] = tmp;
	else
	  prev->next = tmp;
	tmp = adj_list_in[j];
	prev = nullptr;
	while(tmp != NULL)
	{
  	  prev = tmp;
  	  tmp = tmp->next;
	}
	tmp = new Node;
	tmp->vertex = i;
	tmp->edge = edge;
	tmp->next = nullptr;
	if (adj_list_in[j] == NULL)
	  adj_list_in[j] = tmp;
	else
	{
	  prev->next = tmp;
	}
      }
    }
  }
}

void AdjacencyList::set_edge(int v1, int edge_label, int v2)
{
  if (v1 >= 0 && v1 < number_of_vertices && v2 >= 0 && v2 < number_of_vertices)
  {
    Node* tmp;
    Node* prev;
    if (adj_list_out[v1] != nullptr)
    {
	   // std::cout << "here2\n";
      tmp = adj_list_out[v1];
      prev = nullptr;
      while (tmp != nullptr && tmp->vertex < v2)//find next empty node, or duplicate edge
      {
        prev = tmp;
        tmp = tmp->next;	
      }
      //std::cout << "here\n";
      if (tmp && tmp->vertex == v2)
      {
        tmp->edge = edge_label;//update if its a duplicate
      }
      else
      {
        tmp = new Node;
        tmp->vertex = v2;
        tmp->edge = edge_label;
	tmp->next = NULL;
        if (prev)
	{
	  tmp->next = prev->next;	
	  prev->next = tmp;//else, make sure the previous node points to tmp
	//std::cout << "her3\n";
	}
	else
	{
 	  Node* tmp2 = adj_list_out[v1];
	  tmp->next = tmp2;
	  adj_list_out[v1] = tmp;
	}
        number_of_edges++;
      }
    }
    else
    {
      adj_list_out[v1] = new Node;
      adj_list_out[v1]->vertex = v2;
      adj_list_out[v1]->edge = edge_label;
      adj_list_out[v1]->next = NULL;
      number_of_edges++;
    }
    if (adj_list_in[v2])
    {
      tmp = adj_list_in[v2];
      prev = nullptr;
      while (tmp != NULL && tmp->vertex < v1)
      {
        prev = tmp;
       	tmp = tmp->next;
      }
      if (tmp && tmp->vertex == v1)
          tmp->edge = edge_label;//update if its a duplicate
      else
      {
        tmp = new Node;
	tmp->vertex = v1;
	tmp->edge = edge_label;
	tmp->next = nullptr;
	if (prev)
	{ 
	  tmp->next = prev->next;
          prev->next = tmp;//else make the previous node point to tmp
	}
	else
	{
	  Node* tmp2 = adj_list_in[v2];
	  tmp->next = tmp2;
	  adj_list_in[v2] = tmp;
	}
      }
    }
    else
    {
      adj_list_in[v2] = new Node;
      adj_list_in[v2]->vertex = v1;
      adj_list_in[v2]->edge = edge_label;
      adj_list_in[v2]->next = NULL;
    }
  }
} 


bool AdjacencyList::has_edge(int v1, int v2) const
{
  if (v1 >= 0 && v1 < number_of_vertices && v2 >= 0 && v2 < number_of_vertices)
  {
    Node* tmp = adj_list_out[v1];
    while (tmp)
    {
      if (tmp->vertex == v2)//searching for v2 in out edge list
        return true;
      tmp = tmp->next;
    }
  }
  return false;
}

bool AdjacencyList::get_edge(int v1, int v2, int& edge) const
{
  if (v1 >= 0 && v1 < number_of_vertices && v2 >= 0 && v2 < number_of_vertices)
  {
    Node* tmp = adj_list_out[v1];
    while(tmp != NULL)
    {
      if (tmp->vertex == v2)
      {
        edge = tmp->edge;	//same as has_edge, but returns an edge weight as well
        return true;
      }
      tmp = tmp->next;
    }
  }
  return false;
}

void AdjacencyList::remove_edge(int v1, int v2)
{
	Node* tmp = adj_list_out[v1];
	Node* prev = NULL;
	bool found = false;
//std::cout << "removing " << v1 << " to " << v2 << std::endl;
	while (tmp && !found)
	{
	  if (tmp->vertex == v2)
	  {
	    if (prev)
	      prev->next = tmp->next;
	    else
	      adj_list_out[v1] = tmp->next;
	    delete tmp;
	    found = true;
	  }
	  prev = tmp;
	  tmp = tmp->next;
	}
        //std::cout << "here2\n";
	tmp = adj_list_in[v2];
	prev = NULL;
	found = false;
	while (tmp && !found)
	{
	  if (tmp->vertex == v1)
	  {
	    if (prev)
	      prev->next = tmp->next;
	    else
	      adj_list_in[v2] = tmp->next;
	    delete tmp;
	    found = true;
	  }
	  prev = tmp;
	  tmp = tmp->next;
	}

	if (found)
		number_of_edges--;
	//std::cout << "here3\n";
}
void AdjacencyList::connected_to(int v1, std::list<int>& vertices) const
{
  if (v1 >= 0 && v1 < number_of_vertices)
  {
    Node* tmp = adj_list_out[v1];
    while (tmp != NULL)
    {
      vertices.push_front(tmp->vertex);//if there is an out edge, add the vertex to list
      tmp = tmp->next;
    }
  }
}

void AdjacencyList::connected_from(int v2, std::list<int>& vertices) const
{
  if (v2 >= 0 && v2 < number_of_vertices)
  {
    Node* tmp = adj_list_in[v2];
    while (tmp != NULL)
    {
      vertices.push_front(tmp->vertex);//if there is an in edge, add the vertex to list
      tmp = tmp->next;
    }
  }
}

void AdjacencyList::adjacent(int v, std::list<int>& vertices) const
{
  if (v >= 0 && v < number_of_vertices)
  {
    Node* tmp = adj_list_out[v];
    while (tmp != NULL)
    {
      vertices.push_front(tmp->vertex);//basically just doing connected_to
      tmp = tmp->next;
    }
    tmp = adj_list_in[v];
    while (tmp != NULL)
    {
      vertices.push_front(tmp->vertex);//basically just doing connected_from
      tmp = tmp->next;
    }
  }
}

int AdjacencyList::vertex_count() const
{
  return number_of_vertices;
}

int AdjacencyList::edge_count() const
{
  return number_of_edges;
}

void AdjacencyList::make_empty()
{
  bool not_empty = false;
  Node* tmp = nullptr;
  for (int i = 0; i < number_of_vertices; i++)
  {
    not_empty = true;
    if (adj_list_out[i] != nullptr)
    {
      tmp = adj_list_out[i];
      Node* next = nullptr;
      while (tmp != nullptr)
      {
        next = tmp->next;
	delete tmp;
	tmp = next;//free all nodes in list
      }
      adj_list_out[i] = nullptr;
    }
    if (adj_list_in[i] != nullptr)
    {
      tmp = adj_list_in[i];
      Node* next = nullptr;
      while (tmp)
      {
        next = tmp->next;
	delete tmp;//free all nodes in list
	tmp = next;
      }
      adj_list_in[i] = nullptr;
    }
    }
  if (not_empty)
  {
    delete[] adj_list_in;//delete matrices
    delete[] adj_list_out;//delete matrices
  }
  }
#endif

