// Copyright (c) Steinwurf ApS 2018.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <bisp/spin_mirror.hpp>

#include <thread>
#include <chrono>

#include <gtest/gtest.h>


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

    // any spin is valid when starting
    sm.incomming(2U);
    EXPECT_EQ(boost::none, sm.rtt());
    EXPECT_EQ(2U, sm.outgoing());

    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    sm.incomming(3U);
    EXPECT_EQ(3U, sm.outgoing());
    EXPECT_EQ(std::chrono::milliseconds(20), sm.rtt());

    std::this_thread::sleep_for(std::chrono::milliseconds(17));
    sm.incomming(0U);
    EXPECT_EQ(0U, sm.outgoing());
    EXPECT_EQ(std::chrono::milliseconds(17), sm.rtt());

    std::this_thread::sleep_for(std::chrono::milliseconds(42));
    sm.incomming(1U);
    EXPECT_EQ(1U, sm.outgoing());
    EXPECT_EQ(std::chrono::milliseconds(42), sm.rtt());

    //invalid spin do nothing
    sm.incomming(3U);
    EXPECT_EQ(1U, sm.outgoing());
    EXPECT_EQ(std::chrono::milliseconds(42), sm.rtt());

    //invalid spin do nothing
    sm.incomming(0U);
    EXPECT_EQ(1U, sm.outgoing());
    EXPECT_EQ(std::chrono::milliseconds(42), sm.rtt());


}
