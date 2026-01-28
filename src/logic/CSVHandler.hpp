#ifndef CSV_HANDLER_HPP
#define CSV_HANDLER_HPP

#include <fstream>
#include <sstream>
#include <string>
#include "../dsa/CustomVector.hpp"
#include "../dsa/CustomHashMap.hpp"
#include "../models/Models.hpp"

class CSVHandler {
public:
    // Escape special characters for CSV format
    static std::string escape(std::string str) {
        bool needsQuotes = false;
        if (str.find(',') != std::string::npos || str.find('"') != std::string::npos || str.find('\n') != std::string::npos) {
            needsQuotes = true;
        }

        if (!needsQuotes) return str;

        std::string escaped = "\"";
        for (char c : str) {
            if (c == '"') escaped += "\"\"";
            else escaped += c;
        }
        escaped += "\"";
        return escaped;
    }

    // Unescape CSV-formatted string
    static std::string unescape(std::string str) {
        if (str.length() >= 2 && str.front() == '"' && str.back() == '"') {
            str = str.substr(1, str.length() - 2);
            std::string unescaped = "";
            for (size_t i = 0; i < str.length(); ++i) {
                if (str[i] == '"' && i + 1 < str.length() && str[i + 1] == '"') {
                    unescaped += '"';
                    i++;
                } else {
                    unescaped += str[i];
                }
            }
            return unescaped;
        }
        return str;
    }

    // Split CSV line by comma (respecting quoted fields)
    static CustomVector<std::string> splitCSV(const std::string& line) {
        CustomVector<std::string> result;
        std::string current = "";
        bool inQuotes = false;
        for (size_t i = 0; i < line.length(); ++i) {
            if (line[i] == '"') {
                inQuotes = !inQuotes;
                current += line[i];
            } else if (line[i] == ',' && !inQuotes) {
                result.push_back(unescape(current));
                current = "";
            } else {
                current += line[i];
            }
        }
        result.push_back(unescape(current));
        return result;
    }

    // Convert Donor object to CSV format
    static std::string donorToCSV(const Donor& d) {
        std::stringstream ss;
        ss << escape(d.id) << ","
           << escape(d.name) << ","
           << d.age << ","
           << escape(d.gender) << ","
           << escape(d.cnic) << ","
           << escape(d.email) << ","
           << escape(d.phone) << ","
           << escape(d.address) << ","
           << escape(d.city) << ","
           << escape(d.area) << ","
           << escape(d.bloodGroup) << ","
           << escape(d.status) << ","
           << escape(d.lastDonationDate) << ","
           << d.totalDonations << ","
           << escape(d.badgeLevel) << ","
           << (d.isVerified ? "1" : "0") << ","
           << escape(d.nextEligibleDate) << ","
           << escape(d.locationNodeId) << ","
           << escape(d.passwordHash);
        return ss.str();
    }

    static std::string recipientToCSV(const Recipient& r) {
        std::stringstream ss;
        ss << escape(r.id) << ","
           << escape(r.patientName) << ","
           << escape(r.patientId) << ","
           << escape(r.bloodGroupNeeded) << ","
           << escape(r.urgency) << ","
           << escape(r.locationType) << ","
           << escape(r.hospitalName) << ","
           << escape(r.locationNodeId) << ","
           << escape(r.contactPerson) << ","
           << escape(r.contactPhone) << ","
           << escape(r.status) << ","
           << escape(r.timestamp) << ","
           << escape(r.matchedDonorId) << ","
           << escape(r.createdByUserId) << ","
           << r.age << ","
           << escape(r.medicalCondition) << ","
           << r.unitsNeeded;
        return ss.str();
    }

    static std::string transactionToCSV(const Transaction& t) {
        std::stringstream ss;
        ss << escape(t.id) << ","
           << escape(t.donorId) << ","
           << escape(t.recipientId) << ","
           << escape(t.bloodGroup) << ","
           << t.units << ","
           << escape(t.hospitalId) << ","
           << t.distance << ","
           << escape(t.matchTime) << ","
           << escape(t.travelTime) << ","
           << escape(t.status) << ","
           << (t.receiptGenerated ? "1" : "0") << ","
           << escape(t.timestamp);
        return ss.str();
    }

