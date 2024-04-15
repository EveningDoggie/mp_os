#include <iostream>
#include <client_logger_builder.h>
int main(
    int argc,
    char* argv[])
{

    client_logger_builder* builder = new client_logger_builder();
     logger* logger1 = builder
        ->add_file_stream("file1.txt", logger::severity::debug)
        //->add_file_stream("file2.txt", logger::severity::debug)
      //  ->add_file_stream("file3.txt", logger::severity::debug)
        //->add_console_stream(logger::severity::debug)
        ->build();

     //см лист - переделать так же как там


    client_logger_builder* builder2 = new client_logger_builder();
    logger * logger2 = builder
        ->add_file_stream("file1.txt", logger::severity::debug)
        //->add_file_stream("file1.txt", logger::severity::debug)
        //->add_console_stream(logger::severity::debug)
        ->build();

    //см лист - переделать так же как там


    logger1->log("sdaf1", logger::severity::debug);
    logger2->log("sdaf2", logger::severity::debug);

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