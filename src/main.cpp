#define CROW_MAIN
#include "crow_all.h"
#include "dsa/CustomVector.hpp"
#include "dsa/CustomHashMap.hpp"
#include "dsa/CustomLinkedList.hpp"
#include "dsa/CustomGraph.hpp"
#include "models/Models.hpp"
#include "logic/BloodCompatibility.hpp"
#include "logic/MatchingEngine.hpp"
#include "logic/CSVHandler.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <iomanip>

CustomHashMap<std::string, Donor*> donorDatabase;
CustomHashMap<std::string, Recipient*> recipientDatabase;
CustomLinkedList<Transaction*> transactionHistory;
CustomGraph cityGraph;
MatchingEngine* matchingEngine;

int donorCounter = 1;
int recipientCounter = 1;
int transactionCounter = 1;

std::string getCurrentTimestamp() {
    time_t now = time(0);
    char buf[80];
    strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%SZ", gmtime(&now));
    return std::string(buf);
}

std::string generateDonorId() {
    std::stringstream ss;
    ss << "DON-" << std::setfill('0') << std::setw(3) << donorCounter++;
    return ss.str();
}

std::string generateRecipientId() {
    std::stringstream ss;
    ss << "REC-" << std::setfill('0') << std::setw(3) << recipientCounter++;
    return ss.str();
}

const std::string DONORS_CSV = "c:\\Users\\hp\\Desktop\\for vscode\\data\\donors.csv";
const std::string RECIPIENTS_CSV = "c:\\Users\\hp\\Desktop\\for vscode\\data\\recipients.csv";
const std::string TRANSACTIONS_CSV = "c:\\Users\\hp\\Desktop\\for vscode\\data\\transactions.csv";

void loadData() {
    std::cout << "Loading data from CSV files..." << std::endl;
    
    // Initialize geographic nodes (hospitals and donor areas)
    cityGraph.addNode("H1", "PIMS", "hospital", 100, 150);
    cityGraph.addNode("H2", "Shifa International", "hospital", 180, 140);
    cityGraph.addNode("H3", "Aga Khan Hospital", "hospital", 300, 400);
    cityGraph.addNode("D1", "F-8 Sector", "donor_area", 120, 130);
    cityGraph.addNode("D2", "G-9 Sector", "donor_area", 110, 160);
    cityGraph.addNode("D3", "DHA Phase 5", "donor_area", 290, 390);
    
    // Add weighted edges for distance calculation
    cityGraph.addEdge("H1", "D1", 5.2);
    cityGraph.addEdge("H1", "D2", 3.8);
    cityGraph.addEdge("H2", "D1", 7.5);
    cityGraph.addEdge("H2", "D2", 9.2);
    cityGraph.addEdge("H3", "D3", 4.5);
    cityGraph.addEdge("D1", "D2", 2.1);
    
    // Load donors from CSV
    std::ifstream donorFile(DONORS_CSV);
    std::string line;
    if (donorFile.is_open()) {
        std::getline(donorFile, line);
        while (std::getline(donorFile, line)) {
            if (line.empty()) continue;
            Donor* d = CSVHandler::csvToDonor(line);
            if (d) {
                donorDatabase.insert(d->id, d);
                matchingEngine->addDonor(d);
                int idNum = std::stoi(d->id.substr(4));
                if (idNum >= donorCounter) donorCounter = idNum + 1;
            }
        }
        donorFile.close();
    }
    
    // Load recipients from CSV
    std::ifstream recipientFile(RECIPIENTS_CSV);
    if (recipientFile.is_open()) {
        std::getline(recipientFile, line);
        while (std::getline(recipientFile, line)) {
            if (line.empty()) continue;
            Recipient* r = CSVHandler::csvToRecipient(line);
            if (r) {
                recipientDatabase.insert(r->id, r);
                int idNum = std::stoi(r->id.substr(4));
                if (idNum >= recipientCounter) recipientCounter = idNum + 1;
            }
        }
        recipientFile.close();
    }
    
    // Load transactions (newest first - reverse chronological order)
    std::ifstream transFile(TRANSACTIONS_CSV);
    if (transFile.is_open()) {
        std::getline(transFile, line);
        while (std::getline(transFile, line)) {
            if (line.empty()) continue;
            Transaction* t = CSVHandler::csvToTransaction(line);
            if (t) {
                transactionHistory.push_front(t);
                int idNum = std::stoi(t->id.substr(4));
                if (idNum >= transactionCounter) transactionCounter = idNum + 1;
            }
        }
        transFile.close();
    }
    
    std::cout << "Data loaded: Donors=" << donorDatabase.getSize() 
              << ", Recipients=" << recipientDatabase.getSize() << std::endl;
}

