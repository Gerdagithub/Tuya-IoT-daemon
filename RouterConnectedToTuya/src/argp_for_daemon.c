#include "argp_for_daemon.h"

error_t parse_opt(int key, char *arg, struct argp_state *state)
{
  struct Arguments *arguments = state->input;

  switch (key)
  {
    case 'p':
      arguments->productId = arg;
      break;
        
    case 'd':
      arguments->deviceId = arg;
      break;
        
    case 's':
      arguments->deviceSecret = arg;
      break;
    
    case 'D':
      arguments->becomeDaemon = true;
        
    case ARGP_KEY_END:
      if (!arguments->productId || !arguments->deviceId || !arguments->deviceSecret)
      /* Missing arguments. */
        argp_usage (state);
      break;
        
    default:
      return ARGP_ERR_UNKNOWN;
  }
    
  return 0;
}