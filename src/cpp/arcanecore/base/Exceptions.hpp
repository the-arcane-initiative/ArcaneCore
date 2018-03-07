/*!
 * \file
 * \author David Saxon
 * \brief Defines the exceptions that can be thrown by ArcaneCore.
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
#ifndef ARCANECORE_BASE_EXCEPTIONS_HPP_
#define ARCANECORE_BASE_EXCEPTIONS_HPP_

#include <stdexcept>
#include <string>

#include <deus/UnicodeStorage.hpp>
#include <deus/UnicodeView.hpp>

#include "arcanecore/base/BaseAPI.hpp"


namespace arc
{
ARC_BASE_VERSION_NS_BEGIN

/*!
 * \brief Generic exceptions that can be thrown by ArcaneCore.
 */
namespace ex
{

//------------------------------------------------------------------------------
//                                   ARC ERROR
//------------------------------------------------------------------------------
/*!
 * \brief Base exception type which is derived from std::runtime_error that all
 *        exceptions thrown by the Deus library inherit from.
 */
class ArcError
    : public std::runtime_error
{
public:

    /*!
     * \brief Constructs a new ArcErrpr exception with the given message.
     */
    ArcError(const deus::UnicodeView& what)
        : std::runtime_error(what.convert_if_not(
            deus::ASCII_COMPATIBLE_ENCODINGS,
            deus::Encoding::kUTF8,
            m_what_converted
        ).c_str())
        , m_type(deus::UnicodeView("ArcError", deus::Encoding::kUTF8))
    {
    }

    virtual ~ArcError() throw()
    {
    }

    /*!
     * \brief Returns the type string of this exception.
     */
    const deus::UnicodeStorage& get_type() const
    {
        return m_type;
    }


protected:

    /*!
     * \brief Super constructor which should be called by derived classes of
     *        ArcError.
     *
     * \param what The error message of the exception.
     * \param type The type string of the exception, usually just the name of
     *             the class (e.g. "ValueError").
     */
    ArcError(
            const deus::UnicodeView& what,
            const deus::UnicodeView& type)
        : std::runtime_error(what.convert_if_not(
            deus::ASCII_COMPATIBLE_ENCODINGS,
            deus::Encoding::kUTF8,
            m_what_converted
        ).c_str())
        , m_type(type.convert(deus::Encoding::kUTF8))
    {
    }

private:

    // for converted the what parameter to ascii compatible (if needed)
    deus::UnicodeStorage m_what_converted;
    // the type string of this exception
    deus::UnicodeStorage m_type;
};

//------------------------------------------------------------------------------
//                                 RUNTIME ERROR
//------------------------------------------------------------------------------

/*!
 * \brief Generic exception which is thrown when an error is encountered during
 *        runtime, and more specific details are not known.
 */
class RuntimeError
    : public ArcError
{
public:

    RuntimeError(const deus::UnicodeView& what)
        : ArcError(what, "RuntimeError")
    {
    }
};

//------------------------------------------------------------------------------
//                                  VALUE ERROR
//------------------------------------------------------------------------------

/*!
 * \brief Warns that an invalid value has been used for an operation.
 */
class ValueError
    : public ArcError
{
public:

    ValueError(const deus::UnicodeView& what)
        : ArcError(what, "ValueError")
    {
    }
};

//------------------------------------------------------------------------------
//                                  STATE ERROR
//------------------------------------------------------------------------------

/*!
 * \brief Warns that an action has been requested that is not valid for the
 *        current state.
 */
class StateError
    : public ArcError
{
public:

    StateError(const deus::UnicodeView& what)
        : ArcError(what, "StateError")
    {
    }
};

} // namespace ex
ARC_BASE_VERSION_NS_END
} // namespace arc

#endif
