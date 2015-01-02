/**
 * @file event.c
 */
#include <stdio.h>      /* fprintf */
#include <stdlib.h>     /* free, exit */
#include <string.h>     /* strdup */
#include <assert.h>     /* assert */
#include "mv/event.h"

/*
 * Functions for the event API.
 */
int mv_event_service_init()
{
  fprintf(stdout, "Event service initiated...\n");
  return 0;
}

int mv_event_register(const char *dev, const char *ev)
{
  assert(0);
  return 0;
}

int mv_event_deregister(const char *dev, const char *ev)
{
  assert(0);
  return 0;
}

int mv_event_subscribe(const char *dev, const char *ev, const char *subdev)
{
  /* TODO: RPC */
  assert(0);
  return 0;
}

int mv_event_publish(const char *dev, const char *ev, mv_value_t val)
{
  assert(0);
  return 0;
}
