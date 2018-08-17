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
#ifndef ARCANECORE_ARG_FLAG_HPP_
#define ARCANECORE_ARG_FLAG_HPP_

#include <vector>

#include <deus/UnicodeStorage.hpp>
#include <deus/UnicodeView.hpp>

#include "arcanecore/base/BaseAPI.hpp"
#include "arcanecore/base/lang/Restrictors.hpp"


namespace arc
{
ARC_BASE_VERSION_NS_BEGIN
namespace arg
{

//------------------------------------------------------------------------------
//                              FORWARD DECLARATIONS
//------------------------------------------------------------------------------

class Parser;


/*!
 * \brief Object that should be inherited from in order to define a command line
 *        flag.
 *
 * Flags a have a long version that starts with `--` and optionally a short
 * version that is usually a single character and starts with '-'.
 */
class Flag
{
private:

    //--------------------------------------------------------------------------
    //                                  FRIENDS
    //--------------------------------------------------------------------------

    friend class Parser;

public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTOR
    //--------------------------------------------------------------------------

    /*!
     * \brief Super constructor for Flag implementations.
     *
     * \param key The command line argument string that signifies this action.
     * \param long_key The long version of the flag (e.g. --long_key). Note:
     *                 if the provided string does not begin with the `--`
     *                 prefix it will be appended to the long key.
     * \param short_key The short version of the flag (e.g. -v). If an empty
     *                  string is used, this flag will be considered not to have
     *                  a short version. Note: if the provided string is
     *                  non-empty and does not begin with the `-` prefix, it
     *                  will be appended to the short key.
     * \param description Text explains the purpose and the use of this flag
     *                    If an empty string is used, this flag is considered
     *                    not to have a description.
     *
     * \throw arc::ex::ValueError If the long_key parameter is empty.
     */
    Flag(
            const deus::UnicodeView& long_key,
            const deus::UnicodeView& short_key,
            const deus::UnicodeView& description);

    /*!
     * \brief Super constructor for Flag implementations that should be followed
     *        by one or more variable arguments.
     *
     * \param key The command line argument string that signifies this action.
     * \param long_key The long version of the flag (e.g. --long_key). Note:
     *                 if the provided string does not begin with the `--`
     *                 prefix it will be appended to the long key.
     * \param short_key The short version of the flag (e.g. -v). If an empty
     *                  string is used, this flag will be considered not to have
     *                  a short version. Note: if the provided string is
     *                  non-empty and does not begin with the `-` prefix, it
     *                  will be appended to the short key.
     * \param variable_names The names of the variables this flag should be
     *                       followed by, this is used for displaying the usage
     *                       of this flag.
     * \param description Text explains the purpose and the use of this flag
     *                    If an empty string is used, this flag is considered
     *                    not to have a description.
     *
     * \throw arc::ex::ValueError If the long_key parameter is empty.
     */
    Flag(
            const deus::UnicodeView& long_key,
            const deus::UnicodeView& short_key,
            const std::vector<std::string>& variable_names,
            const deus::UnicodeView& description);

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    virtual ~Flag();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns the long key of this flag.
     */
    const deus::UnicodeView& get_long_key() const;

    /*!
     * \brief Returns the short key of this flag.
     */
    const deus::UnicodeView& get_short_key() const;

    /*!
     * \brief Returns the variables names that will be parsed after this flag.
     */
    const std::vector<deus::UnicodeStorage>& get_variable_names() const;

    /*!
     * \brief Returns the description of this flag.
     */
    const deus::UnicodeView& get_description() const;

    /*!
     * \brief Is called if this flag is matched in the command line arguments so
     *        that any addition parsing can be performed.
     *
     * \param argi The index of the current argument being parsed in argv. This
     *             will be the index of this flag's key that was just matched
     *             plus one.
     * \param argc The total number of arguments in argv.
     * \param argv String array of the application's command line arguments.
     * \param out_increment The amount to increment argi by after parsing has
     *                      completed.
     * \param exit_code The exit code that will be used if parsing is
     *                  unsuccessful.
     *
     * \return Whether extra parsing was successful or not. If ```false`` the
     *         program will exit immediately after calling this function using
     *         the exit_code parameter.
     */
    virtual bool parse_extra(
            std::size_t argi,
            std::size_t argc,
            char** argv,
            std::size_t& out_increment,
            int& out_exit_code);

    /*!
     * \brief Is called to execute this command line flag.
     *
     * This function will be called once all command line actions and flags have
     * been successfully parsed.
     *
     * \param out_exit_code Returns the exit code that will be used if this flag
     *                      fails.
     *
     * \return Whether this flag completed successfully or not. If ```false```
     *         the program will exit immediately after calling this function
     *         using the exit_code parameter.
     */
    virtual bool execute(int& out_exit_code) = 0;

protected:

    //--------------------------------------------------------------------------
    //                            PROTECTED ATTRIBUTES
    //--------------------------------------------------------------------------

    /*!
     * \brief The parser object which is the parent of this action.
     */
    const Parser* m_parser_parent;

private:

    //--------------------------------------------------------------------------
    //                             PRIVATE ATTRIBUTES
    //--------------------------------------------------------------------------

    deus::UnicodeStorage m_long_key;
    deus::UnicodeStorage m_short_key;
    std::vector<deus::UnicodeStorage> m_variable_names;
    deus::UnicodeStorage m_description;

    //--------------------------------------------------------------------------
    //                          PRIVATE MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Ensures that a long flag is supplied, and that the flags have the
     *        correct prefixes.
     */
    void init_flags();

    /*!
     * \brief Sets the parser parent of this action.
     */
    void set_parser_parent(const Parser* parser_parent);

    /*!
     * \brief Parses whether the current command line argument matches this
     *        flag.
     *
     * If the current argument matches this flag's key, this call will also
     * perform any required extra parsing if a match was made. The flag is then
     * queued for execution once parsing has fully completed.
     *
     * \param argi The index of the current argument being parsed in argv.
     * \param argc The total number of arguments in argv.
     * \param argv String array of the application's command line arguments.
     * \param out_increment The amount to increment argi by after parsing has
     *                      completed.
     * \param out_exit_program Whether the program should exit after this
     *                         function has completed.
     * \param out_exit_code The exit code that will be used if the
     *                      out_exit_program parameter is ```true```.
     *
     * \return Whether this flag successfully matched the current argument.
     */
    bool parse(
            std::size_t argi,
            std::size_t argc,
            char** argv,
            std::size_t& out_increment,
            bool& out_exit_program,
            int& out_exit_code);
};

} // namespace arg
ARC_BASE_VERSION_NS_END
} // namespace arc

#endif
