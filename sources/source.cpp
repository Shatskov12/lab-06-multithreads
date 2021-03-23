// Copyright 2020 Your Name <your_email>
#include <header.hpp>

Hasher::Hasher(int argc, char **argv) {
  if (argc == 1) {
    NumberThreads = std::thread::hardware_concurrency();
  } else {
    NumberThreads = static_cast<unsigned int>(std::stoi(argv[1]));
  }
  Duration = 0;
}

void Hasher::Start(const bool& key) {
  boost::log::add_common_attributes();
  boost::log::add_console_log(std::clog, boost::log::keywords::format = "[%Severity%] %TimeStamp%: %Message%");

  boost::log::add_file_log
      (
          boost::log::keywords::file_name = "sample_%N.log",
          boost::log::keywords::rotation_size = 10 * 1024 * 1024,
          boost::log::keywords::time_based_rotation = boost::log::sinks::file::rotation_at_time_point(0, 0, 0),
          boost::log::keywords::format = "[%Severity%][%TimeStamp%]: %Message%"
      );
  for(unsigned int i = 0; i < NumberThreads; ++i){
    Threads.emplace_back(std::thread([&](){
      DoHashing(key);
    }));
  }
}

void Hasher::DoHashing(const bool& key) {
  auto StartTime = std::chrono::steady_clock::now();
  while(key){
    Mutex.lock();
    SrcStr = std::to_string(std::rand());
    HashHexStr = picosha2::hash256_hex_string(SrcStr);
    auto End = std::chrono::steady_clock::now();
    Duration += static_cast<int>(std::chrono::nanoseconds(End - StartTime).count());
    StartTime = End;
    SortHash(HashHexStr);
    Mutex.unlock();
    std::this_thread::sleep_for(std::chrono::microseconds(2));
  }
}

void Hasher::SortHash(string &hash) {
  if(HashHexStr.substr(60, 4) == HexEnd) {
    BOOST_LOG_SEV(Slg, info) << endl
                             << "source: " << std::hex << std::stol(SrcStr) << std::dec
                             << " hash: " << hash
                             << " duration: " << Duration
                             << " thread: " << std::this_thread::get_id() << endl;
    json j = {
        {"source", SrcStr},
        {"hash hex", HashHexStr},
        {"duration", Duration}
    };
    RightHashs.push_back(j);
  } else {
    BOOST_LOG_SEV(Slg, trace) << endl
                              << "source: " << std::hex << std::stol(SrcStr) << std::dec
                              << " hash: " << HashHexStr
                              << " thread: " << std::this_thread::get_id() << endl;
  }
}

Hasher::~Hasher() {
  for(unsigned int i = 0; i < NumberThreads; ++i) {
    Threads[i].join();
  }
}

vector<json> Hasher::RightHashs;
