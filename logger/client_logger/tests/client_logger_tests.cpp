#include <gtest/gtest.h>
#include <iostream>
#include <client_logger_builder.h>


int main(
    int argc,
    char *argv[])
{
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}

TEST(clientLoggerTests, test1)
{
    //�������: 
    // ��� ��� ������ ���������
    // ���� �� ������ �������� � ������������? c ��������� �� ���������������
    // ����� �� ������� ��� ��� � ����� �� �����?
     //         �� ����������� �������� ������� � ������� � ��������� ��� ���
     //         � ������ ����� �������� ������� � ���� ������������
     //         ��� �����: ���. �� ����������� ��� ������������� �������������->����� �������

    //console:
    //severity:critical
    //severity : debug
    //��������� ������������� ������ ������

    try
    {
        auto builder = new client_logger_builder();
        auto logger1 = builder
            ->add_file_stream("filecheck.txt", logger::severity::debug)
            ->add_file_stream("file2.txt", logger::severity::debug)
            ->clear()
            ->add_file_stream("filecheck.txt", logger::severity::debug)
            ->add_file_stream("file2.txt", logger::severity::debug)
            ->add_console_stream(logger::severity::debug)
            ->add_console_stream(logger::severity::critical)
            ->build();

        
       /*auto builder2 = new client_logger_builder();
        auto logger2 = builder2
            ->add_file_stream("file1.txt", logger::severity::debug)
            ->add_file_stream("file2.txt", logger::severity::debug)
            ->add_file_stream("file3.txt", logger::severity::debug)
            ->add_console_stream(logger::severity::debug)
            ->build();
            */

       /* client_logger_builder* builder3 = new client_logger_builder();
        auto logger3 = builder3
            ->transform_with_configuration("C:/Users/lifem/Desktop/config.txt")
            ->build();
        logger3->debug("kek"); 
        */



        auto logger2 = std::move(logger1);
      //  logger1->log("sdaf1", logger::severity::debug);
        logger2->log("asds", logger::severity::debug);
        delete builder;
        //delete builder2;
        delete logger2;
       

    }
    catch (const char* error_message)
    {
        std::cout << error_message;
    }


    int a;
    std::cin >> a;

}

