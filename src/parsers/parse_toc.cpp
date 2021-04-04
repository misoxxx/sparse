//
// Created by a on 4/2/2021.
//
#include <optional>
#include <regex>
#include <sparse/common/types.hpp>
#include <sparse/parsers/parsers.hpp>
#include <string>



/*
 * Files with low points:
 *  - 0782V5b_pdf.txt
 *      - this file's expected json contains also subsections, which are not in the table of content
 *  - 0879V4a_pdf.json
 *      - this file contains also letters in section numbers
 *      - also, the section numbers are not ended with '.' in the expected json
 *  - 0939V3b_pdf.json
 *      - toc is made from two columns
 *  -
 *
 *
 */








namespace {
const std::regex toc_beginning(".*contents.*", std::regex_constants::icase);
const std::regex toc_split(R"(^\s*([0-9\.]+)\s*(.+[^\.\s])[\.\s]+([0-9]+)\s*$)");


std::vector<std::size_t> find_possible_toc_beginnings(const std::string& s) noexcept
{
    std::vector<std::size_t> possible_toc_beginnings;

    for (auto iter = std::sregex_iterator(s.begin(), s.end(), toc_beginning); iter != std::sregex_iterator(); ++iter)
    {
        possible_toc_beginnings.push_back(static_cast<std::size_t>(iter->position(0)));
    }
    return possible_toc_beginnings;
}

sparse::common::table_of_contents_t get_toc_beginning_at(const std::string& s, std::size_t beg) noexcept
{
    constexpr int max_length_of_toc = 10;

    std::size_t current_position;
    std::size_t previous_position = beg;
    sparse::common::table_of_contents_t table_of_contents;

    for (auto lines_since_last_toc_entry = 0; lines_since_last_toc_entry < max_length_of_toc &&
                                                       (current_position = s.find('\n', previous_position + 1)) != std::string::npos;
         ++lines_since_last_toc_entry)
    {
        const auto line = s.substr(previous_position, current_position - previous_position);

        std::smatch split_matches;
        std::regex_match(line, split_matches, toc_split);

        if(split_matches.size() >= 1){
            sparse::common::section_t section;
            section.id = split_matches[1];
            section.name = split_matches[2];
            section.page_number = stoi(split_matches[3]);
            table_of_contents.push_back(section);
            lines_since_last_toc_entry = 0;
        }
        previous_position = current_position;
    }
    return table_of_contents;
}

}

namespace sparse::parsers
{
std::optional<common::table_of_contents_t> parse_toc(const std::string& whole_file) noexcept {
    const auto possible_toc = find_possible_toc_beginnings(whole_file);

    common::table_of_contents_t biggest_toc;


    for (const auto i : possible_toc)
    {
        /*
         * Usually there are few false positives - for example if text bibliography is in table of contents
         * In the case there is more of them, we try to parse each one
         * And take the one that contains most entries, as it is the one that is most likely to not be a false positive
         */
        const auto possible_toc = get_toc_beginning_at(whole_file, i);
        if (possible_toc.size() > biggest_toc.size())
        {
            biggest_toc = possible_toc;
        }
    }

    if (biggest_toc.empty())
    {
        return {};
    }

    return biggest_toc;
}
}