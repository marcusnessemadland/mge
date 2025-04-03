/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/mge/blob/main/LICENSE
 */

#pragma once

#include <algorithm>
#include <chrono>

namespace mge
{
	/// Sample data to keep track of statistics.
	/// 
    class SampleData
    {
    public:
        SampleData();

        /// Begins a timer.
        ///
        void begin();

        /// Ends a timer.
        ///
        /// @remark This does not push value.
        /// 
        /// @returns How long it took from begin to end in milliseconds.
        /// 
        float end();

        /// Resets the data to default.
        ///
        void reset();

        /// Push a sample value.
        ///
        /// @param[in] _value The value to store.
        /// 
        void pushSample(float _value);

        /// Get the minimum sample value in array.
        ///
        /// @returns Minimum value.
        /// 
        float getMin() const;

        /// Get the maximum sample value in array.
        ///
        /// @returns Maximum value.
        /// 
        float getMax() const;

        /// Get the average sample value in the array.
        ///
        /// @returns Average value.
        /// 
        float getAverage() const;

    private:
        std::chrono::high_resolution_clock::time_point timer;
        int32_t m_offset;
        float m_values[100];
        float m_min;
        float m_max;
        float m_avg;
    };

} // namespace mge