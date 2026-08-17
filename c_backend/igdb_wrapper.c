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
  const char *fields = "fields name,url,platforms.name,cover.url; ";
  
  char *name_query = malloc(128);
  strcpy(name_query,"name ~ *\"");
  strcat(name_query,game_name);
  strcat(name_query,"\"*");

  char *platform_query = malloc(128);

  if(platform) {
    strcpy(platform_query,"(platforms.name ~ *\"");
    strcat(platform_query,platform);
    strcat(platform_query,"\"* | platforms.abbreviation ~ *\"");
    strcat(platform_query,platform);
    strcat(platform_query,"\"*)");
  }

  char *where = malloc(256);
  strcpy(where,"where ");
  strcat(where,name_query);
  
  if(platform) {
    strcat(where," & ");
    strcat(where, platform_query);
  }
  
  strcat(where,"; sort rating desc; limit 10;");
  
  char *query = malloc(256);
  strcpy(query,fields);
  strcat(query,where);

  return query;  
}

int main(int argc, char *argv[]) {
  CURL *curl;
  CURLcode res;

  struct MemoryStruct chunk;
  chunk.memory = malloc(1);
  chunk.size = 0;

  curl_global_init(CURL_GLOBAL_ALL);
  curl = curl_easy_init();

  if(curl) {
    curl_easy_setopt(curl,CURLOPT_URL,construct_url("games"));
    //curl_easy_setopt(curl,CURLOPT_URL,"https://id.twitch.tv/oauth2/token?client_id=&client_secret=&grant_type=client_credentials");

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeMemoryCallback);
    curl_easy_setopt(curl,CURLOPT_WRITEDATA, (void *)&chunk);

    struct curl_slist *headers = NULL;
    
    headers = curl_slist_append(headers, "Client-ID: ");
    headers = curl_slist_append(headers, "Authorization: Bearer ");
    headers = curl_slist_append(headers, "Accept: application/json");
    
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    const char *query = construct_query("Rayman 2","N64");
    printf("%s\n",query);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, query);

    res = curl_easy_perform(curl);
    
    if(res != CURLE_OK) {
      printf("ERROR");
    }
    else {
      printf("%lu Bytes received\n",(unsigned long)chunk.size);
      printf("Result:\n%s\n",chunk.memory);
    }
  
    curl_easy_cleanup(curl);
    free(chunk.memory);
    }
  curl_global_cleanup();
  return 0;
}