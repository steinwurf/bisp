// Copyright (c) Steinwurf ApS 2018.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <bisp/spin_mirror.hpp>

#include <chrono>

#include <gtest/gtest.h>


namespace
{
using clock_type = std::chrono::high_resolution_clock;

void wait(std::chrono::milliseconds ms)
{
    auto start = clock_type::now();

    while (clock_type::now() - start < ms)
    {}
}
}


TEST(test_spin_mirror, basic)
{
    bisp::spin_mirror<> sm;
    EXPECT_EQ(boost::none, sm.rtt());

    sm.incomming(1U);
    EXPECT_EQ(1U, sm.outgoing());

    sm.incomming(2U);
    EXPECT_EQ(2U, sm.outgoing());
    EXPECT_EQ(std::chrono::milliseconds(0), sm.rtt());
}

TEST(test_spin_receiver, delay)
{
    bisp::spin_mirror<> sm;

    sm.incomming(2U);
    EXPECT_EQ(boost::none, sm.rtt());
    EXPECT_EQ(2U, sm.outgoing());

    wait(std::chrono::milliseconds(20));
    sm.incomming(3U);
    EXPECT_EQ(3U, sm.outgoing());
    EXPECT_EQ(std::chrono::milliseconds(20), sm.rtt());

    wait(std::chrono::milliseconds(7));
    sm.incomming(0U);
    EXPECT_EQ(0U, sm.outgoing());
    EXPECT_EQ(std::chrono::milliseconds(7), sm.rtt());

    wait(std::chrono::milliseconds(12));
    sm.incomming(1U);
    EXPECT_EQ(1U, sm.outgoing());
    EXPECT_EQ(std::chrono::milliseconds(12), sm.rtt());

    sm.incomming(3U);
    EXPECT_EQ(1U, sm.outgoing());
    EXPECT_EQ(std::chrono::milliseconds(12), sm.rtt());

    sm.incomming(0U);
    EXPECT_EQ(1U, sm.outgoing());
    EXPECT_LE(std::chrono::milliseconds(12), sm.rtt());
}
