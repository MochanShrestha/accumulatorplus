
#include "catch.hpp"

#include <boost/circular_buffer.hpp>
#include <boost/circular_buffer/space_optimized.hpp>

#include <deque>
#include <chrono>
#include <ctime>

#include "circular_time_buffer.h"

// Test from the boost 
TEST_CASE("Test Circular BUffer", "[Circular Buffer Tests]")
{
    // Create a circular buffer with a capacity for 3 integers
    boost::circular_buffer<int> cb(3);

    // Insert the three elements into the buffer
    cb.push_back(1);
    cb.push_back(2);
    cb.push_back(3);

    REQUIRE(cb[0] == 1);
    REQUIRE(cb[1] == 2);
    REQUIRE(cb[2] == 3);

    // The buffer is full now, so pushing subsequent elements will overwrite the front-most elements
    cb.push_back(4);
    cb.push_back(5);

    // The buffer now contains 3, 4 and 5
    REQUIRE(cb[0] == 3);
    REQUIRE(cb[1] == 4);
    REQUIRE(cb[2] == 5);

    // Elements can be popped from either the front or the back
    cb.pop_back();
    cb.pop_front();

    REQUIRE(cb[0] == 4);
}

TEST_CASE("Test Space Optimized Circular BUffer", "[Circular Buffer Tests]")
{
    boost::circular_buffer_space_optimized<int> cb(3);

    // Insert the three elements into the buffer
    cb.push_back(1);
    cb.push_back(2);
    cb.push_back(3);

    REQUIRE(cb[0] == 1);
    REQUIRE(cb[1] == 2);
    REQUIRE(cb[2] == 3);

    // The buffer is full now, so pushing subsequent elements will overwrite the front-most elements
    cb.push_back(4);
    cb.push_back(5);

    // The buffer now contains 3, 4 and 5
    REQUIRE(cb[0] == 3);
    REQUIRE(cb[1] == 4);
    REQUIRE(cb[2] == 5);

    // Elements can be popped from either the front or the back
    cb.pop_back();
    cb.pop_front();

    REQUIRE(cb[0] == 4);
}

// Check the dequeue data structure for writing adding and removing items from the end of the list
TEST_CASE("Dequeue Circular BUffer", "[Circular Buffer Tests]")
{
    std::deque<int> q;

    q.push_back(1);
    q.push_back(2);
    q.push_back(3);

    REQUIRE(q[0] == 1);
    REQUIRE(q[1] == 2);
    REQUIRE(q[2] == 3);

    q.pop_front();

    REQUIRE(q[0] == 2);

    q.push_back(4);

    REQUIRE(q[2] == 4);
}

TEST_CASE("Circular Time BUffer", "[Circular Buffer Tests]")
{
    // Create the time circular buffer
    circular_time_buffer<int, double> cb(3.1, 0.0);

    // Add a few elements to the circular buffer
    cb.insert(2, 0.1);
    cb.insert(3, 0.8);
    cb.insert(4, 1.8);
    cb.insert(5, 2.5);

    // Check that the values were inserted properly
    REQUIRE(cb[0].first == 2);
    REQUIRE(cb[0].second == Approx(0.1));
    REQUIRE(cb[1].first == 3);
    REQUIRE(cb[1].second == Approx(0.8));
    REQUIRE(cb[2].first == 4);
    REQUIRE(cb[2].second == Approx(1.8));

    // Check that the get older works
    REQUIRE(cb.get_older(3.3) == 1);
    REQUIRE(cb.get_older(4.0) == 2);
    REQUIRE(cb.get_older(5.0) == 3);

    // Check that values start falling off when we add new values after the duration
    cb.insert(6, 3.3);
    REQUIRE(cb[0].first == 3);
    cb.insert(7, 4.0);
    REQUIRE(cb[0].first == 4);
    cb.insert(8, 5.0);
    REQUIRE(cb[0].first == 5);
}

TEST_CASE("Circular Chrono Time BUffer", "[Circular Buffer Tests]")
{
    typedef std::chrono::duration<double> double_secs;

    circular_time_buffer<int, double_secs> cb(double_secs(3.1), double_secs(0.0));

    cb.insert(2, double_secs(0.1));
    cb.insert(3, double_secs(0.8));
    cb.insert(4, double_secs(1.8));
    cb.insert(5, double_secs(2.5));

    REQUIRE(cb[0].first == 2);
    REQUIRE(cb[0].second.count() == Approx(0.1));
    REQUIRE(cb[1].first == 3);
    REQUIRE(cb[1].second.count() == Approx(0.8));
    REQUIRE(cb[2].first == 4);
    REQUIRE(cb[2].second.count() == Approx(1.8));

    // Check that the get older works
    REQUIRE(cb.get_older(double_secs(3.3)) == 1);
    REQUIRE(cb.get_older(double_secs(4.0)) == 2);
    REQUIRE(cb.get_older(double_secs(5.0)) == 3);

    // Check that values start falling off when we add new values after the duration
    cb.insert(6, double_secs(3.3));
    REQUIRE(cb[0].first == 3);
    cb.insert(7, double_secs(4.0));
    REQUIRE(cb[0].first == 4);
    cb.insert(8, double_secs(5.0));
    REQUIRE(cb[0].first == 5);
}

