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
    ASSERT_EQ(boost::none, sf.rtt());

    EXPECT_EQ(1U, sf.outgoing());
    sf.incoming(1U);
    EXPECT_GE(sf.rtt().get(), std::chrono::milliseconds(0));

    EXPECT_EQ(2U, sf.outgoing());
    sf.incoming(2U);
    EXPECT_GE(sf.rtt().get(), std::chrono::milliseconds(0));
    EXPECT_EQ(3U, sf.outgoing());

    sf.incoming(1U);
    EXPECT_EQ(3U, sf.outgoing());
}

TEST(test_spin_sender, delay)
{
    bisp::spin_flipper sf;
    EXPECT_EQ(1U, sf.outgoing());

    wait(std::chrono::milliseconds(30));
    sf.incoming(1U);
    EXPECT_GE(sf.rtt().get(), std::chrono::milliseconds(30));
    EXPECT_EQ(2U, sf.outgoing());

    wait(std::chrono::milliseconds(20));
    sf.incoming(2U);
    EXPECT_GE(sf.rtt().get(), std::chrono::milliseconds(20));
    EXPECT_EQ(3U, sf.outgoing());

    wait(std::chrono::milliseconds(10));
    sf.incoming(3U);
    EXPECT_GE(sf.rtt().get(), std::chrono::milliseconds(10));
    EXPECT_EQ(0U, sf.outgoing());

    wait(std::chrono::milliseconds(30));
    sf.incoming(0U);
    EXPECT_GE(sf.rtt().get(), std::chrono::milliseconds(30));
    EXPECT_EQ(1U, sf.outgoing());
}
