//Name: John Maes
//Class: CPSC450
//Dr. Bowers
//HW8: Bellman-Ford

#ifndef GRAPH_CPP
#define GRAPH_CPP

#include "graph.h"
#include <iostream>
#include <queue>
#include <stack>
#include <algorithm>

void Graph::bfs(bool dir, int src, Map& tree) const//breadth first search algorithm
{
  tree[src] = -1; //source doesnt have a parent
  std::queue<int> Q;
  Q.push(src);//Q starts with the source vertex
  if (!dir)//if the graph is undirected
  {
    while (!Q.empty())//While there are still undiscovered vertices
    {
      int u = Q.front();//u is the vertex at the front of queue
      Q.pop();
      int v;
      std::list<int> adj;
      adjacent(u,adj);
      while (!adj.empty())//looks at every adjacent vertex
      {
        v = adj.front();
	adj.remove(v);
	if (tree.count(v) == 0)
	{
	  tree[v] = u;//parent of v is u (because it is adjacent, it was reached from v
	  Q.push(v);//add v to the queue
	}
      }
    }
  }
  else//if the graph is directed
  {
    while (!Q.empty())
    {
      int u = Q.front();
      Q.pop();
      int v;
      std::list<int> con;
      connected_to(u,con);//this is the only thing different, because its directed
      while (!con.empty())
      {
        v = con.front();
	con.remove(v);
	if (tree.count(v) == 0)
	{
	  tree[v] = u;
	  Q.push(v);
	}
      }
    }	
  }
}
//this uses the same algorithm as BFS, but stops when we find the vertex we want
void Graph::shortest_path_length(bool dir, int src, int dst, std::list<int>& path) const
{
  Map parent; 
  parent[src] = -1;
  std::queue<int> Q;
  Q.push(src);
  if (!dir)
  {
    while (!Q.empty())
    {
      int u = Q.front();
      Q.pop();
      int v;
      std::list<int> adj;
      adjacent(u,adj);
      while (!adj.empty())
      {	
        v = adj.front();
	adj.remove(v);
	if (parent.count(v) == 0)
	{
  	  parent[v] = u;
	  if (v == dst)//only thing different from BFS, we are looking for the path to dst
	  {
  	    while (parent[v] != -1)//loop until we reach end of path
	    {
	      path.push_front(v);
	      v = parent[v];//traces back through the path
	    }
	    path.push_front(v);
	    return;
	  }
	  Q.push(v);
	}
      }
    }
  }
  else
  {
    while (!Q.empty())
    {
      int u = Q.front();
      Q.pop();
      int v;
      std::list<int> con;
      connected_to(u,con);
      while (!con.empty())
      {
        v = con.front();
	con.remove(v);
	if (parent.count(v) == 0)
	{
  	  parent[v] = u;
	  if (v == dst)//same as above, this is the only difference from BFS
	  {
	    while (parent[v] != -1)
	    {
	      path.push_front(v);
	      v = parent[v];
	    }
	    path.push_front(v);
	    return;
	  }
	  Q.push(v);
	}
      }
    }	
  }
}

void Graph::bfs_connected_components(Map& components) const
{
  for (int i = 0; i < vertex_count(); i++)//this algorithm is also similar to bfs, but we dont care if its directed or not
  {
    if (components.count(i) == 0)
    {
      components[i] = i;
      std::queue<int> Q;
      Q.push(i);
      while (!Q.empty())
      {
        int u = Q.front();
	Q.pop();
	int v;
	std::list<int> adj;
	adjacent(u,adj);
	while (!adj.empty())
 	{
	  v = adj.front();
	  adj.remove(v);
	  if (components.count(v) == 0)
	  {
	    components[v] = i;//all connected components will have the same value
	    Q.push(v);
	  }
	}
      }	
    }
  }
}
//slightly modified BFS algorithm to check graph for two coloring
bool Graph::bipartite_graph(Map& coloring) const 
{
  for (int i = 0; i < vertex_count(); i++)//do bfs from every undiscovered vertex
  {
    std::queue<int> Q;
    if (coloring.count(i) == 0)//if undiscovered
    {
      Q.push(i);
      coloring[i] == 1;
      while (!Q.empty())//same algorithm as undirected BFS
      {
        int u = Q.front();
	Q.pop();
	int v;
	std::list<int> adj;
	adjacent(u,adj);
	while (!adj.empty())
	{
	  v = adj.front();
	  adj.remove(v);
	  if (coloring.count(v) == 0)//if not colored
	  {
  	    coloring[v] = 1 - coloring[u];//set color opposite of v
	    Q.push(v);

	  }
	  else
	  {
	    if (coloring[u] == coloring[v])
	    {
              return false;//is not bipartite if two vertices with an edge have the same color 
	    }
	  }
	}
      }
    }
  }
  return true;
}

