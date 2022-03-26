#pragma once

#include "Base.h"
#include "Types/DeployApproval.h"
#include "Types/DeployHeader.h"
#include "Types/ExecutableDeployItem.h"
#include "Types/PublicKey.h"
#include "nlohmann/json.hpp"

namespace Casper {
/// <summary>
/// Header information of a Deploy.
/// </summary>
struct Deploy {
  // TODO: check with CEP57 Checksum for json serialization
  /// <summary>
  /// A hash over the header of the deploy.
  /// </summary>
  std::string hash;

  /// <summary>
  /// Contains metadata about the deploy.
  /// </summary>
  DeployHeader header;

  /// <summary>
  /// Contains the payment amount for the deploy.
  /// </summary>
  ExecutableDeployItem payment;

  /// <summary>
  /// Contains the session information for the deploy.
  /// </summary>
  ExecutableDeployItem session;

  /// <summary>
  /// List of signers and signatures for this Deploy.
  /// </summary>
  std::vector<DeployApproval> approvals;

  Deploy() {}
};

/**
 * @brief Construct a JSON object from a Deploy object.
 *
 * @param j JSON object to construct.
 * @param p Deploy object to construct from.
 */
inline void to_json(nlohmann::json& j, const Deploy& p) {
  j = nlohmann::json{{"hash", p.hash},
                     {"header", p.header},
                     {"payment", p.payment},
                     {"session", p.session},
                     {"approvals", p.approvals}};
}

/**
 * @brief Construct a Deploy object from a JSON object.
 *
 * @param j JSON object to construct the object from.
 * @param p Deploy object to construct.
 */
inline void from_json(const nlohmann::json& j, Deploy& p) {
  j.at("hash").get_to(p.hash);
  j.at("header").get_to(p.header);
  j.at("payment").get_to(p.payment);
  j.at("session").get_to(p.session);
  j.at("approvals").get_to(p.approvals);
}

}  // namespace Casper