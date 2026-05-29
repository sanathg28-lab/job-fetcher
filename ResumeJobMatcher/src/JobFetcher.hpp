#pragma once
#include <string>
#include "ApiResponse.hpp"
#include <climits>


class JobFetcher {
public:
    ApiResponse fetchJobs(const std::string& company);
};
