#ifndef IGDB_WRAPPER
#define IGDB_WRAPPER

typedef struct IGDBEntry {
  int igdb_id;
  char *game_name;
  char *url;
  char *cover_url;
  char **platforms;
  int num_platforms;
} IGDBEntry;

IGDBEntry getGame(char *game_name, char *platform);
void free_entry(IGDBEntry *entry);
void print_entry(IGDBEntry entry);

void init_wrapper();
void exit_wrapper();

#endif