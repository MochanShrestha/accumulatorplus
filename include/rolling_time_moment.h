///////////////////////////////////////////////////////////////////////////////
// rolling_time_moment.h
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
            // rolling_moment_impl
            //    returns the moment of the sample in time of the rolling window
            template<typename N, typename Sample>
            struct rolling_time_moment_impl
                : accumulator_base
            {
                BOOST_MPL_ASSERT_RELATION(N::value, >, 0);

                //typedef typename numeric::functional::fdiv<Sample, std::size_t, void, void>::result_type result_type;
                typedef Sample result_type;

                template<typename Args>
                rolling_time_moment_impl(Args const &args)
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
                        this->sum_.first -= numeric::pow(buffer[i].first, N());
                    }

                    // Remove the old ones from the list
                    //buffer.remove_old_data(args[sample].second);

                    this->sum_.first += numeric::pow(args[sample].first, N());
                    this->size_ = buffer.size() - falling_off;
                }

                template<typename Args>
                result_type result(Args const & args) const
                {
                    auto r = sum_;
                    r.first /= size_;
                    return r;
                    //return numeric::fdiv(this->sum_, rolling_count(args));
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
            template <int N>
            struct rolling_time_moment
                : depends_on< rolling_time_window >
            {
                /// INTERNAL ONLY
                ///
                typedef accumulators::impl::rolling_time_moment_impl< mpl::int_<N>, mpl::_1 > impl;

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
            //extractor<tag::rolling_time_moment> const rolling_time_moment = {};
            BOOST_ACCUMULATORS_DEFINE_EXTRACTOR(tag, rolling_time_moment, (int))

            //BOOST_ACCUMULATORS_IGNORE_GLOBAL(rolling_time_moment)
        }

        using extract::rolling_time_moment;
    }
} // namespace boost::accumulators


