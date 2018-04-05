///////////////////////////////////////////////////////////////////////////////
// rolling_time_min.h
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
            struct rolling_time_min_impl
                : accumulator_base
            {
                typedef Sample result_type;

                template<typename Args>
                rolling_time_min_impl(Args const &args)
                    : min_(args[sample | Sample()])
                {
                    min_.first = numeric::as_max(args[sample | Sample()].first);
                }

                template<typename Args>
                void operator ()(Args const &args)
                {
                    // Figure out the ones that are going to fall off
                    auto falling_off = get_rolling_time_window_falling_off(args);

                    // The circular buffer that we will have stored the data
                    auto&& buffer = rolling_time_window(args);
                    
                    // Flag to check if we need to recalculate the min or not
                    bool recalculate_min = false;
                    
                    // Go through and check if we need to recalculate the min or not
                    for (auto i = 0u; i < falling_off; i++)
                    {
                        // Check if the min is rolling off or not
                        if (this->min_.first >= buffer[i].first)
                        {
                            recalculate_min = true;
                            break;
                        }

                        //this->min_.first = std::min(min_.first, buffer[i].first);
                    }

                    // Remove the old ones from the list
                    //if (falling_off > 0)
                    //    buffer.remove_old_data(args[sample].second);

                    // Check if we need to recalculate the entire min or not
                    if (recalculate_min)
                    {
                        // Set the min as the first element
                        min_.first = buffer[falling_off].first;

                        // Go through the rest of the elements and see if it is the min
                        for (auto i = falling_off + 1; i < buffer.size(); i++)
                        {
                            min_.first = std::min(min_.first, buffer[i].first);
                        }
                    }
                    else
                    {
                        // Check if the new value is the min. We didn't recalculate the whole buffer
                        this->min_.first = std::min(min_.first, args[sample].first);
                    }
                }

                template<typename Args>
                result_type result(Args const & /*args*/) const
                {
                    return this->min_;
                }

            private:
                Sample min_;
            };
        } // namespace impl

          ///////////////////////////////////////////////////////////////////////////////
          // tag::rolling_sum
          //
        namespace tag
        {
            struct rolling_time_min
                : depends_on< rolling_time_window >
            {
                /// INTERNAL ONLY
                ///
                typedef accumulators::impl::rolling_time_min_impl< mpl::_1 > impl;

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
            extractor<tag::rolling_time_min> const rolling_time_min = {};

            BOOST_ACCUMULATORS_IGNORE_GLOBAL(rolling_time_min)
        }

        using extract::rolling_time_min;
    }
} // namespace boost::accumulators


