#include <iostream>
#include <client_logger_builder.h>
int main(
    int argc,
    char *argv[])
{

    client_logger_builder * builder = new client_logger_builder();
    logger * logger = builder
        ->add_file_stream("debug.txt", logger::severity::debug)
        ->add_console_stream(logger::severity::debug)
        ->build();

 
    logger->log("sdaf", logger::severity::debug);

    std::cout << "ok";
    int a;
    std::cin >> a;
    return 0;
}



/*
#include <gtest/gtest.h>



int main(
    int argc,
    char *argv[])
{
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
*/