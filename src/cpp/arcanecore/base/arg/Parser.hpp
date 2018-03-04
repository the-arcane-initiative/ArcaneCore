/*!
 * \file
 * \author David Saxon
 *
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
#ifndef ARCANECORE_BASE_ARG_PARSER_HPP_
#define ARCANECORE_BASE_ARG_PARSER_HPP_

#include <memory>
#include <list>

#include "arcanecore/base/BaseAPI.hpp"
#include "arcanecore/base/lang/Restrictors.hpp"


namespace arc
{
inline namespace ARC_BASE_VERSION_NS
{
namespace arg
{

//------------------------------------------------------------------------------
//                              FORWARD DECLARATIONS
//------------------------------------------------------------------------------

class Definition;

/*!
 * \brief The arc::arg::Parser is used to parse command line arguments and
 *        execute actions based on them.
 *
 * Argument definitions are defined via arc::arg::Definiton objects which are
 * added to the Parser via arc::arg::Parser::add_definition().
 */
class Parser
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTOR
    //--------------------------------------------------------------------------

    /*!
     * \brief Constructs a new Parser object.
     *
     * \param error_exit_code Option parameter that defines the default exit
     *                        code that will be used when an error is
     *                        encountered.
     */
    Parser(int error_exit_code = 1);

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    ~Parser();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Runs the command line argument parser.
     *
     * This function returns once all arguments have been parsed and all actions
     * executed.
     *
     * \param argc The number of command line arguments in argv.
     * \param argv The command line arguments (the first argument should be the
     *             name of the application).
     *
     * \return The exit code.
     */
    int execute(int argc, char** argv);

    /*!
     * \brief Adds an argument definition to the parser.
     *
     * \note The Parser will take ownership of the definition pointer.
     *
     * \throws arc::ex::StateError If this function is called during
     *                             arc::arg::Parser::execute().
     */
    void add_definition(arc::arg::Definition* def);

private:

    //--------------------------------------------------------------------------
    //                             PRIVATE ATTRIBUTES
    //--------------------------------------------------------------------------

    int m_error_exit_code;

    std::list<std::unique_ptr<arc::arg::Definition>> m_defs;

    // list of definitions to be executed (in order)
    std::list<arc::arg::Definition*> m_execute;
};

} // namespace arg
} // namespace ARC_BASE_VERSION_NS
} // namespace arc

#endif
