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

struct MySolver : public Context {
    void Solve() {
        std::vector<int> streetCounts(StreetN, 0);
        for (int i = 0; i < CarN; ++i) {
            for (int j = 0; j < Path[i].size(); ++j) {
                ++streetCounts[Path[i][j]];
            }
        }
        Solution.resize(IntersectionN);
        for (int i = 0; i < IntersectionN; i++) {
            Solution[i].IncomingStreetDuration.resize(IncomingStreets[i].size());
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
                    Solution[i].IncomingStreetDuration[j] = int(sqrt(streetCounts[street]));
                }
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
    cerr << "Done!" << endl;
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    cerr << "Test time: " << elapsed_seconds.count() << endl;

    cerr << "Outputting" << endl;
    solver.Output();

    cerr << solver.GetScore() << endl;
    return 0;
}
