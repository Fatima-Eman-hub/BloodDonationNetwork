#ifndef CUSTOM_HASHMAP_HPP
#define CUSTOM_HASHMAP_HPP

#include "CustomVector.hpp"
#include <string>
#include <functional>

// ==================== HASH MAP BASICS ====================
// HashMap jaise phone book hota hai
// Name se directly number mil jata hai - Search karne ki zarurat nahi
// O(1) lookup - Bilkul instant!
//
// Real-world: Dictionary - Word likhte ho to directly meaning mil jata hai
// ===========================================================

// HASH NODE: Ek key-value pair ko store karte hain
// Agar collision ho to next pointer se linked list banate hain
template<typename K, typename V>
class HashNode {
public:
    K key;              // Key - Search ke liye use hota hai
    V value;            // Value - Actual data
    HashNode* next;     // Collision handling - Chain banate hain
    
    // Constructor - Node banate hain
    HashNode(const K& k, const V& v) : key(k), value(v), next(nullptr) {}
};

// CUSTOM HASH MAP CLASS
// Key ko hash kar ke table mein index nikalta hai - O(1) access!
template<typename K, typename V>
class CustomHashMap {
private:
    CustomVector<HashNode<K, V>*> table;  // Hash table - Buckets ki array
    size_t capacity;                       // Table ki size - Kitne buckets hain
    size_t size;                          // Abhi kitne elements add ho gaye
    float load_factor_threshold;          // Kab rehash karni hai - 0.75 par rehash
    
    // FNV-1a HASH FUNCTION: String ko number mein convert karte hain
    // Same string hamishah same number deta hai - Deterministic hai
    // Ye number table ka index banta hai
    size_t hashString(const std::string& str) const {
        size_t hash = 2166136261u; // Magic number se shuru karte hain
        // String ke har character ko process karte hain
        for (char c : str) {
            hash ^= static_cast<size_t>(c); // XOR operation
            hash *= 16777619u;              // Multiply operation - Hash spread karte hain
        }
        return hash; // Final hash code
    }
    
    // HASH FUNCTION: Key ko table index mein convert karte hain
    // String ke liye custom hash function, baki types ke liye std::hash
    size_t hashFunction(const K& key) const {
        if constexpr (std::is_same<K, std::string>::value) {
            // String keys ke liye custom hash use karte hain
            return hashString(key) % capacity; // Modulo se capacity ke andar rakhte hain
        } else {
            // Doosre types ke liye default hash function
            return std::hash<K>{}(key) % capacity;
        }
    }
    
    // REHASH: Jab load factor badh jaye to capacity badha dete hain
    // Tablemein space kam pad jaye to rehashing karte hain
    // Old table ke sab elements ko naye table mein insert karte hain
    void rehash() {
        size_t old_capacity = capacity;
        capacity *= 2; // Capacity double kar do
        
        CustomVector<HashNode<K, V>*> old_table = table;
        table = CustomVector<HashNode<K, V>*>(capacity); // Naya table banao
        
        // Naye table ko null se initialize karo
        for (size_t i = 0; i < capacity; ++i) {
            table.push_back(nullptr);
        }
        
        size = 0; // Size reset kar do
        
        // Purane table se elements nikal kar naye table mein insert karte hain
        for (size_t i = 0; i < old_capacity; ++i) {
            HashNode<K, V>* current = old_table[i];
            // Agar collision chain tha to sab nodes process karte hain
            while (current != nullptr) {
                insert(current->key, current->value); // Naye table mein insert
                HashNode<K, V>* temp = current;
                current = current->next;
                delete temp; // Purana node delete kar do
            }
        }
    }
    
public:
    // CONSTRUCTOR: HashMap banate hain - Initial capacity 16
    CustomHashMap(size_t initial_capacity = 16) 
        : capacity(initial_capacity), size(0), load_factor_threshold(0.75f) {
        table = CustomVector<HashNode<K, V>*>(capacity);
        // Sab buckets ko null se initialize karte hain
        for (size_t i = 0; i < capacity; ++i) {
            table.push_back(nullptr);
        }
    }
    
    // DESTRUCTOR: Memory cleanup - Sab nodes delete kar dete hain
    ~CustomHashMap() {
        // Har bucket mein chain ho sakti hai, sab nodes delete karte hain
        for (size_t i = 0; i < capacity; ++i) {
            HashNode<K, V>* current = table[i];
            while (current != nullptr) {
                HashNode<K, V>* temp = current;
                current = current->next;
                delete temp; // Memory free
            }
        }
    }
    
