#include <getopt.h>

#ifndef SKIP_SETTINGS_H
#define SKIP_SETTINGS_H

struct skip_settings {
  int lines;
  FILE *fd;
};


void skip_settings_get(int argc, char **argv, struct skip_settings *set);

#endif
