///////////////////////////////////////////////////////////////////////////////
// rolling_time_mean.h
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

#include "rolling_time_window.h"

namespace boost
{
    namespace accumulators
    {
        namespace impl
        {
            ///////////////////////////////////////////////////////////////////////////////
            // rolling_sum_impl
            //    returns the sum of the samples in the rolling window
            template<typename Sample>
            struct rolling_time_mean_impl
                : accumulator_base
            {
                typedef Sample result_type;

                template<typename Args>
                rolling_time_mean_impl(Args const &args)
                    : sum_(args[sample | Sample()])
                {}

                template<typename Args>
                void operator ()(Args const &args)
                {
                    // Figure out the ones that are going to fall off
                    auto falling_off = get_rolling_time_window_falling_off(args);
                    auto&& buffer = rolling_time_window(args);
                    for (auto i = 0u; i < falling_off; i++)
                    {
                        this->sum_.first -= buffer[i].first;
                    }

                    // Remove the old ones from the list
                    //buffer.remove_old_data(args[sample].second);

                    this->sum_.first += args[sample].first;
                    this->size_ = buffer.size() - falling_off;
                }

                template<typename Args>
                result_type result(Args const & args) const
                {
                    auto r = sum_;

                    r.first /= size_;
                    
                    return r;
                }

            private:
                Sample sum_;

                unsigned int size_;
            };
        } // namespace impl

          ///////////////////////////////////////////////////////////////////////////////
          // tag::rolling_sum
          //
        namespace tag
        {
            struct rolling_time_mean
                : depends_on< rolling_time_window >
            {
                /// INTERNAL ONLY
                ///
                typedef accumulators::impl::rolling_time_mean_impl< mpl::_1 > impl;

#ifdef BOOST_ACCUMULATORS_DOXYGEN_INVOKED
                /// tag::rolling_time_window::window_time named parameter
                static boost::parameter::keyword<tag::rolling_time_window_size> const window_time;
#endif
            };
        } // namespace tag

          ///////////////////////////////////////////////////////////////////////////////
          // extract::rolling_sum
          //
        namespace extract
        {
            extractor<tag::rolling_time_mean> const rolling_time_mean = {};

            BOOST_ACCUMULATORS_IGNORE_GLOBAL(rolling_time_mean)
        }

        using extract::rolling_time_mean;
    }
} // namespace boost::accumulators


