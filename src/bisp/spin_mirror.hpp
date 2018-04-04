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

/// Mirrors the incomming spin.
template<uint8_t SpinValues = 4>
class spin_mirror
{

    static_assert(SpinValues > 1,
                  "The number of spin values must be greater than 1");

public:

    /// clock type
    using clock_type = std::chrono::high_resolution_clock;

public:

    /// Receive a spin
    /// @param spin the spin
    void incomming(uint8_t spin)
    {
        assert(spin < SpinValues && "Received invalid spin value");

        // skip update if this is the first received spin
        if (m_last_edge != boost::none)
        {
            if (spin != ((m_spin + 1) % SpinValues))
                return;

            m_rtt = std::chrono::duration_cast<std::chrono::milliseconds>(
                clock_type::now() - m_last_edge.get());
        }

        m_spin = spin;
        m_last_edge = clock_type::now();
    }

    /// Get the outgoing spin
    /// @return the outgoing spin
    uint8_t outgoing() const
    {
        return m_spin;
    }

    /// Get the instantanios measured RTT
    /// @return the measured rtt between the last two received spins
    boost::optional<std::chrono::milliseconds> rtt() const
    {
        return m_rtt;
    }

private:

    uint8_t m_spin = 0;

    boost::optional<std::chrono::time_point<clock_type>>
        m_last_edge = boost::none;

    boost::optional<std::chrono::milliseconds> m_rtt = boost::none;

};

}
