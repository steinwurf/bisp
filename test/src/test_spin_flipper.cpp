// Copyright (c) Steinwurf ApS 2018.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <bisp/spin_flipper.hpp>

#include <chrono>

#include <gtest/gtest.h>

namespace
{
void wait(std::chrono::milliseconds ms)
{
    auto start = std::chrono::high_resolution_clock::now();

    while (std::chrono::high_resolution_clock::now() - start < ms)
    {}
}
}

TEST(test_spin_flipper, basic)
{
    bisp::spin_flipper sf;
    EXPECT_EQ(boost::none, sf.rtt());

    EXPECT_EQ(1U, sf.outgoing());
    sf.incoming(1U);
    EXPECT_EQ(std::chrono::milliseconds(0), sf.rtt());

    EXPECT_EQ(2U, sf.outgoing());
    sf.incoming(2U);
    EXPECT_EQ(std::chrono::milliseconds(0), sf.rtt());
    EXPECT_EQ(3U, sf.outgoing());

    sf.incoming(1U);
    EXPECT_EQ(3U, sf.outgoing());
}

TEST(test_spin_sender, delay)
{
    // Allow variation
    uint32_t variation = 5;
    bisp::spin_flipper sf;
    EXPECT_EQ(1U, sf.outgoing());

    wait(std::chrono::milliseconds(30));
    sf.incoming(1U);
    EXPECT_GE(std::chrono::milliseconds(30), sf.rtt());
    EXPECT_LT(sf.rtt(), std::chrono::milliseconds(30 + variation));
    EXPECT_EQ(2U, sf.outgoing());

    wait(std::chrono::milliseconds(20));
    sf.incoming(2U);
    EXPECT_GE(std::chrono::milliseconds(20), sf.rtt());
    EXPECT_LT(sf.rtt(), std::chrono::milliseconds(20 + variation));
    EXPECT_EQ(3U, sf.outgoing());

    wait(std::chrono::milliseconds(10));
    sf.incoming(3U);
    EXPECT_GE(std::chrono::milliseconds(10), sf.rtt());
    EXPECT_LT(sf.rtt(), std::chrono::milliseconds(10 + variation));
    EXPECT_EQ(0U, sf.outgoing());

    wait(std::chrono::milliseconds(30));
    sf.incoming(0U);
    EXPECT_GE(std::chrono::milliseconds(30), sf.rtt());
    EXPECT_LT(sf.rtt(), std::chrono::milliseconds(30 + variation));
    EXPECT_EQ(1U, sf.outgoing());
}
