
#ifndef COUNTING_H
#define COUNTING_H

#include "concord/discord.h"

void counting(struct discord *client,
              const struct discord_message *msg,
              struct counting_bot_options *options,
              unsigned long uinputnumber);

#endif /* COUNTING_H */
