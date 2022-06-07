#include <iostream>
#include <vector>
#include <string>

int main() {
    int n;
    bool first = true;
    std::vector<std::string> output;
    while (std::cin >> n) {
        if (n == 0) break;
        int pages = (n % 4) ? n / 4 + 1 : n / 4;
        std::vector<int> order(pages * 4, -1);
        int pos = 0;
        for (auto i = 0; i < pages * 2 && i < n; i += 2) {
            order[pos + 1] = i;
            if (i + 1 < n) order[pos + 2] = i + 1;
            pos += 4;
        }
        pos -= 4;
        for (auto i = pages * 2; i < n; i += 2) {
            order[pos + 3] = i;
            if (i + 1 < n) order[pos] = i + 1;
            pos -= 4;
        }
        /*for (auto i = 0; i < order.size(); ++i) {
            std::cout << order[i] << " ";
        }
        std::cout << std::endl;*/
        {
            std::string out;
            out += "Printing order for " + std::to_string(n) + " pages:\n";
            for (auto i = 0; i < pages; ++i) {
                if (order[4 * i] >= 0 || order[4 * i + 1] >= 0) {
                    out += "Sheet " + std::to_string(i + 1) + ", front: ";
                    if (order[4 * i] >= 0) out += std::to_string(order[4 * i] + 1);
                    else out += "NA";
                    out += ", ";
                    if (order[4 * i + 1] >= 0) out += std::to_string(order[4 * i + 1] + 1);
                    else out += "NA";
                    out += "\n";
                }
                if (order[4 * i + 2] >= 0 || order[4 * i + 3] >= 0) {
                    out += "Sheet " + std::to_string(i + 1) + ", back : ";
                    if (order[4 * i + 2] >= 0) out += std::to_string(order[4 * i + 2] + 1);
                    else out += "NA";
                    out += ", ";
                    if (order[4 * i + 3] >= 0) out += std::to_string(order[4 * i + 3] + 1);
                    else out += "NA";
                    out += "\n";
                }
            }
            out.pop_back();
            output.push_back(out);
        }
    }
    for (const auto& s : output) {
        if (first) {
            std::cout << s;
            first = false;
        } else {
            std::cout << "\n" << s;
        }
    }
    return 0;
}