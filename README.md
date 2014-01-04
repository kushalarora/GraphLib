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
 
