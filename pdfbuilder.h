#ifndef PDFBUILDER_H
#define PDFBUILDER_H

#include <QString>

namespace PDF {
    enum class DocumentClass {
        article,
        letter
    };

    
}

class PDFBuilder
{
public:
    PDFBuilder();
    //QString simple_output();
    ~PDFBuilder();

private:
    
};

#endif // PDFBUILDER_H
