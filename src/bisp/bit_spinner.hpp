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

class bit_spinner
{
public:
    /// Receive a spin
    /// @param spin the spin
    virtual void incoming(uint8_t spin) = 0;

    /// Get the outgoing spin
    /// @return the outgoing spin
    virtual uint8_t outgoing() = 0;

    /// Get the instantanios measured RTT
    /// @return the measured rtt between the last two received spins
    virtual boost::optional<std::chrono::milliseconds> rtt() const = 0;

    virtual ~bit_spinner() { }
};
}
