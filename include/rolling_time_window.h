///////////////////////////////////////////////////////////////////////////////
// rolling_time_window.hpp
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

#include "circular_time_buffer.h"

namespace boost 
{
    namespace accumulators
    {
        ///////////////////////////////////////////////////////////////////////////////
        // tag::rolling_window::size named parameter
        BOOST_PARAMETER_NESTED_KEYWORD(tag, rolling_time_window_size, window_time)

        BOOST_ACCUMULATORS_IGNORE_GLOBAL(rolling_time_window_size)

        // clang requires forward declaration
        namespace tag {
            struct rolling_time_window;
        }

        namespace impl
        {
            ///////////////////////////////////////////////////////////////////////////////
            // rolling_time_window_impl
            //    stores the latest samples in time t, where t is specified at construction time
            //    with the rolling_time_window_size named parameter
            template<typename Sample>
            struct rolling_time_window_impl 
                : accumulator_base
            {
                typedef typename circular_time_buffer<Sample, double>::const_iterator const_iterator;
                //typedef iterator_range<const_iterator> result_type;
                typedef circular_time_buffer<double, double>& result_type;

                template<typename Args>
                rolling_time_window_impl(Args const & args)
                    : buffer_(args[rolling_time_window_size], 0.0), n_older(0u)
                {}

                template<typename Args>
                void operator ()(Args const &args)
                {
                    // Separate out the data that we are getting
                    double data = args[sample].first;
                    double timestamp = args[sample].second;

                    // Check if we have to remove older values
                    buffer_.remove_old_data_by_count(n_older);

                    // Figure out how many are older
                    n_older = this->buffer_.get_older(timestamp);

                    // Add the new value to the buffer
                    this->buffer_.insert_only(data, timestamp);
                }

                unsigned int get_older(double timestamp) const
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
                circular_time_buffer<double, double> buffer_;

                // These are the number of items that are older than the current value
                unsigned int n_older;
            };

            template<typename Args>
            unsigned int get_rolling_time_window_falling_off(Args const &args)
            {
                return find_accumulator<tag::rolling_time_window>(args[accumulator]).get_older(args[sample].second);
            }
            
            /////////////////////////////////////////////////////////////////////////////////
            //// rolling_time_window_impl
            ////    stores the latest N samples, where N is specified at construction type
            ////    with the rolling_window_size named parameter
            //template<typename Sample>
            //struct rolling_time_window_impl
            //    : accumulator_base
            //{
            //    typedef typename circular_buffer<Sample>::const_iterator const_iterator;
            //    typedef iterator_range<const_iterator> result_type;

            //    rolling_time_window_impl(dont_care)
            //    {}

            //    template<typename Args>
            //    result_type result(Args const &args) const
            //    {
            //        return rolling_time_window(args).advance_begin(is_rolling_time_window_full(args));
            //    }
            //};

        } // namespace impl

          ///////////////////////////////////////////////////////////////////////////////
          // tag::rolling_time_window
          //
        namespace tag
        {
            struct rolling_time_window
                : depends_on<>
                , tag::rolling_time_window_size
            {
                /// INTERNAL ONLY
                ///
                typedef accumulators::impl::rolling_time_window_impl< mpl::_1 > impl;

#ifdef BOOST_ACCUMULATORS_DOXYGEN_INVOKED
                /// tag::rolling_window::size named parameter
                static boost::parameter::keyword<tag::rolling_time_window_size> const window_time;
#endif
            };
        } // namespace tag

          ///////////////////////////////////////////////////////////////////////////////
          // extract::rolling_window
          //
        namespace extract
        {
            extractor<tag::rolling_time_window> const rolling_time_window = {};

            BOOST_ACCUMULATORS_IGNORE_GLOBAL(rolling_time_window)
        }
        using extract::rolling_time_window;

    }
} // namespace boost::accumulators



