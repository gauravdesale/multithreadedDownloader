#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/cURLpp.hpp>

#include <iostream>
#include <fstream>
#include <string>

#include <thread>


using namespace std::literals;//using just the literals usage from standard library

namespace {
    unsigned ln = 1;
    auto Color(const std::string& s) {//a method that returns anything 
        return "\x1B[32m" + s + ": " + "\x1B[0m";//returning some stuff that i dont get
    }

    auto Line(int l) {
        int m = l - ln;
        ln = l;
        return "\r"+(m<0?"\33["+std::to_string(-m)+'A':std::string(m,'\n'));//just returning status and stuff from the file to be used later 
    }

    std::string getURLFile(const std::string& url) {//getting the url using the curl functionality we imported earlier
        const auto pos = url.find_last_of("/");//trying to see where the last position of the / symbol in the url was
        return pos == std::string::npos ? url : url.substr(pos+1);//checking if the position of / was the same as npos and if so url is correct and if not modify url a bit
    }

    void Download(const std::string& url, unsigned line) {
        std::ofstream of(getURLFile(url));//main download functionality 

        cURLpp::Easy req;//following setup for displaying the progress
        req.setOpt(cURLpp::Options::Url(url));
        req.setOpt(cURLpp::Options::NoProgress(false));//setting up stuff like last time 
        req.setOpt(cURLpp::Options::FollowLocation(true));
        req.setOpt(cURLpp::Options::ProgressFunction([&](std::size_t total, std::size_t done, auto...) {
            std::cout << Line(line) << Color(getURLFile(url)) << done << " of " << total << " bytes downloaded (" << int(total ? done*100./total : 0) << "%)" << std::flush;//displaying progress
            return 0;
        }));
        req.setOpt(cURLpp::Options::WriteFunction([&](const char* p, std::size_t size, std::size_t nmemb) {//and writing to the file if needed too once it has been downloaded
            of.write(p, size*nmemb);//giving file back to the user
            return size*nmemb;
        }));
        req.perform();
    }
}


int main() {
    cURLpp::initialize();
    unsigned line = 1;

    for(const auto& p: {"http://i.imgur.com/Wt6xNSA.jpg"s,//testing the downloader
                        "http://i.imgur.com/RxfpuNO.jpg"s,
                        "http://i.imgur.com/TTGRX5D.png"s,
                        "http://i.imgur.com/LCdmRya.png"s}) {
        Download(p, line++);
    }
}
