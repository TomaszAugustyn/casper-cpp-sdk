#pragma once

#include "Base.h"
#include "Definitions.h"

#include "Types/ExecutionEffect.h"
#include "nlohmann/json.hpp"

namespace Casper {

struct ExecutionResultFailure {
  ExecutionEffect effect;
  // TODO: Make TransferKey instead of std::string
  std::vector<std::string> transfers;
  big_int cost;
  std::string error_message;

  ExecutionResultFailure() {}
};

// to_json of ExecutionResultFailure
inline void to_json(nlohmann::json& j, const ExecutionResultFailure& p) {
  j = nlohmann::json{{"effect", p.effect},
                     {"transfers", p.transfers},
                     {"cost", p.cost.toString()},
                     {"error_message", p.error_message}};
}

// from_json of ExecutionResultFailure
inline void from_json(const nlohmann::json& j, ExecutionResultFailure& p) {
  j.at("effect").get_to(p.effect);
  j.at("transfers").get_to(p.transfers);
  j.at("cost").get_to(p.cost);
  j.at("error_message").get_to(p.error_message);
}

struct ExecutionResultSuccess {
  ExecutionEffect effect;
  // TODO: Make TransferKey instead of std::string
  std::vector<std::string> transfers;
  big_int cost;

  ExecutionResultSuccess() {}
};

// to_json of ExecutionResultSuccess
inline void to_json(nlohmann::json& j, const ExecutionResultSuccess& p) {
  j = nlohmann::json{{"effect", p.effect},
                     {"transfers", p.transfers},
                     {"cost", p.cost.toString()}};
}

// from_json of ExecutionResultSuccess
inline void from_json(const nlohmann::json& j, ExecutionResultSuccess& p) {
  j.at("effect").get_to(p.effect);
  j.at("transfers").get_to(p.transfers);
  j.at("cost").get_to(p.cost);
}

struct ExecutionResult {
  std::optional<ExecutionResultFailure> failure = std::nullopt;
  std::optional<ExecutionResultSuccess> success = std::nullopt;
  std::string block_hash;

  ExecutionResult() {}
};

// to_json of ExecutionResult
inline void to_json(nlohmann::json& j, const ExecutionResult& p) {
  nlohmann::json result;
  if (p.failure.has_value()) {
    result = nlohmann::json{{"Failure", p.failure.value()}};
  } else if (p.success.has_value()) {
    result = nlohmann::json{{"Success", p.success.value()}};
  }
  j = nlohmann::json{{"result", result}, {"block_hash", p.block_hash}};
}

// from_json of ExecutionResult
inline void from_json(const nlohmann::json& j, ExecutionResult& p) {
  if (j.at("result").find("Success") != j.at("result").end()) {
    p.success = j.at("result").at("Success").get<ExecutionResultSuccess>();
  } else if (j.at("result").find("Failure") != j.at("result").end()) {
    p.failure = j.at("result").at("Failure").get<ExecutionResultFailure>();
  }

  j.at("block_hash").get_to(p.block_hash);
}

}  // namespace Casper
