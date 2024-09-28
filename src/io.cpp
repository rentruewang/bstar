#include "io.hpp"

#include <algorithm>
#include <cassert>
#include <ctime>
#include <vector>

#include "sa.hpp"

using namespace std;

pair<size_t, size_t> read_pin_file(ifstream& file,
                                   vector<pin>& pin_list,
                                   unordered_map<string, size_t>& pin_map,
                                   size_t& num_blocks) {
    auto buffer = string();

    size_t out_width, out_height;

    file >> buffer >> out_width >> out_height;
    assert(buffer == "Outline:");

    file >> buffer >> num_blocks;
    assert(buffer == "NumBlocks:");

    size_t num_terminals;
    file >> buffer >> num_terminals;
    assert(buffer == "NumTerminals:");

    auto name = string();

    size_t width, height;
    for (size_t i = 0; i < num_blocks; ++i) {
        file >> name >> width >> height;
        pin_map[name] = pin_list.size();
        pin_list.emplace_back(width, height, true, name);
    }
    assert(pin_map.size() == num_blocks);

    size_t horiz, verti;
    for (size_t i = 0; i < num_terminals; ++i) {
        file >> name >> buffer >> horiz >> verti;
        assert(buffer == "terminal");
        pin_map[name] = pin_list.size();
        pin_list.emplace_back(horiz, verti, false, name);
    }
    assert(pin_map.size() == num_blocks + num_terminals);
    assert(pin_list.size() == num_blocks + num_terminals);

    for (int i = 0; i < (int)num_blocks; ++i) {
        assert(pin_list[i].area());
    }

    for (int i = num_blocks; (size_t)i < num_blocks + num_terminals; ++i) {
        assert(!pin_list[i].area());
    }

    return make_pair(out_width, out_height);
}

void read_net_file(ifstream& file,
                   const vector<pin>& pin_list,
                   vector<net>& net_list,
                   const unordered_map<string, size_t>& pin_map) {
    auto buffer = string();

    size_t num_nets;

    file >> buffer >> num_nets;
    assert(buffer == "NumNets:");

    for (size_t i = 0, num_degree; i < num_nets; ++i) {
        file >> buffer >> num_degree;
        assert(buffer == "NetDegree:");
        vector<size_t> connected{};
        connected.reserve(num_degree);
        for (size_t j = 0; j < num_degree; ++j) {
            auto pin_name = string();
            file >> pin_name;
            connected.push_back(pin_map.at(pin_name));
        }
        sort(connected.rbegin(), connected.rend(), [&](int i, int j) {
            return (pin_list[connected[i]].area() != 0) <
                   (pin_list[connected[j]].area() != 0);
        });
        assert(connected.size() == num_degree);
        net_list.emplace_back(std::move(connected), pin_list);
    }
    assert(net_list.size() == num_nets);
}

void save_file(ofstream& file,
               time_t start_time,
               double alpha,
               pair<int, int> dimension,
               const vector<pin>& pin_list,
               const vector<net>& net_list) {
    const size_t size = pin_list.size();

    const size_t width = dimension.first, height = dimension.second;
    const size_t area = total_area(dimension);
    const size_t hpwl = total_hpwl(net_list);
    const double cost = total_cost(area, hpwl, alpha);

    file << cost << "\n"
         << hpwl << "\n"
         << area << "\n"
         << width << " " << height << "\n"
         << difftime(time(NULL), start_time) << "\n";

    for (size_t i = 0; i < size; ++i) {
        const pin& pin = pin_list[i];
        const string& name = pin.name();

        if (pin.area()) {
            const int x = pin.x(), y = pin.y(), w = pin.width(),
                      h = pin.height();

            file << name << " " << x << " " << y << " " << x + w << " " << y + h
                 << "\n";
        }
    }
}
