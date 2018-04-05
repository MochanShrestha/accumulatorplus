///////////////////////////////////////////////////////////////////////////////
// rolling_timestamp_comp.h
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
            template<typename Sample, typename TimeStamp, const Sample& (*cmp)(const Sample&, const Sample&)>
            struct rolling_timestamp_comp_impl
                : accumulator_base
            {
                typedef Sample result_type;

                template<typename Args>
                rolling_timestamp_comp_impl(Args const &args)
                {
                }

                template<typename Args>
                void operator ()(Args const &args)
                {
                    // Figure out the ones that are going to fall off
                    auto falling_off = get_rolling_timestamp_window_falling_off(args);

                    // The circular buffer that we will have stored the data
                    auto&& buffer = rolling_timestamp_window(args);

                    // Flag to check if we need to recalculate the min or not
                    bool recalculate_ = false;

                    // Go through and check if we need to recalculate the min or not
                    for (auto i = 0u; i < falling_off; i++)
                    {
                        // Check if the min is rolling off or not
                        if (this->val_ >= buffer[i].first)
                        {
                            recalculate_ = true;
                            break;
                        }
                    }

                    // Check if we need to recalculate the entire min or not
                    if (recalculate_)
                    {
                        // Set the min as the first element
                        val_ = buffer[falling_off].first;

                        // Go through the rest of the elements and see if it is the min
                        for (auto i = falling_off + 1; i < buffer.size(); i++)
                        {
                            val_ = cmp(val_, buffer[i].first);
                        }
                    }
                    else
                    {
                        // Check if the new value is the min. We didn't recalculate the whole buffer
                        this->val_ = cmp(val_, args[sample]);
                    }
                }

                template<typename Args>
                result_type result(Args const & /*args*/) const
                {
                    return this->val_;
                }

            protected:
                Sample val_;
            };
        } // namespace impl
    }
} // namespace boost::accumulators
