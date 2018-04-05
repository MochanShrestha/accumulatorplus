
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/min.hpp>
#include <boost/accumulators/statistics/max.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/moment.hpp>
#include <boost/accumulators/statistics/sum.hpp>
#include <boost/accumulators/statistics/weighted_sum.hpp>
#include <boost/accumulators/statistics/kurtosis.hpp>
#include <boost/accumulators/statistics/rolling_sum.hpp>
#include <boost/accumulators/statistics/rolling_mean.hpp>
#include <boost/accumulators/statistics/rolling_moment.hpp>
#include <boost/accumulators/framework/accumulator_base.hpp>
#include <boost/accumulators/framework/parameters/sample.hpp>
using namespace boost::accumulators;

#include "catch.hpp"

// Basic test to check that the accumulators compiles and works and computes the
// basic mean, std and count correctly
TEST_CASE("Hello World", "[Accumulator]")
{
    // Define an accumulator set for calculating the mean and the
    // 2nd moment ...
    accumulator_set<double, stats<tag::mean, tag::moment<2>> > acc;

    // push in some data ...
    for (double f : {1.2, 2.3, 3.4, 4.5})
        acc(f);

    // Check that the results are correct
    REQUIRE(mean(acc) == Approx(2.85));
    REQUIRE(moment<2>(acc) == Approx(9.635));
    REQUIRE(count(acc) == Approx(4));
}

template<class Sample>
Sample MaxAccTest(std::initializer_list<Sample> num_list)
{
	// Create a rolling window function
	accumulator_set<Sample, stats<tag::max>> acc;
	for (Sample f : num_list)
		acc(f);
	return max(acc);
}

TEST_CASE("Weighted Sum", "[Accumulator]")
{
	// Since we specified a weight, tag::sum becomes tag::weighted_sum
	accumulator_set< int, features< tag::weighted_sum >, int > acc;

	acc(1, weight = 2); //   1 * 2
	acc(2, weight = 4); //   2 * 4
	acc(3, weight = 6); // + 3 * 6
						// -------
						// =    28

	REQUIRE(sum(acc) == 28);
}

TEST_CASE("Max", "[Accumulator]")
{
	REQUIRE(MaxAccTest<double>({ 1.2, 2.3, 3.4, 4.5 }) == Approx(4.5));
}


