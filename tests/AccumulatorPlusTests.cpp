// BoostAccTest.cpp : Defines the entry point for the console application.
//

#include <algorithm>
#include <iostream>

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

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

// Rolling min, max and previous
#include "rolling_min.h"
#include "rolling_max.h"
#include "rolling_prev.h"

// Rolling time versions
#include "rolling_time_window.h"
#include "rolling_time_sum.h"
#include "rolling_time_min.h"
#include "rolling_time_max.h"
#include "rolling_time_mean.h"
#include "rolling_time_moment.h"

// Rolling time-stamp versions
#include "rolling_timestamp_window.h"
#include "rolling_timestamp_sum.h"
#include "rolling_timestamp_min.h"
#include "rolling_timestamp_max.h"
#include "rolling_timestamp_mean.h"
#include "rolling_timestamp_count.h"
#include "rolling_timestamp_moment.h"
#include "rolling_timestamp_std.h"
#include "rolling_timestamp_prev.h"

// Zero crossings
#include "zxings.h"

TEST_CASE("Check previous value", "[Accumulator Plus Tests]")
{
    accumulator_set<double, stats<tag::rolling_timestamp_prev>, double> acc(tag::rolling_timestamp_window::window_time = 2.0);

    acc(1.0, timestamp = 1.0);
    REQUIRE(rolling_timestamp_prev(acc) == Approx(1.0));

    acc(1.3, timestamp = 1.8);
	REQUIRE(rolling_timestamp_prev(acc) == Approx(1.0));

    acc(2.0, timestamp = 2.5);
	REQUIRE(rolling_timestamp_prev(acc) ==  Approx(1.0));

    acc(2.2, timestamp = 3.1);
	REQUIRE(rolling_timestamp_prev(acc) == Approx(1.0));

    acc(1.2, timestamp = 3.9);
	REQUIRE(rolling_timestamp_prev(acc) == Approx(1.3));

    acc(0.5, timestamp = 5.5);
	REQUIRE(rolling_timestamp_prev(acc) == Approx(2.2));

    acc(0.5, timestamp = 5.8);
	REQUIRE(rolling_timestamp_prev(acc) == Approx(2.2));

    acc(0.5, timestamp = 5.9);
	REQUIRE(rolling_timestamp_prev(acc) == Approx(1.2));

    acc(0.5, timestamp = 6.0);
	REQUIRE(rolling_timestamp_prev(acc) == Approx(1.2));
}

TEST_CASE("Check standard deviation with time-stamp", "[Accumulator Plus Tests]")
{
    accumulator_set<double, stats<tag::rolling_timestamp_std>, double> acc(tag::rolling_timestamp_window::window_time = 2.0);

    acc(1.0, timestamp = 1.0);
    REQUIRE(rolling_timestamp_std(acc) == Approx(0.0));

    acc(1.0, timestamp = 1.8);
    REQUIRE(rolling_timestamp_std(acc) == Approx(0.0));

    acc(2.0, timestamp = 2.5);
    REQUIRE(rolling_timestamp_std(acc) == Approx(5.77350269189626e-001));

    acc(2.2, timestamp = 3.1);
	REQUIRE(rolling_timestamp_std(acc) == Approx(6.42910050732864e-001));

    acc(1.2, timestamp = 3.9);
	REQUIRE(rolling_timestamp_std(acc) == Approx(5.29150262212918e-001));

    acc(0.5, timestamp = 5.5);
	REQUIRE(rolling_timestamp_std(acc) == Approx(4.94974746830583e-001));
}

// Since Matlab calculates by the central moment, we will just test it from the central moment
TEST_CASE("Check moment with time-stamp", "[Accumulator Plus Tests]")
{
    accumulator_set<double, stats<tag::rolling_timestamp_mean, tag::rolling_timestamp_moment<2>>, double> 
        acc(tag::rolling_timestamp_window::window_time = 2.0);

    acc(1.0, timestamp = 1.0);
	REQUIRE(rolling_timestamp_moment<2>(acc) - std::pow(rolling_timestamp_mean(acc), 2) == Approx(0.0));

    acc(1.0, timestamp = 1.8);
	REQUIRE(rolling_timestamp_moment<2>(acc) - std::pow(rolling_timestamp_mean(acc), 2) == Approx(0.0));

    acc(2.0, timestamp = 2.5);
	REQUIRE(rolling_timestamp_moment<2>(acc) - std::pow(rolling_timestamp_mean(acc), 2) == Approx(2.0 / 9));

    acc(2.2, timestamp = 3.1);
	REQUIRE(rolling_timestamp_moment<2>(acc) - std::pow(rolling_timestamp_mean(acc), 2) == Approx(9.84 / 3 - std::pow(26.0 / 15, 2)));

    acc(1.2, timestamp = 3.9);
	REQUIRE(rolling_timestamp_moment<2>(acc) - std::pow(rolling_timestamp_mean(acc), 2) == Approx(10.280 / 3 - std::pow(9.0 / 5, 2)));

    acc(0.5, timestamp = 5.5);
	REQUIRE(rolling_timestamp_moment<2>(acc) - std::pow(rolling_timestamp_mean(acc), 2) == Approx(1.69 / 2 - std::pow(17.0 / 20, 2)));
}

