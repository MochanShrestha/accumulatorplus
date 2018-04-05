///////////////////////////////////////////////////////////////////////////////
// rolling_timestamp_sum.h
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
            struct rolling_timestamp_sum_impl
                : accumulator_base
            {
                typedef Sample result_type;

                template<typename Args>
                rolling_timestamp_sum_impl(Args const &args)
                    : sum_(args[sample | Sample()])
                {}

                template<typename Args>
                void operator ()(Args const &args)
                {
                    // Figure out the ones that are going to fall off
                    auto falling_off = get_rolling_timestamp_window_falling_off(args);
                    auto&& buffer = rolling_timestamp_window(args);
                    for (auto i = 0u; i < falling_off; i++)
                    {
                        this->sum_ -= buffer[i].first;
                    }

                    // Remove the old ones from the list
                    //buffer.remove_old_data(args[sample].second);

                    this->sum_ += args[sample];
                }

                template<typename Args>
                result_type result(Args const & /*args*/) const
                {
                    return this->sum_;
                }

            private:
                Sample sum_;
            };
        } // namespace impl

          ///////////////////////////////////////////////////////////////////////////////
          // tag::rolling_sum
          //
        namespace tag
        {
            struct rolling_timestamp_sum
                : depends_on< rolling_timestamp_window >
            {
                /// INTERNAL ONLY
                ///
                typedef accumulators::impl::rolling_timestamp_sum_impl< mpl::_1, mpl::_2 > impl;
            };
        } // namespace tag

          ///////////////////////////////////////////////////////////////////////////////
          // extract::rolling_sum
          //
        namespace extract
        {
            extractor<tag::rolling_timestamp_sum> const rolling_timestamp_sum = {};

            BOOST_ACCUMULATORS_IGNORE_GLOBAL(rolling_timestamp_sum)
        }

        using extract::rolling_timestamp_sum;
    }
} // namespace boost::accumulators


