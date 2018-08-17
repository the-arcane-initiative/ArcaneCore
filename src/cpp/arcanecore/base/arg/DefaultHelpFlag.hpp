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
#ifndef ARCANECORE_ARG_FLAG_DEFAULTHELPFLAG_HPP_
#define ARCANECORE_ARG_FLAG_DEFAULTHELPFLAG_HPP_

#include <deus/UnicodeStorage.hpp>
#include <deus/UnicodeView.hpp>

#include "arcanecore/base/BaseAPI.hpp"
#include "arcanecore/base/arg/Flag.hpp"
#include "arcanecore/base/lang/Restrictors.hpp"


namespace arc
{
ARC_BASE_VERSION_NS_BEGIN
namespace arg
{

/*!
 * \brief Command line flag that immediately prints help text for the parent
 *        Parser or Action and then exits.
 */
class DefaultHelpFlag
    : public arc::arg::Flag
{
public:

    //--------------------------C O N S T R U C T O R---------------------------

    /*!
     * \brief Constructs a new DefaultHelpText Flag.
     *
     * \param usage_text Simple example of usage of the command line tool. If
     *                   an empty string is used the help displayed will not
     *                   contain an usage section.
     */
    DefaultHelpFlag(const deus::UnicodeView& usage_text);

    //---------------------------D E S T R U C T O R----------------------------

    virtual ~DefaultHelpFlag();

    //-------------P U B L I C    M E M B E R    F U N C T I O N S--------------

    virtual bool parse_extra(
            std::size_t argi,
            std::size_t argc,
            char** argv,
            std::size_t& out_increment,
            int& out_exit_code) override;

    virtual bool execute(int& out_exit_code) override;

private:

    //--------------------------------------------------------------------------
    //                             PRIVATE ATTRIBUTES
    //--------------------------------------------------------------------------

    const deus::UnicodeStorage m_usage_text;
};

} // namespace arg
ARC_BASE_VERSION_NS_END
} // namespace arc

#endif
