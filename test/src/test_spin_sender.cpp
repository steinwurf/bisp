// Copyright (c) Steinwurf ApS 2018.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <bisp/spin_sender.hpp>

#include <thread>
#include <chrono>

#include <gtest/gtest.h>


TEST(test_spin_sender, basic)
{
    bisp::spin_sender<> ss;
    uint8_t spin_bits;

    EXPECT_EQ(boost::none, ss.rtt());

    spin_bits = ss.outgoing();
    ss.incomming(spin_bits);
    EXPECT_EQ(1U, spin_bits);
    EXPECT_EQ(std::chrono::milliseconds(0), ss.rtt());

    spin_bits = ss.outgoing();
    ss.incomming(spin_bits);
    EXPECT_EQ(2U, spin_bits);
    EXPECT_EQ(std::chrono::milliseconds(0), ss.rtt());
}

TEST(test_spin_sender, delay)
{
    bisp::spin_sender<> ss;
    uint8_t spin_bits;


    spin_bits = ss.outgoing();
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    ss.incomming(spin_bits);
    EXPECT_EQ(1U, spin_bits);
    EXPECT_EQ(std::chrono::milliseconds(20), ss.rtt());


    spin_bits = ss.outgoing();
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    ss.incomming(spin_bits);
    EXPECT_EQ(2U, spin_bits);
    EXPECT_EQ(std::chrono::milliseconds(20), ss.rtt());

    spin_bits = ss.outgoing();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    ss.incomming(spin_bits);
    EXPECT_EQ(3U, spin_bits);
    EXPECT_EQ(std::chrono::milliseconds(50), ss.rtt());

    spin_bits = ss.outgoing();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    ss.incomming(spin_bits);
    EXPECT_EQ(0U, spin_bits);
    EXPECT_EQ(std::chrono::milliseconds(10), ss.rtt());

}
