// Copyright (c) Steinwurf ApS 2018.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cassert>
#include <cstdint>
#include <chrono>

#include <boost/optional.hpp>

namespace bisp
{

template<uint8_t SpinValues = 4>
class spin_sender
{

    static_assert(SpinValues > 1,
                  "The number of spin values must be greater than 1");

    static_assert(SpinValues < 128,
                  "The number of spin values must be below 128");

public:

    /// clock type
    using clock_type = std::chrono::high_resolution_clock;

public:

    void incomming(uint8_t spin)
    {
        assert(spin < SpinValues && "Received invalid spin value");

        if (spin != ((m_spin + 1) % SpinValues))
            return;

        if (m_last_edge != boost::none)
        {
            m_rtt = std::chrono::duration_cast<std::chrono::milliseconds>(
                clock_type::now() - m_last_edge.get());
        }

        m_spin = spin;
        m_last_edge = clock_type::now();
    }

    uint8_t outgoing()
    {
        if (m_last_edge == boost::none)
            m_last_edge = clock_type::now();

        return (m_spin + 1) % SpinValues;
    }

    boost::optional<std::chrono::milliseconds> rtt() const
    {
        return m_rtt;
    }

private:

    uint8_t m_spin = 0;

    boost::optional<std::chrono::time_point<clock_type>>
        m_last_edge =boost::none;

    boost::optional<std::chrono::milliseconds> m_rtt = boost::none;

};

}
