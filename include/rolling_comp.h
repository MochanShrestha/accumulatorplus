#pragma once

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/moment.hpp>
#include <boost/accumulators/statistics/rolling_sum.hpp>
#include <boost/accumulators/statistics/rolling_mean.hpp>
#include <boost/accumulators/statistics/rolling_moment.hpp>
#include <boost/accumulators/statistics/min.hpp>

namespace boost
{
    namespace accumulators
    {
        namespace impl
        {
            ///////////////////////////////////////////////////////////////////////////////
            // impl:rolling_comp_impl
            //    uses the rolling window to keep track. We use a cmp as a comparator function (min, max)
            template <typename Sample, const Sample& (*cmp)(const Sample&, const Sample&)>
            struct rolling_comp_impl : accumulator_base
            {
                typedef Sample result_type;

                template<typename Args>
                rolling_comp_impl(Args const&)
                {
                }

                template <typename Args>
                void operator() (Args const& args)
                {
                    // Compare the min with the new value we just received
                    this->val_ = cmp(args[sample], this->val_);

                    // Check if some values are rolling off
                    if (is_rolling_window_plus1_full(args))
                    {
                        // Check if the value rolling off is the min
                        if (this->val_ >= rolling_window_plus1(args).front())
                        {
                            // The value rolling off is the min. We have to recalculate the entire min
                            // from the contents of the circular buffer

                            // Get the circular buffer and advance past the value that will be rolling off
                            auto circ_buffer = rolling_window_plus1(args);
                            circ_buffer.advance_begin(1);

                            // Set the new min to the first element that we will be looking at in the circular buffer
                            this->val_ = circ_buffer.front();
                            circ_buffer.advance_begin(1);

                            // Iterate over the circular buffer to find the value
                            for (auto i : circ_buffer)
                            {
                                this->val_ = cmp(i, this->val_);
                            }
                        }
                    }

                }

                result_type result(dont_care) const
                {
                    return this->val_;
                }

            protected:
                Sample val_;
            };
        }
    }
}
