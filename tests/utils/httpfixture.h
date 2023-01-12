#pragma once

#include <unistd.h>
#include <fstream>
#include <gtest/gtest.h>
#include <httplib.h>
#include <statsig.h>

#include "commonfixture.h"

#ifdef TESTS_ROOT_DIR
#define TESTS_DIR TESTS_ROOT_DIR
#else
#define TESTS_DIR "/"
#endif

class HttpFixture : public CommonFixture
{
protected:
  httplib::Server server;
  std::string host = "localhost";
  int port = 8000;
  std::vector<statsig::Event> logEvents;
  HttpFixture() : CommonFixture()
  {
    this->options.localMode = false;
    this->options.api = "http://" + this->host + ":" + std::to_string(this->port);
  }
  void SetUp() override
  {
    server.Post("/v1/download_config_specs", [](const httplib::Request &req, httplib::Response &res)
                {
        std::string path = std::string(TESTS_DIR) + "/testdata/download_config_specs.json";
        std::ifstream file(path);
        nlohmann::json j = nlohmann::json::parse(file);
        res.body = j.dump();
        res.status = 200; });

    server.Post("/v1/log_event", [&](const httplib::Request &req, httplib::Response &res)
                { 
      nlohmann::json j = nlohmann::json::parse(req.body);
      auto events = j.at("events").get<std::vector<statsig::Event>>();
      this->logEvents.insert(this->logEvents.end(), events.begin(), events.end()); });

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
