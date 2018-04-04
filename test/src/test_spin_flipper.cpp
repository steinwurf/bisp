// Copyright (c) Steinwurf ApS 2018.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <bisp/spin_flipper.hpp>

#include <thread>
#include <chrono>

#include <gtest/gtest.h>


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
}

TEST(test_spin_sender, delay)
{
    bisp::spin_flipper<> sf;
    EXPECT_EQ(1U, sf.outgoing());

    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    sf.incomming(1U);
    EXPECT_LE(std::chrono::milliseconds(200), sf.rtt());
    EXPECT_GE(std::chrono::milliseconds(1000), sf.rtt());
    EXPECT_EQ(2U, sf.outgoing());

    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    sf.incomming(2U);
    EXPECT_LE(std::chrono::milliseconds(200), sf.rtt());
    EXPECT_GE(std::chrono::milliseconds(1000), sf.rtt());
    EXPECT_EQ(3U, sf.outgoing());

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    sf.incomming(3U);
    EXPECT_LE(std::chrono::milliseconds(100), sf.rtt());
    EXPECT_GE(std::chrono::milliseconds(1000), sf.rtt());
    EXPECT_EQ(0U, sf.outgoing());

    std::this_thread::sleep_for(std::chrono::milliseconds(110));
    sf.incomming(0U);
    EXPECT_LE(std::chrono::milliseconds(110), sf.rtt());
    EXPECT_GE(std::chrono::milliseconds(1000), sf.rtt());
    EXPECT_EQ(1U, sf.outgoing());

}
