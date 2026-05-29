#include <climits>
#include "JobFetcher.hpp"
#include "ApiResponse.hpp"
#include "JobData.hpp"
#include <string>
#include <curl/curl.h>

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s) {
    s->append((char*)contents, size * nmemb);
    return size * nmemb;
}

ApiResponse JobFetcher::fetchJobs(const std::string& company) {

    ApiResponse result;
    result.status_code = 0; // default

    std::string url = "https://boards-api.greenhouse.io/v1/boards/" + company + "/jobs";

    CURL* curl = curl_easy_init();
    std::string response = std::string(result.body);
    
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        CURLcode res = curl_easy_perform(curl);

        if (res == CURLE_OK) {
            long http_code = 0;
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

            result.status_code = (int)http_code;
            result.body = response;
        }

        curl_easy_cleanup(curl);
    }

    return result;
}