    static Transaction* csvToTransaction(const std::string& line) {
        auto fields = splitCSV(line);
        if (fields.getSize() < 12) return nullptr;
        Transaction* t = new Transaction();
        t->id = fields[0];
        t->donorId = fields[1];
        t->recipientId = fields[2];
        t->bloodGroup = fields[3];
        t->units = std::stoi(fields[4]);
        t->hospitalId = fields[5];
        t->distance = std::stod(fields[6]);
        t->matchTime = fields[7];
        t->travelTime = fields[8];
        t->status = fields[9];
        t->receiptGenerated = fields[10] == "1";
        t->timestamp = fields[11];
        return t;
    }

    static void saveAllDonors(const std::string& filename, const CustomHashMap<std::string, Donor*>& database) {
        std::ofstream file(filename);
        if (file.is_open()) {
            file << "id,name,age,gender,cnic,email,phone,address,city,area,bloodGroup,status,lastDonationDate,totalDonations,badgeLevel,isVerified,nextEligibleDate,locationNodeId,passwordHash\n";
            CustomVector<std::string> keys = database.getKeys();
            for (size_t i = 0; i < keys.getSize(); ++i) {
                Donor* d;
                if (database.get(keys[i], d)) {
                    file << donorToCSV(*d) << "\n";
                }
            }
            file.close();
        }
    }

    static void saveAllRecipients(const std::string& filename, const CustomHashMap<std::string, Recipient*>& database) {
        std::ofstream file(filename);
        if (file.is_open()) {
            file << "id,patientName,patientId,bloodGroupNeeded,urgency,locationType,hospitalName,locationNodeId,contactPerson,contactPhone,status,timestamp,matchedDonorId,createdByUserId,age,medicalCondition,unitsNeeded\n";
            CustomVector<std::string> keys = database.getKeys();
            for (size_t i = 0; i < keys.getSize(); ++i) {
                Recipient* r;
                if (database.get(keys[i], r)) {
                    file << recipientToCSV(*r) << "\n";
                }
            }
            file.close();
        }
    }

    static Donor* csvToDonor(const std::string& line) {
        auto fields = splitCSV(line);
        if (fields.getSize() < 19) return nullptr;
        
        Donor* d = new Donor();
        d->id = fields[0];
        d->name = fields[1];
        d->age = std::stoi(fields[2]);
        d->gender = fields[3];
        d->cnic = fields[4];
        d->email = fields[5];
        d->phone = fields[6];
        d->address = fields[7];
        d->city = fields[8];
        d->area = fields[9];
        d->bloodGroup = fields[10];
        d->status = fields[11];
        d->lastDonationDate = fields[12];
        d->totalDonations = std::stoi(fields[13]);
        d->badgeLevel = fields[14];
        d->isVerified = fields[15] == "1";
        d->nextEligibleDate = fields[16];
        d->locationNodeId = fields[17];
        d->passwordHash = fields[18];
        return d;
    }

    static Recipient* csvToRecipient(const std::string& line) {
        auto fields = splitCSV(line);
        if (fields.getSize() < 17) return nullptr;

        Recipient* r = new Recipient();
        r->id = fields[0];
        r->patientName = fields[1];
        r->patientId = fields[2];
        r->bloodGroupNeeded = fields[3];
        r->urgency = fields[4];
        r->locationType = fields[5];
        r->hospitalName = fields[6];
        r->locationNodeId = fields[7];
        r->contactPerson = fields[8];
        r->contactPhone = fields[9];
        r->status = fields[10];
        r->timestamp = fields[11];
        r->matchedDonorId = fields[12];
        r->createdByUserId = fields[13];
        r->age = std::stoi(fields[14]);
        r->medicalCondition = fields[15];
        r->unitsNeeded = std::stoi(fields[16]);
        return r;
    }
};

#endif // CSV_HANDLER_HPP
