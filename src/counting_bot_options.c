
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <concord/discord.h>
#include "counting_bot_options.h"

static struct counting_bot_options default_options = {
    .count = 1,
    .channel = 0,
    .last_author = 0,
    .prefix = "++",
};

void set_server_options(u64snowflake guild_id, struct counting_bot_options *data)
{
    /* create variable to store the path of the directory for server data */
    char path[256] = "";
    sprintf(path, "data/servers/%lu", guild_id);

    /* check if path exists, if it doesnt, create it */
    struct stat sb;
    if (stat(path, &sb) == -1)
    {
        mkdir(path, S_IRWXU);
    }

    /* create a variable to store the name of the data file to store options in */
    char data_file_name[512];
    sprintf(data_file_name, "%s/%s", path, "options.dat");

    /* open file in write mode, this will create it if it doesn't exist */
    FILE *data_file = fopen(data_file_name, "w");

    /* write the data */
    fwrite(data, sizeof(struct counting_bot_options), 1, data_file);

    /* close the file */
    fclose(data_file);
}

void get_server_options(u64snowflake guild_id, struct counting_bot_options *data)
{
    /* create variable to store the path of the directory for server data */
    char path[256] = "";
    sprintf(path, "data/servers/%lu", guild_id);

    /* check if path exists, if it doesnt, create it */
    struct stat sb;
    if (stat(path, &sb) == -1)
    {
        mkdir(path, S_IRWXU);
    }

    /* create a variable to store the name of the data file to store options in */
    char data_file_name[512];
    sprintf(data_file_name, "%s/%s", path, "options.dat");

    /* check if data file exists, if it doesn't, write default values into it */
    if (access(data_file_name, F_OK) == -1)
    {
        set_server_options(guild_id, &default_options);
    }

    /* open file in read mode */
    FILE *data_file = fopen(data_file_name, "r");

    /* read the data */
    size_t sz = fread(data, sizeof(struct counting_bot_options), 1, data_file);

    if (sz != sizeof(struct counting_bot_options))
    {
        fprintf(stderr, "CountingBot: error: bad write to %s: %s\n", data_file_name, strerror(errno));
        exit(1);
    }

    /* close the file */
    fclose(data_file);
}
