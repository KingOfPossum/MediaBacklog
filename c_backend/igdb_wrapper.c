#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

const char *API_URL = "https://api.igdb.com/v4/";

char *construct_url(char *endpoint);
char *construct_query(char *game_name, char *platform);

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
    "sort rating desc; limit 10; ",
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

int main(int argc, char *argv[]) {
  curl_global_init(CURL_GLOBAL_ALL);

  char *url = construct_url("games");
  char *query  = construct_query("Metroid Prime","GameCube");

  char *result = make_request(url,query);

  if(result != NULL) {
    printf("%s\n",result);
    free(result);
  }

  free(url);
  free(query);

  curl_global_cleanup();
  return 0;
}