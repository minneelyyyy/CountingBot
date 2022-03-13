
#include <concord/discord.h>
#include "counting_bot_options.h"
#include "counting.h"


void counting(struct discord *client,
              const struct discord_message *msg,
              struct counting_bot_options *options,
              unsigned long uinputnumber)
{
    /* if the same person goes twice in a row, it is important that
       this comes after checking for if it is a number */
    if (msg->author->id == options->last_author)
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

        options->count = 1;
        options->last_author = 0;
        set_server_data(msg->guild_id, options);
        return;
    }

    /* check if the number is the correct next number, if not: reset and tell the user */
    if (uinputnumber == options->count)
    {
        options->count++;
        discord_create_reaction(client, msg->channel_id, msg->id, 0, "✅", NULL);
        options->last_author = msg->author->id;
        set_server_data(msg->guild_id, options);
    }
    else
    {
        char buffer[128] = "";
        sprintf(buffer, "you messed it up! the correct number was `%lu`, count set back to `1`", options->count);

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

        options->count = 1;
        options->last_author = 0;
        set_server_data(msg->guild_id, options);
    }
}