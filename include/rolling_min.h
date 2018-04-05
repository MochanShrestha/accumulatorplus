
///////////////////////////////////////////////////////////////////////////////
// rolling_min.h
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
            // impl:rolling_min_impl
            //    uses the rolling window to keep the min of the last N samples
            template <typename Sample>
            struct rolling_min_impl : rolling_comp_impl<Sample, std::min>
            {
                template<typename Args>
                rolling_min_impl(Args const& args) : rolling_comp_impl<Sample, std::min>(args)
                {
                    rolling_comp_impl<Sample, std::min>::val_ = numeric::as_max(args[sample | Sample()]);
                }
            };
        }

        namespace tag
        {
            ///////////////////////////////////////////////////////////////////////////////
            // tag::rolling_min
            //
            struct rolling_min : depends_on<rolling_window_plus1>
            {
                typedef accumulators::impl::rolling_min_impl<mpl::_1> impl;
            };
        }

        namespace extract
        {
            ///////////////////////////////////////////////////////////////////////////////
            // extract::rolling_min
            // 
            extractor<tag::rolling_min> const rolling_min = {};
            BOOST_ACCUMULATORS_IGNORE_GLOBAL(rolling_min)
        }
        using extract::rolling_min;
    }
}