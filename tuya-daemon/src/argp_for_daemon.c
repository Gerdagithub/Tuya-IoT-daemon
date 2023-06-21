#include "argp_for_daemon.h"

error_t parse_opt(int key, char *arg, struct argp_state *state)
{
  struct Arguments *arguments = state->input;

  switch (key)
  {
    case 'p':
      strncpy(arguments->productId, arg, strlen(arg) + 1); 
      break;
        
    case 'd':
      strncpy(arguments->deviceId, arg, strlen(arg) + 1); 
      break;
        
    case 's':
      strncpy(arguments->deviceSecret, arg, strlen(arg) + 1); 
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