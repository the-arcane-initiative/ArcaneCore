#include <gtest/gtest.h>

#include <arcanecore/base/clock/ClockOperations.hpp>


//------------------------------------------------------------------------------
//                                     TESTS
//------------------------------------------------------------------------------

TEST(Proto, proto1)
{
    std::cout
        << "current time: " << arc::clock::get_current_time() << std::endl;

    std::cout
        << "timestamp: "
        << arc::clock::get_timestamp(
            arc::clock::get_current_time(),
            "%Y/%m/%d/%d___%S - %H:%M:%S"
        )
        << std::endl;
}
