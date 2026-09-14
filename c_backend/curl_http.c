#include <curl/curl.h>
#include "curl_http.h"

static size_t writeMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);

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

char *curl_make_igdb_request(char *url, char *query) {
  printf("MAKE REQUEST\n");
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

    char client_id[128];
    char access_token[128];

    snprintf(client_id, sizeof(client_id), "Client-ID: %s", getenv("IGDB_CLIENT_ID") ? getenv("IGDB_CLIENT_ID") : "");
    snprintf(access_token, sizeof(access_token), "Authorization: Bearer %s", getenv("IGDB_ACCESS_TOKEN") ? getenv("IGDB_ACCESS_TOKEN") : "");

    headers = curl_slist_append(headers, client_id);
    headers = curl_slist_append(headers, access_token);
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