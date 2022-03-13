
#ifndef EVENTS_H
#define EVENTS_H

#include <concord/discord.h>

void on_ready(struct discord *client);

void on_message(struct discord *client, const struct discord_message *msg);

#endif /* EVENTS_H */
