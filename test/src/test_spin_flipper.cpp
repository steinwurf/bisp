// Copyright (c) Steinwurf ApS 2018.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <bisp/spin_flipper.hpp>

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


TEST(test_spin_flipper, basic)
{
    bisp::spin_flipper<> sf;
    EXPECT_EQ(boost::none, sf.rtt());

    EXPECT_EQ(1U, sf.outgoing());
    sf.incomming(1U);
    EXPECT_EQ(std::chrono::milliseconds(0), sf.rtt());

    EXPECT_EQ(2U, sf.outgoing());
    sf.incomming(2U);
    EXPECT_EQ(std::chrono::milliseconds(0), sf.rtt());
    EXPECT_EQ(3U, sf.outgoing());

    sf.incomming(1U);
    EXPECT_EQ(3U, sf.outgoing());
}

TEST(test_spin_sender, delay)
{
    bisp::spin_flipper<> sf;
    EXPECT_EQ(1U, sf.outgoing());

    wait(std::chrono::milliseconds(30));
    sf.incomming(1U);
    EXPECT_EQ(std::chrono::milliseconds(30), sf.rtt());
    EXPECT_EQ(2U, sf.outgoing());

    wait(std::chrono::milliseconds(20));
    sf.incomming(2U);
    EXPECT_EQ(std::chrono::milliseconds(20), sf.rtt());
    EXPECT_EQ(3U, sf.outgoing());

    wait(std::chrono::milliseconds(10));
    sf.incomming(3U);
    EXPECT_EQ(std::chrono::milliseconds(10), sf.rtt());
    EXPECT_EQ(0U, sf.outgoing());

    wait(std::chrono::milliseconds(30));
    sf.incomming(0U);
    EXPECT_EQ(std::chrono::milliseconds(30), sf.rtt());
    EXPECT_EQ(1U, sf.outgoing());
}