void Graph::dfs(bool dir, int src, Map& Tree) const
{
  std::stack<int> stck;
  int u,v;
  Tree[src] = -1;
  stck.push(src);//push source onto stack
  bool unexplored = false;
  if (dir)//directed graph
  {
    std::list<int> con;
    while (!stck.empty())
    {
      unexplored = false;
      u = stck.top();
      connected_to(u, con);
      while (!con.empty())//look at all connected vertices
      {
        v = con.front();
	con.remove(v);
	if (Tree.count(v) == 0)
	{
 	  unexplored = true;
 	  stck.push(v);//push onto stack if unexplored
 	  Tree[v] = u;
	} 
      }
      if (!unexplored)
      {
        stck.pop();//if we read a dead end, pop
      }
    }
  }
  else//undirected
  {
    std::list<int> adj;
    while (!stck.empty())
    {
      unexplored = false;
      u = stck.top();
      adjacent(u, adj);
      while (!adj.empty())//looks at all adjacenct vertices
      {
        v = adj.front();
	adj.remove(v);
	if (Tree.count(v) == 0)
        {
  	  unexplored = true;
	  stck.push(v);//push if unexplored
	  Tree[v] = u;
        } 
      }
      if (!unexplored)
      {
        stck.pop();//pop from stack if we reach a dead end
      }
    }
  }
}

bool Graph::acyclic(bool dir) const
{
  std::stack<int> stck;
  int u,v;
  int coloring[vertex_count()];
  bool unexplored = false;
  Map parent;
  for (int i = 0; i < vertex_count(); i++)
  {
    coloring[i] = 0;//0 for white, 1 for grey, and 2 for black
  }
  for (int i = 0; i < vertex_count(); i++)//look at all vertices if unexplored
  {
    if (dir && coloring[i] != 2)//if vertex is black, it is explored
    {
      parent[i] = -1;
      stck.push(i);
      std::list<int> con;
      while (!stck.empty())
      {
	//std::cout << "stuck\n";
        unexplored = false;
	u = stck.top();
	connected_to(u, con);
	coloring[u] = 1;//color u grey
	while (!con.empty())
	{
          v = con.front();
	  con.remove(v);
	  if (coloring[v] == 0 && parent.count(v) == 0)//if v is unexplored
	  {
  	    unexplored = true;
	    stck.push(v);//push v if unexplored
	    parent[v] = u;
	  }
	  else if(coloring[v] == 1 && v != u)
	  {
	    return false;//if we reach a grey vertex, it is a cycle
	  } 
	}
	if (!unexplored)
	{
   	  stck.pop();
	  coloring[u] = 2;//color it black if its a dead end
	}
      }
    }
    else if (coloring[i] != 2)
    { 
      std::list<int> adj;
      stck.push(i); 
      parent[i] = -1;
      //coloring[i] = 1;
      while (!stck.empty())
      {
        unexplored = false;
	u = stck.top();
	adjacent(u, adj);
	coloring[u] = 1;
	while (!adj.empty())
        {
  	  v = adj.front();
	  adj.remove(v);
	  if (coloring[v] == 0)//if its white, its unexplored
	  {
  	    unexplored = true;
  	    stck.push(v);
	    parent[v] = u;
    	  }
	  else if (coloring[v] == 1 && v != parent[u] && v != u)
	  {
	    return false;//if we reach a grey vertex, its a cycle
	  }  
	}
	if (!unexplored)
        {
  	  stck.pop();
	  coloring[u] = 2;//if its a dead end, color black
    	}
      }
    }
  }
  return true;//not cyclical if we reach this point
}

