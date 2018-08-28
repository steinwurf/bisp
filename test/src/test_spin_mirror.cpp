// Copyright (c) Steinwurf ApS 2018.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <bisp/spin_mirror.hpp>

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

TEST(test_spin_mirror, basic)
{
    bisp::spin_mirror sm;
    EXPECT_EQ(boost::none, sm.rtt());

    sm.incoming(1U);
    EXPECT_EQ(1U, sm.outgoing());

    sm.incoming(2U);
    EXPECT_EQ(2U, sm.outgoing());
    EXPECT_EQ(std::chrono::milliseconds(0), sm.rtt().get());
}

TEST(test_spin_receiver, delay)
{
    bisp::spin_mirror sm;

    sm.incoming(2U);
    EXPECT_EQ(boost::none, sm.rtt());
    EXPECT_EQ(2U, sm.outgoing());

    wait(std::chrono::milliseconds(20));
    sm.incoming(3U);
    EXPECT_EQ(3U, sm.outgoing());
    EXPECT_GE(sm.rtt().get(), std::chrono::milliseconds(20));

    wait(std::chrono::milliseconds(7));
    sm.incoming(0U);
    EXPECT_EQ(0U, sm.outgoing());
    EXPECT_GE(sm.rtt().get(), std::chrono::milliseconds(7));

    wait(std::chrono::milliseconds(12));
    sm.incoming(1U);
    EXPECT_EQ(1U, sm.outgoing());
    EXPECT_GE(sm.rtt().get(), std::chrono::milliseconds(12));

    sm.incoming(3U);
    EXPECT_EQ(1U, sm.outgoing());
    EXPECT_GE(sm.rtt().get(), std::chrono::milliseconds(12));

    sm.incoming(0U);
    EXPECT_EQ(1U, sm.outgoing());
    EXPECT_GE(sm.rtt().get(), std::chrono::milliseconds(12));
}
