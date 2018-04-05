///////////////////////////////////////////////////////////////////////////////
// rolling_timestamp_min.h
//
// Copyright 2016 Mochan Shrestha. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <boost/mpl/placeholders.hpp>
#include <boost/accumulators/framework/accumulator_base.hpp>
#include <boost/accumulators/framework/extractor.hpp>
#include <boost/accumulators/numeric/functional.hpp>
#include <boost/accumulators/framework/parameters/sample.hpp>
#include <boost/accumulators/framework/depends_on.hpp>
#include <boost/accumulators/statistics_fwd.hpp>
#include <boost/accumulators/statistics/rolling_window.hpp>

#include "rolling_timestamp_window.h"
#include "rolling_timestamp_comp.h"

namespace boost
{
    namespace accumulators
    {
        namespace impl
        {
            ///////////////////////////////////////////////////////////////////////////////
            // rolling_sum_impl
            //    returns the sum of the samples in the rolling window
            template<typename Sample, typename TimeStamp>
            struct rolling_timestamp_min_impl
                : rolling_timestamp_comp_impl<Sample, TimeStamp, std::min>
            {
                template<typename Args>		
                rolling_timestamp_min_impl(Args const &args)
                    : rolling_timestamp_comp_impl<Sample, TimeStamp, std::min>(args)
                {
                    rolling_timestamp_comp_impl<Sample, TimeStamp, std::min>::val_ = numeric::as_max(args[sample | Sample()]);
                }
            };
        } // namespace impl

          ///////////////////////////////////////////////////////////////////////////////
          // tag::rolling_sum
          //
        namespace tag
        {
            struct rolling_timestamp_min
                : depends_on< rolling_timestamp_window >
            {
                /// INTERNAL ONLY
                ///
                typedef accumulators::impl::rolling_timestamp_min_impl < mpl::_1, mpl::_2 > impl;
            };
        } // namespace tag

          ///////////////////////////////////////////////////////////////////////////////
          // extract::rolling_sum
          //
        namespace extract
        {
            extractor<tag::rolling_timestamp_min> const rolling_timestamp_min = {};
            BOOST_ACCUMULATORS_IGNORE_GLOBAL(rolling_timestamp_min)
        }

        using extract::rolling_timestamp_min;
    }
} // namespace boost::accumulators


