#include "sa.hpp"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include "costs.hpp"
#include "mutations.hpp"

using namespace std;

unsigned total_area(const pair<int, int> dim) {
    return (unsigned)dim.first * (unsigned)dim.second;
}

unsigned total_hpwl(const vector<net>& net_list) {
    unsigned idx, hpwl;
    for (idx = hpwl = 0; idx < net_list.size(); ++idx) {
        const net& net = net_list[idx];
        hpwl += net.hpwl();
    }
    return hpwl;
}

double total_cost(const double area, const double hpwl, const double alpha) {
    return alpha * area + (1 - alpha) * hpwl;
}

static void backup(const vector<pin>& pin_list,
                   vector<pin>& best,
                   const unsigned size) {
    best = vector<pin>(pin_list.begin(), pin_list.begin() + size);
}

static void restore(vector<pin>& pin_list, const vector<pin>& best) {
    copy(best.begin(), best.end(), pin_list.begin());
}

enum class mutation_type { delete_insert, permute, swap, mirror };

static mutation_type random_cdf(double a, double b, double c) {
    constexpr double rand_range = RAND_MAX + 1.;

    double rand_double = (double)rand() / rand_range;

    if (rand_double < a) {
        return mutation_type::delete_insert;
    }

    if (rand_double < b) {
        return mutation_type::permute;
    }

    if (rand_double < c) {
        return mutation_type::swap;
    }

    return mutation_type::mirror;
}

pair<int, int> sim_anneal(pair<unsigned, unsigned> boundary,
                          b_star& tree,
                          vector<pin>& pin_list,
                          const vector<net>& net_list,
                          pair<unsigned, unsigned> iter_info,
                          unsigned num_blocks,
                          unsigned episodes,
                          unsigned high_temp,
                          double alpha,
                          double ratio,
                          double P,
                          double C) {
    const unsigned width = boundary.first, height = boundary.second;
    const unsigned epochs = iter_info.first, interrupt = iter_info.second;
    const unsigned size = tree.nodes().size();
    const unsigned net_upper_bound = (width + height) * net_list.size();

    // Setup the different strategies.
    permuter permute{tree};
    swapper swap{tree};
    delete_inserter delete_insert{tree};
    mirrorer mirror{tree};

    auto initial = tree.update();

    printf("initial = (%d, %d)\n", initial.first, initial.second);

    unsigned init_width = initial.first, init_height = initial.second;
    unsigned init_hpwl = total_hpwl(net_list);
    double init_temp = 1. / log(1. / P);

    auto cost_func = cost{static_cast<double>(init_width * init_height),
                          static_cast<double>(init_hpwl), alpha, ratio};

    vector<pin> best_solution, best_accepted_solution;

    double best_cost = cost_func(init_width, width, init_height, height,
                                 init_hpwl, net_upper_bound, false);
    int best_width = init_width, best_height = init_height;

    backup(pin_list, best_solution, size);

    bool has_accepted;
    int iteration;

    for (has_accepted = false, iteration = -(high_temp * num_blocks);
         iteration < (int)(epochs * num_blocks); ++iteration) {
        double T = (iteration < 0) ? init_temp
                   : (iteration < (int)interrupt)
                       ? num_blocks * init_temp / (iteration * C)
                       : num_blocks * init_temp / iteration;

        unsigned idx;
        unsigned score_rdi, score_rp, score_rs, score_rm;
        double rdi_rp, rp_rs, rs_rm;

        for (idx = 0, rdi_rp = 1. / 4., rp_rs = 2. / 4., rs_rm = 3. / 4.;
             idx < episodes; ++idx) {
            score_rdi = score_rp = score_rs = score_rm = 0;

            mutation* strategy = nullptr;
            switch (random_cdf(rdi_rp, rp_rs, rs_rm)) {
                case mutation_type::delete_insert:
                    strategy = &delete_insert;
                    break;
                case mutation_type::permute:
                    strategy = &permute;
                    break;
                case mutation_type::swap:
                    strategy = &swap;
                    break;
                case mutation_type::mirror:
                    strategy = &mirror;
                    break;
            }

            strategy->random();

            auto plan{tree.update()};

            unsigned wire_len = total_hpwl(net_list);

            bool width_okay = plan.first <= (int)width;
            bool height_okay = plan.second <= (int)height;
            bool either_okay =
                (width_okay && height_okay) ||
                (plan.first <= (int)height && plan.second <= (int)width);

            if (!has_accepted && either_okay) {
                printf(
                    "size: (%d, %d) type: (%d, %d)\n", plan.first, plan.second,
                    (width_okay && height_okay),
                    (plan.first <= (int)height && plan.second <= (int)width));

                best_cost = INFINITY;
                has_accepted = true;
                best_width = best_height = -1;
            }

            double cost = cost_func(plan.first, width, plan.second, height,
                                    wire_len, net_upper_bound, has_accepted);

            bool acpt;

            if (acpt = !has_accepted || either_okay; acpt) {
                if (cost <= best_cost) {
                    best_cost = cost;
                    best_width = plan.first;
                    best_height = plan.second;
                    backup(pin_list, best_solution, size);
                    acpt = true;
                } else {
                    acpt = ((double)rand() / (double)RAND_MAX) <
                           expf((best_cost - cost) / T);
                }
            }

            if (!acpt) {
                strategy->revert();
            }

            cost_func.width(!width_okay, episodes);
            cost_func.height(!height_okay, episodes);

            double total_score = score_rdi + score_rp + score_rs + score_rm;
            rdi_rp = (double)score_rdi / total_score;
            rp_rs = rdi_rp + (double)score_rp / total_score;
            rs_rm = rp_rs + (double)score_rs / total_score;
        }
    }
    printf("a=%lf, w=%lf, h=%lf\n", cost_func.alpha(), cost_func.width(),
           cost_func.height());

    printf("solution found: %d\n", has_accepted);
    restore(pin_list, best_solution);

    if ((best_width > best_height) != (width > height)) {
        tree.flip();
        std::swap(best_width, best_height);
    }

    return make_pair(best_width, best_height);
}
