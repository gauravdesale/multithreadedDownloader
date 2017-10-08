#include <curlpp/Easy.hpp>//this the curl command that makes it possible to copy urls 
#include <curlpp/Options.hpp>
#include <curlpp/cURLpp.hpp>

#include <iostream>
#include <fstream>
#include <string>

#include <thread>//here is the string library


using namespace std::literals;//standard thread functions

namespace {
    void Download(const std::string& url, const std::string& filename) {//this function is the functionality
        std::ofstream of(filename);//read from the url

        cURLpp::Easy req;//make the req object from the easy and this makes an easy request
        req.setOpt(cURLpp::Options::Url(url));//setting the preferrences for the set opt function from the req object 
        req.setOpt(cURLpp::Options::NoProgress(false));
        req.setOpt(cURLpp::Options::FollowLocation(true));//where is the actuall downloading happening?
        req.setOpt(cURLpp::Options::ProgressFunction([&](std::size_t total, std::size_t done, auto...) {//use a lambda function for temporary usage 
            std::cout << "\r" << done << " of " << total << " bytes downloaded (" << int(total ? done*100./total : 0) << "%)" << std::flush;//output in a certain format
            return 0;
        }));
        req.setOpt(cURLpp::Options::WriteFunction([&](const char* p, std::size_t size, std::size_t nmemb) {//another temporary function that is writing the preferrences 
            of.write(p, size*nmemb);
            return size*nmemb;
        }));
        req.perform();//implement the thread and all the functionality we defined
    }
}


int main() {
    Download("http://i.imgur.com/kcM5x79.jpg"s, "two.jpg"s);
}
