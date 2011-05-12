#include <stdio.h>
#include <string.h>
#include <curl/curl.h>

size_t write(void *ptr, size_t size, size_t nmemb, FILE *stream) {
	return fwrite(ptr, size, nmemb, stream);
}

int wget(char *in, char *out) {
	CURL *curl;
	CURLcode res;
	FILE *fp;

	if(!(curl = curl_easy_init()))
		return -1;

	if(!(fp = fopen(out, "wb")))	// Open in binary
		return -1;

	// Set the curl easy options
	curl_easy_setopt(curl, CURLOPT_URL, in);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

	res = curl_easy_perform(curl);	// Perform the download and write

	curl_easy_cleanup(curl);
	fclose(fp);
	return res;
}

int main(int args, char *argc[]) {
	if(args != 3) {
		printf("%s url file\n", argc[0]);
		return -1;
	}

	if(wget(argc[1], argc[2]) != 0)
		printf("An error occured!\n");

	return 0;
}
