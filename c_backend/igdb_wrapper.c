#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include <curl/curl.h>

#include "igdb_wrapper.h"

const char *API_URL = "https://api.igdb.com/v4/";

static size_t writeMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);
static char *construct_url(char *endpoint);
static char *construct_query(char *game_name, char *platform);
static char *make_request(char *url, char *query);
static IGDBEntry parseResult(char *result);

struct MemoryStruct {
  char *memory;
  size_t size;
};

static size_t writeMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;

  char *ptr = realloc(mem->memory, mem->size + realsize + 1);
  if(ptr == NULL) {
    printf("Not enough memory!\n");
    return 0;
  }

  mem->memory = ptr;
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;

  return realsize;
}

static char *construct_url(char *endpoint) {
  size_t length = strlen(API_URL) + strlen(endpoint) + 1;
  char *url = malloc(length);
  if(url == NULL) {
    return NULL;
  }

  strcpy(url,API_URL);
  strcat(url,endpoint);

  return url;
}

static char *construct_query(char *game_name, char *platform) {
  char *query = malloc(512);

  if(query == NULL) {
    return NULL;
  }
  
  if(platform) {
    snprintf(query,512,
    "fields name,url,platforms.name,genres.name,cover.url,summary; "
    "where name ~ *\"%s\"* & (platforms.name ~ *\"%s\"* | platforms.abbreviation ~ *\"%s\"*); "
    "sort rating desc; limit 1; ",
    game_name, platform, platform
    );
  }
  else {
    snprintf(query,512,
    "fields name,url,platforms.name,genres.name,cover.url,summary; "
    "where name ~ *\"%s\"*; ",
    game_name
    );
  }
 
  return query;  
}

static char *make_request(char *url, char *query) {
  CURL *curl;
  CURLcode res;

  struct MemoryStruct chunk;
  chunk.memory = malloc(1);
  chunk.size = 0;

  curl = curl_easy_init();

  if(curl) {
    curl_easy_setopt(curl,CURLOPT_URL, url);
    curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION, writeMemoryCallback);
    curl_easy_setopt(curl,CURLOPT_WRITEDATA, (void *)&chunk);

    struct curl_slist *headers = NULL;

    headers = curl_slist_append(headers, "Client-ID: m4nkh7koxu6lq6ndaj4bzs3n1148l5");
    headers = curl_slist_append(headers, "Authorization: Bearer rj6vnmvto0non59us71cglnb4fdhn1");
    headers = curl_slist_append(headers, "Accept: application/json");

    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, query);

    res = curl_easy_perform(curl);

    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);

    if(res != CURLE_OK) {
      printf("ERROR!\n");
      free(chunk.memory);
      return NULL;
    }
    else {
      return chunk.memory;
    }
  }

  free(chunk.memory);
  return NULL;
}

