#pragma once

#include <algorithm>
#include <vector>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <cassert>
#include <cmath>
#include <string>
#include <queue>
#include <set>
#include <map>
#include <cstdlib>
#include <unordered_map>

using namespace std;

const size_t MAX_CARS = 1005;
const size_t MAX_STREETS = 100005;
const size_t MAX_INTERSECTIONS = 100005;

struct TStreet {
    int U, V;
    string Name;
    int Cost;
};

struct TScheduleEntry {
    vector<int> IncomingStreetDuration;
};

struct Context {
    using TSolution = vector<TScheduleEntry>;
    TSolution Solution;

    int SimDuration;
    int IntersectionN;
    int StreetN;
    int CarN;
    int EarlyBonus;
    TStreet Street[MAX_STREETS];

    vector<int> Path[MAX_CARS];

    unordered_map<string, int> StreetNameToID;
    vector<int> IncomingStreets[MAX_INTERSECTIONS];

    void Input() {
        cin >> SimDuration >> IntersectionN >> StreetN  >> CarN >> EarlyBonus;

        for (int i = 0; i < StreetN; i++) {
            cin >> Street[i].U >> Street[i].V >> Street[i].Name >> Street[i].Cost;
            StreetNameToID[Street[i].Name] = i;
            IncomingStreets[Street[i].V].push_back(i);
        }

        for (int i = 0; i < CarN; i++) {
            int p;
            cin >> p;
            for (int i = 0; i < p; i++) {
                string streetName;
                cin >> streetName;
                Path[i].push_back(StreetNameToID[streetName]);
            }
        }
    }

    void Output() {
        assert(Solution.size() == IntersectionN);

        cout << IntersectionN << endl;
        for (int i = 0; i < IntersectionN; i++) {
            cout << i << endl;
            auto& entry = Solution[i];

            int nIn = IncomingStreets[i].size();
            auto& durations = entry.IncomingStreetDuration;
            assert(durations.size() == nIn);
            {
                int nPositive = nIn - count(durations.begin(), durations.end(), 0);
                if (nPositive == 0) {
                    durations[0] = 1;
                    nPositive = 1;
                }
                cout << nPositive << endl;
            }
            for (int j = 0; j < nIn; j++) {
                assert(durations[j] >= 0);
                if (durations[j]) {
                    cout << Street[IncomingStreets[i][j]].Name << " " << durations[j] << endl;
                }
            }
        }
    }

    uint64_t GetScore() {
        return 0;
    }
    
};
