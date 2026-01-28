#ifndef MATCHING_ENGINE_HPP
#define MATCHING_ENGINE_HPP

#include "../dsa/CustomPriorityQueue.hpp"
#include "../dsa/CustomHashMap.hpp"
#include "../dsa/CustomVector.hpp"
#include "../dsa/CustomGraph.hpp"
#include "../models/Models.hpp"
#include "BloodCompatibility.hpp"
#include <limits>

// Matching Engine - Donor aur Recipient ko ek dusre se match karte hain
// Sabse behtar donor nikal te hain recipient ke liye
class MatchingEngine {
private:
    // Priority queue mein recipients ko store karte hain - Urgent wale pehle
    CustomPriorityQueue<Recipient*, RecipientUrgencyComparator> recipientQueue;
    // Blood group wise donors ke liye HashMap - O+ mein sab O+ donors rehte hain
    CustomHashMap<std::string, CustomVector<Donor*>> donorMap; // bloodGroup -> donors
    // Location graph - Cities aur hospitals ka connection
    CustomGraph* locationGraph;
    // Blood compatibility checker
    BloodCompatibility compatibility;
    
public:
    // Constructor - graph pointer pass karte hain
    MatchingEngine(CustomGraph* graph) : locationGraph(graph) {}
    
    // Recipient request queue mein add karte hain
    // Priority queue ko automatic sort kar dega urgency ke hisaab se
    void addRecipientRequest(Recipient* recipient) {
        recipientQueue.push(recipient);
    }
    
    // Donor ko donorMap mein add karte hain - Blood group wise
    void addDonor(Donor* donor) {
        CustomVector<Donor*> donors;
        // Pehle check karte hain ke is blood group ke donors pehle se hain ya nahi
        if (donorMap.get(donor->bloodGroup, donors)) {
            // Agar pehle se hain to new donor add kar do
            donors.push_back(donor);
            donorMap.insert(donor->bloodGroup, donors);
        } else {
            // Agar nahi hain to naya vector banao aur donor add kar do
            CustomVector<Donor*> newDonors;
            newDonors.push_back(donor);
            donorMap.insert(donor->bloodGroup, newDonors);
        }
    }
    
    // Donor ko remove karte hain - Shayd busy ho gaya ya donation de diya
    void removeDonor(const std::string& donorId, const std::string& bloodGroup) {
        CustomVector<Donor*> donors;
        // Pehle us blood group ke sab donors nikal te hain
        if (donorMap.get(bloodGroup, donors)) {
            CustomVector<Donor*> updated;
            // Sab donors ko loop karte hain
            for (size_t i = 0; i < donors.getSize(); ++i) {
                // Agar ye donor nahi hai jo remove karna hai to add kar do
                if (donors[i]->id != donorId) {
                    updated.push_back(donors[i]);
                }
            }
            // Updated list ko back insert karte hain
            donorMap.insert(bloodGroup, updated);
        }
    }
    
    // Ye sabse important function hai - Best donor find karte hain recipient ke liye
    Donor* findBestDonorFor(Recipient* recipient) {
        // Pehle check karte hain ke konse blood types compatible hain
        CustomVector<std::string> compatibleTypes = compatibility.getCompatibleDonors(recipient->bloodGroupNeeded);
        
        Donor* bestDonor = nullptr;
        double minDistance = std::numeric_limits<double>::infinity(); // Sabse zyada distance se shuru
        
        // Sab compatible blood types ke liye
        for (size_t i = 0; i < compatibleTypes.getSize(); ++i) {
            CustomVector<Donor*> donors;
            // Us blood group ke sab donors nikal te hain
            if (donorMap.get(compatibleTypes[i], donors)) {
                // Har donor ko check karte hain
                for (size_t j = 0; j < donors.getSize(); ++j) {
                    // Agar donor available hai tab hi check karte hain
                    if (donors[j]->status == "Available") {
                        // Dijkstra algorithm use karke distance nikal te hain
                        // Donor ke location se recipient ke location tak
                        auto result = locationGraph->dijkstra(donors[j]->locationNodeId, recipient->locationNodeId);
                        
                        // Agar ye distance pehle se chhota hai to ye best donor hai
                        if (result.distance < minDistance) {
                            minDistance = result.distance;
                            bestDonor = donors[j];
                        }
                    }
                }
            }
        }
        
        return bestDonor; // Sabse paas wala donor return karte hain
    }
    
    // Match karne mein kitna time lagega ye estimate karte hain
    int estimateMatchTime(Recipient* recipient) {
        // Base time 5 minutes
        int baseTime = 5; // 5 minutes base
        // Urgency se multiply karte hain - Zyada urgent = zyada jaldi
        int urgencyMultiplier = recipient->getUrgencyPriority();
        return baseTime * urgencyMultiplier;
    }
    
    // Ek blood group ke sab available donors return karte hain
    CustomVector<Donor*> getAllAvailableDonors(const std::string& bloodGroup) {
        CustomVector<Donor*> result;
        CustomVector<Donor*> donors;
        // Pehle us blood group ke sab donors nikal te hain
        if (donorMap.get(bloodGroup, donors)) {
            // Ab sirf available donors ko result mein add karte hain
            for (size_t i = 0; i < donors.getSize(); ++i) {
                if (donors[i]->status == "Available") {
                    result.push_back(donors[i]);
                }
            }
        }
        return result; // Available donors ki list return karte hain
    }
};

#endif // MATCHING_ENGINE_HPP
