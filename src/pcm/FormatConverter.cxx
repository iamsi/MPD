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

#include "config.h"
#include "FormatConverter.hxx"
#include "PcmFormat.hxx"
#include "Domain.hxx"
#include "util/ConstBuffer.hxx"
#include "util/Error.hxx"

#include <assert.h>

bool
PcmFormatConverter::Open(SampleFormat _src_format, SampleFormat _dest_format,
			 gcc_unused Error &error)
{
	assert(_src_format != SampleFormat::UNDEFINED);
	assert(_dest_format != SampleFormat::UNDEFINED);

	src_format = _src_format;
	dest_format = _dest_format;
	return true;
}

void
PcmFormatConverter::Close()
{
#ifndef NDEBUG
	src_format = SampleFormat::UNDEFINED;
	dest_format = SampleFormat::UNDEFINED;
#endif
}

ConstBuffer<void>
PcmFormatConverter::Convert(ConstBuffer<void> src, Error &error)
{
	switch (dest_format) {
	case SampleFormat::UNDEFINED:
		assert(false);
		gcc_unreachable();

	case SampleFormat::S8:
	case SampleFormat::DSD:
		error.Format(pcm_domain,
			     "PCM conversion from %s to %s is not implemented",
			     sample_format_to_string(src_format),
			     sample_format_to_string(dest_format));
		return nullptr;

	case SampleFormat::S16:
		return pcm_convert_to_16(buffer, dither,
					 src_format,
					 src).ToVoid();

	case SampleFormat::S24_P32:
		return pcm_convert_to_24(buffer,
					 src_format,
					 src).ToVoid();

	case SampleFormat::S32:
		return pcm_convert_to_32(buffer,
					 src_format,
					 src).ToVoid();

	case SampleFormat::FLOAT:
		return pcm_convert_to_float(buffer,
					    src_format,
					    src).ToVoid();
	}

	assert(false);
	gcc_unreachable();
}
