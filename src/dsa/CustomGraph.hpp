#ifndef CUSTOM_GRAPH_HPP
#define CUSTOM_GRAPH_HPP

#include "CustomVector.hpp"
#include "CustomHashMap.hpp"
#include "CustomPriorityQueue.hpp"
#include <string>
#include <limits>           //or infinity values in shortest path algorithm
#include <utility>          //for pair data structure in priority queue//

// ==================== GRAPH BASICS ====================
// Graph jaise map hota hai - Cities aur roads
// Nodes = Cities, Edges = Roads between cities
//
// Real-world Application in Blood Donation:
// - Nodes = Blood centers/hospitals (locations)
// - Edges = Distance between centers
// - Find shortest path from donor to recipient center
// - Dijkstra algorithm se fastest route determine karte hain
//  Positive Weights: Aap ke graph mein weight ka matlab hai
//  distance ya time.
//  Distance kabhi negative nahi ho sakta. Dijkstra isi cheez ka 
//  faida uthata hai.
//
// ADJACENCY LIST REPRESENTATION:
// Har node ke liye linked list rakhte hain
// Neighbors aur weights store karte hain
// Kyunke aap ke graph mein har hospital ya donor location har 
// doosri location se nahi judi, isliye Adjacency List ne aap ki
// memory bacha li.
// ===========================================================

class CustomGraph {
private:
    // EDGE STRUCTURE: Ek connection ko represent karte hain
    // Weighted edge - Distance ya cost assign karte hain
    struct Edge {           //nested structs
        int to;           // Target node ka index
        double weight;    // Distance/cost - "5 km door hai" type
        Edge* next;       // Next edge (adjacency list)
        
        // Constructor
        Edge(int t, double w) : to(t), weight(w), next(nullptr) {}
    };
    
    // NODE STRUCTURE: Ek location/center ko represent karte hain
    struct Node {
        std::string id;     // Unique identifier - "H1", "C2" type
        std::string name;   // Hospital/Center ka name
        std::string type;   // Hospital/BloodCenter type
        int x, y;           // Geographic coordinates - Map par location
        Edge* edges;        // Linked list of outgoing edges
        
        // Default Constructor
        Node() : x(0), y(0), edges(nullptr) {}
        
        // Parameterized Constructor
        Node(const std::string& _id, const std::string& _name, const std::string& _type, int _x = 0, int _y = 0)
            : id(_id), name(_name), type(_type), x(_x), y(_y), edges(nullptr) {}
    };
    
    CustomVector<Node*> nodes;                      // Sab nodes ka vector
    CustomHashMap<std::string, int> nodeIndex;     // "H1" -> index lookup
    
public:
    // SHORTEST PATH RESULT STRUCTURE
    struct ShortestPathResult {
        double distance;                // Total distance ya cost
        CustomVector<std::string> path; // Path jo follow karna hai
        
        ShortestPathResult() : distance(std::numeric_limits<double>::infinity()) {}
    };
    
    // CONSTRUCTOR
    CustomGraph() {}
    
    // DESTRUCTOR: Memory cleanup - Sab nodes aur edges delete
    ~CustomGraph() {
        for (size_t i = 0; i < nodes.getSize(); ++i) {
            Edge* current = nodes[i]->edges;
            // Sab edges ka linked list delete
            while (current != nullptr) {
                Edge* temp = current;
                current = current->next;
                delete temp; // Memory free
            }
            delete nodes[i]; // Node delete
        }
    }
    
    // ADD NODE: Graph mein naya location add karte hain
    // Hospital/Center ko map par mark karte hain
    // Coordinates (x,y) define karte hain - Map par position
    void addNode(const std::string& id, const std::string& name, const std::string& type, int x = 0, int y = 0) {
        if (nodeIndex.contains(id)) {
            return; // Already exists - Duplicate nahi add
        }
        
        // Naya node banao
        Node* new_node = new Node(id, name, type, x, y);
        nodeIndex.insert(id, nodes.getSize()); // HashMap mein index store
        nodes.push_back(new_node);
    }
    
    // ADD EDGE: Dono locations ke beech connection banate hain
    // Undirected graph - Dono taraf se ja sakte ho
    // Weight = distance between locations
    void addEdge(const std::string& from, const std::string& to, double weight) {
        int from_idx, to_idx;
        // Check karte hain - Dono nodes exist karte hain?
        if (!nodeIndex.get(from, from_idx) || !nodeIndex.get(to, to_idx)) {
            return; // Invalid nodes
        }
        
        // ADD FROM -> TO EDGE
        Edge* new_edge = new Edge(to_idx, weight);
        new_edge->next = nodes[from_idx]->edges;
        nodes[from_idx]->edges = new_edge;
        
        // ADD TO -> FROM EDGE (Undirected - Dono taraf connection)
        Edge* reverse_edge = new Edge(from_idx, weight);
        reverse_edge->next = nodes[to_idx]->edges;
        nodes[to_idx]->edges = reverse_edge;
    }
    
