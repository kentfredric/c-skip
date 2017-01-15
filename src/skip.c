#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <errno.h>

struct skip_settings {
  int lines;
  FILE *fd;
};

void skip_settings_get(int argc, char **argv, struct skip_settings *set);

int main (int argc, char **argv) {

  struct skip_settings s;
  skip_settings_get(argc, argv, &s);

  int current_line = 0;

  char *line = NULL;
  size_t len = 0;
  ssize_t read;

  while( current_line < s.lines && ( read = getline(&line, &len, s.fd) ) != -1 ) {
    current_line++;
  }
  if ( read != -1 ) { 
    while ((read = getline(&line, &len, s.fd)) != -1) {
      fwrite(line, read, 1, stdout);
    }
  }
  free(line);
  exit(0);
}

static inline void diehelp(char **argv) {
    fprintf(stderr, "Usage: %s [-n lines] [file]\n", argv[0] );
    exit(EXIT_FAILURE);
}

void skip_settings_get(int argc, char **argv, struct skip_settings *set) {

  static char           *short_options = "n:";
  static struct option  long_options[] = {
    { "lines",  1,  0,    'n' },
    { NULL,     0,  NULL, 0   }
  };

  set->lines = 20;
  set->fd    = stdin;

  int current_opt;
  int option_index;
 
  while(( current_opt = getopt_long(argc, argv, short_options, long_options, &option_index) ) != -1 ) {
    if( current_opt == 'n' || ( current_opt == 0 && long_options[option_index].name == "lines" ) ) {
      set->lines = strtol( optarg, NULL, 10);
      continue;
    }
    diehelp(argv);
  }
  if ( optind < argc ) {
    if ( argc - optind > 1 ) {
      fprintf(stderr, "Too many input stream arguments, only one accepted\n");
      diehelp(argv);
    }
    if ( strcmp(argv[optind],"-") == 0 ) {
      /* noop */
    } else {
      if( ( set->fd = fopen( argv[optind], "r" ) ) == NULL ) {
        int errsv = errno;
        fprintf(stderr, "Can't read %s: %s\n", argv[optind], strerror(errsv) );
        diehelp(argv);
      }
    }
  }
}
