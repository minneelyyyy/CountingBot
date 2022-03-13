
#ifndef COUNTING_BOT_H
#define COUNTING_BOT_H

#include <concord/discord.h>

struct counting_bot_server_data {
    unsigned long count;
    u64snowflake channel;
    u64snowflake last_author;
    char prefix[32];
};
void set_server_data(u64snowflake guild_id, struct counting_bot_server_data *data);
void get_server_data(u64snowflake guild_id, struct counting_bot_server_data *data);

#endif /* COUNTING_BOT_H */