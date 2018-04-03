// Copyright (c) Steinwurf ApS 2018.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <bisp/spin_receiver.hpp>

#include <thread>
#include <chrono>

#include <gtest/gtest.h>


TEST(test_spin_receiver, basic)
{
    bisp::spin_receiver<> sr;

    EXPECT_EQ(boost::none, sr.rtt());

    sr.incomming(1U);
    EXPECT_EQ(1U, sr.outgoing());

    sr.incomming(2U);
    EXPECT_EQ(2U, sr.outgoing());
    EXPECT_EQ(std::chrono::milliseconds(0), sr.rtt());
}

TEST(test_spin_receiver, delay)
{
    bisp::spin_receiver<> sr;
    sr.incomming(0U);
    EXPECT_EQ(boost::none, sr.rtt());

    sr.incomming(1U);
    EXPECT_EQ(1U, sr.outgoing());
    EXPECT_EQ(boost::none, sr.rtt());

    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    sr.incomming(2U);
    EXPECT_EQ(2U, sr.outgoing());
    EXPECT_EQ(std::chrono::milliseconds(20), sr.rtt());

    std::this_thread::sleep_for(std::chrono::milliseconds(17));
    sr.incomming(3U);
    EXPECT_EQ(3U, sr.outgoing());
    EXPECT_EQ(std::chrono::milliseconds(17), sr.rtt());

    std::this_thread::sleep_for(std::chrono::milliseconds(42));
    sr.incomming(0U);
    EXPECT_EQ(0U, sr.outgoing());
    EXPECT_EQ(std::chrono::milliseconds(42), sr.rtt());
}
