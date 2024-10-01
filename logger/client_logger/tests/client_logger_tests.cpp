#include <gtest/gtest.h>
#include <logger.h>
#include <client_logger.h>
#include <client_logger_builder.h>

int main(
    int argc,
    char *argv[])
{

    auto clb = client_logger_builder();
    clb.transform_with_configuration("logger_test_cfg.json", "logger");
    clb.add_console_stream(logger::severity::critical | logger::severity::information);
    clb.add_file_stream("logger_output_1.txt", logger::severity::debug);
    clb.add_file_stream("logger_output_2.txt", logger::severity::error | logger::severity::warning);
    clb.set_output_format("%d - %t - %s - %m");

    auto cl = clb.build();
    auto sev1 = logger::severity::debug | logger::severity::warning;
    cl->log("How are you?", sev1);

    auto sev2 = logger::severity::error | logger::severity::information | logger::severity::critical;
    cl->log("IDK", sev2);

    return 0;
    //testing::InitGoogleTest(&argc, argv);

    //return RUN_ALL_TESTS();
}