#ifndef CUSTOM_LINKED_LIST_HPP
#define CUSTOM_LINKED_LIST_HPP

#include <stdexcept>

// ==================== LINKED LIST BASICS ====================
// Linked List jaise chain hota hai - phone se phone mein joda
// Har node apna data aur next/prev node ka address store karta hai
//
// Real-world: Train wagons - Ek wagon ke peeche doosra wagon
// Wagons ko connect karte hain - Bilkul linked list!
//
// ADVANTAGES vs Array:
// - O(1) insertion at front (array mein O(n) hai!)
// - Flexible size - capacity nahi badhani padhti
// - No memory waste - Sirf jitna chahiye utna use
//
// DISADVANTAGES:
// - O(n) random access (array mein O(1))
// - Extra memory for pointers
// ==============================================================

// LIST NODE: Ek element ko store karte hain
// Prev aur next pointers se double linked list banate hain
template<typename T>
class ListNode {
public:
    T data;          // Actual data
    ListNode* next;  // Next node ka address
    ListNode* prev;  // Previous node ka address (double linked)
    
    // Constructor - Node banate hain
    ListNode(const T& value) : data(value), next(nullptr), prev(nullptr) {}
};

// CUSTOM LINKED LIST CLASS
// Insert at front O(1), delete from front O(1)
// Ye Vector se bilkul different hai
template<typename T>
class CustomLinkedList {
private:
    ListNode<T>* head;  // List ka pehla node
    ListNode<T>* tail;  // List ka akhri node
    size_t size;        // Kitne elements add ho gaye
    
public:
    // CONSTRUCTOR: Empty list banate hain
    CustomLinkedList() : head(nullptr), tail(nullptr), size(0) {}
    
    // DESTRUCTOR: Memory cleanup - Sab nodes delete
    ~CustomLinkedList() {
        clear();
    }
    
    // PUSH BACK: End mein element add karte hain - O(1) operation
    // Direct tail pointer se tail update - Bilkul fast!
    void push_back(const T& value) {
        ListNode<T>* new_node = new ListNode<T>(value); // Naya node banao
        if (tail == nullptr) {
            // List empty hai - Pehla element
            head = tail = new_node;
        } else {
            // List mein pehle se elements hain
            tail->next = new_node;     // Purane tail ke next mein naya node
            new_node->prev = tail;     // Naye node ka prev purana tail
            tail = new_node;           // Tail update
        }
        ++size; // Count badhao
    }
    
    // PUSH FRONT: Starting mein element add karte hain - O(1) operation!
    // Vector mein ye O(n) tha, LinkedList mein O(1) hai - Alag hi baat!
    void push_front(const T& value) {
        ListNode<T>* new_node = new ListNode<T>(value); // Naya node
        if (head == nullptr) {
            // List empty hai - Pehla element
            head = tail = new_node;
        } else {
            // List mein pehle se elements
            new_node->next = head;     // Naye node ka next purana head
            head->prev = new_node;     // Purane head ka prev naya node
            head = new_node;           // Head update
        }
        ++size; // Count badhao
    }
    
    // POP BACK: End se element remove - O(1) operation
    // Tail pointer se efficiently remove kar sakte hain
    void pop_back() {
        if (tail == nullptr) return; // Empty hai - kuch nahi kar sakte
        
        ListNode<T>* to_delete = tail; // Remove karne wala node
        if (head == tail) {
            // Sirf ek element tha
            head = tail = nullptr;
        } else {
            // Multiple elements hain
            tail = tail->prev;          // Tail ko pehle element par move karo
            tail->next = nullptr;       // Last node ko remove karo
        }
        delete to_delete; // Memory free
        --size; // Count kam karo
    }
    
    // POP FRONT: Starting se element remove - O(1) operation
    // Head pointer se efficiently remove
    void pop_front() {
        if (head == nullptr) return; // Empty - kuch nahi kar sakte
        
        ListNode<T>* to_delete = head; // Remove karne wala node
        if (head == tail) {
            // Sirf ek element tha
            head = tail = nullptr;
        } else {
            // Multiple elements
            head = head->next;          // Head ko next element par move
            head->prev = nullptr;       // Pehle node ko disconnect
        }
        delete to_delete; // Memory free
        --size; // Count kam karo
    }
    
    // EMPTY CHECK: Koi elements nahi hain?
    bool empty() const { return size == 0; }
    
    // GET SIZE: Kitne elements hain
    size_t getSize() const { return size; }
    
    // CLEAR: Sab elements delete kar do - Naya shuruaat
    void clear() {
        while (head != nullptr) {
            ListNode<T>* temp = head;
            head = head->next;
            delete temp; // Memory free
        }
        tail = nullptr;
        size = 0;
    }
    
    // ITERATOR CLASS: List ko iterate karte hain
    // Index use na karte iterate - LinkedList friendly
    class Iterator {
    private:
        ListNode<T>* current; // Current position
    public:
        Iterator(ListNode<T>* node) : current(node) {}
        
        // DEREFERENCE: Current element ka value
        T& operator*() { return current->data; }
        const T& operator*() const { return current->data; }
        
        // INCREMENT: Next element par move - ++it
        Iterator& operator++() {
            if (current) current = current->next; // Agar valid hai to next element
            return *this;
        }
        
        // COMPARISON: Dono iterators alag position mein hain?
        bool operator!=(const Iterator& other) const {
            return current != other.current;
        }
        
        // EQUALITY CHECK: Same position mein hain?
        bool operator==(const Iterator& other) const {
            return current == other.current;
        }
    };
    
    // BEGIN: List ka pehla element
    // Iteration ke liye start point
    Iterator begin() { return Iterator(head); }
    
    // END: List ke baad (nullptr)
    // Iteration ka end point - Jab tak yahan tak nahi pahunch jate
    Iterator end() { return Iterator(nullptr); }
};

// ===========================================================
// LINKED LIST SUMMARY - Key Properties:
// ===========================================================
// 1. O(1) Front Insertion: push_front sirf head update
// 2. O(1) Front Deletion: pop_front sirf head change
// 3. O(1) Back Operations: push_back/pop_back direct tail se
// 4. O(n) Random Access: Index se access nahi - chain traverse
// 5. No Capacity Issues: Size flexible - jab chahiye badha sakte hain
// 6. Double Linked: Prev/Next dono se traverse kar sakte hain
// 7. Iterator Pattern: Easy iteration without indexing
// 8. Time Complexities:
//    - Insert at front: O(1)
//    - Insert at back: O(1)
//    - Delete from front: O(1)
//    - Delete from back: O(1)
//    - Search: O(n) - Linear search
//    - Access by index: O(n) - Chain traverse
// ===========================================================

#endif // CUSTOM_LINKED_LIST_HPP
