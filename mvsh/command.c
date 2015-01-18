/**
 * @file command.c
 */
#include <stdio.h>       /* fprintf */
#include <stdlib.h>      /* exit */
#include <string.h>      /* strchr */
#include <mv/device.h>   /* mv_device_addr */
#include <mv/value.h>    /* mv_value_t */
#include <mv/message.h>  /* mv_message_send */
#include "command.h"

struct {
  const char *str;
  int nargs;
  const char *sig;
  const char *detail;
} _cmdinfo[] = {
  { "prop_get",   1, "prop_get dev:prop",     "prints the property value" },
  { "prop_set",   2, "prop_set dev:prop val", "updates the property value" },
  { "exit",       0, "exit",                  "exit the shell" },
  { "quit",       0, "quit",                  "exit the shell"  },
  { "help",       0, "help",                  "prints help info"  },
  { "",           0}
};


static int _command_tokenize(char *line, char **cmd, char **a0, char **a1);
static int _command_prop_get(char *arg0);
static int _command_help();


int _command_tokenize(char *line, char **cmd, char **arg0, char **arg1)
{
  char *token;
  if ((token = strtok(line, " \t")) == NULL)
    return -1;
  *cmd = token;
  if ((token = strtok(NULL, " \t")) == NULL)
    return 0;
  *arg0 = token;
  if ((token = strtok(NULL, " \t")) == NULL)
    return 0;
  *arg1 = token;

  return 0;
}

static int _command_prop_get(char *arg0)
{
  char *charp;
  if (!arg0 || ((charp = strstr(arg0, ":")) == NULL) || charp == arg0) {
    fprintf(stdout, "Invalid property, %s: should be \"dev:name\"\n", arg0);
    return -1;
  }
  
  char *prop = strdup(charp + 1);
  char save = *charp;
  *charp = '\0';
  char *dev = strdup(arg0);
  *charp = save;

  const char *destaddr = mv_device_addr(dev);

  char arg[4096];
  sprintf(arg, "\"arg\":{\"name\":\"%s\", \"retid\":0, \"retaddr\": \"%s\"}",
          prop, mv_message_selfaddr());
  fprintf(stdout, "PROP_GET: %s\n", arg);
  mv_message_send(destaddr, MV_MESSAGE_PROP_GET, arg);

  char *reply = NULL;
  while ((reply = mv_message_recv()) == NULL) ;
  fprintf(stdout, "reply: %s\n", reply);

  mv_value_t reply_v = mv_value_from_str(reply);
  mv_value_t argstr_v = mv_value_string("arg");
  mv_value_t arg_v = mv_value_map_lookup(reply_v, argstr_v);
  mv_value_t retvalstr_v = mv_value_string("retval");
  mv_value_t retval_v = mv_value_map_lookup(arg_v, retvalstr_v);
  fprintf(stdout, "Value: "); 
  mv_value_print(retval_v);
  fprintf(stdout, "\n");

  return 0;
}

static int _command_prop_set(char *arg0, char *arg1)
{
  char *charp;
  if (!arg0 || ((charp = strstr(arg0, ":")) == NULL) || charp == arg0) {
    fprintf(stdout, "Invalid property, %s: should be \"dev:name\"\n", arg0);
    return -1;
  }
  if (!arg1) {
    fprintf(stdout, "Property value must be provided.\n");
    return -1;
  }
  
  char *prop = strdup(charp + 1);
  char save = *charp;
  *charp = '\0';
  char *dev = strdup(arg0);
  *charp = save;

  const char *destaddr = mv_device_addr(dev);

  char arg[4096];
  sprintf(arg, " \"arg\": {\"name\":\"%s\", \"value\":%s}", prop, arg1);
  fprintf(stdout, "PROP_SET: %s\n", arg);
  mv_message_send(destaddr, MV_MESSAGE_PROP_SET, arg);

  return 0;
}

int _command_help()
{
  int i;
  fprintf(stdout, "  %-25s   %s\n", "COMMAND", "DETAILS");
  for (i = 0; i < MVSH_CMD_NTAGS; i++) { 
    fprintf(stdout, "  %-25s   %s\n", _cmdinfo[i].sig, _cmdinfo[i].detail);
 }

  return 0;
}

/*
 * Functions for command interface.
 */
int mvsh_command_process(char *line)
{
  char *cmd = NULL;
  char *arg0 = NULL;
  char *arg1 = NULL;
  if (_command_tokenize(line, &cmd, &arg0, &arg1) == -1) {
    fprintf(stderr, "Failed to process command: %s\n", line);
    return -1;
  }

  if (!cmd)
    return -1;
    
  switch (mvsh_command_tag(cmd)) {
  case MVSH_CMD_PROP_GET:
    _command_prop_get(arg0);
    break;
  case MVSH_CMD_PROP_SET:
    _command_prop_set(arg0, arg1);
    break;
  case MVSH_CMD_HELP:
    _command_help();
    break;
  case MVSH_CMD_EXIT:
  case MVSH_CMD_QUIT:
    exit(EXIT_SUCCESS);
  default:
    fprintf(stdout, "Invalid command: %s\n", cmd);
  }
    

  return 0;
}

int mvsh_command_tag(const char *s)
{
  int i;
  for (i = 0; i < MVSH_CMD_NTAGS; i++) {
    if (!strcmp(_cmdinfo[i].str, s)) 
      return i;
  }

  return -1;
}

extern const char *mvsh_command_tagstr(mvsh_cmdtag_t cmd)
{
  return _cmdinfo[cmd].str;
}

extern int mvsh_command_nargs(mvsh_cmdtag_t cmd)
{
  return _cmdinfo[cmd].nargs;
}



