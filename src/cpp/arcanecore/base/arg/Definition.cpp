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
#include "arcanecore/base/arg/Definition.hpp"


namespace arc
{
inline namespace ARC_BASE_VERSION_NS
{
namespace arg
{

//------------------------------------------------------------------------------
//                                  CONSTRUCTORS
//------------------------------------------------------------------------------

Definition::Definition(
        const deus::UnicodeView& long_key,
        const deus::UnicodeView& short_key,
        const deus::UnicodeView& description)
    : m_long_key    ("--" + long_key)
    , m_short_key   ("-" + short_key)
    , m_description (description)
{
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

Definition::~Definition()
{
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

const deus::UnicodeView& Definition::get_long_key() const
{
    return m_long_key.get_view();
}

const deus::UnicodeView& Definition::get_short_key() const
{
    return m_short_key.get_view();
}

const deus::UnicodeView& Definition::get_description() const
{
    return m_description.get_view();
}

bool Definition::parse(
        std::size_t argi,
        std::size_t argc,
        char** argv,
        std::size_t& increment,
        int& exit_code)
{
    // no extra parsing needed by default
    return true;
}

//------------------------------------------------------------------------------
//                            PRIVATE MEMBER FUNCTIONS
//------------------------------------------------------------------------------

bool Definition::check(
        std::size_t argi,
        std::size_t argc,
        char** argv,
        std::size_t& increment,
        bool& exit_program,
        int& exit_code)
{
    // TODO: which encoding to use for Windows? Is there a way to detect this?
    deus::UnicodeView current(argv[argi], deus::Encoding::kUTF8);

    // long key match?
    if(current == m_long_key || current == m_short_key)
    {
        // perform any extra parsing
        std::size_t increment_extra = 0;
        if(!parse(argi + 1, argc, argv, increment_extra, exit_code))
        {
            // parsing failed
            exit_program = true;
            return true;
        }

        increment = 1 + increment_extra;
        return true;
    }
    // not a match
    return false;
}

} // namespace arg
} // namespace ARC_BASE_VERSION_NS
} // namespace arc
