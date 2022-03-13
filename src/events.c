
#include <concord/discord.h>
#include "commands/commands.h"

unsigned long count = 1;
u64snowflake channel = 698016948847378472;
u64snowflake last_author = 0;

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
    /* exit if it isn't counting channel */
    if (!(msg->channel_id == channel))
        return;
    
    /* exit if the author is a bot */
    if (msg->author->bot)
        return;
    
    unsigned long uinputnumber = atoi(msg->content);

    /* if the message does not contain a number at the start */
    if (uinputnumber == 0 && !is_zero(msg->content))
        return;
    
    /* if the same person goes twice in a row, it is important that
       this comes after checking for if it is a number */
    if (msg->author->id == last_author)
    {
        struct discord_create_message params = {
            .content = "you are not allowed to go twice in a row! count set back to `1`",
            .message_reference = &(struct discord_message_reference) {
                .message_id = msg->id,
                .channel_id = msg->channel_id,
                .guild_id = msg->guild_id,
            }
        };

        discord_create_reaction(client, msg->channel_id, msg->id, 0, "❌", NULL);
        discord_create_message(client, msg->channel_id, &params, NULL);

        count = 1;
        last_author = 0;
        return;
    }

    /* check if the number is the correct next number, if not: reset and tell the user */
    if (uinputnumber == count)
    {
        count++;
        discord_create_reaction(client, msg->channel_id, msg->id, 0, "✅", NULL);
        last_author = msg->author->id;
    }
    else
    {
        char buffer[128] = "";
        sprintf(buffer, "you messed it up! the correct number was `%lu`, count set back to `1`", count);

        struct discord_create_message params = {
            .content = buffer,
            .message_reference = &(struct discord_message_reference) {
                .message_id = msg->id,
                .channel_id = msg->channel_id,
                .guild_id = msg->guild_id,
            }
        };

        discord_create_reaction(client, msg->channel_id, msg->id, 0, "❌", NULL);
        discord_create_message(client, msg->channel_id, &params, NULL);

        count = 1;
        last_author = 0;
    }
}
