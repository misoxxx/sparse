#pragma once

#include <map>
#include <nlohmann/json.hpp>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

namespace sparse::common
{
/**
 * @brief Parsed title of the certificate.
 */
using title_t = std::string;

/**
 * @brief Parsed versions of the certificate.
 */
struct versions_t
{
    std::vector<std::string> eal;             ///< versions of eal
    std::vector<std::string> global_platform; ///< versions of global platform
    std::vector<std::string> java_card;       ///< versions of java card
    std::vector<std::string> sha;             ///< versions of sha
    std::vector<std::string> rsa;             ///< versions of rsa
    std::vector<std::string> ecc;             ///< versions of ecc
    std::vector<std::string> des;             ///< versions of des
};

/**
 * @brief to_json
 * @param j json
 * @param versions versions object
 */

inline void to_json(nlohmann::json& j, const versions_t& versions)
{
    const auto add_not_empty = [&j](const std::string& key, const std::vector<std::string>& value) {
        if (!value.empty())
        {
            j[key] = value;
        }
    };

    add_not_empty("eal", versions.eal);
    add_not_empty("global_platform", versions.global_platform);
    add_not_empty("java_card", versions.java_card);
    add_not_empty("sha", versions.sha);
    add_not_empty("rsa", versions.rsa);
    add_not_empty("ecc", versions.ecc);
    add_not_empty("des", versions.des);
}


/**
 * @brief Parsed section of the certificate.
 */
struct section_t
{
    std::string id;       ///< id
    std::string name;     ///< name
    uint32_t page_number; ///< page number of the beginning of the section
};

/**
 * @brief Parsed table of contents of the certificate
 */
using table_of_contents_t = std::vector<section_t>;

/**
 * @brief Parsed revision of the certificate.
 */
struct revision_t
{
    std::string version;     ///< version
    std::string date;        ///< date
    std::string description; ///< description
};

/**
 * @brief Parsed revisions of the certificate.
 */
using revisions_t = std::vector<revision_t>;

/**
 * @brief Parsed bibliography of the certificate.
 */
using bibliography_t = std::unordered_map<std::string, std::string>;

/**
 * @brief Parsed section other of the certificate.
 */
using other_t = std::map<std::string, std::string>;
} // namespace sparse::common
