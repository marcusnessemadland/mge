/*
 * Copyright 2025 Marcus Nesse Madland. All rights reserved.
 * License: https://github.com/marcusnessemadland/vulkan-renderer/blob/main/LICENSE
 */

#pragma once

#include <algorithm>

class SampleData
{
public:
	SampleData()
	{
		reset();
	}

	void reset()
	{
		m_offset = 0;
		memset(m_values, 0, sizeof(m_values));

		m_min = 0.0f;
		m_max = 0.0f;
		m_avg = 0.0f;
	}

	void pushSample(float value)
	{
		m_values[m_offset] = value;
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

	float getMin() const
	{
		return m_min;
	}

	float getMax() const
	{
		return m_max;
	}

	float getAverage() const
	{
		return m_avg;
	}

private:
	int32_t m_offset;
	float m_values[100];

	float m_min;
	float m_max;
	float m_avg;
};