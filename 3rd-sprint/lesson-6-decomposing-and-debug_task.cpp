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
    // Реализуйте эту функцию
    return is;
}

struct BusesForStopResponse {
    string stop_name;
    vector<string> buses;
};

ostream& operator<<(ostream& os, const BusesForStopResponse& r) {
    // Реализуйте эту функцию
    return os;
}

struct StopsForBusResponse {
    string bus_name;
    vector<string> stops;
};

ostream& operator<<(ostream& os, const StopsForBusResponse& r) {
    // Реализуйте эту функцию
    return os;
}

struct AllBusesResponse {
    map<string, vector<string>> buses_for_stops;
};

ostream& operator<<(ostream& os, const AllBusesResponse& r) {
    // Реализуйте эту функцию
    return os;
}

class BusManager {
public:
    void AddBus(const string& bus, const vector<string>& stops) {
        stops_for_bus_[bus] = stops;
        for (const string& stop : stops) {
            buses_for_stops_[stop].push_back(bus);
        }
    }

    BusesForStopResponse GetBusesForStop(const string& stop) const {
        BusesForStopResponse response;
        if(buses_for_stops_.count(stop)) {
            response.stop_name = stop;
            response.buses = buses_for_stops_.at(stop);
        }
        return response;
    }

    StopsForBusResponse GetStopsForBus(const string& bus) const {
        StopsForBusResponse response;
        if (stops_for_bus_.count(bus)) {
            response.bus_name = bus;
            response.stops = stops_for_bus_.at(bus);
        }
        return response;
    }

    AllBusesResponse GetAllBuses() const {
        AllBusesResponse response;
        response.buses_for_stops = buses_for_stops_;
        return response;
    }

private:
    map<string, vector<string>> buses_for_stops_, // маршруты автобуса на остановке
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