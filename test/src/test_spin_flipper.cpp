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
    uint8_t spin_bits;

    EXPECT_EQ(boost::none, sf.rtt());

    spin_bits = sf.outgoing();
    sf.incomming(spin_bits);
    EXPECT_EQ(1U, spin_bits);
    EXPECT_EQ(std::chrono::milliseconds(0), sf.rtt());

    spin_bits = sf.outgoing();
    sf.incomming(spin_bits);
    EXPECT_EQ(2U, spin_bits);
    EXPECT_EQ(std::chrono::milliseconds(0), sf.rtt());
}

TEST(test_spin_sender, delay)
{
    bisp::spin_flipper<> sf;
    uint8_t spin_bits;


    spin_bits = sf.outgoing();
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    sf.incomming(spin_bits);
    EXPECT_EQ(1U, spin_bits);
    EXPECT_EQ(std::chrono::milliseconds(20), sf.rtt());


    spin_bits = sf.outgoing();
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    sf.incomming(spin_bits);
    EXPECT_EQ(2U, spin_bits);
    EXPECT_EQ(std::chrono::milliseconds(20), sf.rtt());

    spin_bits = sf.outgoing();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    sf.incomming(spin_bits);
    EXPECT_EQ(3U, spin_bits);
    EXPECT_EQ(std::chrono::milliseconds(50), sf.rtt());

    spin_bits = sf.outgoing();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    sf.incomming(spin_bits);
    EXPECT_EQ(0U, spin_bits);
    EXPECT_EQ(std::chrono::milliseconds(10), sf.rtt());

}
