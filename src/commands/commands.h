
#ifndef COMMANDS_H
#define COMMANDS_H

// sends a message, then edits the message to contain the bots ping
void ping(struct discord *client, const struct discord_message *msg);

#endif /* COMMANDS_H */