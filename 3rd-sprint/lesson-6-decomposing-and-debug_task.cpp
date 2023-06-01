#include <cassert>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <cassert>

using namespace std;

enum class QueryType {
    NewBus,
    BusesForStop,
    StopsForBus,
    AllBuses,
};

struct Query {
    QueryType type;
    string bus;
    string stop;
    vector<string> stops;
};

istream& operator>>(istream& is, Query& q) {
    string command;
    is >> command;

    if (command == "NEW_BUS"s) {
        q.type = QueryType::NewBus;
        is >> q.bus;
        int stops_count;
        is >> stops_count;
        q.stops.resize(stops_count);
        for(int i = 0; i < stops_count; ++i) {
            is >> q.stops[i];
        }

    } else if (command == "BUSES_FOR_STOP"s) {
        q.type = QueryType::BusesForStop;
        is >> q.stop;
    } else if (command == "STOPS_FOR_BUS"s) {
        q.type = QueryType::StopsForBus;
        is >> q.bus;
    } else if (command == "ALL_BUSES"s) {
        q.type = QueryType::AllBuses;
    }
    return is;
}

struct BusesForStopResponse {
    string stop_name;
    vector<string> buses;
};

ostream& operator<<(ostream& os, const BusesForStopResponse& r) {
    if (r.stop_name.empty()) {
        os << "No stop"s;
        return os;
    } else {
        for (const auto& bus : r.buses) {
            os << bus << " "s;
        }
        return os;
    }
}

struct StopsForBusResponse {
    string bus_name;
    vector<string> stops;
    map<string, vector<string>> buses_for_stop;
};

ostream& operator<<(ostream& os, const StopsForBusResponse& r) {
    if (r.bus_name.empty()) {
        os << "No bus"s;
    } else {
        for (const string& stop : r.stops) {
        os << "Stop "s << stop << ": "s;
        if (r.buses_for_stop.at(stop).size() == 1) {
            os << "no interchange\n"s;
            
        } else {
            for (const string& bus : r.buses_for_stop.at(stop)) {
                if (bus == r.bus_name) continue;
                os << bus << " "s;
            }
            if (stop == r.stops.back()) continue;
            os << "\n"s;
        }
        }

    }
    return os;
}

struct AllBusesResponse {
    map<string, vector<string>> buses_and_stops;
};

ostream& operator<<(ostream& os, const AllBusesResponse& r) {
    if (r.buses_and_stops.empty()) {
        os << "No buses"s;
    } else {
        for (const auto& [bus_name, stops] : r.buses_and_stops) {
            os << "Bus "s << bus_name << ": "s;
            for (const string& stop : stops) {
                os << stop << " "s;
                if (stop == stops.back()) {
                    os << "\n"s;
                }
            }
        }
    }
    return os;
}

class BusManager {
public:
    void AddBus(const string& bus, const vector<string>& stops) {
        stops_for_bus_[bus] = stops;
        for (const string& stop : stops) {
            buses_for_stop_[stop].push_back(bus);
        }
    }

    BusesForStopResponse GetBusesForStop(const string& stop) const {
        BusesForStopResponse response;
        if(buses_for_stop_.count(stop)) {
            response.stop_name = stop;
            response.buses = buses_for_stop_.at(stop);
        }
        return response;
    }

    StopsForBusResponse GetStopsForBus(const string& bus) const {
        StopsForBusResponse response;
        if (stops_for_bus_.count(bus)) {
            response.bus_name = bus;
            response.stops = stops_for_bus_.at(bus);
        }
        response.buses_for_stop = buses_for_stop_;
        return response;
    }

    AllBusesResponse GetAllBuses() const {
        return AllBusesResponse {stops_for_bus_};
    }

private:
    map<string, vector<string>> buses_for_stop_, // автобусы на остановке
                                stops_for_bus_;  //остановки автобуса
};


// Модульные тесты

void TestingNewBusMethod() {
    BusManager bm;
    bm.AddBus("golden_ring"s, {"sergiev_posad"s, "rostov"s, "ivanovo"s, "vladimir"s});

    assert(bm.GetStopsForBus("golden_ring"s).bus_name == "golden_ring"s);
    assert(bm.GetStopsForBus("golden_ring"s).stops.size() == 4);
}

void TestingGetBusesForStopMethod() {
    BusManager bm;
    bm.AddBus("golden_ring"s, {"sergiev_posad"s, "rostov"s, "ivanovo"s, "vladimir"s});
    bm.AddBus("golden_apple"s, { "rostov"s, "ivanovo"s});
    
    assert(bm.GetBusesForStop("rostov"s).stop_name == "rostov"s);
    assert(bm.GetBusesForStop("rostov"s).buses.size() == 2);
    assert(bm.GetBusesForStop("vladimir"s).buses.size() == 1);
}



// Тестирование
void Test() {
    TestingNewBusMethod();
    cout << "TestingNewBusMethod() complete!" << endl;

    TestingGetBusesForStopMethod();
    cout << "TestingGetBusesForStopMethod() complete!" << endl;

    cout << "Tests complete!"s << endl;
}


int main() {
    Test();

    int query_count;
    Query q;

    cin >> query_count;

    BusManager bm;
    for (int i = 0; i < query_count; ++i) {
        cin >> q;
        switch (q.type) {
            case QueryType::NewBus:
                bm.AddBus(q.bus, q.stops);
                break;
            case QueryType::BusesForStop:
                cout << bm.GetBusesForStop(q.stop) << endl;
                break;
            case QueryType::StopsForBus:
                cout << bm.GetStopsForBus(q.bus) << endl;
                break;
            case QueryType::AllBuses:
                cout << bm.GetAllBuses() << endl;
                break;
        }
    }
}