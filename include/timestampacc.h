#pragma once

///////////////////////////////////////////////////////////////////////////////
// timestamp.hpp
//
//  Copyright 2016 Mochan Shrestha. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/parameter/keyword.hpp>
#include <boost/accumulators/accumulators_fwd.hpp>

namespace boost 
{
    namespace accumulators
    {
        // The timestamp of a single sample
        BOOST_PARAMETER_KEYWORD(tag, timestamp)
        BOOST_ACCUMULATORS_IGNORE_GLOBAL(timestamp)

    }
} // namespace boost::accumulators

