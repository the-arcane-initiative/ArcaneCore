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
#include "arcanecore/base/arg/Action.hpp"

#include "arcanecore/base/Exceptions.hpp"
#include "arcanecore/base/arg/Parser.hpp"


namespace arc
{
ARC_BASE_VERSION_NS_BEGIN
namespace arg
{

//------------------------------------------------------------------------------
//                                  CONSTRUCTOR
//------------------------------------------------------------------------------

Action::Action(
        const deus::UnicodeView& key,
        const deus::UnicodeView& description)
    : m_parser_parent(nullptr)
    , m_key          (key)
    , m_description  (description)
{
    // empty long key?
    if(m_key.get_view().empty())
    {
        throw arc::ex::ValueError(
            "Flag cannot be constructed with an empty long key."
        );
    }
}

Action::Action(
        const deus::UnicodeView& key,
        const std::vector<std::string>& variable_names,
        const deus::UnicodeView& description)
{
    // empty long key?
    if(m_key.get_view().empty())
    {
        throw arc::ex::ValueError(
            "Flag cannot be constructed with an empty long key."
        );
    }

    // set variable names
    m_variable_names.reserve(variable_names.size());
    for(const std::string& var : variable_names)
    {
        m_variable_names.push_back(deus::UnicodeView(var));
    }
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

Action::~Action()
{
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

const deus::UnicodeView& Action::get_key() const
{
    return m_key.get_view();
}

const std::vector<deus::UnicodeStorage>& Action::get_variable_names() const
{
    return m_variable_names;
}

const deus::UnicodeView& Action::get_description() const
{
    return m_description.get_view();
}

//------------------------------------------------------------------------------
//                            PRIVATE MEMBER FUNCTIONS
//------------------------------------------------------------------------------

void Action::set_parser_parent(const Parser* parser_parent)
{
    m_parser_parent = parser_parent;
}

bool Action::parse(
        std::size_t argi,
        std::size_t argc,
        char** argv,
        std::size_t& out_increment,
        bool& out_exit_program,
        int& out_exit_code)
{
    // TODO: which encoding to use for Windows? Is there a way to detect this?
    deus::UnicodeView current(argv[argi], deus::Encoding::kUTF8);

    // match?
    if(current == m_key)
    {
        std::size_t increment_extra = 0;
        // TODO: parse sub actions and flags

        out_increment = 1 + increment_extra;
        return true;
    }
    // not a match
    return false;
}

} // namespace arg
ARC_BASE_VERSION_NS_END
} // namespace arc
