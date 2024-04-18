#include <iostream>
#include <client_logger_builder.h>
int main(
    int argc,
    char* argv[])
{
    //�������: 
    // ��� ��� ������ ���������
    // ���� �� ������ �������� � ������������? c ��������� �� ���������������
    // ����� �� ������� ��� ��� � ����� �� �����?
     //         �� ����������� �������� ������� � ������� � ��������� ��� ���
     //         � ������ ����� �������� ������� � ���� ������������
     //         ��� �����: ���. �� ����������� ��� ������������� �������������->����� �������


    //��������: 1) ��������� ������������� ������ ������
    //          2) ���������� throw �����. ��������� � �������� try catch
           //   3) �������� ��� ���� ����� ��������. ��� ������� ���������, ������, ������ � ��
        //      4) ��������� ��� �� ������ ������. ����� �� �� ����

    auto builder = new client_logger_builder();
    auto logger1 = builder
         ->add_file_stream("file1.txt", logger::severity::debug)
         ->add_file_stream("file2.txt", logger::severity::debug)
         ->clear()
         ->add_file_stream("file1.txt", logger::severity::debug)
         ->add_file_stream("file1.txt", logger::severity::critical)
         ->add_file_stream("file2.txt", logger::severity::debug)
         ->add_console_stream(logger::severity::debug)
         ->add_console_stream(logger::severity::critical)
         ->build();

    auto builder2 = new client_logger_builder();
    auto logger2 = builder2
        ->add_file_stream("file1.txt", logger::severity::debug)
        ->add_file_stream("file2.txt", logger::severity::debug)
        ->add_file_stream("file3.txt", logger::severity::debug)
        ->add_console_stream(logger::severity::debug)
        ->build();

    
    try 
    {
        client_logger_builder* builder3 = new client_logger_builder();
        auto logger3 = builder3
            ->transform_with_configuration("C:/Users/lifem/Desktop/config.txt")
            ->build();
        logger3->critical("kek");
    }
    catch(...)
    {
        std::cout << "incorrect path for logger 3\n";
    }
    

    logger1->log("sdaf1", logger::severity::debug);
    logger2->debug("sd");


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