void Graph::unweighted_transitive_closure(bool dir, Graph& closed_graph) const
{
  std::stack<int> stck;
  int u,v;
  Map discovered;
  bool unexplored = false;
  for (int i = 0; i < vertex_count(); i++)
  {
    discovered.clear();//must reset to get all reachable vertices
    stck.push(i);
    if (dir)
    {
      std::list<int> con;
      while (!stck.empty())
      {
        unexplored = false;
        u = stck.top();
        connected_to(u, con);
        while (!con.empty())
        {
          v = con.front();
	  con.remove(v);
	  if (discovered.count(v) == 0 && v != i)
	  {
 	    unexplored = true;
 	    stck.push(v);
	    closed_graph.set_edge(i, -1, v);//if its an undiscovered vertex, add an edge
	    discovered[v] = u;
	  } 
        }
        if (!unexplored)
        {
          stck.pop();
        }
      }
    }
    else
    {
      std::list<int> adj;
      while (!stck.empty())
      {
        unexplored = false;
        u = stck.top();
        adjacent(u, adj);
        while (!adj.empty())
        {
          v = adj.front();
	  adj.remove(v);
	  if (discovered.count(v) == 0 && v != i)
          {
  	    unexplored = true;
	    stck.push(v);
	    closed_graph.set_edge(i, -1, v);//if its an undicovered vertex, add an edge
	    discovered[v] = u;
          } 
        }
        if (!unexplored)
        {
          stck.pop();
        }
      }
    }
  }
}

void Graph::dfs_topological_sort(Map& vertex_ordering) const
{
  std::stack<int> stck;
  int u,v;
  int sort_order = vertex_count();//we add in reverse order once we reach a dead end
  Map discovered;
  bool unexplored;
  for (int i = 0; i < vertex_count(); i++)
  { 
    if (vertex_ordering.count(i) == 0)
    {
      stck.push(i);
      std::list<int> con;
      while (!stck.empty())
      {
        unexplored = false;
        u = stck.top();
	con.clear();
        connected_to(u, con);
        while (!con.empty())
        {
          v = con.front();
	  con.remove(v);
	  if (vertex_ordering.count(v) == 0)
	  {
            unexplored = true;
	    stck.push(v);
	  } 
        }
        if (!unexplored)
        {
          stck.pop();//pop regardless if its a dead end
	  if (vertex_ordering.count(u) == 0)
	  {
	    vertex_ordering[u] = sort_order;//if it hasnt been ordered, add it
	    sort_order--;//decrement 
	  }
        }
      } 
    }
  }
}

void Graph::strongly_connected_components(Map& components) const
{
  std::stack<int> stck;
  int u,v;
  int vertex_index = vertex_count() - 1;
  Map vertex_order; 
  bool unexplored = false;
  bool discovered[vertex_count()];
  bool discovered2[vertex_count()];
  for (int i = 0; i < vertex_count(); i++)
  {
    discovered[i] = false;
    discovered2[i] = false;//initializing arrays
  }
  for (int i = 0; i < vertex_count(); i++)//dfs over whole graph to determine search order
  {
    if (discovered[i] == false)
    {
      stck.push(i);
      discovered[i] = true;
      unexplored = false;
      std::list<int> con;
      while (!stck.empty())
      {
        unexplored = false;
        u = stck.top();
        connected_from(u, con);
        while (!con.empty())//look at all connected vertices
        { 
          v = con.front();
          con.remove(v);
          if (discovered[v] == false)
          {
            unexplored = true;
	    stck.push(v);//push onto stack if unexplored
	    discovered[v] = true;
          }  
        }
        if (!unexplored)
        {
          stck.pop();//if we reach a dead end, pop
	  vertex_order[vertex_index] = u;//add to end of vertex order
	  vertex_index--;
        }
      }
    }
  }
  int component_number = 0;
  for (int i = 0; i < vertex_count(); i++)
  {
    if (discovered2[vertex_order[i]] == false)
    {
      component_number++;// increment when we have a new undiscovered vertex in the for loop
      stck.push(vertex_order[i]);
      discovered2[vertex_order[i]] = true;
      std::list<int> con;
      while (!stck.empty())
      {
        unexplored = false;
        u = stck.top();
	components[u] = component_number;//add each vertex to its component
        connected_to(u, con);
        while (!con.empty())//look at all connected vertices
        { 
          v = con.front();
          con.remove(v);
          if (discovered2[v] == false)
          {
            unexplored = true;
	    stck.push(v);//push onto stack if unexplored
	    discovered2[v] = true;
          }  
        }
        if (!unexplored)
        {
          stck.pop();//if we reach a dead end, pop
        }
      }
    }
  }
}


