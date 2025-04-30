#include <stdio.h>
#include <err.h>
#include <ccan/tal/tal.h>

// A structure containing a popened command.
struct command {
    FILE *f;
    char *command;
};

// When struct command is freed, we also want to pclose pipe.
static void close_cmd(struct command *cmd)
{
    pclose(cmd->f);
}

// This function opens a writable pipe to the given command.
static struct command *open_output_cmd(const tal_t *ctx,
                                       const char *a0, const char *a1)
{
    struct command *cmd = tal(ctx, struct command);

    if (!cmd) {
        return NULL;
    }

    // Note that tal/str has helpers to make this much easier!
    cmd->command = tal_arrz(cmd, char, strlen(a0) + strlen(a1) + 2);
    if (!cmd->command) {
        tal_free(cmd);
        return NULL;
    }
    strcat(cmd->command, a0);
    strcat(cmd->command, " ");
    strcat(cmd->command, a1);

    cmd->f = popen(cmd->command, "w");
    if (!cmd->f) {
        tal_free(cmd);
        return NULL;
    }
    tal_add_destructor(cmd, close_cmd);
    return cmd;
}

int main(int argc, char *argv[])
{
    struct command *cmd;

    if (argc != 2) {
        errx(1, "Usage: %s <command>\n", argv[0]);
    }

    cmd = open_output_cmd(NULL, argv[1], "hello");
    if (!cmd) {
        err(1, "Running '%s hello'", argv[1]);
    }
    fprintf(cmd->f, "This is a test\n");
    tal_free(cmd);
    return 0;
}