int main() {
    crow::SimpleApp app;
    
    // Initialize matching engine - matches donors and recipients
    // Uses Dijkstra algorithm to find nearest donors
    matchingEngine = new MatchingEngine(&cityGraph);
    
    // Load all data from CSV files
    loadData();
    
    // Enable CORS - accept requests from web frontend
    app.loglevel(crow::LogLevel::Info);
    
    // Serve static files (HTML/CSS/JS)
    CROW_ROUTE(app, "/")([](){
        auto page = crow::mustache::load("index.html");
        return page.render();
    });
    
    // API: Register Donor
    // Method: POST /api/auth/register/donor
    // Called when: New donor signs up
    // What it does:
    //   1. Create new Donor object
    //   2. Add unique ID (DON-001) and data
    //   3. Add to HashMap in O(1) speed
    //   4. Tell matching engine
    //   5. Save to CSV file (persistent data)
    // Alternative: Use Database (SQL/MongoDB)
    //   - But CSV is simple and sufficient for this project
    //   - CSV data is escaped/unescaped by CSVHandler
    CROW_ROUTE(app, "/api/auth/register/donor").methods("POST"_method)
    ([](const crow::request& req){
        auto body = crow::json::load(req.body);
        if (!body) {
            return crow::response(400, "Invalid JSON");
        }
        
        Donor* newDonor = new Donor();
        newDonor->id = generateDonorId();
        newDonor->name = body["name"].s();
        newDonor->cnic = body["cnic"].s();
        newDonor->age = body["age"].i();
        newDonor->gender = body["gender"].s();
        newDonor->email = body["email"].s();
        newDonor->phone = body["phone"].s();
        newDonor->address = body["address"].s();
        newDonor->bloodGroup = body["bloodGroup"].s();
        newDonor->city = body["city"].s();
        newDonor->area = body["area"].s();
        newDonor->locationNodeId = body.has("locationNodeId") ? body["locationNodeId"].s() : std::string("D1");
        newDonor->status = "Available";
        newDonor->totalDonations = 0;
        newDonor->badgeLevel = "Bronze";
        newDonor->isVerified = false;
        newDonor->passwordHash = body["password"].s();
        
        // Add to HashMap - for fast lookup
        donorDatabase.insert(newDonor->id, newDonor);
        matchingEngine->addDonor(newDonor);
        
        // Persist to CSV file permanently
        std::ofstream donorFile(DONORS_CSV, std::ios::app);
        donorFile << CSVHandler::donorToCSV(*newDonor) << std::endl;
        donorFile.close();
        
        crow::json::wvalue response;
        response["success"] = true;
        response["message"] = "Donor registered successfully";
        response["donorId"] = newDonor->id;
        response["role"] = "donor";
        
        return crow::response(200, response);
    });

    // WebSocket: Real-time updates
    // This is direct server-client communication via persistent connection
    // Instead of HTTP request-response - connection stays open
    // Can send instant notifications about new matches
    CROW_WEBSOCKET_ROUTE(app, "/ws")
    .onopen([&](crow::websocket::connection& conn) {
        std::cout << "WebSocket opened" << std::endl;
    })
    .onclose([&](crow::websocket::connection& conn, const std::string& reason, unsigned short code) {
        std::cout << "WebSocket closed: " << reason << " (code: " << code << ")" << std::endl;
    })
    .onmessage([&](crow::websocket::connection& conn, const std::string& data, bool is_binary) {
        // Send direct messages (e.g., new match notifications)
        conn.send_text("Message received: " + data);
    });
    
    // API: ریسیپینٹ کو رجسٹر کریں
    // طریقہ: POST /api/auth/register/recipient
    // کہا جاتا ہے: جب مریض کو خون کی ضرورت ہو
    // کیا کرتا ہے:
    //   1. نیا Recipient آبجیکٹ بنائیں
    //   2. منفرد ID (REC-001) اور ضروری ڈیٹا شامل کریں
    //   3. HashMap میں شامل کریں - O(1) میں
    //   4. CSV فائل میں محفوظ کریں
    // اہم: اس کے بعد میچنگ انجن نزدیک ترین ڈونرز تلاش کرے گا
    CROW_ROUTE(app, "/api/auth/register/recipient").methods("POST"_method)
    ([](const crow::request& req){
        auto body = crow::json::load(req.body);
        if (!body) {
            return crow::response(400, "Invalid JSON");
        }
        
        Recipient* newRecipient = new Recipient();
        newRecipient->id = generateRecipientId();
        newRecipient->patientName = body["patientName"].s();
        newRecipient->age = body.has("age") ? body["age"].i() : 0;
        newRecipient->bloodGroupNeeded = body["bloodGroupNeeded"].s();
        newRecipient->urgency = body["urgency"].s();
        newRecipient->hospitalName = body.has("hospitalName") ? body["hospitalName"].s() : std::string("");
        newRecipient->locationNodeId = body.has("locationNodeId") ? body["locationNodeId"].s() : std::string("H1");
        newRecipient->contactPerson = body["contactPerson"].s();
        newRecipient->contactPhone = body["contactPhone"].s();
        newRecipient->status = "Pending";
        newRecipient->timestamp = getCurrentTimestamp();
        
        recipientDatabase.insert(newRecipient->id, newRecipient);
        
        // Persist to CSV
        std::ofstream recipFile(RECIPIENTS_CSV, std::ios::app);
        recipFile << CSVHandler::recipientToCSV(*newRecipient) << std::endl;
        recipFile.close();
        
        crow::json::wvalue response;
        response["success"] = true;
        response["message"] = "Recipient registered successfully";
        response["recipientId"] = newRecipient->id;
        response["role"] = "recipient";
        
        return crow::response(200, response);
    });
    
    // API: Login
    CROW_ROUTE(app, "/api/auth/login").methods("POST"_method)
    ([](const crow::request& req){
        auto body = crow::json::load(req.body);
        if (!body) {
            return crow::response(400, "Invalid JSON");
        }
        
        std::string email = body["email"].s();
        std::string password = body["password"].s();
        
        // Check donors
        CustomVector<std::string> donorIds = donorDatabase.getKeys();
        for (size_t i = 0; i < donorIds.getSize(); ++i) {
            Donor* donor;
            if (donorDatabase.get(donorIds[i], donor)) {
                if (donor->email == email && donor->passwordHash == password) {
                    crow::json::wvalue response;
                    response["success"] = true;
                    response["role"] = "donor";
                    response["userId"] = donor->id;
                    response["name"] = donor->name;
                    return crow::response(200, response);
                }
            }
        }

        // Check recipients
        CustomVector<std::string> recipientIds = recipientDatabase.getKeys();
        for (size_t i = 0; i < recipientIds.getSize(); ++i) {
            Recipient* rec;
            if (recipientDatabase.get(recipientIds[i], rec)) {
                // For recipients, we'll check against patientName as name for now
                // REAL APP would have email/pass fields in Recipient struct too
                // For this project, let's assume contactPhone as pass or simulate match
                if (rec->patientName + "@blood.com" == email) { 
                    crow::json::wvalue response;
                    response["success"] = true;
                    response["role"] = "recipient";
                    response["userId"] = rec->id;
                    response["name"] = rec->patientName;
                    return crow::response(200, response);
                }
            }
        }
        
        crow::json::wvalue response;
        response["success"] = false;
        response["message"] = "Invalid credentials";
        return crow::response(401, response);
    });

    // API: Update Donor Info
    CROW_ROUTE(app, "/api/donor/update").methods("POST"_method)
    ([](const crow::request& req){
        auto body = crow::json::load(req.body);
        if (!body || !body.has("donorId")) return crow::response(400);

        std::string donorId = body["donorId"].s();
        Donor* d;
        if (donorDatabase.get(donorId, d)) {
            if (body.has("name")) d->name = body["name"].s();
            if (body.has("age")) d->age = body["age"].i();
            if (body.has("phone")) d->phone = body["phone"].s();
            if (body.has("city")) d->city = body["city"].s();
            if (body.has("area")) d->area = body["area"].s();
            if (body.has("address")) d->address = body["address"].s();

            CSVHandler::saveAllDonors("data/donors.csv", donorDatabase);
            return crow::response(200, "Update successful");
        }
        return crow::response(404, "Donor not found");
    });
    CROW_ROUTE(app, "/api/donor/status").methods("POST"_method)
    ([](const crow::request& req){
        auto body = crow::json::load(req.body);
        if (!body || !body.has("donorId") || !body.has("status")) return crow::response(400);

        std::string donorId = body["donorId"].s();
        std::string status = body["status"].s();

        Donor* d;
        if (donorDatabase.get(donorId, d)) {
            d->status = status;
            CSVHandler::saveAllDonors("data/donors.csv", donorDatabase);
            return crow::response(200, "Status updated");
        }
        return crow::response(404, "Donor not found");
    });
    
    // API: Accept Blood Request
    CROW_ROUTE(app, "/api/donor/accept-request").methods("POST"_method)
    ([](const crow::request& req){
        auto body = crow::json::load(req.body);
        if (!body || !body.has("donorId") || !body.has("requestId")) return crow::response(400);

        std::string donorId = body["donorId"].s();
        std::string requestId = body["requestId"].s();

        Donor* d;
        Recipient* r;
        if (donorDatabase.get(donorId, d) && recipientDatabase.get(requestId, r)) {
            r->status = "Completed";
            r->matchedDonorId = donorId;
            d->status = "Available";
            d->totalDonations++;
            
            // Create a transaction record
            Transaction* t = new Transaction();
            t->id = "TRN-" + std::to_string(transactionCounter++);
            t->donorId = donorId;
            t->recipientId = requestId;
            t->bloodGroup = d->bloodGroup;
            t->status = "Success";
            t->timestamp = getCurrentTimestamp();
            transactionHistory.push_front(t);

            // Persist all changes
            CSVHandler::saveAllDonors("data/donors.csv", donorDatabase);
            CSVHandler::saveAllRecipients("data/recipients.csv", recipientDatabase);
            std::ofstream transFile("data/transactions.csv", std::ios::app);
            transFile << CSVHandler::transactionToCSV(*t) << std::endl;
            transFile.close();

            return crow::response(200, "Request Accepted & Completed");
        }
        return crow::response(404, "Not found");
    });
    CROW_ROUTE(app, "/api/recipient/request").methods("POST"_method)
    ([](const crow::request& req){
        auto body = crow::json::load(req.body);
        if (!body) {
            return crow::response(400, "Invalid JSON");
        }
        
        Recipient* newRequest = new Recipient();
        newRequest->id = generateRecipientId();
        newRequest->patientName = body["patientName"].s();
        newRequest->bloodGroupNeeded = body["bloodGroupNeeded"].s();
        newRequest->urgency = body["urgency"].s();
        newRequest->hospitalName = body.has("hospitalName") ? body["hospitalName"].s() : std::string("");
        newRequest->locationNodeId = body.has("locationNodeId") ? body["locationNodeId"].s() : std::string("H1");
        newRequest->contactPerson = body["contactPerson"].s();
        newRequest->contactPhone = body["contactPhone"].s();
        newRequest->status = "Searching";
        newRequest->timestamp = getCurrentTimestamp();
        
        recipientDatabase.insert(newRequest->id, newRequest);
        matchingEngine->addRecipientRequest(newRequest);
        
        // Try to find a match
        Donor* matchedDonor = matchingEngine->findBestDonorFor(newRequest);
        
        crow::json::wvalue response;
        response["success"] = true;
        response["requestId"] = newRequest->id;
        
        if (matchedDonor) {
            newRequest->matchedDonorId = matchedDonor->id;
            newRequest->status = "Matched";
            matchedDonor->status = "Busy";
            
            auto route = cityGraph.dijkstra(matchedDonor->locationNodeId, newRequest->locationNodeId);
            
            response["matched"] = true;
            response["donorName"] = matchedDonor->name;
            response["donorId"] = matchedDonor->id;
            response["distance"] = route.distance;
            response["estimatedTime"] = static_cast<int>(route.distance * 3); // 3 min per km
            
            // Persist both databases to reflect match and donor status
            CSVHandler::saveAllDonors("data/donors.csv", donorDatabase);
            CSVHandler::saveAllRecipients("data/recipients.csv", recipientDatabase);
        } else {
            response["matched"] = false;
            response["message"] = "Searching for compatible donors...";
        }
        
        return crow::response(200, response);
    });
    
    // API: Get Donor Dashboard
    CROW_ROUTE(app, "/api/donor/dashboard/<string>")
    ([](std::string donorId){
        Donor* donor;
        if (!donorDatabase.get(donorId, donor)) {
            return crow::response(404, "Donor not found");
        }
        
        crow::json::wvalue response;
        response["id"] = donor->id;
        response["name"] = donor->name;
        response["bloodGroup"] = donor->bloodGroup;
        response["status"] = donor->status;
        response["totalDonations"] = donor->totalDonations;
        response["badgeLevel"] = donor->badgeLevel;
        response["city"] = donor->city;
        response["area"] = donor->area;
        
        return crow::response(200, response);
    });
    
    // API: Health check
    CROW_ROUTE(app, "/api/health")
    ([](){
        crow::json::wvalue response;
        response["status"] = "healthy";
        response["timestamp"] = getCurrentTimestamp();
        response["donors"] = donorDatabase.getSize();
        response["recipients"] = recipientDatabase.getSize();
        return crow::response(200, response);
    });

    // DEBUG: Get all donors from CSV
    CROW_ROUTE(app, "/api/debug/donors")
    ([]{
        crow::json::wvalue response;
        CustomVector<std::string> ids = donorDatabase.getKeys();
        // Return just the count - total donors in database
        response["total"] = static_cast<int>(ids.getSize());
        response["message"] = "Total donors in database";
        return crow::response(200, response);
    });

    // DEBUG: Get all recipients from CSV
    CROW_ROUTE(app, "/api/debug/recipients")
    ([]{
        crow::json::wvalue response;
        CustomVector<std::string> ids = recipientDatabase.getKeys();
        // Return just the count - total recipients in database
        response["total"] = static_cast<int>(ids.getSize());
        response["message"] = "Total recipients in database";
        return crow::response(200, response);
    });
    
    std::cout << "🩸 Smart Blood Donation System Server Starting..." << std::endl;
    std::cout << "🌐 Server running on http://localhost:18080" << std::endl;
    std::cout << "📊 Loaded " << donorDatabase.getSize() << " donors" << std::endl;
    
    // Start server on port 18080
    // multithreaded() = can handle multiple requests at once
    // This is needed for WebSocket and HTTP support
    app.port(18080)
       .multithreaded()
       .run();
    
    return 0;
}
