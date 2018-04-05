///////////////////////////////////////////////////////////////////////////////
// rolling_prev.hpp
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

namespace boost {
    namespace accumulators
    {
        namespace impl
        {
            ///////////////////////////////////////////////////////////////////////////////
            // rolling_prev_impl
            //    returns the sum of the samples in the rolling window
            template<typename Sample>
            struct rolling_prev_impl
                : accumulator_base
            {
                typedef Sample result_type;

                template<typename Args>
                rolling_prev_impl(Args const &args)
                    : prev_(args[sample | Sample()])
                {}

                template<typename Args>
                void operator ()(Args const &args)
                {
                    if (is_rolling_window_plus1_full(args))
                    {
                        // If the buffer is full, get the next one that isn't fallen off
                        this->prev_ = rolling_window_plus1(args)[1];
                    }
                    else
                    {
                        // The buffer is not full so we will just get the oldest
                        this->prev_ = rolling_window_plus1(args).front();
                    }
                }

                template<typename Args>
                result_type result(Args const & /*args*/) const
                {
                    return this->prev_;
                }

            private:
                Sample prev_;
            };
        } // namespace impl

          ///////////////////////////////////////////////////////////////////////////////
          // tag::rolling_prev
          //
        namespace tag
        {
            struct rolling_prev
                : depends_on< rolling_window_plus1 >
            {
                /// INTERNAL ONLY
                ///
                typedef accumulators::impl::rolling_prev_impl< mpl::_1 > impl;
            };
        } // namespace tag

          ///////////////////////////////////////////////////////////////////////////////
          // extract::rolling_prev
          //
        namespace extract
        {
            extractor<tag::rolling_prev> const rolling_prev = {};

            BOOST_ACCUMULATORS_IGNORE_GLOBAL(rolling_prev)
        }

        using extract::rolling_prev;
    }
} // namespace boost::accumulators


