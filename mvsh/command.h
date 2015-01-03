/**
 * @file command.h
 *
 * @brief Interface to MVSH commands.
 */
#ifndef MVSH_COMMAND_H
#define MVSH_COMMAND_H

typedef enum {
  MVSH_CMD_PROP_ADD,
  MVSH_CMD_PROP_GET,
  MVSH_CMD_PROP_SET,
  MVSH_CMD_NTAGS
} mvsh_cmdtag_t;


mvsh_cmdtag_t mvsh_command_tag(const char *s);

int mvsh_command_nargs(mvsh_cmdtag_t cmd);


#endif /* MVSH_COMMAND_H */
