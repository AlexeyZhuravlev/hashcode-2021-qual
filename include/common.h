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
            for (int j = 0; j < p; j++) {
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

    struct CarState {
        int streetId;
        int positionInQueue;
    };


    uint64_t GetScore() {
        vector<tuple<int, int, int>> green;  // moment, street, duration
        for (int i = 0; i < IntersectionN; i++) {

            auto& entry = Solution[i];
            auto& durations = entry.IncomingStreetDuration;

            int moment = 0;
            int j = 0;
            while (moment < SimDuration) {
                if (durations[j]) {
                    green.emplace_back(moment, IncomingStreets[i][j], durations[j]);
                    moment += durations[j];
                }
                j = (j + 1) % durations.size();
            }
        }
        std::sort(green.begin(), green.end());

        vector<set<tuple<int, int, int>>> StreetEndQueue(StreetN);
        for (int i = 0; i < CarN; ++i) {
            StreetEndQueue[Path[i][0]].emplace(0, i, 0);
        }


        int score = 0;
        for (const auto& [moment, street, duration] : green) {
            auto& queue = StreetEndQueue[street];
            int curMoment = moment;
            while (curMoment < duration && queue.size()) {
                const auto it = queue.begin();
                const auto& [getMoment, carId, pathIdx] = *it;
//                std::cerr << getMoment << " " << carId << " " << pathIdx << std::endl;
                if (getMoment < moment + duration) {
                    curMoment = max(curMoment, getMoment); // be aware
                    const auto& nextStreet = Path[carId][pathIdx + 1];
                    if (pathIdx + 1 == Path[carId].size() - 1) {  // the last street for car
                        if (curMoment + Street[nextStreet].Cost <= SimDuration) {
                            score += SimDuration - (curMoment + Street[nextStreet].Cost) + EarlyBonus;
                        }
                    } else {
                        StreetEndQueue[nextStreet].emplace(curMoment + Street[nextStreet].Cost,
                                                                         carId, pathIdx + 1);
                    }
                    ++curMoment;
                    queue.erase(queue.begin());
                } else {
                    break;
                }
            }
        }

        return score;
    }
    
};
