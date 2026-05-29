//
//  ui.hpp
//  ResumeJobMatcher
//
//  Created by Sanath on 5/22/26.
//

#ifndef JobData_hpp
#define JobData_hpp
#pragma once
#include <string>

struct JobData {
    std::string title;
    std::string jobUrl;
    std::string location;
    std::string department;
    std::string updatedAt;
    long jobId;
};


#endif // !JobData_hpp
