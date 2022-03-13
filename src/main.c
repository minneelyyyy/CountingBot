#include <concord/discord.h>

#include "events.h"
#include "secret.h"

int main(void)
{
    struct discord *client = discord_init(BOT_TOKEN);
    discord_set_on_ready(client, &on_ready);
    discord_set_on_message_create(client, &on_message);
    discord_run(client);
}
