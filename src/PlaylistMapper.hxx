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

#ifndef MPD_PLAYLIST_MAPPER_HXX
#define MPD_PLAYLIST_MAPPER_HXX

class Mutex;
class Cond;
class SongEnumerator;
struct InputStream;

/**
 * Opens a playlist from an URI relative to the playlist or music
 * directory.
 *
 * @param is_r on success, an input_stream object may be returned
 * here, which must be closed after the playlist_provider object is
 * freed
 */
SongEnumerator *
playlist_mapper_open(const char *uri, Mutex &mutex, Cond &cond,
		     InputStream **is_r);

#endif
