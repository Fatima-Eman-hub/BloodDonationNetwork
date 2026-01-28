#ifndef CUSTOM_PRIORITY_QUEUE_HPP
#define CUSTOM_PRIORITY_QUEUE_HPP

#include "CustomVector.hpp"
#include <functional>

// ==================== PRIORITY QUEUE BASICS ====================
// Priority Queue jaise hospital waiting room hota hai
// Emergency wale patients pehle dekhe jate hain - Priority se!
// 
// Min-Heap Implementation:
// Sabse chhota element (sabse high priority) top mein rehta hai
// 
// Real-world: Airport queue - Business class wale pehle board karte hain
// Ya hospital - Critical patients pehle treatment milta hai
//
// BINARY HEAP STRUCTURE:
// Complete binary tree - Array mein store karte hain
// Parent = (i-1)/2, Left = 2i+1, Right = 2i+2
//
// APPLICATIONS in Blood Donation System:
// - Emergency requests ko pehle process karte hain
// - Critical blood requirements priority se match karte hain
// ==============================================================

// PRIORITY QUEUE CLASS
// Min-Heap - Sabse chhota element sabse pehle
// Generic Compare function se custom ordering bhi ho sakti hai
template<typename T, typename Compare = std::less<T>>
class CustomPriorityQueue {
private:
    CustomVector<T> heap;  // Array mein heap store karte hain
    Compare comp;          // Comparison function - Priority decide karta hai
    
    // PARENT INDEX: Binary heap mein parent node find karte hain
    // Formula: (index - 1) / 2
    // Index 1,2 ka parent 0, index 3,4 ka parent 1
    size_t parent(size_t index) const {
        return (index - 1) / 2;
    }
    
    // LEFT CHILD: Ek node ke left child ka index
    // Formula: 2 * index + 1
    // Index 0 ke left child 1 hai
    size_t leftChild(size_t index) const {
        return 2 * index + 1;
    }
    
    // RIGHT CHILD: Ek node ke right child ka index
    // Formula: 2 * index + 2
    // Index 0 ke right child 2 hai
    size_t rightChild(size_t index) const {
        return 2 * index + 2;
    }
    
    // HEAPIFY UP: Bottom se top tak element move karte hain
    // Jab element add karte hain to sahi position mein aata hai
    // Min-heap maintain karte hain - Parent < Child
    // Time Complexity: O(log n) - Tree ki height jitna
    void heapifyUp(size_t index) {
        // Jab tak root nahi pahunch jate aur parent se chhota ho
        while (index > 0 && comp(heap[index], heap[parent(index)])) {
            // Swap - Element apne parent ke saath swap karte hain
            T temp = heap[index];
            heap[index] = heap[parent(index)];
            heap[parent(index)] = temp;
            // Parent index par move karte hain
            index = parent(index);
        }
    }
    
    // HEAPIFY DOWN: Top se bottom tak element move karte hain
    // Jab top element remove karte hain to last element top par aata hai
    // Phir heapify down karte hain sahi position find karne ke liye
    // Min-Heap maintain karte hain
    // Time Complexity: O(log n) - Tree height
    void heapifyDown(size_t index) {
        size_t size = heap.getSize();
        size_t smallest = index; // Pehle current element smallest assume
        size_t left = leftChild(index);
        size_t right = rightChild(index);
        
        // Check left child - Agar smaller hai to smallest update
        if (left < size && comp(heap[left], heap[smallest])) {
            smallest = left;
        }
        
        // Check right child - Agar smallest se smaller hai to update
        if (right < size && comp(heap[right], heap[smallest])) {
            smallest = right;
        }
        
        // Agar smallest change hua to swap aur recursively heapify down
        if (smallest != index) {
            T temp = heap[index];
            heap[index] = heap[smallest];
            heap[smallest] = temp;
            heapifyDown(smallest); // Recursive call
        }
    }
    
public:
    // CONSTRUCTOR: Default Compare function
    CustomPriorityQueue() : comp(Compare()) {}
    
    // CONSTRUCTOR: Custom Compare function se
    // Different ordering ke liye - Max-heap bhi bana sakte hain
    explicit CustomPriorityQueue(const Compare& cmp) : comp(cmp) {}
    
    // PUSH: Element add karte hain aur priority maintain karte hain
    // Insert at end, phir heapify up - O(log n) time
    // Heap property maintain rehti hai
    void push(const T& value) {
        heap.push_back(value); // Vector ke end mein add
        heapifyUp(heap.getSize() - 1); // New element ko sahi position par bhej
    }
    
    // POP: Sabse high priority element remove karte hain
    // Root (index 0) remove karte hain
    // Last element ko root par rakhte hain, phir heapify down
    // O(log n) time - Heap re-organize karte hain
    void pop() {
        if (heap.empty()) {
            throw std::runtime_error("Priority queue is empty");
        }
        
        // Last element ko root par rakhte hain
        heap[0] = heap[heap.getSize() - 1];
        heap.pop_back(); // Last element remove
        
        // Agar abhi elements hain to heapify down
        if (!heap.empty()) {
            heapifyDown(0);
        }
    }
    
    // TOP: Sabse high priority element dekh sakte hain - Remove nahi
    // Direct root return - O(1) operation
    // Just peek - Is element dekh sakte hain, remove nahi
    const T& top() const {
        if (heap.empty()) {
            throw std::runtime_error("Priority queue is empty");
        }
        return heap[0]; // Root element return
    }
    
    // EMPTY CHECK: Koi elements nahi hain?
    bool empty() const {
        return heap.empty();
    }
    
    // SIZE: Kitne elements hain queue mein?
    size_t size() const {
        return heap.getSize();
    }
};

// ===========================================================
// PRIORITY QUEUE SUMMARY - Key Properties:
// ===========================================================
// 1. Min-Heap by Default: Smallest element (highest priority) top mein
// 2. Complete Binary Tree: Array mein efficiently store
// 3. O(log n) Push: Element add karte hain aur heapify up
// 4. O(log n) Pop: Top element remove aur heapify down
// 5. O(1) Top: Peek operation - High priority element dekh sakte hain
// 6. Heap Property: Parent < Child (min-heap)
// 7. Array Representation: Binary tree ko array ke saath represent
// 8. Parent-Child Relations:
//    - Parent(i) = (i-1)/2
//    - LeftChild(i) = 2i+1
//    - RightChild(i) = 2i+2
// 9. Applications:
//    - Dijkstra's algorithm (shortest path)
//    - Huffman coding
//    - Load balancing (priority scheduling)
//    - Blood matching (urgent requests first)
// ===========================================================

#endif // CUSTOM_PRIORITY_QUEUE_HPP
