#ifndef IGDB_WRAPPER
#define IGDB_WRAPPER

typedef struct IGDBTimeEntry {
  int hastily;
  int normally;
  int completely;
} IGDBTimeEntry;

typedef struct IGDBEntry {
  int igdb_id;
  char *game_name;
  char *url;
  char *cover_url;
  char *summary;
  char **platforms;
  int num_platforms;
  char **genres;
  int num_genres;
  IGDBTimeEntry times;
} IGDBEntry;

IGDBEntry getGame(char *game_name, char *platform);
void free_entry(IGDBEntry *entry);
void print_entry(IGDBEntry entry);

void init_wrapper();
void exit_wrapper();

#endif