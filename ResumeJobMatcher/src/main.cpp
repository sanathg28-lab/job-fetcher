#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <nlohmann/json.hpp>
#include "JobFetcher.hpp"

using json = nlohmann::json;

int main() {

    JobFetcher fetcher;

    std::vector<std::string> companies = {
        "stripe",
        "airbnb",
        "hubspot",
        "datadog"
    };

    std::ofstream csvFile("job_results.csv");

    if (!csvFile.is_open()) {
        std::cerr << "Failed to create CSV file.\n";
        return 1;
    }

    csvFile << "Title,Job URL,Location,Compensation,Company ID,Skills\n";

    for (const auto& company : companies) {

        try {

            ApiResponse result =
                fetcher.fetchJobs(company);

            json root =
                json::parse(result.body);

            if (!root.contains("jobs")) {
                std::cout << "No jobs found for "
                          << company << std::endl;
                continue;
            }

            for (const auto& job : root["jobs"]) {

                // Title
                std::string title =
                    job.value("title", "N/A");

                // URL
                std::string url =
                    job.value("url", "N/A");

                // Compensation
                std::string compensation =
                    job.value("compensation",
                              "Not Available");

                // Company ID
                std::string companyId =
                    std::to_string(
                        job.value("id", 0)
                    );

                // Location
                std::string location = "N/A";

                if (job.contains("location")) {

                    if (job["location"].is_string()) {

                        location =
                            job["location"];

                    }
                    else if (job["location"].is_object()) {

                        location =
                            job["location"]
                                .value("name", "N/A");
                    }
                }

                // Skills
                std::string skills = "N/A";

                if (job.contains("skills") &&
                    job["skills"].is_array()) {

                    skills.clear();

                    for (const auto& skill :
                         job["skills"]) {

                        skills +=
                            skill.get<std::string>() + " ";
                    }
                }

                // Write to CSV
                csvFile
                    << '"' << title << '"' << ","
                    << '"' << url << '"' << ","
                    << '"' << location << '"' << ","
                    << '"' << compensation << '"' << ","
                    << '"' << companyId << '"' << ","
                    << '"' << skills << '"'
                    << "\n";
            }

            std::cout << "Processed company: "
                      << company << std::endl;
        }

        catch (const std::exception& e) {

            std::cerr
                << "Error processing "
                << company
                << ": "
                << e.what()
                << std::endl;
        }
    }

    csvFile.close();

    std::cout
        << "job_results.csv created successfully.\n";

    return 0;
}