void Graph::transitive_reduction(Graph& reduced_graph) const
{
  Map components;
  strongly_connected_components(components);
  int last_vertex;
  int first_vertex = -1;//for completing cycles
  int last_vertex_from_component = -1;//helps with bridges from one component to the next
  int num_components = components[0];
  for (int i = 1; i < vertex_count(); i++)
  {
    if (components[i] > num_components)
      num_components = components[i];//determine number of components
  }
  for (int i = 0; i < num_components; i++)
  {
    last_vertex = -1;
    for (int j = 0; j < vertex_count(); j++)
    {
      if (components[j] == i + 1)//if its a part of the components we're searching
      {
        if (last_vertex != -1)
	{
          reduced_graph.set_edge(last_vertex, -1, j);//cycle vertices in component
	}
	else if (last_vertex_from_component != -1)
	{
	  reduced_graph.set_edge(last_vertex_from_component, -1, j);//bridge from last vertex
	  first_vertex = j;//set for finishing cycle later
	}
	else
	{
	  first_vertex = j;//this case its the first component, so we dont need bridge
	}
	last_vertex = j;//always set last vertex we came from
      }
    }
    if(first_vertex != -1)
    {
     reduced_graph.set_edge(last_vertex, -1, first_vertex);//finish cycle
    } 
     //std::cout << "set edge: " << last_vertex << " to " << first_vertex << std::endl;
    last_vertex_from_component = last_vertex;//last vertex we visit will be used for a bridge   
  }
  for (int i = 0; i < reduced_graph.vertex_count(); i++)
  {
    std::list<int> con;
    reduced_graph.connected_to(i, con);
    int v;
    while (!con.empty())
    {
      v = con.front();
      con.remove(v);
      reduced_graph.remove_edge(i, v);
      std::list<int> path;
      reduced_graph.shortest_path_length(true, i, v, path);
      if (path.size() == 0)//if there is no path without this edge, add it back to the graph
      {
        reduced_graph.set_edge(i, -1, v);
	//std::cout << "kept edge " << i << " to " << v << std::endl;
      }
    }
  }
}

bool Graph::directed_eulerian_path(std::list<int>& path) const
{
  int source_vertex = 0;
  int out_edges[vertex_count()];
  int in_edges[vertex_count()];
  int edges_left[vertex_count()];
  int num_sources = 0;
  int num_sinks = 0;
  std::list<int> con;
  for (int i = 0; i < vertex_count(); i++)
  {
    connected_from(i, con);
    in_edges[i] = con.size();
    con.clear();
    connected_to(i, con);
    out_edges[i] = con.size();
    con.clear();
    edges_left[i] = out_edges[i];
    
    if (out_edges[i] - in_edges[i] == 1)//source vertex
    {
      num_sources++;
      source_vertex = i;
      if (num_sources > 1)
        return false;
    }
    else if (in_edges[i] - out_edges[i] == 1)//sink vertex
    {
      num_sinks++;
      if (num_sinks > 1)
        return false;
    }
    else if (in_edges[i] != out_edges[i])
      return false;
  }
  con.clear();
  std::stack<int> stck;
  stck.push(source_vertex);//start search from source
  int v, u;
  while (!stck.empty())
  {
    u = stck.top();
    int removes = out_edges[u] - edges_left[u];//how many edges we need to remove
    connected_to(u, con);
    while (removes > 0)
    {
      v = con.front();
      con.remove(v);//remove edges we have already searched
      removes--;
    }
    if (edges_left[u] != 0)//if we still have edge to search
    {
      v = con.front();
      stck.push(v);//push next vertex onto the stack
      edges_left[u]--;
    }
    else
    {
      path.push_front(stck.top());
      stck.pop();//if we reach a dead end, pop
    }
  }
  return true;
}

bool Graph::directed_hamiltonian_path(std::list<int>& path) const
{
  bool discovered[vertex_count()];
  for (int i = 0; i < vertex_count(); i++)
  {
    path.clear();//clear path each iteration
    path.push_back(i);//add start vertex
    for (int j = 0; j < vertex_count(); j++)
	    discovered[j] = false;
    discovered[i] = true;
    if (directed_hamiltonian_rec(i, path, discovered))//call recursive
	    return true;
    //if path is there, return
  }
  return false;
}