    // DIJKSTRA'S ALGORITHM: Sabse chotta path find karte hain
    // Start se End tak shortest distance nikalta hai
    // + Path bhi return karte hain "H1 -> C2 -> H5" type
    //
    // Algorithm:
    // 1. Sab distances ko infinity set karte hain
    // 2. Start node ka distance 0
    // 3. Priority queue mein start node add
    // 4. Jab tak end node nahi mil jata tab tak traverse
    // 5. Har neighbor ke distance check karte hain aur update
    //
    // Time Complexity: O((V+E) log V)
    // V = vertices (hospitals), E = edges (roads)
    ShortestPathResult dijkstra(const std::string& start, const std::string& end) {
        ShortestPathResult result;
        
        int start_idx, end_idx;
        if (!nodeIndex.get(start, start_idx) || !nodeIndex.get(end, end_idx)) {
            return result; // Invalid nodes
        }
        
        size_t n = nodes.getSize();
        CustomVector<double> dist;           // Start se har node ka distance
        CustomVector<int> parent;            // Path reconstruction - Parent track
        CustomVector<bool> visited;          // Processed nodes - Processed kar chuke
        
        // Initialization
        for (size_t i = 0; i < n; ++i) {
            dist.push_back(std::numeric_limits<double>::infinity());
            parent.push_back(-1);
            visited.push_back(false);
        }
        
        dist[start_idx] = 0; // Start point se apna distance 0
        
        // Min-heap: (distance, node_index) - Sabse chhota distance pehle
        auto comp = [](const std::pair<double, int>& a, const std::pair<double, int>& b) {
            return a.first > b.first; // Min-heap ke liye greater than
        };
        CustomPriorityQueue<std::pair<double, int>, decltype(comp)> pq(comp);
        pq.push({0, start_idx}); // Start mein start node add
        
        // Main loop - Jab tak priority queue empty nahi
        while (!pq.empty()) {
            auto [d, u] = pq.top(); // Sabse chhote distance wala node nikalo
            pq.pop();
            
            if (visited[u]) continue; // Already processed - Skip
            visited[u] = true;        // Mark as visited
            
            if (u == end_idx) break;  // Destination mil gya - Stop
            
            // Sab neighbors check karte hain
            Edge* edge = nodes[u]->edges;
            while (edge != nullptr) {
                int v = edge->to;
                double weight = edge->weight;
                
                // Relaxation: Shorter path mil gya to update
                if (!visited[v] && dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight; // Distance update
                    parent[v] = u;              // Parent track karo
                    pq.push({dist[v], v});     // Priority queue mein add
                }
                
                edge = edge->next;
            }
        }
        
        // PATH RECONSTRUCTION: Start se end tak path build karte hain
        if (dist[end_idx] != std::numeric_limits<double>::infinity()) {
            result.distance = dist[end_idx]; // Total distance
            
            // Backward path - End se start tak parent follow
            CustomVector<int> path_indices;
            int current = end_idx;
            while (current != -1) {
                path_indices.push_back(current);
                current = parent[current];
            }
            
            // Reverse - Start se end tak path order karte hain
            for (int i = path_indices.getSize() - 1; i >= 0; --i) {
                result.path.push_back(nodes[path_indices[i]]->id);
            }
        }
        
        return result;
    }
    
    // BFS TRAVERSAL: Breadth-First Search
    // Level by level sab nodes visit karte hain
    // Dekhna: "H1 se H5 tak kaun kaun se centers pass karte hain"
    // Time Complexity: O(V + E)
    CustomVector<std::string> bfs(const std::string& start) {
        CustomVector<std::string> result;
        int start_idx;
        if (!nodeIndex.get(start, start_idx)) {
            return result;
        }
        
        size_t n = nodes.getSize();
        CustomVector<bool> visited;
        // Sab nodes ko initially unvisited mark karte hain
        for (size_t i = 0; i < n; ++i) {
            visited.push_back(false);
        }
        
        // Queue - FIFO - Pehle wala nikale, baad mein add
        CustomVector<int> queue;
        queue.push_back(start_idx);
        visited[start_idx] = true;
        
        // BFS loop
        size_t front = 0; // Queue ka front pointer
        while (front < queue.getSize()) {
            int u = queue[front++]; // Current node nikalo
            result.push_back(nodes[u]->id); // Result mein add
            
            // Sab neighbors add karte hain queue mein
            Edge* edge = nodes[u]->edges;
            while (edge != nullptr) {
                int v = edge->to;
                if (!visited[v]) {
                    visited[v] = true; // Mark visited
                    queue.push_back(v); // Queue mein add
                }
                edge = edge->next;
            }
        }
        
        return result;
    }
    
    // GET NODE COUNT: Kitne centers/hospitals hain
    size_t getNodeCount() const {
        return nodes.getSize();
    }
    
    // GET NODE NAME: ID se name nikalo
    std::string getNodeName(const std::string& id) const {
        int idx;
        if (nodeIndex.get(id, idx)) {
            return nodes[idx]->name;
        }
        return "";
    }
};

// ===========================================================
// GRAPH SUMMARY - Key Properties:
// ===========================================================
// 1. Adjacency List: Har node ke liye linked list edges
// 2. Undirected Graph: Dono taraf se connected nodes
// 3. Weighted Edges: Roads mein distance/cost
// 4. Dijkstra Algorithm:
//    - Shortest path find karte hain
//    - O((V+E)logV) time complexity
//    - Priority queue use karte hain
//    - Greedy approach - Locally optimal choice
// 5. BFS Traversal:
//    - Level by level nodes visit
//    - O(V+E) time complexity
//    - Queue use karte hain (FIFO)
// 6. Node Structure: ID, Name, Type, Coordinates
// 7. Edge Structure: Target node, Weight (distance)
// 8. Applications:
//    - Navigation/Maps (find shortest route)
//    - Social networks (friend suggestions)
//    - Flight routes (best path with stops)
//    - Blood network (nearest blood centers)
// ===========================================================

#endif // CUSTOM_GRAPH_HPP
