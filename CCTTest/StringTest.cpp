#include "StringTest.hpp"

StringTest::StringTest()
{

}

bool StringTest::test() try{

    test0();

    return true;
}
catch (...) {
    return false;
}


void StringTest::test0() {

    {
        cct::TemplateString< std::string > t0(u8R"(today is a fine ${a}
${b}
)");

        std::cout<<t0("a","123","b","444")<<std::endl;
        std::string view=t0("a","123","b","444");
        std::cout<<view.size()<<std::endl;
    }

    {
        cct::TemplateString< QString > t0(u8R"(today is a fine ${a}
${b}
)");
        qDebug().noquote()<<t0("a","123","b","444") ;
        QString view=t0("a","123","b","444");
        qDebug()<<view.size() ;
    }

}


