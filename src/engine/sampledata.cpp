/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/mge/blob/main/LICENSE
 */

#include "engine/sampledata.h"

#include <limits>

namespace mge
{
    SampleData::SampleData()
    {
        reset();
    }

    void SampleData::begin()
    {
        timer = std::chrono::high_resolution_clock::now();
    }

    float SampleData::end()
    {
        auto endTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float, std::milli> duration = endTime - timer;
        return duration.count(); 
    }

    void SampleData::reset()
    {
        m_offset = 0;
        std::memset(m_values, 0, sizeof(m_values));

        m_min = 0.0f;
        m_max = 0.0f;
        m_avg = 0.0f;
    }

    void SampleData::pushSample(float _value)
    {
        m_values[m_offset] = _value;
        m_offset = (m_offset + 1) % 100;

        float min = std::numeric_limits<float>::max();
        float max = std::numeric_limits<float>::min();
        float avg = 0.0f;

        for (uint32_t ii = 0; ii < 100; ++ii)
        {
            const float val = m_values[ii];
            min = std::min(min, val);
            max = std::max(max, val);
            avg += val;
        }

        m_min = min;
        m_max = max;
        m_avg = avg / 100;
    }

    float SampleData::getMin() const
    {
        return m_min;
    }

    float SampleData::getMax() const
    {
        return m_max;
    }

    float SampleData::getAverage() const
    {
        return m_avg;
    }

} // namespace mge