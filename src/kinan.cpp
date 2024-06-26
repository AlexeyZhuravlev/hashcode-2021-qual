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
        for (int i = 0; i < CarN; i++) {
            int len = 0;
            for (int j = 0; j < Path[i].size(); j++) {
                if (Street[Path[i][j]].U == 499 || Street[Path[i][j]].V == 499) {
                    cerr << j << endl;
                    break;
                }
                // len += Street[Path[i][j]].Cost;
            }
            if (len != 0) {
                // cerr << len << endl;
            }
        }
        Solution.resize(IntersectionN);
        for (int i = 0; i < IntersectionN; i++) {
            Solution[i].IncomingStreetDuration.resize(IncomingStreets[i].size());
            for (int j = 0; j < IncomingStreets[i].size(); j++) {
                Solution[i].IncomingStreetDuration[j] = 1;
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
