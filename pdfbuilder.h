#ifndef PDFBUILDER_H
#define PDFBUILDER_H

#include <QString>
#include <QVector>

namespace PDF {
    enum class DocumentClass {
        article,
        letter
    };

    enum class textSize {
        Huge,
        huge,
        LARGE,
        Large,
        large,
        normalsize,
        small,
        footnotesize,
        scriptsize,
        tiny
    };
}

class PDFBuilder
{
public:
    PDFBuilder();
    QString simple_output();
    ~PDFBuilder();

private:
    PDF::DocumentClass docClass;
    QString *preamble;
    QString *biller_info;
    QString *client_info;
    QString *invoice_number;
    QVector<QString*> line_items;
    void build_preamble();
};

#endif // PDFBUILDER_H
