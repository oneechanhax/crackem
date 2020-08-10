
/*
 * Nekohack: Break things in cool ways!
 * Copyright (C) 2018 Rebekah Rowe
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <cassert>
#include <algorithm>
#include <vector>
#include <string>

#include "signature.hpp"

namespace crackem::sig {

// Original function by learn_more -- https://github.com/learn-more/findpattern-bench
static bool InRange(uint8_t x, uint8_t a, uint8_t b) { return x >= a && x <= b; }
static uint8_t GetBits(uint8_t x) { return InRange((x & (~0x20)), 'A', 'F') ? ((x & (~0x20)) - 'A' + 0xA): (InRange(x, '0', '9') ? x - '0': 0); }
static uint8_t GetByte(const char* x) { return GetBits(x[0]) << 4 | GetBits(x[1]); }

uintptr_t FindPattern(uintptr_t start_address, uintptr_t end_address, const char* target_pattern) {
	const char* pattern = target_pattern;

	uintptr_t first_match = 0;

	for (uintptr_t position = start_address; position < end_address; position++) {
		if (!*pattern)
			return first_match;

		const uint8_t pattern_current = *reinterpret_cast<const uint8_t*>(pattern);
		const uint8_t memory_current = *reinterpret_cast<const uint8_t*>(position);

		if (pattern_current == '\?' || memory_current == GetByte(pattern)) {
			if (!first_match)
				first_match = position;

			if (!pattern[2])
				return first_match;

			pattern += pattern_current != '\?' ? 3 : 2;
		}
		else {
			pattern = target_pattern;
			first_match = 0;
		}
	}

	return NULL;
}

}