TEST_CASE("Rolling count with time-stamp", "[Accumulator Plus Tests]")
{
    accumulator_set<double, stats<tag::rolling_timestamp_count>, double> acc(tag::rolling_timestamp_window::window_time = 2.0);

    acc(1.0, timestamp = 1.0);
    REQUIRE(rolling_timestamp_count(acc) == Approx(1));

    acc(1.0, timestamp = 1.8);
    REQUIRE(rolling_timestamp_count(acc) == Approx(2));

    acc(2.0, timestamp = 2.5);
    REQUIRE(rolling_timestamp_count(acc) == Approx(3));

    acc(2.2, timestamp = 3.1);
    REQUIRE(rolling_timestamp_count(acc) == Approx(3));

    acc(1.2, timestamp = 3.9);
    REQUIRE(rolling_timestamp_count(acc) == Approx(3));

    acc(0.5, timestamp = 5.5);
    REQUIRE(rolling_timestamp_count(acc) == Approx(2));
}

TEST_CASE("Mean with time-stamp", "[Accumulator Plus Tests]")
{
    accumulator_set<double, stats<tag::rolling_timestamp_mean>, double> acc(tag::rolling_timestamp_window::window_time = 2.0);

    acc(1.0, timestamp = 1.0);
    REQUIRE(rolling_timestamp_mean(acc) == Approx(1.0));

    acc(1.0, timestamp = 1.8);
    REQUIRE(rolling_timestamp_mean(acc) == Approx(1.0));

    acc(2.0, timestamp = 2.5);
    REQUIRE(rolling_timestamp_mean(acc) == Approx(4.0 / 3));

    acc(2.2, timestamp = 3.1);
    REQUIRE(rolling_timestamp_mean(acc) == Approx(5.2 / 3));

    acc(1.2, timestamp = 3.9);
    REQUIRE(rolling_timestamp_mean(acc) == Approx(5.4 / 3));

    acc(0.5, timestamp = 5.5);
    REQUIRE(rolling_timestamp_mean(acc) == Approx(1.7 / 2));
}

TEST_CASE("Check max time-stamp values", "[Accumulator Plus Tests]")
{
    accumulator_set<double, stats<tag::rolling_timestamp_max>, double> acc(tag::rolling_timestamp_window::window_time = 2.0);

    acc(1.0, timestamp = 1.0);
    REQUIRE(rolling_timestamp_max(acc) ==  Approx(1.0));

    acc(1.0, timestamp = 1.8);
	REQUIRE(rolling_timestamp_max(acc) == Approx(1.0));

    acc(2.0, timestamp = 2.5);
	REQUIRE(rolling_timestamp_max(acc) == Approx(2.0));

    acc(2.2, timestamp = 3.1);
	REQUIRE(rolling_timestamp_max(acc) == Approx (2.2));

    acc(1.2, timestamp = 3.9);
	REQUIRE(rolling_timestamp_max(acc) == Approx(2.2));

    acc(0.5, timestamp = 5.5);
	REQUIRE(rolling_timestamp_max(acc) == Approx(1.2));
}

TEST_CASE("Check min time-stamp values", "[Accumulator Plus Tests]")
{
    accumulator_set<double, stats<tag::rolling_timestamp_min>, double> acc(tag::rolling_timestamp_window::window_time = 2.0);

    acc(1.0, timestamp = 1.0);
    REQUIRE(rolling_timestamp_min(acc) == Approx(1.0));

    acc(1.0, timestamp = 1.8);
    REQUIRE(rolling_timestamp_min(acc) == Approx(1.0));

    acc(2.0, timestamp = 2.5);
    REQUIRE(rolling_timestamp_min(acc) == Approx(1.0));

    acc(2.2, timestamp = 3.1);
    REQUIRE(rolling_timestamp_min(acc) == Approx(1.0));

    acc(3.0, timestamp = 3.9);
    REQUIRE(rolling_timestamp_min(acc) == Approx(2.0));
}

