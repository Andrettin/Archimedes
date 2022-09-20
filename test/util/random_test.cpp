#include "archimedes.h"

#include "util/random.h"

#include <boost/test/unit_test.hpp>

using namespace archimedes;

BOOST_AUTO_TEST_CASE(random_seed_test)
{
    std::vector<unsigned> random_seeds;

    for (size_t i = 0; i < 1000; ++i) {
        random::get()->reset_seed(false);
        random_seeds.push_back(random::get()->get_seed());
    }

    for (size_t i = 0; i < 1000; ++i) {
        for (size_t j = 0; j < 1000; ++j) {
            if (i == j) {
                continue;
            }

            //check if none of the two generated numbers are equal; in principle it is possible that they would be, but in practice we wouldn't expect this to happen
            BOOST_CHECK(random_seeds[i] != random_seeds[j]);
        }
    }
}

BOOST_AUTO_TEST_CASE(random_number_generation_test)
{
    random::get()->reset_seed(false);

    std::vector<int> random_numbers;

    for (size_t i = 0; i < 1000; ++i) {
        random_numbers.push_back(random::get()->generate(std::numeric_limits<int>::max()));
    }

    for (size_t i = 0; i < 1000; ++i) {
        for (size_t j = 0; j < 1000; ++j) {
            if (i == j) {
                continue;
            }

            //check if none of the two generated numbers are equal; in principle it is possible that they would be, but in practice we wouldn't expect this to happen
            BOOST_CHECK(random_numbers[i] != random_numbers[j]);
        }
    }
}