    // INSERT: Key-value pair add karte hain ya update karte hain
    // Agar key pehle se hai to value update, nahi to naya insert
    void insert(const K& key, const V& value) {
        // Check karte hain - Rehash karna chahiye ya nahi
        if (static_cast<float>(size) / capacity > load_factor_threshold) {
            rehash(); // Zyada load ho gaya, rehash kar do
        }
        
        // Key ka hash calculate kar te hain - Index nikalta hai
        size_t index = hashFunction(key);
        HashNode<K, V>* current = table[index];
        
        // Check karte hain - Ye key pehle se hai ya nahi
        while (current != nullptr) {
            if (current->key == key) {
                // Key mil gya! Value update kar do
                current->value = value;
                return;
            }
            current = current->next;
        }
        
        // Naya node banate hain
        // Chain ke beginning mein add karte hain - O(1) insertion!
        HashNode<K, V>* new_node = new HashNode<K, V>(key, value);
        new_node->next = table[index];
        table[index] = new_node;
        ++size; // Count badha do
    }
    
    // GET: Key se value nikal te hain
    // Hash function se index nikalta hai, phir chain search karte hain
    // Time Complexity: O(1) average case, O(n) worst case (collision chain)
    // Bilkul direct - Key se directly value!
    bool get(const K& key, V& value) const {
        size_t index = hashFunction(key); // Key ka hash - Index milta hai
        HashNode<K, V>* current = table[index];
        
        // Chain ke sab nodes check karte hain
        while (current != nullptr) {
            if (current->key == key) {
                // Key mil gya! Value assign kar do
                value = current->value;
                return true;
            }
            current = current->next;
        }
        return false; // Key nahi mila
    }
    
    // CONTAINS: Check karte hain - Ye key exist karti hai ya nahi
    // Simple lookup - True/False return
    bool contains(const K& key) const {
        size_t index = hashFunction(key);
        HashNode<K, V>* current = table[index];
        
        // Chain ke sab nodes check karte hain
        while (current != nullptr) {
            if (current->key == key) {
                return true; // Key found!
            }
            current = current->next;
        }
        return false; // Key not found
    }
    
    // REMOVE: Key-value pair delete karte hain
    // Chain se node remove karte hain aur memory free karte hain
    bool remove(const K& key) {
        size_t index = hashFunction(key);
        HashNode<K, V>* current = table[index];
        HashNode<K, V>* prev = nullptr;
        
        // Chain search karte hain
        while (current != nullptr) {
            if (current->key == key) {
                // Node ko chain se remove karte hain
                if (prev == nullptr) {
                    // Pehla node tha - bucket update
                    table[index] = current->next;
                } else {
                    // Middle ya end mein tha - pointer adjust
                    prev->next = current->next;
                }
                delete current; // Memory free
                --size; // Count kam kar do
                return true;
            }
            prev = current;
            current = current->next;
        }
        return false; // Key nahi mila
    }
    
    // GET SIZE: Kitne elements hain
    size_t getSize() const { return size; }
    
    // EMPTY CHECK: Koi elements nahi hain ya hain?
    bool empty() const { return size == 0; }
    
    // GET ALL KEYS: Sab keys nikka te hain HashMap se
    // Sab buckets mein se har chain ko traverse karte hain
    // Keys ko vector mein store karte hain
    CustomVector<K> getKeys() const {
        CustomVector<K> keys;
        // Sab buckets iterate karte hain
        for (size_t i = 0; i < capacity; ++i) {
            HashNode<K, V>* current = table[i];
            // Agar collision chain hai to sab nodes se keys collect karte hain
            while (current != nullptr) {
                keys.push_back(current->key); // Key add karte hain
                current = current->next;
            }
        }
        return keys; // Sab keys return
    }
};

// ===========================================================
// HASH MAP SUMMARY - Key Properties:
// ===========================================================
// 1. O(1) Average Case Lookup: Key se directly value mil jata hai
// 2. Hash Function: String/Key ko table index mein convert karta hai
// 3. Collision Handling: Chaining - Multiple keys ek bucket mein chain banate hain
// 4. Load Factor: Jab size/capacity > 0.75, rehash karte hain
// 5. Rehashing: Capacity double karti hai, sab elements re-insert karte hain
// 6. Time Complexities:
//    - Insert: O(1) average, O(n) worst case
//    - Search/Get: O(1) average, O(n) worst case  
//    - Remove: O(1) average, O(n) worst case
//    - Rehash: O(n) but amortized O(1) per insertion
// ===========================================================

#endif // CUSTOM_HASHMAP_HPP