bool Graph::bipartite_graph_matching(Graph& max_matched_graph) const
{
  Map coloring;
  std::queue<int> Q;
  Map labels;
  if(bipartite_graph(coloring))
  {
    Set v1, v2, tmp;
    int w,v;
    for (int i = 0; i <vertex_count(); i++)//initializing sets and the queue
    {
      if (coloring[i] == 0)
      {
        v1.insert(i);
	Q.push(i);
      }
      else
      {
	v2.insert(i);
      }
    }
    while (!Q.empty())
    {
      w = Q.front();
      Q.pop();
      bool is_in = v1.find(w) != v1.end();
      if (is_in)//if w is in v1
      {
        std::list<int> adj;
        adjacent(w, adj);
	while (!adj.empty())//for every adjacent vertex
	{
	  int u = adj.front();
	  adj.remove(u);
	  std::list<int> adj2;
	  max_matched_graph.adjacent(u, adj2);
	  if (adj2.empty())//if u is free
	  {
	    max_matched_graph.set_edge(w, -1, u);//add edge between u and w
	    int v = w;
	    while (labels.count(v) != 0)//augment
	    {
              u = labels[v];
	      max_matched_graph.remove_edge(v, u);

	      v = labels[u];
	      max_matched_graph.set_edge(v, -1, u);

	    }
	    labels.clear();//reset labels
	    while (!Q.empty())
	    {
	      Q.pop();
	    }
	    std::set<int>::iterator it = v1.begin();
	    while (it != v1.end())
	    {
              adj.clear();
              max_matched_graph.adjacent(*it, adj);
	      if (adj.empty())
	      {
	        Q.push(*it);
	      }
	      it++;
	    }//re-initialize Q with all free vertices left in v1
	    break;//exit while loop
	 }
	 else
	 {
	   if (!max_matched_graph.has_edge(w, u) && labels.count(u) == 0)//if the edge is not in the graph and is not labeled
	   {
	     labels[u] = w;
	     Q.push(u);
	   }
	 }
	}
      }
      else
      {
        std::list<int> adj;
        max_matched_graph.adjacent(w, adj);
        labels[adj.front()] = w;//label w's mate v with w
        Q.push(adj.front());//enqueue
        adj.clear();	
      }
    }
    return true;
  }
  return false;
}

void Graph::cliques(std::list<Set>& cliques) const
{
  Set p, r, x;
  for (int i = 0; i < vertex_count(); i++)
  {
    p.insert(i);
  }
  cliques_rec(p, r, x, cliques);//call recursive function
}

bool Graph::directed_hamiltonian_rec(int v, std::list<int>& path, bool discovered[]) const
{
  if (path.size() == vertex_count())//if the path has reached every vertex, return true
	  return true;
  std::list<int> con;
  connected_to(v, con);
  while (!con.empty())//for every vertex that v is connected to
  {
    int u = con.front();
    con.remove(u);
    if (discovered[u] == false)
    {
      path.push_back(u);//add it to the path if its undiscovered
      discovered[u] = true;
      if (directed_hamiltonian_rec(u, path, discovered))
        return true;
      else
      {
        path.remove(u);//if no path is found, remove from path
	discovered[u] = false;
      }
    }
  }
  return false;
}

void Graph::cliques_rec(Set& p, Set& r, Set& x, std::list<Set>& cliques) const
{
  if (p.empty() && x.empty())//if both are empty, r is a clique
  {
    cliques.push_front(r);
    return;
  }

  while(!p.empty())
  {
    std::set<int>::iterator it = p.begin();//it points to the first element in p
    std::list<int> tmp, adj;
    adjacent(*it, adj);
    Set adj2(adj.begin(), adj.end());
    set_intersection(p.begin(), p.end(), adj2.begin(), adj2.end(), back_inserter(tmp));//intersect p with adjacent vertices to the first element in p
    Set newP(tmp.begin(), tmp.end());//this is the p we send to the recursive function
    Set newR = r;
    newR.insert(*it);//add this element to r
    tmp.clear();
    set_intersection(x.begin(), x.end(), adj2.begin(), adj2.end(), back_inserter(tmp));//intersect x with adjacent vertices to the first element in p
    Set newX(tmp.begin(), tmp.end());//this is the x we send to the recursive function 
    cliques_rec(newP, newR, newX, cliques);
    p.erase(*it);//remove this element from potential vertices
    x.insert(*it);//and add it to the exclusion set
  }

   
}

