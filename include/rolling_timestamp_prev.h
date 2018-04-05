///////////////////////////////////////////////////////////////////////////////
// rolling_timestamp_prev.h
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
            struct rolling_timestamp_prev_impl
                : accumulator_base
            {
                typedef Sample result_type;

                template<typename Args>
                rolling_timestamp_prev_impl(Args const &args)
                    : prev_(args[sample | Sample()]), first_assign(true)
                {}

                template<typename Args>
                void operator ()(Args const &args)
                {
                    // Figure out the ones that are going to fall off
                    auto falling_off = get_rolling_timestamp_window_falling_off(args);
                    auto&& buffer = rolling_timestamp_window(args);

                    // Check if the element right on the edge has the exact timestamp
                    if (buffer[falling_off].second == args[timestamp] - buffer.getBufferTime())
                    {
                        this->prev_ = buffer[falling_off].first;
                    }
                    else
                    {
                        // If elements are falling off, then the prev value has changed
                        if (falling_off > 0)
                        {
                            this->prev_ = buffer[falling_off - 1].first;
                        }
                        else
                        {
                            // If no elements have fallen off, then the prev value has stayed the same
                            // unless it was never assigned to anything.
                            if (first_assign)
                            {
                                // This is the first assignment to _prev.
                                // If there is no data older than time window size, we will use the first value
                                this->prev_ = buffer[0].first;
                                first_assign = false;
                            }
                        }
                    }
                }

                template<typename Args>
                result_type result(Args const & /*args*/) const
                {
                    return this->prev_;
                }

            private:
                Sample prev_;

                bool first_assign;
            };
        } // namespace impl

          ///////////////////////////////////////////////////////////////////////////////
          // tag::rolling_sum
          //
        namespace tag
        {
            struct rolling_timestamp_prev
                : depends_on< rolling_timestamp_window >
            {
                /// INTERNAL ONLY
                ///
                typedef accumulators::impl::rolling_timestamp_prev_impl< mpl::_1, mpl::_2 > impl;
            };
        } // namespace tag

          ///////////////////////////////////////////////////////////////////////////////
          // extract::rolling_sum
          //
        namespace extract
        {
            extractor<tag::rolling_timestamp_prev> const rolling_timestamp_prev = {};

            BOOST_ACCUMULATORS_IGNORE_GLOBAL(rolling_timestamp_prev)
        }

        using extract::rolling_timestamp_prev;
    }
} // namespace boost::accumulators