TEST_CASE("Rolling Timestamp Sum Test", "[Accumulator Plus Tests]")
{
    // samples: double, accumulator: sum, timestamp: double
    accumulator_set<double, stats<tag::rolling_timestamp_sum>, double> acc(tag::rolling_timestamp_window::window_time = 2.0);

    acc(1.0, timestamp = 1.0);
    REQUIRE(rolling_timestamp_sum(acc) == Approx(1.0));

    acc(1.0, timestamp = 1.8);
    REQUIRE(rolling_timestamp_sum(acc) == Approx(2.0));

    acc(2.0, timestamp = 2.5);
    REQUIRE(rolling_timestamp_sum(acc) == Approx(4.0));

    acc(2.2, timestamp = 3.1);
    REQUIRE(rolling_timestamp_sum(acc) == Approx(5.2));

    acc(1.0, timestamp = 3.7);
    REQUIRE(rolling_timestamp_sum(acc) == Approx(6.2));
}

TEST_CASE("Rolling Time Window Moment Test", "[Accumulator Plus Tests]")
{
    accumulator_set<std::pair<double, double>, stats<tag::rolling_time_mean, tag::rolling_time_moment<2>>> acc(tag::rolling_time_window::window_time = 2.0);

    acc(std::pair<double, double>(1.0, 1.0));
    REQUIRE(rolling_time_moment<2>(acc).first - std::pow(rolling_time_mean(acc).first, 2) == Approx(0.0));

    acc(std::pair<double, double>(1.0, 1.8));
    REQUIRE(rolling_time_moment<2>(acc).first - std::pow(rolling_time_mean(acc).first, 2) == Approx(0.0));

    acc(std::pair<double, double>(2.0, 2.5));
    REQUIRE(rolling_time_moment<2>(acc).first - std::pow(rolling_time_mean(acc).first, 2) == Approx(2.0 / 9));

    acc(std::pair<double, double>(2.2, 3.1));
    REQUIRE(rolling_time_moment<2>(acc).first - std::pow(rolling_time_mean(acc).first, 2) == Approx(9.84/3 - std::pow(26.0/15,2)));

    acc(std::pair<double, double>(1.2, 3.9));
    REQUIRE(rolling_time_moment<2>(acc).first - std::pow(rolling_time_mean(acc).first, 2) == Approx(10.280 / 3 - std::pow(9.0/5,2)));

    acc(std::pair<double, double>(0.5, 5.5));
    REQUIRE(rolling_time_moment<2>(acc).first - std::pow(rolling_time_mean(acc).first, 2) == Approx(1.69/2 - std::pow(17.0/20,2)));
}

TEST_CASE("Rolling time window mean test", "[Accumulator Plus Tests]")
{
    accumulator_set<std::pair<double, double>, stats<tag::rolling_time_mean>> acc(tag::rolling_time_window::window_time = 2.0);

    acc(std::pair<double, double>(1.0, 1.0));
    REQUIRE(rolling_time_mean(acc).first == Approx(1.0));

    acc(std::pair<double, double>(1.0, 1.8));
    REQUIRE(rolling_time_mean(acc).first == Approx(1.0));

    acc(std::pair<double, double>(2.0, 2.5));
    REQUIRE(rolling_time_mean(acc).first == Approx(4.0/3));

    acc(std::pair<double, double>(2.2, 3.1));
    REQUIRE(rolling_time_mean(acc).first == Approx(5.2/3));

    acc(std::pair<double, double>(1.2, 3.9));
    REQUIRE(rolling_time_mean(acc).first == Approx(5.4/3));

    acc(std::pair<double, double>(0.5, 5.5));
    REQUIRE(rolling_time_mean(acc).first == Approx(1.7/2));
}

