/**
 * @file command.h
 *
 * @brief Interface to MVSH command processor.
 */
#ifndef MVSH_COMMAND_H
#define MVSH_COMMAND_H

typedef enum {
  MVSH_CMD_PROP_GET,
  MVSH_CMD_PROP_SET,
  MVSH_CMD_EXIT,
  MVSH_CMD_QUIT,
  MVSH_CMD_HELP,
  MVSH_CMD_NTAGS
} mvsh_cmdtag_t;


/* Processes the given command. */
extern int mvsh_command_process(char *line);

extern int mvsh_command_tag(const char *s);
extern int mvsh_command_nargs(mvsh_cmdtag_t cmd);
extern const char *mvsh_command_tagstr(mvsh_cmdtag_t cmd);

#endif /* MVSH_COMMAND_H */
