///////////////////////////////////////////////////////////////////////////////
// zxings.h
//
//  Copyright 2016 Mochan Shrestha. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <limits>
#include <boost/mpl/placeholders.hpp>
#include <boost/accumulators/framework/accumulator_base.hpp>
#include <boost/accumulators/framework/extractor.hpp>
#include <boost/accumulators/framework/parameters/sample.hpp>
#include <boost/accumulators/numeric/functional.hpp>
#include <boost/accumulators/framework/depends_on.hpp>
#include <boost/accumulators/statistics_fwd.hpp>

namespace boost 
{
    namespace accumulators
    {
        namespace impl
        {
            ///////////////////////////////////////////////////////////////////////////////
            // zxings_impl
            template<typename Sample>
            struct zxings_impl
                : accumulator_base
            {
                // for boost::result_of
                typedef unsigned int result_type;

                template<typename Args>
                zxings_impl(Args const &args)
                    : prev_val_(args[sample | Sample()]), zxings_(0u)
                {
                }

                // Utility function to check if the value has been crossed or not
                static bool is_crossing(Sample v1, Sample v2, Sample x)
                {
                    // Make sure that v1 is greater or equal to v2
                    if (v1 < v2)
                        std::swap(v1, v2);

                    // Check if this is a crossing or not.
                    // A touching of the value would not be considering a crossing
                    if (v2 < x && x < v1)
                        return true;
                    else
                        return false;
                }

                template<typename Args>
                void operator ()(Args const &args)
                {
                    // Check if the new value was a crossing
                    auto new_value = args[sample];
                    if (is_crossing(new_value, prev_val_, 0))
                        zxings_++;

                    // Set the previous value as the current value
                    if (new_value != 0)
                        prev_val_ = new_value;
                }

                result_type result(dont_care) const
                {
                    return this->zxings_;
                }

            private:
                // The number of crossings
                unsigned int zxings_;

                // The previous value 
                Sample prev_val_;
            };

        } // namespace impl

          ///////////////////////////////////////////////////////////////////////////////
          // tag::zxings
          //
        namespace tag
        {
            struct zxings
                : depends_on<>
            {
                /// INTERNAL ONLY
                ///
                typedef accumulators::impl::zxings_impl<mpl::_1> impl;
            };
        }

        ///////////////////////////////////////////////////////////////////////////////
        // extract::zxings
        //
        namespace extract
        {
            extractor<tag::zxings> const zxings = {};

            BOOST_ACCUMULATORS_IGNORE_GLOBAL(zxings)
        }
        using extract::zxings;

    }
} // namespace boost::accumulators
