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
#include <functional>
#include <string>

using LogHandler = std::function<void (std::string& message)> ;

namespace spdlog {
namespace sinks {
template<typename Mutex>
class customized_sink final : public base_sink<Mutex>
{
public:
    explicit customized_sink(LogHandler handler): 
		handler_(handler)
    {
    }

    customized_sink(const customized_sink &) = delete;
    customized_sink &operator=(const customized_sink &) = delete;

protected:
    void sink_it_(const details::log_msg &msg) override
    {
        fmt::memory_buffer formatted;
        sink::formatter_->format(msg, formatted);
        std::string message(formatted.data(), formatted.size());

        if (handler_){
            handler_(message);
        }
    }

    void flush_() override {}

private:
	LogHandler handler_;
};

using customized_sink_mt = customized_sink<std::mutex>;
using customized_sink_st = customized_sink<details::null_mutex>;

} // namespace sinks
} // namespace spdlog
