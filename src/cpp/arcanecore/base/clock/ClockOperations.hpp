/*!
 * \file
 * \author David Saxon
 * \brief Operations for measuring time.
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
#ifndef ARCANECORE_BASE_CLOCK_CLOCKOPERATIONS_HPP_
#define ARCANECORE_BASE_CLOCK_CLOCKOPERATIONS_HPP_

#include <deus/UnicodeStorage.hpp>
#include <deus/UnicodeView.hpp>

#include "arcanecore/base/BaseAPI.h"
#include "arcanecore/base/clock/ClockDefinitions.hpp"


namespace arc
{
inline namespace ARC_BASE_VERSION_NS
{

namespace clock
{

/*!
 * \brief Returns the time elapsed since Linux Epoch (1st January 1970).
 *
 * \param metric The time measurement metric the result will be returned in.
 */
TimeInt get_current_time(TimeMetric metric = TimeMetric::kMilliseconds);

/*!
 * \brief Returns the current the given time as a formated string.
 *
 * \param t The time value (since Linux Epoch) that should be formatted as a
 *          string.
 * \param format Specifies the layout of the formatted string. See strftime for
 *               syntax.
 * \param metric The time measurement metric which t is measured in.
 */
// TODO: throws (can we use throw keyword)
deus::UnicodeStorage get_timestamp(
        TimeInt t,
        const deus::UnicodeView& format =
            deus::UnicodeView("%Y/%m/%d - %H:%M:%S", deus::Encoding::kASCII),
        TimeMetric metric = TimeMetric::kMilliseconds);

} // namespace clock
} // namespace ARC_BASE_VERSION_NS
} // namespace arc


#endif
