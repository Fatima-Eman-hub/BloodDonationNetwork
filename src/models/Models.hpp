#ifndef MODELS_HPP
#define MODELS_HPP

#include <string>
#include "../dsa/CustomVector.hpp"

struct Donor {
    std::string id;
    std::string name;
    int age;
    std::string gender;
    std::string cnic;
    std::string email;
    std::string phone;
    std::string address;
    std::string city;
    std::string area;
    std::string bloodGroup;
    std::string status; // Available/Unavailable/Busy
    std::string lastDonationDate;
    int totalDonations;
    std::string badgeLevel; // Bronze/Silver/Gold/Platinum/Hero
    bool isVerified;
    CustomVector<std::string> medicalConditions;
    std::string nextEligibleDate;
    std::string locationNodeId;
    std::string passwordHash;
    
    Donor() : age(0), totalDonations(0), isVerified(false) {}
};

struct Recipient {
    std::string id;
    std::string patientName;
    std::string patientId;
    std::string bloodGroupNeeded;
    std::string urgency; // Immediate/High/Medium/Low
    std::string locationType; // Hospital/Lab/Home
    std::string hospitalName;
    std::string locationNodeId;
    std::string contactPerson;
    std::string contactPhone;
    std::string status; // Pending/Matched/Completed/Cancelled
    std::string timestamp;
    std::string matchedDonorId;
    std::string createdByUserId;
    int age;
    std::string medicalCondition;
    int unitsNeeded;
    
    Recipient() : age(0), unitsNeeded(1) {}
    
    // Urgency priority (lower = higher priority)
    int getUrgencyPriority() const {
        if (urgency == "Immediate") return 1;
        if (urgency == "High") return 2;
        if (urgency == "Medium") return 3;
        if (urgency == "Low") return 4;
        return 5;
    }
};

struct Transaction {
    std::string id;
    std::string donorId;
    std::string recipientId;
    std::string bloodGroup;
    int units;
    std::string hospitalId;
    double distance;
    std::string matchTime;
    std::string travelTime;
    std::string status;
    bool receiptGenerated;
    std::string timestamp;
    
    Transaction() : units(1), distance(0.0), receiptGenerated(false) {}
};

struct HospitalNode {
    std::string nodeId;
    std::string name;
    std::string type; // Hospital/Lab/DonorHub
    std::string address;
    std::string city;
    int x;
    int y;
    
    HospitalNode() : x(0), y(0) {}
};

// Comparator for priority queue (min-heap by urgency)
struct RecipientUrgencyComparator {
    bool operator()(const Recipient* a, const Recipient* b) const {
        return a->getUrgencyPriority() < b->getUrgencyPriority();
    }
};

#endif // MODELS_HPP
