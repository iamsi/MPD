/*
 * Copyright (C) 2003-2014 The Music Player Daemon Project
 * http://www.musicpd.org
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef MPD_PCM_DITHER_HXX
#define MPD_PCM_DITHER_HXX

#include <stdint.h>

enum class SampleFormat : uint8_t;

class PcmDither {
	int32_t error[3];
	int32_t random;

public:
	constexpr PcmDither()
		:error{0, 0, 0}, random(0) {}

	/**
	 * Shift the given sample by #SBITS-#DBITS to the right, and
	 * apply dithering.
	 *
	 * @param ST the input sample type
	 * @param SBITS the input bit width
	 * @param DBITS the output bit width
	 * @param sample the input sample value
	 */
	template<typename ST, unsigned SBITS, unsigned DBITS>
	ST DitherShift(ST sample);

	void Dither24To16(int16_t *dest, const int32_t *src,
			  const int32_t *src_end);

	void Dither32To16(int16_t *dest, const int32_t *src,
			  const int32_t *src_end);

private:
	/**
	 * Shift the given sample by #scale_bits to the right, and
	 * apply dithering.
	 *
	 * @param T the input sample type
	 * @param MIN the minimum input sample value
	 * @param MAX the maximum input sample value
	 * @param scale_bits the number of bits to be discarded
	 * @param sample the input sample value
	 */
	template<typename T, T MIN, T MAX, unsigned scale_bits>
	T Dither(T sample);

	/**
	 * Convert the given sample from one sample format to another,
	 * discarding bits.
	 *
	 * @param ST the input #SampleTraits class
	 * @param ST the output #SampleTraits class
	 * @param sample the input sample value
	 */
	template<typename ST, typename DT>
	typename DT::value_type DitherConvert(typename ST::value_type sample);

	template<typename ST, typename DT>
	void DitherConvert(typename DT::pointer_type dest,
			   typename ST::const_pointer_type src,
			   typename ST::const_pointer_type src_end);
};

#endif
