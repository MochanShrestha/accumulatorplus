
#pragma once

#ifndef _CIRCULAR_TIME_BUFFER_H_
#define _CIRCULAR_TIME_BUFFER_H_

#include <deque>

template <class data, class timestamp>
class circular_time_buffer : public std::deque<std::pair<data, timestamp>>
{
protected:
    // Denote the duration that the buffer is in the 
    timestamp buff_time;

    // The difference at the boundary to 
    timestamp delta;

public:
    // Takes the argument of the duration of data that we will store
    circular_time_buffer(timestamp buffer_duration, timestamp delta)
    {
        this->buff_time = buffer_duration;
        this->delta = delta;
    }

    // Get how many data items are older than the current timestamp
    unsigned int get_older(timestamp time_point) const
    {
        unsigned int older = 0;
        for (auto i : *this)
        {
            if (time_point - i.second >= buff_time + delta)
                older++;
            //else
            //    break;
        }
        return older;
    }

    // Remove the old data
    void remove_old_data(timestamp t)
    {
        // Get how many are older
        remove_old_data_by_count(get_older(t));
    }

    // Remove the old data by the number to remove
    void remove_old_data_by_count(unsigned int n_older)
    {
        // Remove the old data
        for (auto i = 0u; i < n_older; i++)
            this->pop_front();
    }

    // Insert the data. All the data are older than the current data minus the duration will be removed
    void insert(data d, timestamp t)
    {
        // remove the old data
        remove_old_data(t);

        // Insert the new data
        std::deque<std::pair<data, timestamp>>::push_back(std::pair<data, timestamp>(d, t));
    }

    // Insert the data only. Do not remove old data. Note that this will result in a non-conformal data where
    // elements that are longer than the duration will be present in the queue
    void insert_only(data d, timestamp t)
    {
        std::deque<std::pair<data, timestamp>>::push_back(std::pair<data, timestamp>(d, t));
    }

    // Return the current value of the length of the buffer
    timestamp getBufferTime() { return buff_time; }
};

#endif