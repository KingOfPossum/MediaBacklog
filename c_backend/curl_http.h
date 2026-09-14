#ifndef CURL_HTTP_H
#define CURL_HTTP_H

struct MemoryStruct {
  char *memory;
  size_t size;
};

char *curl_make_igdb_request(char *url, char *query);

#endif