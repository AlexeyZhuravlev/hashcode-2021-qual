#include <common.h>

#include <cstdio>
#include <cstring>
#include <algorithm>
#include <vector>
#include <iostream>
#include <cassert>
#include <cmath>
#include <string>
#include <queue>
#include <set>
#include <map>
#include <cstdlib>
#include <chrono>

using namespace std;

int s[MAX_INTERSECTIONS] = { 0 };
int e[MAX_INTERSECTIONS] = { 0 };

struct MySolver : public Context {
    
    void Solve() {
        std::vector<int> streetCounts(StreetN, 0);
        std::vector<int> routeBegin(StreetN, 0);
        for (int i = 0; i < CarN; ++i) {
            for (int j = 0; j < Path[i].size() - 1; ++j) {
                ++streetCounts[Path[i][j]];
            }
            ++routeBegin[Path[i][0]];
        }
        Solution.resize(IntersectionN);
        for (int i = 0; i < IntersectionN; i++) {
            Solution[i].IncomingStreetDuration.resize(IncomingStreets[i].size());
            sort(IncomingStreets[i].begin(), IncomingStreets[i].end(), [&routeBegin](int stA, int stB) {
                return routeBegin[stA] > routeBegin[stB];
            });
            int totalStreetCount = 0;
            for (int j = 0; j < IncomingStreets[i].size(); j++) {
                int street = IncomingStreets[i][j];
                totalStreetCount += streetCounts[street];
            }
            if (totalStreetCount == 0) {
                for (int j = 0; j < IncomingStreets[i].size(); j++) {
                    Solution[i].IncomingStreetDuration[j] = 1;
                }
                continue;
            }

            int curCount = IncomingStreets[i].size();
            for (int j = 0; j < IncomingStreets[i].size(); j++) {
                int street = IncomingStreets[i][j];
                double weight = streetCounts[street] / totalStreetCount;
                if (streetCounts[street] == 0) {
                    Solution[i].IncomingStreetDuration[j] = 0;
                } else {
                    Solution[i].IncomingStreetDuration[j] = 1; 
                    //int(pow(double(streetCounts[street]), 0.4));
                    //int(sqrt(streetCounts[street]));
                }
            }
        }
    }
    /*
    void Solve() {
        int nn = 0;
        vector<int> lens[1005];
        for (int i = 0; i < CarN; i++) {
            int len = 0;
            if (499 != Street[Path[i][0]].U) {
                for (int j = 0; j < Path[i].size() - 1; j++) {
                    len += Street[Path[i][j]].Cost;
                    if (Street[Path[i][j]].V == 499) {
                        nn++;
                        cerr << i << " " << len << endl;
                        lens[Street[Path[i][j]].U].push_back(len);
                        break;
                    }
                    // len += Street[Path[i][j]].Cost;
                }
            }
            if (len != 0) {
                // cerr << len << endl;
            }
        }
        vector<pair<int, int>> maxs;
        int l[MAX_INTERSECTIONS] = { 0 };
        for (int i = 0; i < IntersectionN; i++) {
            int mx = 0;
            for (int j = 0; j < lens[i].size(); j++) {
                mx = max(mx, lens[i][j]);
                cerr << lens[i][j] << " ";
            }
            cerr << endl;
            if (mx != 0) {
                maxs.push_back(make_pair(mx, i));
                l[i] = lens[i].size();
            }
        }
        sort(maxs.begin(), maxs.end());
        int SHIFT = 3, cur = 0;
        for (auto &tss : maxs) {
            auto mx = tss.first;
            auto i = tss.second;
            s[i] = max(mx + SHIFT, cur);
            e[i] = s[i] + l[i];
            cur = e[i];
            cerr << i << " " << s[i] << " " << e[i] << endl;
        }
        sort(IncomingStreets[499].begin(), IncomingStreets[499].end(), [&](int i, int j) {
            return s[Street[i].U] < s[Street[j].U];
        });
        cerr << nn << endl;
        Solution.resize(IntersectionN);
        for (int i = 0; i < IntersectionN; i++) {
            Solution[i].IncomingStreetDuration.resize(IncomingStreets[i].size());
            for (int j = 0; j < IncomingStreets[i].size(); j++) {
                Solution[i].IncomingStreetDuration[j] = 1;
            }
        }
        Solution[499].IncomingStreetDuration.clear();
        Solution[499].IncomingStreetDuration.resize(IncomingStreets[499].size());

        for (int i = 0; i < IncomingStreets[499].size(); i++) {
            int k = IncomingStreets[499][i];
            int u = Street[k].U;
            if (s[u] == 0) {
                Solution[499].IncomingStreetDuration[i] = 0;
            } else if (s[u] == maxs[0].first + SHIFT) {
                cerr << "WTFFF " << s[u] << endl;
                Solution[499].IncomingStreetDuration[i] = s[u];
            } else {
                Solution[499].IncomingStreetDuration[i] = e[u] - s[u];
            }
        }
    }
    */

    void Optimize() {
        uint64_t base_score = GetScore();
        for (int z = 0; z < 100; z++) {
            int j = rand() % IntersectionN;
            int a = rand() % Solution[j].IncomingStreetDuration.size();
            int b = rand() % Solution[j].IncomingStreetDuration.size();

            std::swap(IncomingStreets[j][a], IncomingStreets[j][b]);
            uint64_t new_score = GetScore();
            if (new_score > base_score) {
                base_score = new_score;
                cerr << "Upd: " << base_score << endl;
            } else {
                std::swap(IncomingStreets[j][a], IncomingStreets[j][b]);
            }
            
            /*
            int j = rand() % IntersectionN;
            int c = rand() % Solution[j].IncomingStreetDuration.size();
            
            Solution[j].IncomingStreetDuration[c]++;
            

            uint64_t new_score = GetScore();
            if (new_score > base_score) {
                base_score = new_score;
                cerr << "Upd: " << base_score << endl;
            } else {
                Solution[j].IncomingStreetDuration[c]--;
            }
            */

            if (z % 10 == 0) {
                cerr << z << endl;
            }

            
        }
    }
};

int main() {
    MySolver solver;

    solver.Input();

    auto start = std::chrono::system_clock::now();
    cerr << "Started solving..." << endl;
    solver.Solve();
    solver.Optimize();
    cerr << "Done!" << endl;
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    cerr << "Test time: " << elapsed_seconds.count() << endl;

    cerr << "Outputting" << endl;
    solver.Output();

    cerr << solver.GetScore() << endl;
    return 0;
}
