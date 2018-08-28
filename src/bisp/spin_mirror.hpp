// Copyright (c) Steinwurf ApS 2018.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cassert>
#include <cstdint>
#include <chrono>

#include <boost/optional.hpp>

#include "bit_spinner.hpp"

namespace bisp
{

/// Mirrors the incoming spin.
class spin_mirror : public bit_spinner
{
public:

    /// clock type
    using clock_type = std::chrono::high_resolution_clock;

public:

    spin_mirror(uint8_t spin_values = 4) :
        bit_spinner(),
        m_spin_values(spin_values)
    {
        assert(m_spin_values > 1 && "The number of spin values must be greater than 1");
    }

    /// Receive a spin
    /// @param spin the spin
    void incoming(uint8_t spin) override
    {
        assert(spin < m_spin_values && "Received invalid spin value");

        // skip update if this is the first received spin
        if (m_last_edge != boost::none)
        {
            if (spin != ((m_spin + 1) % m_spin_values))
                return;

            m_rtt = std::chrono::duration_cast<std::chrono::milliseconds>(
                clock_type::now() - m_last_edge.get());
        }

        m_spin = spin;
        m_last_edge = clock_type::now();
    }

    /// Get the outgoing spin
    /// @return the outgoing spin
    uint8_t outgoing() override
    {
        return m_spin;
    }

    /// Get the instantanios measured RTT
    /// @return the measured rtt between the last two received spins
    boost::optional<std::chrono::milliseconds> rtt() const override
    {
        return m_rtt;
    }

private:

    const uint8_t m_spin_values;

    uint8_t m_spin = 0;

    boost::optional<std::chrono::time_point<clock_type>>
    m_last_edge = boost::none;

    boost::optional<std::chrono::milliseconds> m_rtt = boost::none;

};

}
