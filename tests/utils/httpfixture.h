#pragma once

#include <unistd.h>
#include <fstream>
#include <gtest/gtest.h>
#include <httplib.h>

#include "commonfixture.h"

#ifdef TESTS_ROOT_DIR
#define TESTS_DIR TESTS_ROOT_DIR
#else
#define TESTS_DIR "/"
#endif

struct Counter
{
  int downloadConfigSpecs;
  int logEvent;
};

class HttpFixture : public CommonFixture
{
public:
  const int TIME_BUFFER = 10; // MS buffer for requests
protected:
  httplib::Server server;
  std::string host;
  int port;
  std::vector<statsig::Event> logEvents;
  Counter counter;
  std::string dcsFilePath;
  HttpFixture() : CommonFixture()
  {
    this->host = "localhost";
    this->port = 8000;
    this->options.localMode = false;
    this->options.api = "http://" + this->host + ":" + std::to_string(this->port);
    this->logEvents = std::vector<statsig::Event>();
    this->counter = Counter{};
    this->dcsFilePath = "/testdata/download_config_specs.json";
  }
  void SetUp() override
  {
    server.Post("/v1/download_config_specs", [this](const httplib::Request &req, httplib::Response &res)
                {
        std::string path = std::string(TESTS_DIR) + this->dcsFilePath;
        std::ifstream file(path);
        nlohmann::json j = nlohmann::json::parse(file);
        res.body = j.dump();
        res.status = 200; 
        this->counter.downloadConfigSpecs ++; });

    server.Post("/v1/log_event", [this](const httplib::Request &req, httplib::Response &res)
                { 
      nlohmann::json j = nlohmann::json::parse(req.body);
      auto events = j.at("events").get<std::vector<statsig::Event>>();
      this->logEvents.insert(this->logEvents.end(), events.begin(), events.end()); 
      this->counter.logEvent ++; });

    statsig::Utils::spawnBackgroundThread([this]()
                                          { server.listen(this->host, this->port); });
    CommonFixture::SetUp();
  }
  void TearDown() override
  {
    CommonFixture::TearDown();
    server.stop();
  }
};
