#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>
#include <unordered_map>

#include "b_star.hpp"
#include "io.hpp"
#include "nets.hpp"
#include "pins.hpp"
#include "sa.hpp"

constexpr double init_prob = .999, constant = 3.;
constexpr size_t iterations = 10000, interrupt = iterations / constant;
constexpr auto iter_info{std::make_pair(iterations, interrupt)};
constexpr size_t episodes = 1000, burning_stage = 100;
constexpr double ratio = .5;

int main(int, char const* argv[]) {
    using namespace std;

    time_t start = time(NULL);

    srand(start);

    printf("start time = %ld\n", start);

    double alpha = atof(argv[1]);

    printf("alpha = %lf\n", alpha);

    unordered_map<string, size_t> pin_map;
    vector<pin> pin_list;
    vector<net> net_list;

    ifstream pin_file{argv[2]};
    ifstream net_file{argv[3]};

    size_t num_blocks;

    auto dimension = read_pin_file(pin_file, pin_list, pin_map, num_blocks);
    read_net_file(net_file, pin_list, net_list, pin_map);

    size_t width = dimension.first;
    size_t height = dimension.second;

    printf("blocks = %lu, nets = %lu, dimension= (%lu, %lu)\n", pin_map.size(),
           net_list.size(), width, height);

    b_star tree{pin_list};

    auto floorplan{sim_anneal(dimension, tree, pin_list, net_list, iter_info,
                              num_blocks, episodes, burning_stage, alpha, ratio,
                              init_prob, constant)};

    printf("result = (%d, %d)\n", floorplan.first, floorplan.second);

    ofstream out_file{argv[4]};
    save_file(out_file, start, alpha, floorplan, pin_list, net_list);

    printf("Program Exit.\n");

    return 0;
}
