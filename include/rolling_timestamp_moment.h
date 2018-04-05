///////////////////////////////////////////////////////////////////////////////
// rolling_timestamp_moment.h
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

// Calculates the moment. Note not the central moment that matlab moment calculates.

namespace boost
{
    namespace accumulators
    {
        namespace impl
        {
            ///////////////////////////////////////////////////////////////////////////////
            // rolling_moment_impl
            //    returns the moment of the sample in time of the rolling window
            template<typename N, typename Sample, typename TimeStamp>
            struct rolling_timestamp_moment_impl
                : accumulator_base
            {
                BOOST_MPL_ASSERT_RELATION(N::value, >, 0);

                //typedef typename numeric::functional::fdiv<Sample, std::size_t, void, void>::result_type result_type;
                typedef Sample result_type;

                template<typename Args>
                rolling_timestamp_moment_impl(Args const &args)
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
                        this->sum_ -= numeric::pow(buffer[i].first, N());
                    }

                    this->sum_ += numeric::pow(args[sample], N());
                }

                template<typename Args>
                result_type result(Args const & args) const
                {
                    return sum_/ rolling_timestamp_count(args[accumulator]);
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
            template <int N>
            struct rolling_timestamp_moment
                : depends_on< rolling_timestamp_count, rolling_timestamp_window >
            {
                /// INTERNAL ONLY
                ///
                typedef accumulators::impl::rolling_timestamp_moment_impl< mpl::int_<N>, mpl::_1, mpl::_2 > impl;
            };
        } // namespace tag

          ///////////////////////////////////////////////////////////////////////////////
          // extract::rolling_sum
          //
        namespace extract
        {
            //extractor<tag::rolling_timestamp_moment> const rolling_timestamp_moment = {};
            BOOST_ACCUMULATORS_DEFINE_EXTRACTOR(tag, rolling_timestamp_moment, (int))
            //BOOST_ACCUMULATORS_IGNORE_GLOBAL(rolling_timestamp_moment)
        }

        using extract::rolling_timestamp_moment;
    }
} // namespace boost::accumulators


