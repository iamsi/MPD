/*
 * Copyright 2003-2017 The Music Player Daemon Project
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
#include "MessageCommands.hxx"
#include "Request.hxx"
#include "client/Client.hxx"
#include "client/ClientList.hxx"
#include "client/Response.hxx"
#include "Instance.hxx"
#include "util/ConstBuffer.hxx"

#include <set>
#include <string>

#include <assert.h>

CommandResult
handle_subscribe(Client &client, Request args, Response &r)
{
	assert(args.size == 1);
	const char *const channel_name = args[0];

	switch (client.Subscribe(channel_name)) {
	case Client::SubscribeResult::OK:
		return CommandResult::OK;

	case Client::SubscribeResult::INVALID:
		r.Error(ACK_ERROR_ARG, "invalid channel name");
		return CommandResult::ERROR;

	case Client::SubscribeResult::ALREADY:
		r.Error(ACK_ERROR_EXIST, "already subscribed to this channel");
		return CommandResult::ERROR;

	case Client::SubscribeResult::FULL:
		r.Error(ACK_ERROR_EXIST, "subscription list is full");
		return CommandResult::ERROR;
	}

	/* unreachable */
	assert(false);
	gcc_unreachable();
}

CommandResult
handle_unsubscribe(Client &client, Request args, Response &r)
{
	assert(args.size == 1);
	const char *const channel_name = args[0];

	if (client.Unsubscribe(channel_name))
		return CommandResult::OK;
	else {
		r.Error(ACK_ERROR_NO_EXIST, "not subscribed to this channel");
		return CommandResult::ERROR;
	}
}

CommandResult
handle_channels(Client &client, gcc_unused Request args, Response &r)
{
	assert(args.empty());

	std::set<std::string> channels;
	for (const auto &c : *client.GetInstance().client_list)
		channels.insert(c.subscriptions.begin(),
				c.subscriptions.end());

	for (const auto &channel : channels)
		r.Format("channel: %s\n", channel.c_str());

	return CommandResult::OK;
}

CommandResult
handle_read_messages(Client &client,
		     gcc_unused Request args, Response &r)
{
	assert(args.empty());

	while (!client.messages.empty()) {
		const ClientMessage &msg = client.messages.front();

		r.Format("channel: %s\nmessage: %s\n",
			 msg.GetChannel(), msg.GetMessage());
		client.messages.pop_front();
	}

	return CommandResult::OK;
}

CommandResult
handle_send_message(Client &client, Request args, Response &r)
{
	assert(args.size == 2);

	const char *const channel_name = args[0];
	const char *const message_text = args[1];

	if (!client_message_valid_channel_name(channel_name)) {
		r.Error(ACK_ERROR_ARG, "invalid channel name");
		return CommandResult::ERROR;
	}

	bool sent = false;
	const ClientMessage msg(channel_name, message_text);
	for (auto &c : *client.GetInstance().client_list)
		if (c.PushMessage(msg))
			sent = true;

	if (sent)
		return CommandResult::OK;
	else {
		r.Error(ACK_ERROR_NO_EXIST,
			"nobody is subscribed to this channel");
		return CommandResult::ERROR;
	}
}
