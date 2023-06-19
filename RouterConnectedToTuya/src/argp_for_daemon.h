#ifndef ARGP_FOR_DAEMON_H
#define ARGP_FOR_DAEMON_H

#include <stdlib.h>
#include <argp.h>
#include <string.h>
#include <stdbool.h>

static struct argp_option options[] = {
    {"productId",   'p', "PRODUCT_ID", 0,
    "Product id" },

    {"deviceId",    'd', "DEVICE_ID", 0,
    "Device id" },
  
    {"deviceSecret", 's', "DEVICE_SECRET", 0,
    "Device secret" },

    {"becomeDaemon", 'D', NULL, 0, 
    "Become daemon program" },
  { 0 }
};

/* Used by main to communicate with parse_opt. */
struct Arguments
{
  char *productId;
  char *deviceId;
  char *deviceSecret;
  bool becomeDaemon;
};

error_t parse_opt(int key, char *arg, struct argp_state *state);

static struct argp argp = { options, parse_opt, 0, 0 };

#endif