// Copyright 2020 Your Name <your_email>
#ifndef INCLUDE_HEADER_HPP_
#define INCLUDE_HEADER_HPP_
#include <iostream>
#include "../third-party/PicoSHA2/picosha2.h"
#include <string>
#include <chrono>
#include <thread>
#include <csignal>
#include <nlohmann/json.hpp>
#include <mutex>
#include <vector>
//#include <unistd.h>
#include <fstream>
#include <sstream>

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>


using std::cout;
using std::endl;
using std::string;
using std::vector;
using nlohmann::json;
using boost::shared_ptr;
using namespace boost::log::trivial;


static const string HexEnd = "0000";

class Hasher {
 public:
  Hasher(int argc, char *argv[]);

  ~Hasher();

  void static SignalCatch(int signum){
    WriteToJsonFile("hash hex log.json");
    sleep(1);
    cout << "\nProgram aborted with code " << --signum << "\n";
    exit(signum);
  }

  void static WriteToJsonFile(const string& FileName){
    std::ofstream OutputFile;
    std::ifstream InputFile;
    OutputFile.open(FileName);
    InputFile.open(FileName);
    json OutJson;
    if(InputFile.peek() != EOF)
      InputFile >> OutJson;
    for(auto & RightHash : RightHashs) {
      OutJson["values"].push_back(RightHash);
    }
    OutputFile << OutJson.dump(4);
    OutputFile.close();
  };

  void Start(const bool& key);

  void SortHash(string& hash);

  void DoHashing(const bool& key);
 private:
  std::mutex Mutex;
  unsigned int NumberThreads;
  long Duration;
  string SrcStr;
  string HashHexStr;
  vector<std::thread> Threads;
  static vector<json> RightHashs;
  boost::log::sources::severity_logger< severity_level > Slg;
};

#endif // INCLUDE_HEADER_HPP_
