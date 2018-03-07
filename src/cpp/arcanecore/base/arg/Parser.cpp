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

#include "arcanecore/base/Exceptions.hpp"
#include "arcanecore/base/arg/Action.hpp"
#include "arcanecore/base/arg/Flag.hpp"


namespace arc
{
ARC_BASE_VERSION_NS_BEGIN
namespace arg
{

//------------------------------------------------------------------------------
//                                  CONSTRUCTOR
//------------------------------------------------------------------------------

Parser::Parser(int error_exit_code)
    : m_executing      (false)
    , m_error_exit_code(error_exit_code)
    , m_action_execute (nullptr)
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
    m_executing = true;

    // iterate arguments
    std::size_t i = 1;
    while(i < static_cast<std::size_t>(argc))
    {
        // parse actions on the first iteration
        if(i == 1)
        {
            bool matched = false;
            for(std::unique_ptr<arc::arg::Action>& action : m_actions)
            {
                std::size_t increment = 1;
                bool exit_program = false;
                int return_code = 0;
                matched = action->parse(
                    i,
                    argc,
                    argv,
                    increment,
                    exit_program,
                    return_code
                );
                if(matched)
                {
                    // exit?
                    if(exit_program)
                    {
                        return return_code;
                    }

                    // queue for execution and increment
                    m_action_execute = action.get();
                    i += increment;
                    break;
                }
            }

            if(matched)
            {
                continue;
            }
        }

        // parse flags
        bool matched = false;
        for(std::unique_ptr<arc::arg::Flag>& flag : m_flags)
        {
            std::size_t increment = 1;
            bool exit_program = false;
            int return_code = 0;
            matched = flag->parse(
                i,
                argc,
                argv,
                increment,
                exit_program,
                return_code
            );
            if(matched)
            {
                // exit?
                if(exit_program)
                {
                    return return_code;
                }

                // queue for execution and increment
                m_flags_execute.push_back(flag.get());
                i += increment;
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
    }

    // nothing to do?
    if(m_action_execute == nullptr && m_flags_execute.empty())
    {
        std::cerr
            << "No command line arguments supplied.\nUse \'--help\' or \'-h\' "
            "for program help." << std::endl;
        return 0;
    }

    // execute action?
    if(m_action_execute != nullptr)
    {
        int exit_code = m_error_exit_code;
        if(!m_action_execute->execute(exit_code))
        {
            return exit_code;
        }
    }

    // execute flags
    for(arc::arg::Flag* flag : m_flags_execute)
    {
        int exit_code = m_error_exit_code;
        if(!flag->execute(exit_code))
        {
            return exit_code;
        }
    }

    // done
    return 0;
}

const std::list<std::unique_ptr<arc::arg::Action>>& Parser::get_actions() const
{
    return m_actions;
}

void Parser::add_action(arc::arg::Action* action)
{
    if(m_executing)
    {
        throw arc::ex::StateError(
            "Command line action (" + action->get_key() + ") cannot be added "
            "to parser during parser execution."
        );
    }

    action->set_parser_parent(this);
    m_actions.emplace_back(action);
}

const std::list<std::unique_ptr<arc::arg::Flag>>& Parser::get_flags() const
{
    return m_flags;
}

void Parser::add_flag(arc::arg::Flag* flag)
{
    if(m_executing)
    {
        throw arc::ex::StateError(
            "Command line flag (" + flag->get_long_key() + ") cannot be added "
            "to parser during parser execution."
        );
    }

    flag->set_parser_parent(this);
    m_flags.emplace_back(flag);
}

} // namespace arg
ARC_BASE_VERSION_NS_END
} // namespace arc
