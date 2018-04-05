
///////////////////////////////////////////////////////////////////////////////
// rolling_max.h
//
// Copyright 2015 Mochan Shrestha. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/moment.hpp>
#include <boost/accumulators/statistics/rolling_sum.hpp>
#include <boost/accumulators/statistics/rolling_mean.hpp>
#include <boost/accumulators/statistics/rolling_moment.hpp>
#include <boost/accumulators/statistics/min.hpp>

#include "rolling_comp.h"

namespace boost
{
    namespace accumulators
    {
        namespace impl
        {
            ///////////////////////////////////////////////////////////////////////////////
            // impl:rolling_max_impl
            //    uses the rolling window to keep the min of the last N samples
            template <typename Sample>
            struct rolling_max_impl : rolling_comp_impl<Sample, std::max>
            {
                template<typename Args>
                rolling_max_impl(Args const& args) : rolling_comp_impl<Sample, std::max>(args)
                {
                    rolling_comp_impl<Sample, std::max>::val_ = numeric::as_min(args[sample | Sample()]);
                }
            };
        }

        namespace tag
        {
            ///////////////////////////////////////////////////////////////////////////////
            // tag::rolling_max
            //
            struct rolling_max : depends_on<rolling_window_plus1>
            {
                typedef accumulators::impl::rolling_max_impl<mpl::_1> impl;
            };
        }

        namespace extract
        {
            ///////////////////////////////////////////////////////////////////////////////
            // extract::rolling_max
            // 
            extractor<tag::rolling_max> const rolling_max = {};
            BOOST_ACCUMULATORS_IGNORE_GLOBAL(rolling_max)
        }
        using extract::rolling_max;
    }
}
