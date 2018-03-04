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
#ifndef ARCANECORE_BASE_ARG_DEFINITION_HPP_
#define ARCANECORE_BASE_ARG_DEFINITION_HPP_

#include <deus/UnicodeStorage.hpp>
#include <deus/UnicodeView.hpp>

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

class Parser;

/*!
 * \brief Object that defines a command line argument, how it should be parsed,
 *        and then how the resulting action should be executed.
 */
class Definition
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
private:

    //--------------------------------------------------------------------------
    //                                  FRIENDS
    //--------------------------------------------------------------------------

    friend class Parser;

public:

    //--------------------------------------------------------------------------
    //                                COSNTRUCTORS
    //--------------------------------------------------------------------------

    /*!
     * \brief Constructs a new command line argument Definition.
     *
     * \param long_key The long version of the argument (e.g. --long_key). Note:
     *                 the provided string should not include the '--' prefix.
     * \param short_key The short version of the argument (e.g. -v). If an
     *                  empty string is used, this argument will be considered
     *                  not to have a short version. Note the provided string
     *                  should not include the '-' prefix.
     * \param description Text that explains the purpose and the use of this
     *                    argument. If an empty string is used, this argument
     *                    will be considered not to have description.
     */
    Definition(
            const deus::UnicodeView& long_key,
            const deus::UnicodeView& short_key,
            const deus::UnicodeView& description);

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    virtual ~Definition();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns the long key of this argument definition.
     */
    const deus::UnicodeView& get_long_key() const;

    /*!
     * \brief Returns the short key of this argument definition.
     */
    const deus::UnicodeView& get_short_key() const;

    /*!
     * \brief Returns the description of this argument definition.
     */
    const deus::UnicodeView& get_description() const;

    /*!
     * \brief Definition implementations that need to addition parsing beyond
     *        checking whether the current argument argument matches the key
     *        should implement this function.
     *
     * This function is called after the definition is successfully matched
     * against an argument, and is used to perform any further argument parsing
     * required.
     *
     * \param argi The index of the next argument to parse in argv. The previous
     *             index, is the index of this definition's key that has just
     *             been matched.
     * \param argc The total number of argument number of arguments in argv.
     * \param argv String array of the application's arguments.
     * \param increment The amount to increment argi by.
     * \param exit_code The exit code that will be used if parsing is
     *                  unsuccessful.
     *
     * \return Whether parsing was successful or not. If ```false`` the program
     *         will exit immediately after calling this function using the
     *         exit_code parameter.
     */
    virtual bool parse(
            std::size_t argi,
            std::size_t argc,
            char** argv,
            std::size_t& increment,
            int& exit_code);

    /*!
     * \brief Is called to execute the action of this argument Definition.
     *
     * This function will be called after all arguments have been successfully
     * parsed.
     *
     * \param exit_code THe exit code that will be used if the action failed.
     *
     * \return Whether the action completed successfully or not. If ```false```
     *         the program will exit immediately after calling this function
     *         using the exit_code parameter.
     */
    virtual bool execute(int& exit_code) = 0;

private:

    //--------------------------------------------------------------------------
    //                             PRIVATE ATTRIBUTES
    //--------------------------------------------------------------------------

    deus::UnicodeStorage m_long_key;
    deus::UnicodeStorage m_short_key;
    deus::UnicodeStorage m_description;

    //--------------------------------------------------------------------------
    //                          PRIVATE MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Is called by the Parser to check whether the current argument
     *        matches this definition's long or short key.
     *
     * If a match is made this function will call the parse function to perform
     * any extended parsing, and the Definition is queued for execution.
     *
     * \param argi The index of the current argument being parsed in argv.
     * \param argc The total number of argument number of arguments in argv.
     * \param argv String array of the application's arguments.
     * \param increment The amount to increment argi by.
     * \param exit_program Whether the the program should exit after this
     *                     function has completed.
     * \param exit_code The exit code that will be used if the exit_program
     *                  parameter is ```true```.
     *
     * \return Whether this definition successfully matched the current
     *         argument.
     */
    bool check(
            std::size_t argi,
            std::size_t argc,
            char** argv,
            std::size_t& increment,
            bool& exit_program,
            int& exit_code);
};

} // namespace arg
} // namespace ARC_BASE_VERSION_NS
} // namespace arc

#endif
