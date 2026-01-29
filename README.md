# 🩸 BloodConnect - Blood Donation Matching System

> **An educational blood donation management platform demonstrating custom data structures and algorithms for efficient donor-recipient matching.**

[![Language](https://img.shields.io/badge/Language-C%2B%2B-blue?style=flat-square)](.)
[![Frontend](https://img.shields.io/badge/Frontend-HTML%2FCSS%2FJS-yellow?style=flat-square)](.)
[![License](https://img.shields.io/badge/License-MIT-green?style=flat-square)](.)

**[🔗 View Live Demo](https://fatima-eman-hub.github.io/BloodDonationNetwork/)** | **[📊 GitHub Repository](https://github.com/Fatima-Eman-hub/BloodDonationNetwork)**

---

## 📖 Project Overview

BloodConnect is a practical learning project that implements:

- **5 Custom Data Structures** - HashMap, LinkedList, Graph, Vector, PriorityQueue
- **Core Algorithms** - Linear search, BFS, Dijkstra's shortest path
- **Matching Logic** - Blood compatibility rules and donor-recipient pairing
- **Web Interface** - REST API backend (C++) with HTML/CSS/JS frontend
- **Data Persistence** - CSV-based donor and recipient storage
- **Responsive Design** - Works on desktop, tablet, and mobile

---

## 🚀 Quick Start

### Prerequisites
- C++ Compiler with C++17 support
- CMake 3.10+
- Modern web browser

### Build & Run

```bash
# 1. Clone the repository
git clone https://github.com/yourusername/BloodConnect.git
cd BloodConnect

# 2. Build the project
mkdir build && cd build
cmake ..
make

# 3. Run the server
./bloodconnect
# Server runs at http://localhost:8080
```

### Try It Out
1. Open `http://localhost:8080` in your browser
2. Register as Donor or Recipient
3. Enter blood type and location
4. View matching results

---

## 📋 Features Implemented

### Core Functionality
- **User Registration & Login** - Separate donor/recipient flows
- **Donor Profiles** - Blood type, location, donation history
- **Recipient Profiles** - Medical needs, blood type, location
- **Blood Matching** - Compatible donor search based on blood type
- **Location-Based Search** - Find centers using graph algorithm
- **Donation History** - Track past donations with LinkedList

### User Interface
- Landing page with project overview
- Separate dashboards for donors and recipients
- Blood request system for recipients
- Donation history tracking
- Basic dark mode theme
- Mobile-responsive design

### Technical Features
- REST API with 10+ endpoints
- CSV file-based data persistence
- Custom data structure implementations
- Graph algorithm for distance calculation

---

## 🏗️ Architecture

### Backend Stack
```
┌─────────────────────────────────────────────────┐
│          Crow C++ Web Framework                  │
│          (REST API Server on Port 8080)          │
└────────────────────┬────────────────────────────┘
                     │
         ┌───────────┼───────────┐
         │           │           │
    ┌────▼────┐ ┌───▼────┐ ┌───▼───────┐
    │   DSA   │ │ Logic  │ │ Models    │
    │ Module  │ │ Module │ │ Module    │
    └────┬────┘ └───┬────┘ └───┬───────┘
         │           │           │
    ┌────▼───────────▼───────────▼────┐
    │    CSV Data Persistence Layer    │
    │  (donors.csv, recipients.csv)    │
    └─────────────────────────────────┘
```

### Frontend Stack
```
┌──────────────────────────────┐
│    Responsive HTML/CSS/JS    │
│  15 Pages + Dark Mode Support│
└──────────────┬───────────────┘
               │
         ┌─────▼─────┐
         │  REST API │
         │ (Backend) │
         └───────────┘
```

---

## 💾 Data Structures (Custom Implementations)

### 1. **CustomHashMap**
- **Use Case**: Store and retrieve donors/recipients by ID
- **Operations**: insert, get, contains, clear
- **Trade-offs**: Simple chaining, not optimized for large scale
```cpp
Donor donor = donorMap.get("D001");
```

### 2. **CustomLinkedList**
- **Use Case**: Maintain donation history in insertion order
- **Operations**: push_front, pop_front, traverse, getSize
```cpp
historyList.push_front(donation);
```

### 3. **CustomGraph**
- **Use Case**: Represent blood center locations and connections
- **Operations**: addEdge, dijkstra, getDistance
- **Algorithm**: Dijkstra's shortest path for routing
```cpp
int distance = graph.dijkstra(startCenter, endCenter);
```

### 4. **CustomVector**
- **Use Case**: Dynamic storage for search results and lists
- **Operations**: push_back, at, size, clear
```cpp
results.push_back(donor);
```

### 5. **CustomPriorityQueue**
- **Use Case**: Rank blood type matches by compatibility
- **Operations**: push, pop, top, isEmpty
```cpp
matchQueue.push({score, donor});
```

---

## 🧮 Algorithms Implemented

| Algorithm | Complexity | Purpose |
|-----------|-----------|---------|
| **Linear Search** | O(n) | Find donors by blood type |
| **Binary Search** | O(log n) | Search in sorted donor list |
| **Dijkstra's Algorithm** | O((V+E)logV) | Shortest path to blood center |
| **Hash Function** | O(1) avg | HashMap bucket distribution |
| **LinkedList Traversal** | O(n) | Iterate through history |

These are implemented to demonstrate algorithmic thinking, not optimized for production scale.

---

## 📊 Project Scope

| Component | Status | Notes |
|-----------|--------|-------|
| **Backend** | ✅ Complete | C++ REST API with Crow framework |
| **Frontend** | ✅ Complete | HTML/CSS/JS with responsive design |
| **Data Structures** | ✅ Complete | 5 custom implementations |
| **Algorithms** | ✅ Complete | Core algorithms for matching & routing |
| **Database** | ✅ Complete | CSV-based persistence |
| **Testing** | ⚠️ Limited | Manual testing, no formal test suite |
| **Performance Optimization** | ⚠️ Limited | Functional but not optimized for scale |
| **Production Ready** | ❌ No | Educational project, not for production use |

---

## 🔗 API Endpoints

**Authentication**
- `POST /register` - Register new user
- `POST /login` - User login
- `POST /logout` - User logout

**Donor Operations**
- `GET /donor/:id` - Get donor profile
- `PUT /donor/:id` - Update profile
- `GET /donor/:id/history` - View donation history
- `POST /donate` - Record donation

**Recipient Operations**
- `GET /recipient/:id` - Get recipient profile
- `PUT /recipient/:id` - Update profile
- `POST /request-blood` - Submit blood request
- `GET /requests/:id` - View requests

**Search & Matching**
- `GET /find-donors/:bloodType` - Find compatible donors
- `POST /match` - Get matching recommendations
- `GET /nearby-centers/:location` - Find nearby centers

---

## 🌙 Features

**Dark Mode**
- Toggle button for theme switching
- Persists user preference
- Available on all pages

**Blood Matching**
- Compatible donor search based on blood type
- Simple matching algorithm
- Results displayed in UI

**Location Routing**
- Graph-based representation of centers
- Dijkstra's algorithm for pathfinding
- Distance calculation

---

## 📁 Project Structure

```
BloodConnect/
├── README.md                      # You are here
├── CMakeLists.txt                 # Build configuration
├── data/
│   ├── donors.csv                 # Donor database
│   └── recipients.csv             # Recipient database
├── src/
│   ├── main.cpp                   # REST API server
│   ├── crow_all.h                 # Web framework
│   ├── dsa/
│   │   ├── CustomHashMap.hpp
│   │   ├── CustomLinkedList.hpp
│   │   ├── CustomGraph.hpp
│   │   ├── CustomVector.hpp
│   │   └── CustomPriorityQueue.hpp
│   ├── logic/
│   │   ├── BloodCompatibility.hpp
│   │   ├── CSVHandler.hpp
│   │   └── MatchingEngine.hpp
│   └── controllers/
│       └── [Endpoint handlers]
├── public/
│   ├── index.html                 # Landing page
│   ├── login.html                 # Login page
│   ├── register.html              # Registration
│   ├── dashboard-donor.html       # Donor dashboard
│   ├── dashboard-recipient.html   # Recipient dashboard
│   └── css/
│       ├── style.css              # Main styles
│       └── dark-mode.css          # Dark mode styles
└── docs/
    ├── COMPLETE_PROJECT_SUMMARY.md
    ├── DARK_MODE_DOCUMENTATION.md
    └── PRESENTATION_CHEAT_SHEET.md
```

---

## 🔐 Blood Type Compatibility

### Chart
```
O- ──→ Can donate to: Everyone (Universal Donor)
O+ ──→ Can donate to: O+, A+, B+, AB+
A- ──→ Can donate to: A-, A+, AB-, AB+
A+ ──→ Can donate to: A+, AB+
B- ──→ Can donate to: B-, B+, AB-, AB+
B+ ──→ Can donate to: B+, AB+
AB-──→ Can donate to: AB-, AB+
AB+──→ Can donate to: AB+ (Universal Recipient)
```

The system automatically calculates compatibility and ranks matches.

---

## 📈 Performance Considerations

**Current Implementation:**
- Suitable for learning and demonstration
- Handles dozens to hundreds of donors/recipients
- Not optimized for production scale
- CSV loading happens at startup
- No caching or indexing optimization

**If scaling to 1M+ records, would need:**
- Database (PostgreSQL, MySQL) instead of CSV
- Proper indexing on ID fields
- Connection pooling
- Query optimization
- Load testing and profiling

---

## 🧪 Testing & Validation

Currently, the project uses manual testing. To verify functionality:

```bash
# Test basic API endpoints
curl http://localhost:8080/donor/D001

# Test frontend pages
# Open in browser: http://localhost:8080
```

**What was tested:**
- Basic CRUD operations on donors/recipients
- Blood type compatibility logic
- CSV read/write operations
- Graph pathfinding for center routing
- Frontend page rendering and styling

**What's not included:**
- Automated unit tests
- Performance benchmarks
- Load testing
- Security audit

---

## 🤝 Contributing

Contributions are welcome! To contribute:

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Submit a pull request

---

## 📝 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---

## 📞 Contact

**Author**: [Your Name]  
**Email**: your.email@example.com  
**GitHub**: [@yourusername](https://github.com/yourusername)

---

## 🙏 Acknowledgments

- **Crow Framework** - C++ web framework used for the backend
- **Course Materials** - Data structures and algorithms concepts
- **Community** - Open source tooling and resources

---

## 📚 Additional Documentation

For more details:
- [Complete Project Summary](docs/COMPLETE_PROJECT_SUMMARY.md)
- [Presentation Notes](docs/PRESENTATION_CHEAT_SHEET.md)

---

<div align="center">

Made with ❤️ as an educational project

</div>
