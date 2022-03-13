
#include <concord/discord.h>
#include <sys/time.h>

/* this is supposed to be part of sys/time.h but it isn't for some reason */
#define timersub(a, b, result)	\
  do { \
    (result)->tv_sec = (a)->tv_sec - (b)->tv_sec; \
    (result)->tv_usec = (a)->tv_usec - (b)->tv_usec; \
    if ((result)->tv_usec < 0) { \
      --(result)->tv_sec; \
      (result)->tv_usec += 1000000; \
    } \
  } while (0)

void ping(struct discord *client, const struct discord_message *msg)
{
    struct timeval before, after, result;

    struct discord_create_message params = {
        .content = "pong! :ping_pong:",
        .message_reference = &(struct discord_message_reference) {
            .message_id = msg->id,
            .channel_id = msg->channel_id,
            .guild_id = msg->guild_id,
        }
    };

    struct discord_message sent;
    struct discord_ret_message message = { .sync = &sent };

    gettimeofday(&before, NULL);
    discord_create_message(client, msg->channel_id, &params, &message);
    gettimeofday(&after, NULL);

    timersub(&after, &before, &result);

    double seconds = result.tv_sec + ((double) result.tv_usec / 1e6);
    double milliseconds = seconds * 1000;

    char buffer[64] = "";

    sprintf(buffer, "pong! :ping_pong: `%.2fms`", milliseconds);

    struct discord_edit_message params_edit = { .content = buffer };
    discord_edit_message(client, sent.channel_id, sent.id, &params_edit, &message);
}
