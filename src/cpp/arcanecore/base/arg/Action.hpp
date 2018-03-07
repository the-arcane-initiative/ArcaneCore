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
#ifndef ARCANECORE_BASE_ARG_ACTION_HPP_
#define ARCANECORE_BASE_ARG_ACTION_HPP_

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
 *        action.
 *
 * Command line actions must actions that come immediately after the application
 * name or the a parent action.
 *
 * For example:
 * \code
 * my_application action_1 sub_action <flags>
 * \endcode
 */
class Action
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
    //                                CONSTRUCTOR
    //--------------------------------------------------------------------------

    /*!
     * \brief Super constructor for Action implementations.
     *
     * \param key The command line argument string that signifies this action.
     * \param description Text explains the purpose and the use of this
     *                    argument. If an empty string is used, this action is
     *                    considered not to have a description.
     *
     * \throw arc::ex::ValueError If the key parameter is empty.
     */
    Action(
            const deus::UnicodeView& key,
            const deus::UnicodeView& description);

    /*!
     * \brief Super constructor for Action implementations that should be
     *        followed by one or more variable argument.
     *
     * \param key The command line argument string that signifies this action.
     * \param variable_names The names of the variables this action should be
     *                       followed by, this is used for displaying the usage
     *                       of this action.
     * \param description Text explains the purpose and the use of this
     *                    argument. If an empty string is used, this action is
     *                    considered not to have a description.
     *
     * \throw arc::ex::ValueError If the key parameter is empty.
     */
    Action(
            const deus::UnicodeView& key,
            const std::vector<std::string>& variable_names,
            const deus::UnicodeView& description);

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    virtual ~Action();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns the key of this action.
     */
    const deus::UnicodeView& get_key() const;

    /*!
     * \brief Returns the variables names that will be parsed after this flag.
     */
    const std::vector<deus::UnicodeStorage>& get_variable_names() const;

    /*!
     * \brief Returns the description of this action.
     */
    const deus::UnicodeView& get_description() const;

    // TODO: sub action

    // TODO: sub definition

    /*!
     * \brief Is called to execute this command line action.
     *
     * This function will be called once all command line actions and flags have
     * been successfully parsed.
     *
     * \param out_exit_code Returns the exit code that will be used if this
     *                      action fails.
     *
     * \return Whether this action completed successfully or not. If ```false```
     *         the program will exist immediately after calling this function
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

    deus::UnicodeStorage m_key;
    std::vector<deus::UnicodeStorage> m_variable_names;
    deus::UnicodeStorage m_description;

    //--------------------------------------------------------------------------
    //                          PRIVATE MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Sets the parser parent of this action.
     */
    void set_parser_parent(const Parser* parser_parent);

    /*!
     * \brief Parses whether the current command line argument matches this
     *        action.
     *
     * If the current argument matches this action's key, this call will
     * continue to parse the sub actions and flags of this action. The action
     * is then queued for execution once parsing has fully completed.
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
     * \return Whether this action successfully matched the current argument.
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
