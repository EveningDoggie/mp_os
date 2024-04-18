#include <iostream>
#include <client_logger_builder.h>
int main(
    int argc,
    char* argv[])
{
//¬опросы: как тут вообще присвоить. +надо ли делать мьютексы в деструкторах

    client_logger_builder* builder = new client_logger_builder();
     logger* logger1 = builder
         ->add_file_stream("file1.txt", logger::severity::debug)
         ->add_file_stream("file2.txt", logger::severity::debug)
         ->clear()
         ->add_file_stream("file1.txt", logger::severity::debug)
         ->add_file_stream("file1.txt", logger::severity::critical)
         ->add_file_stream("file2.txt", logger::severity::debug)
         ->add_console_stream(logger::severity::debug)
         ->add_console_stream(logger::severity::critical)
         ->build();

    client_logger_builder* builder2 = new client_logger_builder();
    logger * logger2 = builder
        ->add_file_stream("file1.txt", logger::severity::debug)
        ->add_file_stream("file2.txt", logger::severity::debug)
        ->add_file_stream("file3.txt", logger::severity::debug)
        ->add_console_stream(logger::severity::debug)
        ->build();

    client_logger_builder* builder3 = new client_logger_builder();
    logger* logger3 = builder3
       // ->transform_with_configuration("C:/Users/lifem/Desktop/config.txt")
        ->build();


    logger* logger4 = logger2;
    logger1->log("sdaf1", logger::severity::debug);
    logger2->log("sdaf2", logger::severity::critical);
    //b->debug("111");


    delete builder;
    delete builder2;
    delete logger1;
    delete logger2;


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