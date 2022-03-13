
#ifndef COUNTING_H
#define COUNTING_H

#include "concord/discord.h"

struct counting_bot_server_data {
    unsigned long count;
    u64snowflake channel;
    u64snowflake last_author;
    char prefix[32];
};

void counting(struct discord *client,
              const struct discord_message *msg,
              struct counting_bot_server_data *options,
              unsigned long uinputnumber);

#endif /* COUNTING_H */
