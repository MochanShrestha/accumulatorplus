///////////////////////////////////////////////////////////////////////////////
// rolling_timestamp_window.hpp
//
// Copyright 2016 Mochan Shrestha. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <cstddef>
#include <boost/version.hpp>
#include <boost/assert.hpp>
#include <boost/circular_buffer.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/accumulators/accumulators_fwd.hpp>
#include <boost/accumulators/framework/extractor.hpp>
#include <boost/accumulators/framework/depends_on.hpp>
#include <boost/accumulators/framework/accumulator_base.hpp>
#include <boost/accumulators/framework/parameters/sample.hpp>
#include <boost/accumulators/framework/parameters/accumulator.hpp>
#include <boost/accumulators/numeric/functional.hpp>
#include <boost/accumulators/statistics_fwd.hpp>

#include "timestampacc.h"
#include "circular_time_buffer.h"

namespace boost
{
    namespace accumulators
    {
        ///////////////////////////////////////////////////////////////////////////////
        // tag::rolling_window::size named parameter
        BOOST_PARAMETER_NESTED_KEYWORD(tag, rolling_timestamp_window_size, window_time)
        BOOST_ACCUMULATORS_IGNORE_GLOBAL(rolling_timestamp_window_size)

        // clang requires forward declaration
        namespace tag {
            struct rolling_timestamp_window;
        }

        namespace impl
        {
            ///////////////////////////////////////////////////////////////////////////////
            // rolling_timestamp_window_impl
            //    stores the latest samples in time t, where t is specified at construction time
            //    with the rolling_timestamp_window_size named parameter
            template<typename Sample, typename TimeStamp>
            struct rolling_timestamp_window_impl
                : accumulator_base
            {
                typedef typename circular_time_buffer<Sample, double>::const_iterator const_iterator;
                //typedef iterator_range<const_iterator> result_type;
                typedef circular_time_buffer<double, double>& result_type;

                template<typename Args>
                rolling_timestamp_window_impl(Args const & args)
                    : buffer_(args[rolling_timestamp_window_size], 0.0), n_older(0u)
                {}

                template<typename Args>
                void operator ()(Args const &args)
                {
                    // Separate out the data that we are getting
                    Sample data = args[sample];
                    TimeStamp ts = args[timestamp];

                    // Check if we have to remove older values
                    buffer_.remove_old_data_by_count(n_older);

                    // Figure out how many are older
                    n_older = this->buffer_.get_older(ts);

                    // Add the new value to the buffer
                    this->buffer_.insert_only(data, ts);
                }

                unsigned int get_older(double ts) const
                {
                    //return this->buffer_.get_older(timestamp);
                    return n_older;
                }

                // The result of a shifted rolling window is the range including
                // everything except the most recently added element.
                //template<typename Args>
                //result_type result(Args const &args) const
                result_type result(dont_care)
                {
                    //return result_type(this->buffer_.begin(), this->buffer_.end());
                    return buffer_;
                }

            private:
                // the circular time buffer
                circular_time_buffer<Sample, TimeStamp> buffer_;

                // These are the number of items that are older than the current value
                unsigned int n_older;
            };

            template<typename Args>
            unsigned int get_rolling_timestamp_window_falling_off(Args const &args)
            {
                return find_accumulator<tag::rolling_timestamp_window>(args[accumulator]).get_older(args[timestamp]);
            }
        } // namespace impl

          ///////////////////////////////////////////////////////////////////////////////
          // tag::rolling_timestamp_window
          //
        namespace tag
        {
            struct rolling_timestamp_window
                : depends_on<>
                , tag::rolling_timestamp_window_size
            {
                /// INTERNAL ONLY
                ///
                typedef accumulators::impl::rolling_timestamp_window_impl< mpl::_1, mpl::_2 > impl;
            };
        } // namespace tag

          ///////////////////////////////////////////////////////////////////////////////
          // extract::rolling_window
          //
        namespace extract
        {
            extractor<tag::rolling_timestamp_window> const rolling_timestamp_window = {};

            BOOST_ACCUMULATORS_IGNORE_GLOBAL(rolling_timestamp_window)
        }
        using extract::rolling_timestamp_window;

    }
} // namespace boost::accumulators