static IGDBEntry parseResult(char *result) {  
  printf("%s\n\n",result);
  IGDBEntry entry;

  memset(&entry,0,sizeof(IGDBEntry));
  entry.igdb_id = -1;
  entry.num_platforms = 0;
  entry.num_genres = 0;

  cJSON *json = cJSON_Parse(result);
  if(json == NULL) {
    const char *error_ptr = cJSON_GetErrorPtr();
    if(error_ptr != NULL) {
      printf("Error: %s\n",error_ptr);
    }
    cJSON_Delete(json);
    return entry;
  }

  if(cJSON_IsArray(json)) {
    cJSON *game = cJSON_GetArrayItem(json,0);

    cJSON *id = cJSON_GetObjectItemCaseSensitive(game,"id");
    if(id && cJSON_IsNumber(id)) {
      entry.igdb_id = id->valueint;
    }

    cJSON *name = cJSON_GetObjectItemCaseSensitive(game,"name");
    if(name && cJSON_IsString(name)) {
      entry.game_name = strdup(name->valuestring);
    }

    cJSON *url = cJSON_GetObjectItemCaseSensitive(game,"url");
    if(url && cJSON_IsString(url)) {
      entry.url = strdup(url->valuestring);
    }

    cJSON *cover = cJSON_GetObjectItemCaseSensitive(game,"cover");
    if(cover && cJSON_IsObject(cover)) {
      cJSON *cover_url = cJSON_GetObjectItemCaseSensitive(cover,"url");
      if(cJSON_IsString(cover_url)) {
        entry.cover_url = strdup(cover_url->valuestring);
      }
    }

    cJSON *summary = cJSON_GetObjectItemCaseSensitive(game,"summary");
    if(summary && cJSON_IsString(summary)) {
      entry.summary = strdup(summary->valuestring);
    }

    cJSON *platforms = cJSON_GetObjectItemCaseSensitive(game,"platforms");
    if(platforms && cJSON_IsArray(platforms)) {
      int num_platforms = cJSON_GetArraySize(platforms);
      entry.num_platforms = num_platforms;
      entry.platforms = malloc(num_platforms * sizeof(char *));

      for(int i = 0;i < num_platforms;i++) {
        cJSON *platform = cJSON_GetArrayItem(platforms,i);
        
        entry.platforms[i] = NULL;

        if(platform && cJSON_IsObject(platform)) {
          cJSON *platform_name = cJSON_GetObjectItemCaseSensitive(platform,"name");
          
          if(platform_name && cJSON_IsString(platform_name)) {
            entry.platforms[i] = strdup(platform_name->valuestring);
          }
        }
      }
    }

    cJSON *genres = cJSON_GetObjectItemCaseSensitive(game,"genres");
    if(genres && cJSON_IsArray(genres)) {
      int num_genres = cJSON_GetArraySize(genres);
      entry.num_genres = num_genres;
      entry.genres = malloc(num_genres * sizeof(char *));

      for(int i = 0;i < num_genres;i++) {
        cJSON *genre = cJSON_GetArrayItem(genres,i);

        entry.genres[i] = NULL;

        if(genre && cJSON_IsObject(genre)) {
          cJSON *genre_name = cJSON_GetObjectItemCaseSensitive(genre,"name");

          if(genre_name && cJSON_IsString(genre_name)) {
            entry.genres[i] = strdup(genre_name->valuestring);
          }
        }
      }
    }
  }
  else if(cJSON_IsObject(json)) {
    cJSON *msg = cJSON_GetObjectItemCaseSensitive(json,"message");
    if(msg && cJSON_IsString(msg)) {
      if(strcmp(msg->valuestring,"Authorization Failure. Have you tried:") == 0) {
        printf("ERROR: Authentication failed!\n");
      }
    }
  }
  
  cJSON_Delete(json);
  return entry;
}

void free_entry(IGDBEntry *entry) {
  free(entry->cover_url);
  free(entry->game_name);
  free(entry->url);
  
  for(int i = 0;i < entry->num_platforms;i++) {
    free(entry->platforms[i]);
  }
  free(entry->platforms);
}

IGDBEntry getGame(char *game_name, char *platform) {
  IGDBEntry entry;
  memset(&entry,0,sizeof(IGDBEntry));
  entry.igdb_id = -1;

  char *url = construct_url("games");
  char *query = construct_query(game_name,platform);

  char *result = make_request(url,query);
  
  if(result != NULL) {
    entry = parseResult(result);
  }

  free(url);
  free(query);
  free(result);

  return entry;
}

void init_wrapper() {
  curl_global_init(CURL_GLOBAL_ALL);
}

void exit_wrapper() {
  curl_global_cleanup();
}

void print_entry(IGDBEntry entry) {
  printf("\nIGDB Game Entry:\n");
  printf("  ID: %d\n",entry.igdb_id);
  printf("  Name: %s\n",entry.game_name);
  printf("  URL: %s\n",entry.url);
  printf("  Cover: %s\n",entry.cover_url);
  printf("  Summary: %.150s...\n",entry.summary);
  printf("  Platforms:\n");
  for(int i = 0;i < entry.num_platforms;i++) {
    printf("    -%s\n",entry.platforms[i]);
  }
  printf("  Genres:\n");
  for(int i = 0;i < entry.num_genres;i++) {
    printf("    -%s\n",entry.genres[i]);
  }
  printf("\n");
}