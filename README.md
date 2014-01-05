#GraphLib
Open source implementation of important graph algorithms. The design objective of this library is to keep the interface as simple as possible for an average user while ensuring modular and extensibility for advanced user to write their own algorithm using this as base.

## User Interfaces
A user of library can call a simple Interface `Graph<V> graph` where `V` is the type of user's data(referred to as Value). 

Example of simple instantiation is:

```cpp
// creates an undirected graph
Graph<int> graph
```
By default the graph is undirected. To create a directed Graph, user can pass `directed=true` to constructor.

```cpp
// creates a directed graph
Graph<int> graph(true);
```
###Node
A node can be created by simply calling constructor of Node class. The node is a generic container for user's data in graph. The constructor call to node is

```cpp
// Create data to be held by node
/*Note: Data is a user defined data type.
 It can be be a primitive data type or can be user defined. */
Data data;
// Create node
Node<Data> node(data);
```

User can associated a label with node for ease of identification. It can be done by passing a `string` label to constructor.

```cpp
// Create data to be held by node
Data data;
// Create node
Node<Data> node(data, "label");
```

Each node has a unique id attached to id. This is assigned at the time of construction and can be queried using `getId()` function. Read Only interfaces exposed by Node are

```cpp
int getId();
Data& getValue();
string getLabel();
void printNode();
```
To print Node call `printNode()`. 
Default label can be accessed by `Node::DEFAULT_LABEL`.

###Edge
Edge is a read only quantity to the user as edges are created by using Graph `createEdge(node1, node2, weight)` interface.
The api exposed by edge is 

```cpp
int getCurrentNodeId();  // id of source Node
int getOtherNodeId();   // id of destination node
bool isDirected();	// if edge is directed
float getWeight(); 	
int getId();
void printEdge();
```

###Graph API
1. Insert a node

   To insert, first you need to create a node and pass it to `insertNode`.
  
  ```cpp
     // inserting unlabelled node
     Node<Data> node(Data); 
     graph.insertNode(node);
 
    // inserting labelled node
     Node<Data> node1(Data, 'label');
     graph.insertNode(node1);
  ```

2. Create an edge
   
   To create an edge you can to call `createEdge` function. The arguments are two nodes that are connected via edge and weight if needed.    
   Notice that the edge will be directed if graph is directed.

   ```cpp
      graph.createEdge(node1, node2);
      graph.createEdge(node1, node2, 10.9); // 10.9 is weight of the edge.	
   ```
3. Read Only API's

    Readonly API's helps you query the information about the graph like number of nodes, number of edges, out degree for a node etc.

  ```cpp
     bool isDirected(); // tell if graph is directed or not.
     int getNodeCount(); // number of nodes in Graph
     int getEdgeCount(); // number of edges in Graph
     bool constainsNode(node); // is node present in Graph
     vector<Edge>& getOutEdgesForNode(node); // Edge emanating from node
     int getInDegreeForNode(node); // in degree on node
     int getOutDegreeForNode(node); // out degree of node
     void printGraph(); // prints adj form of graph.
  ```

4. Iterators
   Graph provides iterator to iterate over the nodes present in Graph. You have both `iterator` i.e. normal random bi-directional iterator as well as a `const_iterator` which is a const iterator. 
   To support the iterators you have `begin()` and `cbegin()` to get beginning and `end()` and `cend()` for end of graph vertices for `iterator` and `const_iterator` respectively. 

   ```cpp
	for (iterator it = graph.begin(); it != graph.end(); it++) {
      		vector<Edge> egdes = graph.getOutEdgesForNode(*it);
		it->printNode();
		for (vector<Edge>::iterator it1 = edges.begin(); it1 = edges.end(); it1++) {
			it1->printEdge();
			graph.getNodeById(it1->getOtherNodeId()).printNode();
        }
 }

	for (const_iterator it = graph.begin(); it != graph.end(); it++) {
      		it->printNode();
 }
   ```

5. Reset
 1. Soft Reset

     This can be called by calling `graph.reset()`. This removes all the state data keeping the structure of graph i.e. nodes and edges intact.

 2. Hard Reset

    This is called using interface `graph.reset(Graph::HARD_RESET)`. The removes all edges and nodes and gives you a clean graph to start again.

6. Algorithms

   As of now following algorithms are implemented. The list will keep growing till most of the important and well known algorithms are covered.

  ```cpp
        
        // checks if graph is cyclic.   
        bool isCyclic();  
        // topologically sort vertices.  
        // By default vertices are sorted by insertion order.  
        void topsort();  
        // Does Breadth First Search On Graph starting from Node node.  
        void breadthFirstSearch(node);   
        // Does a Depth First Search  
        void depthFirstSearch();  
        //  Breaks down a graph into multiple strongly connected graphs.
        ComponentGraph& stronglyConnectedComponents();
  ```
