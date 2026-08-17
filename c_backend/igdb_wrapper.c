#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include <curl/curl.h>

const char *API_URL = "https://api.igdb.com/v4/";

char *construct_url(char *endpoint);
char *construct_query(char *game_name, char *platform);

struct MemoryStruct {
  char *memory;
  size_t size;
};

typedef struct IGDBEntry {
  int igdb_id;
  char *game_name;
  char *url;
  char *cover_url;
  char **platforms;
  int num_platforms;
} IGDBEntry;

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

char *construct_url(char *endpoint) {
  size_t length = strlen(API_URL) + strlen(endpoint) + 1;
  char *url = malloc(length);
  if(url == NULL) {
    return NULL;
  }

  strcpy(url,API_URL);
  strcat(url,endpoint);

  return url;
}

char *construct_query(char *game_name, char *platform) {
  char *query = malloc(512);

  if(query == NULL) {
    return NULL;
  }
  
  if(platform) {
    snprintf(query,512,
    "fields name,url,platforms.name,cover.url; "
    "where name ~ *\"%s\"* & (platforms.name ~ *\"%s\"* | platforms.abbreviation ~ *\"%s\"*); "
    "sort rating desc; limit 1; ",
    game_name, platform, platform
    );
  }
  else {
    snprintf(query,512,
    "fields name,url,platforms.name,cover.url; "
    "where name ~ *\"%s\"*; ",
    game_name
    );
  }
 
  return query;  
}

char *make_request(char *url, char *query) {
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

    headers = curl_slist_append(headers, "Client-ID: ");
    headers = curl_slist_append(headers, "Authorization: Bearer ");
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

IGDBEntry parseResult(char *result) {  
  IGDBEntry entry;
  
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

    entry.game_name = malloc(128);
    entry.url = malloc(128);
    entry.cover_url = malloc(128);
    entry.num_platforms = 0;
    entry.platforms = NULL;

    cJSON *id = cJSON_GetObjectItemCaseSensitive(game,"id");
    if(cJSON_IsNumber(id)) {
      entry.igdb_id = id->valueint;
    }
    else {
      entry.igdb_id = -1;
    }

    cJSON *name = cJSON_GetObjectItemCaseSensitive(game,"name");
    if(cJSON_IsString(name)) {
      strcpy(entry.game_name,name->valuestring);
    }
    else {
      entry.game_name = NULL;
    }

    cJSON *url = cJSON_GetObjectItemCaseSensitive(game,"url");
    if(cJSON_IsString(url)) {
      strcpy(entry.url,url->valuestring);
    }
    else {
      entry.url = NULL;
    }

    cJSON *cover = cJSON_GetObjectItemCaseSensitive(game,"cover");
    if(cJSON_IsObject(cover)) {
      cJSON *cover_url = cJSON_GetObjectItemCaseSensitive(cover,"url");
      if(cJSON_IsString(cover_url)) {
        strcpy(entry.cover_url,cover_url->valuestring);
      }
      else {
        entry.cover_url = NULL;
      }
    }
    else {
      entry.cover_url = NULL;
    }

    cJSON *platforms = cJSON_GetObjectItemCaseSensitive(game,"platforms");
    if(cJSON_IsArray(platforms)) {
      int num_platforms = cJSON_GetArraySize(platforms);
      entry.num_platforms = num_platforms;
      entry.platforms = malloc(num_platforms * sizeof(char *));

      for(int i = 0;i < num_platforms;i++) {
        cJSON *platform = cJSON_GetArrayItem(platforms,i);
        
        entry.platforms[i] = NULL;

        if(cJSON_IsObject(platform)) {
          cJSON *platform_name = cJSON_GetObjectItemCaseSensitive(platform,"name");
          
          if(cJSON_IsString(platform_name)) {
            entry.platforms[i] = malloc(128);
            strcpy(entry.platforms[i],platform_name->valuestring);
          }
          else {
            entry.platforms[i] = NULL;
          }
        }
      }
    }
  }
  
  cJSON_Delete(json);
  return entry;
}

int main(int argc, char *argv[]) {
  curl_global_init(CURL_GLOBAL_ALL);

  char *url = construct_url("games");
  char *query  = construct_query("Skyrim","PC");

  char *result = make_request(url,query);

  if(result != NULL) {
    printf("%s\n",result);

    IGDBEntry entry = parseResult(result);

    printf("Game:\n");
    printf("  ID: %d\n",entry.igdb_id);
    printf("  NAME: %s\n",entry.game_name);
    printf("  URL: %s\n",entry.url);
    printf("  COVER_URL: %s\n",entry.cover_url);
    printf("  PLATFORMS:\n");
    
    for(int i = 0;i < entry.num_platforms;i++) {
      printf("    -%s\n",entry.platforms[i]);
    }

    free(result);
  }

  free(url);
  free(query);

  curl_global_cleanup();
  return 0;
}