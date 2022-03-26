#pragma once

#include "Base.h"
#include "Types/KeyAlgo.h"
#include "Utils/CEP57Checksum.h"
#include "cryptopp/hex.h"
#include "cryptopp/secblock.h"

using namespace CryptoPP;

namespace Casper {
/// <summary>
/// A wrapper for a cryptographic signature.
/// </summary>
struct Signature {
  /// <summary>
  /// Byte array without the Key algorithm identifier.
  /// </summary>
  SecByteBlock raw_bytes;

  /// <summary>
  /// The Key algorithm used to create the signature.
  /// </summary>
  KeyAlgo key_algorithm;

  Signature() {}

 protected:
  Signature(SecByteBlock raw_bytes_, KeyAlgo key_algorithm_)
      : raw_bytes(raw_bytes_), key_algorithm(key_algorithm_) {}

 public:
  static Signature FromHexString(std::string signature) {
    try {
      SecByteBlock rawBytes = CEP57Checksum::Decode(signature.substr(2));
      if (signature.substr(0, 2) == "01") {
        return FromRawBytes(rawBytes, KeyAlgo::ED25519);
      } else if (signature.substr(0, 2) == "02") {
        return FromRawBytes(rawBytes, KeyAlgo::SECP256K1);
      } else {
        throw std::invalid_argument("Invalid key algorithm identifier.");
      }
    } catch (const std::exception& e) {
      std::cout << "FromHexString() Exception: " << e.what() << '\n';
    }
  }

  /// <summary>
  /// Creates a PublicKey object from a byte array (containing the
  /// Key algorithm identifier).
  /// </summary>
  static Signature FromBytes(SecByteBlock bytes) {
    if (bytes.empty())
      throw std::invalid_argument("Signature bytes cannot be empty.");

    KeyAlgo algoIdent;

    if (bytes[0] == 0x01) {
      algoIdent = KeyAlgo::ED25519;
    } else if (algoIdent == 0x02) {
      algoIdent = KeyAlgo::SECP256K1;
    } else {
      throw std::invalid_argument("Wrong signature algorithm identifier");
    }

    SecByteBlock data_bytes(bytes.size() - 1);
    std::copy(bytes.begin() + 1, bytes.end(), data_bytes.begin());
    // TODO: copy check
    return Signature(data_bytes, algoIdent);
  }

  /// <summary>
  /// Creates a Signature object from a byte array and the key algorithm
  /// identifier.
  /// </summary>

  static Signature FromRawBytes(SecByteBlock rawBytes, KeyAlgo keyAlgo) {
    return Signature(rawBytes, keyAlgo);
  }

  /// <summary>
  /// Returns the bytes of the signature, including the Key algorithm as the
  /// first byte.
  /// </summary>

  SecByteBlock GetBytes() {
    SecByteBlock bytes = SecByteBlock(raw_bytes.size() + 1);
    if (key_algorithm == KeyAlgo::ED25519) {
      bytes[0] = 0x01;
    } else if (key_algorithm == KeyAlgo::SECP256K1) {
      bytes[0] = 0x02;
    } else {
      bytes[0] = 0x00;
    }
    std::copy(raw_bytes.begin(), raw_bytes.end(), bytes.begin() + 1);
    return bytes;
  }

  std::string ToHexString() const {
    if (key_algorithm == KeyAlgo::ED25519)
      return "01" + CEP57Checksum::Encode(raw_bytes);
    else
      return "02" + CEP57Checksum::Encode(raw_bytes);
  }

  std::string ToString() const { return ToHexString(); }
};

// to_json of Signature
inline void to_json(nlohmann::json& j, const Signature& s) {
  j = s.ToHexString();
}

// from_json of Signature
inline void from_json(const nlohmann::json& j, Signature& s) {
  std::string signatureStr = j.get<std::string>();
  SecByteBlock rawBytes = CEP57Checksum::Decode(signatureStr.substr(2));

  try {
    if (signatureStr.substr(0, 2) == "01") {
      s = Signature::FromRawBytes(rawBytes, KeyAlgo::ED25519);
    } else if (signatureStr.substr(0, 2) == "02") {
      s = Signature::FromRawBytes(rawBytes, KeyAlgo::SECP256K1);
    } else {
      throw std::invalid_argument("Invalid key algorithm identifier.");
    }
  } catch (const std::exception& e) {
    std::cout << "FromHexString() Exception: " << e.what() << '\n';
  }
}

}  // namespace Casper