TEST_CASE("Rolling time window max values", "[Accumulator Plus Tests]")
{
    accumulator_set<std::pair<double, double>, stats<tag::rolling_time_max>> acc(tag::rolling_time_window::window_time = 2.0);

    acc(std::pair<double, double>(1.0, 1.0));
    REQUIRE(rolling_time_max(acc).first == Approx(1.0));

    acc(std::pair<double, double>(1.0, 1.8));
    REQUIRE(rolling_time_max(acc).first == Approx(1.0));

    acc(std::pair<double, double>(2.0, 2.5));
    REQUIRE(rolling_time_max(acc).first == Approx(2.0));

    acc(std::pair<double, double>(2.2, 3.1));
    REQUIRE(rolling_time_max(acc).first == Approx(2.2));

    acc(std::pair<double, double>(1.2, 3.9));
    REQUIRE(rolling_time_max(acc).first == Approx(2.2));

    acc(std::pair<double, double>(0.5, 5.5));
    REQUIRE(rolling_time_max(acc).first == Approx(1.2));
}

TEST_CASE("Rolling time window min values", "[Accumulator Plus Tests]")
{
    accumulator_set<std::pair<double, double>, stats<tag::rolling_time_min>> acc(tag::rolling_time_window::window_time = 2.0);

    acc(std::pair<double, double>(1.0, 1.0));
    REQUIRE(rolling_time_min(acc).first == Approx(1.0));

    acc(std::pair<double, double>(1.0, 1.8));
    REQUIRE(rolling_time_min(acc).first == Approx(1.0));

    acc(std::pair<double, double>(2.0, 2.5));
    REQUIRE(rolling_time_min(acc).first == Approx(1.0));

    acc(std::pair<double, double>(2.2, 3.1));
    REQUIRE(rolling_time_min(acc).first == Approx(1.0));
    
	acc(std::pair<double, double>(3.0, 3.9));
    REQUIRE(rolling_time_min(acc).first == Approx(2.0));
}

TEST_CASE("Rolling time window sum values", "[Accumulator Plus Tests]")
{
    accumulator_set<std::pair<double, double>, stats<tag::rolling_time_sum>> acc(tag::rolling_time_window::window_time = 2.0);

    acc(std::pair<double, double>(1.0, 1.0));
    REQUIRE(rolling_time_sum(acc).first == Approx(1.0));

    acc(std::pair<double, double>(1.0, 1.8));
    REQUIRE(rolling_time_sum(acc).first == Approx(2.0));

    acc(std::pair<double, double>(2.0, 2.5));
    REQUIRE(rolling_time_sum(acc).first == Approx(4.0));

    acc(std::pair<double, double>(2.2, 3.1));
    REQUIRE(rolling_time_sum(acc).first == Approx(5.2));

    acc(std::pair<double, double>(1.0, 3.7));
    REQUIRE(rolling_time_sum(acc).first == Approx(6.2));
}

// Basic test to check that the rolling window functions work
TEST_CASE("Rolling window test", "[Accumulator Plus Tests]")
{
    // Create a rolling window function
    accumulator_set<double, stats<tag::rolling_sum, tag::rolling_mean>> acc(tag::rolling_window::window_size = 3u);

    for (double f : {1.2, 2.3, 3.4, 4.5})
        acc(f);

    REQUIRE(rolling_sum(acc) == Approx(10.2));
    REQUIRE(rolling_mean(acc) == Approx(3.4));
    REQUIRE(rolling_count(acc) == Approx(3));
}

TEST_CASE("Multi Rolling Window Min", "[Accumulator Plus Tests]")
{
	// Fix of weird linker error of not finding std::min for int
	std::min(0, 0);

    accumulator_set<double, stats<tag::rolling_min>> acc1(tag::rolling_window::window_size = 4u);
    accumulator_set<int, stats<tag::rolling_min>> acc2(tag::rolling_window::window_size = 6u);
    for (auto f : { 1.2, 2.3, 3.4, 4.5, 1.4, 2.3, 2.3, 4.2, 3.3 })
    {
        acc1(f);
    }
    for (auto f : { 1, 2, 3, 4, 5, 6, 7, 8 })
    {
        acc2(f);
    }
    REQUIRE(rolling_min(acc1) == Approx(2.3));
    REQUIRE(rolling_min(acc2) == 3);
}

// Test the rolling window min functions
template<class Sample>
Sample RollingWindowPrevTest(unsigned int window_size, std::initializer_list<Sample> num_list)
{
    // Create a rolling window function
    accumulator_set<Sample, stats<tag::rolling_prev>> acc(tag::rolling_window::window_size = window_size);
    for (Sample f : num_list)
        acc(f);
    return rolling_prev(acc);
}

