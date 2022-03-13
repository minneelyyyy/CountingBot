
#include <concord/discord.h>
#include "commands/commands.h"
#include "counting.h"

struct counting_bot_server_data this = {
    .count = 1,
    .channel = 698016948847378472,
    .last_author = 0,
    .prefix = "++",
};

void on_ready(struct discord *client)
{
    const struct discord_user *bot = discord_get_self(client);
    log_info("Logged in as %s!", bot->username);
}

static bool is_zero(char *str)
{
	while (*str)
		if (*str++ != '0')
			return false;

	return true;
}

void on_message(struct discord *client, const struct discord_message *msg)
{
    /* exit if the author is a bot */
    if (msg->author->bot)
        return;

    /* exit if content is null */
    if (!strcmp(msg->content, ""))
        return;

    /* we do not do special count processing if we aren't in #counting */
    if (!(msg->channel_id == this.channel))
        goto just_handle_commands;

    unsigned long uinputnumber = atoi(msg->content);

    /* if the message does not contain a number */
    if (uinputnumber == 0 && !is_zero(msg->content))
        goto just_handle_commands;

    counting(client, msg, &this, uinputnumber);
    return;

just_handle_commands:
    /* check if message starts with the prefix,
       for some reason using ! on this does the opposite? */
    if (strncmp(msg->content, this.prefix, strlen(this.prefix)))
        return;

    struct discord_create_message params = { .content = "command does not exist" };
    discord_create_message(client, msg->channel_id, &params, NULL);
}
