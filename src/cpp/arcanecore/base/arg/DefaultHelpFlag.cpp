/*!
 * \copyright Copyright (c) 2018, The Arcane Initiative
 *            All rights reserved.
 *
 * \license BSD 3-Clause License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#include "arcanecore/base/arg/DefaultHelpFlag.hpp"

#include <iostream>

#include "arcanecore/base/arg/Action.hpp"
#include "arcanecore/base/arg/Parser.hpp"


namespace arc
{
ARC_BASE_VERSION_NS_BEGIN
namespace arg
{

//------------------------------------------------------------------------------
//                                  CONSTRUCTOR
//------------------------------------------------------------------------------

DefaultHelpFlag::DefaultHelpFlag(const deus::UnicodeView& usage_text)
    : arc::arg::Flag("help", "h", "Displays this help text.")
    , m_usage_text  (usage_text)
{
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

DefaultHelpFlag::~DefaultHelpFlag()
{
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

bool DefaultHelpFlag::parse_extra(
        std::size_t argi,
        std::size_t argc,
        char** argv,
        std::size_t& out_increment,
        int& out_exit_code)
{
    // we sneakily run functionality in the parse method - this because we
    // want the help text to display immediately and then exit the
    // application

    // TODO: use ANSI -- need to learn to do this on Windows

    // constants
    static const std::size_t TAB_SIZE = 4;

    // generate the divider string
    deus::UnicodeStorage dash("-");
    deus::UnicodeStorage divider = dash * 80;
    std::cout << divider << std::endl;

    deus::UnicodeStorage space(" ");
    deus::UnicodeStorage tab = space * 4;

    // print usage?
    if(!m_usage_text.get_view().empty())
    {
        std::cout << "Usage:\n\n" << tab << m_usage_text << "\n" << std::endl;
        std::cout << divider << std::endl;
    }

    // print actions?
    const std::list<std::unique_ptr<arc::arg::Action>>& actions =
        m_parser_parent->get_actions();
    if(!actions.empty())
    {
        std::cout << "Actions:\n" << std::endl;

        // build and find longest action key
        std::vector<deus::UnicodeStorage> keys;
        keys.reserve(actions.size());
        std::size_t longest_key = 0;
        for(const std::unique_ptr<arc::arg::Action>& action : actions)
        {
            deus::UnicodeStorage key = tab + action->get_key();
            // add the variable names to the key
            for(const deus::UnicodeStorage& var : action->get_variable_names())
            {
                key = key + " <" + var + ">";
            }

            keys.push_back(key);

            if(key.get_view().length() > longest_key)
            {
                longest_key = key.get_view().length();
            }
        }
        // find how far descriptions should be indented
        std::size_t description_indent = (longest_key + 1) % TAB_SIZE;
        description_indent = longest_key + (TAB_SIZE - description_indent);
        // if there's not enough characters for the descriptions, surpass 80
        // characters and add more
        std::size_t max_chars = 80;
        if((max_chars - description_indent) < 20)
        {
            max_chars = description_indent + 20;
        }

        // actually write actions
        std::size_t offset = 0;
        for(const std::unique_ptr<arc::arg::Action>& action : actions)
        {
            //  build the string for this action
            // deus::UnicodeStorage s = keys[offset];s
            deus::UnicodeStorage s = keys[offset];
            deus::UnicodeStorage indent(" ");
            indent =
                indent *
                static_cast<int32_t>(
                    description_indent -
                    s.get_view().length()
                );
            s = s + indent.get_view() + ":: " + action->get_description();
            ++offset;

            // TOOD: need to word wrap the description still
            std::cout << s << "\n" << std::endl;
        }
        std::cout << divider << std::endl;
    }

    // print flags
    const std::list<std::unique_ptr<arc::arg::Flag>>& flags =
        m_parser_parent->get_flags();
    if(!flags.empty())
    {
        std::cout << "Flags:\n" << std::endl;

        // build and find longest flag key
        std::vector<deus::UnicodeStorage> keys;
        keys.reserve(flags.size());
        std::size_t longest_key = 0;
        for(const std::unique_ptr<arc::arg::Flag>& flag : flags)
        {
            deus::UnicodeStorage key = tab;
            if(!flag->get_short_key().empty())
            {
                key = key + flag->get_short_key() + ", ";
            }
            key = key + flag->get_long_key();
            // add the variable names to the key
            for(const deus::UnicodeStorage& var : flag->get_variable_names())
            {
                key = key + " <" + var + ">";
            }
            keys.push_back(key);

            if(key.get_view().length() > longest_key)
            {
                longest_key = key.get_view().length();
            }
        }
        // find how far descriptions should be indented
        std::size_t description_indent = (longest_key + 1) % TAB_SIZE;
        description_indent = longest_key + (TAB_SIZE - description_indent);
        // if there's not enough characters for the descriptions, surpass 80
        // characters and add more
        std::size_t max_chars = 80;
        if((max_chars - description_indent) < 20)
        {
            max_chars = description_indent + 20;
        }

        // actually write flags
        std::size_t offset = 0;
        for(const std::unique_ptr<arc::arg::Flag>& flag : flags)
        {
            //  build the string for this action
            deus::UnicodeStorage s = keys[offset];
            deus::UnicodeStorage indent(" ");
            indent =
                indent *
                static_cast<int32_t>(
                    description_indent -
                    s.get_view().length()
                );
            s = s + indent.get_view() + ":: " + flag->get_description();
            ++offset;

            // TOOD: need to word wrap the description still
            std::cout << s << "\n" << std::endl;
        }

        std::cout << divider << std::endl;
    }

    // exit successfully
    out_exit_code = 0;
    return false;
}

bool DefaultHelpFlag::execute(int& out_exit_code)
{
    // do nothing - the program has already exited.
    return true;
}

} // namespace arg
ARC_BASE_VERSION_NS_END
} // namespace arc
