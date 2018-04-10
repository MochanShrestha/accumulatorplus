///////////////////////////////////////////////////////////////////////////////
// rolling_timestamp_std.h
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
#include "rolling_timestamp_mean.h"
#include "rolling_timestamp_moment.h"

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
            struct rolling_timestamp_std_impl
                : accumulator_base
            {
                typedef Sample result_type;

                template<typename Args>
                rolling_timestamp_std_impl(Args const &/*args*/)
                {}

                template<typename Args>
                result_type result(Args const & args) const
                {
                    auto n = rolling_timestamp_count(args[accumulator]);
                    double factor = n > 1 ? (double)n / (double)(n - 1) : 1.0;
                    return std::sqrt((rolling_timestamp_moment<2>(args[accumulator]) - std::pow(rolling_timestamp_mean(args[accumulator]), 2))*factor);
                }
            };
        } // namespace impl

          ///////////////////////////////////////////////////////////////////////////////
          // tag::rolling_sum
          //
        namespace tag
        {
            struct rolling_timestamp_std
                : depends_on< rolling_timestamp_mean, rolling_timestamp_moment<2>, rolling_timestamp_count, rolling_timestamp_window >
            {
                /// INTERNAL ONLY
                ///
                typedef accumulators::impl::rolling_timestamp_std_impl < mpl::_1, mpl::_2 > impl;
            };
        } // namespace tag

          ///////////////////////////////////////////////////////////////////////////////
          // extract::rolling_sum
          //
        namespace extract
        {
            extractor<tag::rolling_timestamp_std> const rolling_timestamp_std = {};

            BOOST_ACCUMULATORS_IGNORE_GLOBAL(rolling_timestamp_std)
        }

        using extract::rolling_timestamp_std;
    }
} // namespace boost::accumulators