void Graph::dijkstra_shortest_path(int src, Map& path_costs) const
{
  bool excluded[vertex_count()];
  for (int i = 0; i < vertex_count(); i++)
  {
    path_costs[i] = std::numeric_limits<int>::max();
    excluded[i] = false;
  }
  excluded[src] = true;//exclusions set starts with the source
  path_costs[src] = 0;//no path cost to itself
  bool found = true;
  int u,v;
  while (found)
  {
    found = false;//reset in each loop, tells us when we're done
    int min = -1;
    int weight;
    for (int i = 0; i < vertex_count(); i++)//check all edges
    {
      for (int j = 0; j < vertex_count(); j++)
      {
        if (excluded[i] && !excluded[j] && get_edge(i,j,weight) && (weight + path_costs[i] < min || !found))
	{
          found = true;
          min = path_costs[i] + weight;
	  u = i;
	  v = j;
	}
      }
    }
    if (found)//we've found the cheapest path
    {
      path_costs[v] = min;
      excluded[v] = true;
    }
  }
}

void Graph::prim_min_spanning_tree(Graph& spanning_tree) const
{
  bool excluded[vertex_count()];
  for (int i = 0; i < vertex_count(); i++)
  {
    excluded[i] = false;
  }
  excluded[0] = true;//start with arbitrary vertex
  bool found = true;
  int u,v;
  while (found)//similar algorithm to dijkstra, but we just check weight, not weight + path cost
  {
    int min = -1;
    int weight;
    found = false;
    for (int i = 0; i < vertex_count(); i++)
      for (int j = 0; j < vertex_count(); j++)
      {
        if (excluded[i] && !excluded[j] && (get_edge(i,j,weight) || get_edge(j,i,weight))  && (weight < min || !found))
	{
	  u = i;
	  v = j;
	  min = weight; 
	  found = true;
	}
      }
    if (found)//we've found a minimum edge
    { 
      spanning_tree.set_edge(u, min ,v);
      excluded[v] = true;
    }
  } 
}

void Graph::kruskal_min_spanning_tree(Graph& spanning_tree) const
{
  std::vector<std::tuple<int,int,int>> edges;
  int k = 0;
  for (int i = 0; i < vertex_count(); i++)
    for(int j = 0; j < vertex_count(); j++)
    {
      int weight;
      if (get_edge(i, j, weight))//add all edges to the vector
      {
        std::tuple<int,int,int> edge (std::make_tuple(i,weight,j));
        edges.push_back(edge);;
	k++;
      }
    }
  int size = edges.size();
  sort_by_edge_weight(edges, 0, size - 1);//sort edge weights
  int i = 0;
  while (spanning_tree.edge_count() < vertex_count() - 1 && i < edges.size())//checks if we've found a MST or have ran out of edges
  {
    spanning_tree.set_edge(std::get<0>(edges[i]), std::get<1>(edges[i]), std::get<2>(edges[i]));
    if (!spanning_tree.acyclic(false))
      spanning_tree.remove_edge(std::get<0>(edges[i]), std::get<2>(edges[i]));//if there is a cycle with this edge, remove it
    i++;   
  }
}

void Graph::sort_by_edge_weight(std::vector<std::tuple<int,int,int>>& edges, int start, int end) const //sort helper, nearly identical to the merge sort from hw1
{ 
 if (!(start < end)) return;
 int mid = (start + end - 1) / 2;
 sort_by_edge_weight(edges, start, mid);
 sort_by_edge_weight(edges, mid+1, end);
 int n1 = mid - start + 1;
 int n2 = end - mid;;
 std::vector<std::tuple<int,int,int>> left;
 std::vector<std::tuple<int,int,int>> right; 
 int i = 0, j = 0;

 for (int a = 0; a < n1; a++)
   left.push_back(edges[start + a]);
 for (int b = 0; b < n2; b++)
 {
   right.push_back(edges[1 + mid + b]);
 }
  
 i = 0, j = 0;
 int merged = start;
 std::vector<std::tuple<int,int,int>>::iterator it = edges.begin();
 for (int a = 0; a < start; a++)
 {
   it++;
 }
 while(i < n1 && j < n2)
 { 
   if (std::get<1>(left[i]) <= std::get<1>(right[j]))
   {
     //std::cout << "u: " << std::get<0>(left[i]) << " v: " << std::get<2>(left[i]) << std::endl;
     edges[merged++] =  left[i++];
   }
   else
   {
    // std::cout << "u: " << std::get<0>(right[j]) << " v: " << std::get<2>(right[j]) << std::endl;
     edges[merged++] = right[j++];
   }
 }
 while (i < n1)
 {
   edges[merged++] = left[i++];
 }
 while (j < n2)
 {
   edges[merged++] = right[j++]; 
 }
}	