TEST_CASE("Rolling Window Prev Test", "[Accumulator Plus Tests]")
{
	REQUIRE(RollingWindowPrevTest<double>(4u, { 1.2, 2.3, 3.4, 4.5, 1.4, 2.3, 2.3, 4.2, 3.3 }) == Approx(2.3));
	REQUIRE(RollingWindowPrevTest<double>(4u, { 1.2, 2.3, 3.4, 4.5, 1.4, 2.3, 2.3, 4.2 }) == Approx(1.4));
	REQUIRE(RollingWindowPrevTest<double>(4u, { 1.2, 2.3, 3.4, 4.5, 1.4, 2.3, 2.3 }) == Approx(4.5));
	REQUIRE(RollingWindowPrevTest<int>(5u, { 1, 2, 3, 4, 5, 6, 7, 8 }) == Approx(4));
	REQUIRE(RollingWindowPrevTest<int>(1u, { 1, 2, 3, 4, 5, 6, 7, 8 }) == Approx(8));
	REQUIRE(RollingWindowPrevTest<double>(3u, { 1.2, 2.3, 3.4, 4.5 }) == Approx(2.3));
	REQUIRE(RollingWindowPrevTest<double>(6u, { 1.2, 2.3, 3.4, 4.5 }) == Approx(1.2));
	REQUIRE(RollingWindowPrevTest<double>(6u, { 1.2 }) == Approx(1.2));
}

// Test the rolling window min functions
template<class Sample>
Sample RollingWindowMinTest(unsigned int window_size, std::initializer_list<Sample> num_list)
{
    // Create a rolling window function
    accumulator_set<Sample, stats<tag::rolling_min>> acc(tag::rolling_window::window_size = window_size);
    for (Sample f : num_list)
        acc(f);
    return rolling_min(acc);
}

TEST_CASE("Rolling Window Min", "[Accumulator Plus Tests]")
{
    REQUIRE(RollingWindowMinTest<double>(4u, { 1.2, 2.3, 3.4, 4.5, 1.4, 2.3, 2.3, 4.2, 3.3 }) == Approx(2.3));
    REQUIRE(RollingWindowMinTest<int>(5u, { 1, 2, 3, 4, 5, 6, 7, 8 }) == 4);
    REQUIRE(RollingWindowMinTest<int>(1u, { 1, 2, 3, 4, 5, 6, 7, 8 }) == 8);
    REQUIRE(RollingWindowMinTest<double>(3u, { 1.2, 2.3, 3.4, 4.5 }) == Approx(2.3));
    REQUIRE(RollingWindowMinTest<double>(6u, { 1.2, 2.3, 3.4, 4.5 }) == Approx(1.2));
}

template<class Sample>
Sample RollingWindowMaxTest(unsigned int window_size, std::initializer_list<Sample> num_list)
{
    // Create a rolling window function
    accumulator_set<Sample, stats<tag::rolling_max>> acc(tag::rolling_window::window_size = window_size);
    for (Sample f : num_list)
        acc(f);
    return rolling_max(acc);
}

// Test if a rolling window min can be created in the boost accumulator framework
TEST_CASE("Rolling window Max", "[Accumulator Plus Tests]")
{
	// Some weird linker error of not finding __cdecl std::max<const int&, const int&> ...
	std::max(0, 0);

    REQUIRE(RollingWindowMaxTest(4u, { 1.2, 2.3, 3.4, 4.5, 1.4, 2.3, 2.3, 4.2, 3.3 }) == Approx(4.2));
    REQUIRE(RollingWindowMaxTest(5, { 1, 2, 3, 4, 5, 6, 7, 8 }) == 8);
    REQUIRE(RollingWindowMaxTest(5, { 1, 2, 3, 4, 5, 6, 7, 8 }) == 8);
    REQUIRE(RollingWindowMaxTest<double>(3u, { 1.2, 2.3, 3.4, 4.5 }) == Approx(4.5));
    REQUIRE(RollingWindowMaxTest<double>(6u, { 1.2, 2.3, 3.4, 4.5 }) == Approx(4.5));
}

TEST_CASE("Zero crossings", "[Accumulator Plus Tests]")
{
	accumulator_set<double, stats<tag::zxings>> acc;

	acc(1.0);
	REQUIRE(zxings(acc) == 0);
	acc(2.0);
	REQUIRE(zxings(acc) == 0);
	acc(-1.0);
	REQUIRE(zxings(acc) == 1);
	acc(1.4);
	REQUIRE(zxings(acc) == 2);
}
