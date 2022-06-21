#include <string.h>

#include <sys/stat.h>
#include <concord/discord.h>

#include "commands/commands.h"
#include "counting_bot_options.h"
#include "counting.h"

void on_ready(struct discord *client)
{
    const struct discord_user *bot = discord_get_self(client);
    log_info("Logged in as %s!", bot->username);

    /* make necessary directories */
    mkdir("data", S_IRWXU);
    mkdir("data/servers", S_IRWXU);
}

inline bool is_zero(char *str)
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

    /* store server data */
    struct counting_bot_options this;

    get_server_options(msg->guild_id, &this);

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
    /* check if message starts with the prefix */
    if (strncmp(msg->content, this.prefix, strlen(this.prefix)))
        return;

    char *command = strtok(msg->content + strlen(this.prefix), " ");

    if (!strcmp(command, "ping"))
    {
        ping(client, msg);
    }
    else if (!strcmp(command, "set"))
    {
        char *option = strtok(NULL, " ");

        if (!strcmp(option, "channel"))
        {
            char *channel_mention = strtok(NULL, " ");
            u64snowflake channel_id;

            sscanf(channel_mention, "<#%lu>", &channel_id);

            printf("%lu\n", channel_id);

            this.channel = channel_id;
            set_server_options(msg->guild_id, &this);
        }
        else if (!strcmp(option, "prefix"))
        {
            char *prefix = strtok(NULL, " ");

            strncpy(this.prefix, prefix, 31);
            set_server_options(msg->guild_id, &this);
        }
    }
}