bool Graph::bellman_ford_shortest_path(int src, Map& path_costs) const
{
  for (int i = 0; i < vertex_count(); i++)
	  path_costs[i] = std::numeric_limits<int>::max();
  path_costs[src] = 0;
  std::vector<std::tuple<int,int,int>> edges;
  for (int i = 0; i < vertex_count(); i++)//build up a vector with edges
    for(int j = 0; j < vertex_count(); j++)
    {
      int weight;
      if (get_edge(i, j, weight))//add all edges to the vector
      {
        std::tuple<int,int,int> edge (std::make_tuple(i,weight,j));
        edges.push_back(edge);
      }
    }

  for (int i = 1; i < vertex_count(); i++)//relax path weight v-1 times
    for (int j = 0; j < edge_count(); j++)
    {
	    int u = std::get<0>(edges[j]);
	    int weight = std::get<1>(edges[j]);
	    int v = std::get<2>(edges[j]);
	    if (path_costs[v] > path_costs[u] + weight)
	      path_costs[v] = path_costs[u] + weight;
    }

  for (int i = 0; i < edge_count(); i++)//check for negative cycles
  {
    int u = std::get<0>(edges[i]);
    int weight = std::get<1>(edges[i]);
    int v = std::get<2>(edges[i]);
    if (path_costs[v] > path_costs[u] + weight)
      return false;//if there is a negative cycle, return false
  }
  return true;
}

bool Graph:: path_max_independent_set(int vertex_weights[], Set& S) const
{
  int a[vertex_count() + 1];
  a[0] = 0;
  a[1] = vertex_weights[0];
  for (int i = 2; i < vertex_count() + 1; i++)
  {
    a[i] = std::max(a[i-1], a[i-2] + vertex_weights[i-1]);//building the array
    //std::cout << a[i] << std::endl; 
  }
  for (int i = vertex_count(); i > 0; i--)//reconstructing to set from resulting array
  {
    if (a[i] > a[i-1])
    {
      S.insert(i-1);
      i--;
    }
  }
  return true;

}

bool Graph:: all_pairs_shortest_paths(std::vector<std::vector<int>>& weights) const
{
  int a[vertex_count() + 1][vertex_count()][vertex_count()];
  for (int i = 0; i < vertex_count(); i++)//setting the initial values with no inner vertex
    for (int j = 0; j < vertex_count(); j++)
    {
      int weight;
      if (i == j)//"self edges" have a weight of 0
      {
        a[0][i][j] = 0;
	std::cout << i << " and " << j << std::endl;
      }
      else if (get_edge(i,j,weight))//if there is an edge, assign its weight
      {
        a[0][i][j] = weight;
	std::cout << i << " and " << j << std::endl;
      }
      else//else infinity
      {
	//if (i == 4 && j == 0)
	 // std::cout << "WE IN HERE ";
        a[0][i][j] = std::numeric_limits<int>::max();
	std::cout << a[0][i][j] << std::endl;
      }
    }
  
  for (int k = 1; k < vertex_count() + 1; k++)
  {
    for (int u = 0; u < vertex_count(); u++)
    {
      for (int v = 0; v < vertex_count(); v++)
      {
      
      if (u == v)//self edges wont change value
        a[k][u][v] = a[k-1][u][v];
      else//update values
      {
        if (a[k-1][u][k] == std::numeric_limits<int>::max() || a[k-1][k][v] == std::numeric_limits<int>::max())
        {
          a[k][u][v] = std::min(a[k-1][u][v], std::numeric_limits<int>::max());
        }
        else
          a[k][u][v] = std::min(a[k-1][u][v], a[k-1][u][k] + a[k-1][k][v]);
        }
        if (u == 4 && v == 0)
	{
	  std::cout << "updated to " << a[k][u][v] << std::endl;
	}
      }
    }
  }

  for (int u = 0; u < vertex_count(); u++)//check for negative cycles
  {
    if (a[vertex_count()][u][u] < 0)
    {
      //std::cout << a[vertex_count()][u][u] << " " << u << std::endl;
      return false;
    }
  }
  
  for (int u = 0; u < vertex_count(); u++)
  {
    weights.push_back(std::vector<int>());
    for (int v = 0; v < vertex_count(); v++)
    {
      weights[u].push_back(a[vertex_count()][u][v]);//add weights to vector
    }
  }
  return true;
      
}

#endif
