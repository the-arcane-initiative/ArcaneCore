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
#include "arcanecore/base/clock/ClockOperations.hpp"

#include <chrono>
#include <ctime>

#include "arcanecore/base/Exceptions.hpp"

// allows us to use std::localtime, without warning it's unsafe.
#ifdef ARC_OS_WINDOWS
    #pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS
#endif


namespace arc
{
inline namespace ARC_BASE_VERSION_NS
{

namespace clock
{

TimeInt get_current_time(TimeMetric metric)
{
    return static_cast<TimeInt>(
            std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
            ).count()
    ) / static_cast<TimeInt>(metric);
}

deus::UnicodeStorage get_timestamp(
        TimeInt t,
        const deus::UnicodeView& format,
        TimeMetric metric)
{
    // convert input to time_t
    TimeInt to_time_t =
        static_cast<TimeInt>(TimeMetric::kSeconds) /
        static_cast<TimeInt>(metric);
    time_t t_t = static_cast<time_t>(t / to_time_t);

    // convert the format string
    deus::UnicodeStorage format_converted;
    deus::UnicodeView format_view = format.convert_if_not(
        deus::ASCII_COMPATIBLE_ENCODINGS,
        deus::Encoding::kUTF8,
        format_converted
    );

    // TODO: buffer allocation could be smarter
    // allocate string data (double size to be safe)
    std::size_t buffer_size = format_view.byte_length() * 2;
    char* buffer = new char[buffer_size];
    std::size_t size = strftime(
        buffer,
        buffer_size,
        format_view.c_str(),
        std::localtime(&t_t)
    );
    // failure?
    if(size == 0)
    {
        delete[] buffer;
        throw arc::ex::RuntimeError(
            "Encountered unexpected error calling strftime within: "
            "arc::clock::get_timestamp"
        );
    }

    // clean up and return
    deus::UnicodeStorage ret(buffer, format_view.encoding());
    delete[] buffer;
    if(format_view.encoding() != format.encoding())
    {
        return ret.get_view().convert(format.encoding());
    }
    return ret;
}

} // namespace clock
} // namespace ARC_BASE_VERSION_NS
} // namespace arc
