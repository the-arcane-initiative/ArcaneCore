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
#include "arcanecore/base/arg/Parser.hpp"

#include <iostream>

#include "arcanecore/base/arg/Definition.hpp"


namespace arc
{
inline namespace ARC_BASE_VERSION_NS
{
namespace arg
{

//------------------------------------------------------------------------------
//                                  CONSTRUCTOR
//------------------------------------------------------------------------------

Parser::Parser(int error_exit_code)
    : m_error_exit_code(error_exit_code)
{
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

Parser::~Parser()
{
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

int Parser::execute(int argc, char** argv)
{
    // iterate arguments
    std::size_t i = 1;
    while(i < static_cast<std::size_t>(argc))
    {
        // iterate definitions
        bool matched = false;
        for(std::unique_ptr<arc::arg::Definition>& def : m_defs)
        {
            bool exit_program = false;
            int return_code = 0;
            std::size_t increment = 1;
            if(def->check(i, argc, argv, increment, exit_program, return_code))
            {
                // exit?
                if(exit_program)
                {
                    return return_code;
                }

                // add to the execute list and increment
                m_execute.push_back(def.get());
                i += increment;
                matched = true;
                break;
            }
        }
        if(matched)
        {
            continue;
        }

        // TODO: which encoding to use for Windows? Is there a way to detect
        //       this?
        // unrecognized argument
        deus::UnicodeView current(argv[i], deus::Encoding::kUTF8);
        std::cerr
            << "Unrecognised command line argument: \'" << current << "\'."
            << "\nUse \'--help\' or \'-h\' for program help." << std::endl;
        return m_error_exit_code;

        ++i;
    }

    // execute definition
    for(arc::arg::Definition* def : m_execute)
    {
        int return_code = 0;
        if(!def->execute(return_code))
        {
            return return_code;
        }
    }

    // done
    return 0;
}

void Parser::add_definition(arc::arg::Definition* def)
{
    m_defs.emplace_back(def);
}

} // namespace arg
} // namespace ARC_BASE_VERSION_NS
} // namespace arc
