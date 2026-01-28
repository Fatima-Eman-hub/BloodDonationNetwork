#ifndef BLOOD_COMPATIBILITY_HPP
#define BLOOD_COMPATIBILITY_HPP

#include "../dsa/CustomHashMap.hpp"
#include "../dsa/CustomVector.hpp"
#include <string>

// Khoon ki compatibility check karte hain
// Matlab donor ka khoon recipient ko de sakta hai ya nahi
class BloodCompatibility {
private:
    // Donor se recipient tak mapping - Donor konse recipients ko khoon de sakta hai
    CustomHashMap<std::string, CustomVector<std::string>> donorToRecipient;
    // Recipient ko konse donors se khoon mil sakta hai
    CustomHashMap<std::string, CustomVector<std::string>> recipientToDonor;
    
public:
    BloodCompatibility() {
        // Constructor mein compatibility initialize karte hain
        initializeCompatibility();
    }
    
    void initializeCompatibility() {
        // O+ mein khoon - Ye O+, A+, B+, AB+ ko de sakta hai
        CustomVector<std::string> oPos;
        oPos.push_back("O+");
        oPos.push_back("A+");
        oPos.push_back("B+");
        oPos.push_back("AB+");
        donorToRecipient.insert("O+", oPos);
        
        // O- universal donor hai - Ye sab ko de sakta hai
        // Universal matlab sabko de sakte hain
        CustomVector<std::string> oNeg;
        oNeg.push_back("O+");
        oNeg.push_back("O-");
        oNeg.push_back("A+");
        oNeg.push_back("A-");
        oNeg.push_back("B+");
        oNeg.push_back("B-");
        oNeg.push_back("AB+");
        oNeg.push_back("AB-");
        donorToRecipient.insert("O-", oNeg);
        
        // A+ blood group - Ye A+ aur AB+ ko de sakta hai
        CustomVector<std::string> aPos;
        aPos.push_back("A+");
        aPos.push_back("AB+");
        donorToRecipient.insert("A+", aPos);
        
        // A- blood group - Ye A+, A-, AB+, AB- ko de sakta hai
        CustomVector<std::string> aNeg;
        aNeg.push_back("A+");
        aNeg.push_back("A-");
        aNeg.push_back("AB+");
        aNeg.push_back("AB-");
        donorToRecipient.insert("A-", aNeg);
        
        // B+ blood group - Ye B+ aur AB+ ko de sakta hai
        CustomVector<std::string> bPos;
        bPos.push_back("B+");
        bPos.push_back("AB+");
        donorToRecipient.insert("B+", bPos);
        
        // B- blood group - Ye B+, B-, AB+, AB- ko de sakta hai
        CustomVector<std::string> bNeg;
        bNeg.push_back("B+");
        bNeg.push_back("B-");
        bNeg.push_back("AB+");
        bNeg.push_back("AB-");
        donorToRecipient.insert("B-", bNeg);
        
        // AB+ blood group - Ye sirf AB+ ko de sakta hai
        CustomVector<std::string> abPos;
        abPos.push_back("AB+");
        donorToRecipient.insert("AB+", abPos);
        
        // AB- blood group - Ye AB+ aur AB- ko de sakta hai
        CustomVector<std::string> abNeg;
        abNeg.push_back("AB+");
        abNeg.push_back("AB-");
        donorToRecipient.insert("AB-", abNeg);
        
        // Ab ulta dekhtey hain - Recipient ko kaun sa khoon mil sakta hai
        // AB+ universal recipient hai - Ye sab se khoon le sakta hai
        // Matlab sabka khoon le sakte hain
        CustomVector<std::string> abPosRecv;
        abPosRecv.push_back("O+");
        abPosRecv.push_back("O-");
        abPosRecv.push_back("A+");
        abPosRecv.push_back("A-");
        abPosRecv.push_back("B+");
        abPosRecv.push_back("B-");
        abPosRecv.push_back("AB+");
        abPosRecv.push_back("AB-");
        recipientToDonor.insert("AB+", abPosRecv);
        
        // AB- ko O-, A-, B-, AB- se hi khoon mil sakta hai
        CustomVector<std::string> abNegRecv;
        abNegRecv.push_back("O-");
        abNegRecv.push_back("A-");
        abNegRecv.push_back("B-");
        abNegRecv.push_back("AB-");
        recipientToDonor.insert("AB-", abNegRecv);
        
        // A+ ko O+, O-, A+, A- se khoon mil sakta hai
        CustomVector<std::string> aPosRecv;
        aPosRecv.push_back("O+");
        aPosRecv.push_back("O-");
        aPosRecv.push_back("A+");
        aPosRecv.push_back("A-");
        recipientToDonor.insert("A+", aPosRecv);
        
        // A- ko O- aur A- se hi khoon mil sakta hai
        CustomVector<std::string> aNegRecv;
        aNegRecv.push_back("O-");
        aNegRecv.push_back("A-");
        recipientToDonor.insert("A-", aNegRecv);
        
        // B+ ko O+, O-, B+, B- se khoon mil sakta hai
        CustomVector<std::string> bPosRecv;
        bPosRecv.push_back("O+");
        bPosRecv.push_back("O-");
        bPosRecv.push_back("B+");
        bPosRecv.push_back("B-");
        recipientToDonor.insert("B+", bPosRecv);
        
        // B- ko O- aur B- se hi khoon mil sakta hai
        CustomVector<std::string> bNegRecv;
        bNegRecv.push_back("O-");
        bNegRecv.push_back("B-");
        recipientToDonor.insert("B-", bNegRecv);
        
        // O+ ko O+ aur O- se khoon mil sakta hai
        CustomVector<std::string> oPosRecv;
        oPosRecv.push_back("O+");
        oPosRecv.push_back("O-");
        recipientToDonor.insert("O+", oPosRecv);
        
        // O- ko sirf O- se hi khoon mil sakta hai
        CustomVector<std::string> oNegRecv;
        oNegRecv.push_back("O-");
        recipientToDonor.insert("O-", oNegRecv);
    }
    
    // Ye function check karta hai - Donor ka khoon recipient ko de sakta hai ya nahi
    // Return true agar de sakta hai, false agar nahi de sakta
    bool canDonateTo(const std::string& donorType, const std::string& recipientType) {
        CustomVector<std::string> compatible;
        // donorType ke liye compatible recipients nikal te hain
        if (donorToRecipient.get(donorType, compatible)) {
            // Loop mein check karte hain ke recipientType compatible list mein hai ya nahi
            for (size_t i = 0; i < compatible.getSize(); ++i) {
                if (compatible[i] == recipientType) {
                    return true; // Mil gaya! De sakte hain khoon
                }
            }
        }
        return false; // Nahi mil gaya
    }
    
    // Ye function return karta hai - Ek recipient ko kaun se donors se khoon mil sakta hai
    CustomVector<std::string> getCompatibleDonors(const std::string& recipientType) {
        CustomVector<std::string> result;
        // recipientType ke liye compatible donors nikal te hain
        recipientToDonor.get(recipientType, result);
        return result; // Donors ki list return karte hain
    }
    
    // Ye function return karta hai - Ek donor konse recipients ko khoon de sakta hai
    CustomVector<std::string> getCompatibleRecipients(const std::string& donorType) {
        CustomVector<std::string> result;
        // donorType ke liye compatible recipients nikaltey hain
        donorToRecipient.get(donorType, result);
        return result; // Recipients ki list return karte hain
    }
};

#endif // BLOOD_COMPATIBILITY_HPP
