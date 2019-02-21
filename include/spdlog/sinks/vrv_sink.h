//
// Copyright(c) 2015 Gabi Melman.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)
//

#pragma once

#ifndef SPDLOG_H
#include "spdlog/spdlog.h"
#endif

#include "spdlog/details/null_mutex.h"
#include "spdlog/sinks/base_sink.h"

#include <mutex>
#include <ostream>

#include "curl_client.h"

namespace spdlog {
namespace sinks {
template<typename Mutex>
class vrv_sink final : public base_sink<Mutex>
{
public:
    explicit vrv_sink(std::string &url, std::string &name, CURLClient *client)
        : url_(url)
        , name_(name)
        , client_(client)
    {
    }
    vrv_sink(const vrv_sink &) = delete;
    vrv_sink &operator=(const vrv_sink &) = delete;

protected:
    void sink_it_(const details::log_msg &msg) override
    {
        fmt::memory_buffer formatted;
        sink::formatter_->format(msg, formatted);
        std::string url = url_ + "name=";
        url += name_;
        std::string message(formatted.data(), formatted.size());

        if (client_)
        {
            client_->AddHttpTask(this, url, message);
        }
    }

    void flush_() override {}

    std::string url_;
    std::string name_;
    CURLClient *client_;
};

using vrv_sink_mt = vrv_sink<std::mutex>;
using vrv_sink_st = vrv_sink<details::null_mutex>;

} // namespace sinks
} // namespace spdlog
