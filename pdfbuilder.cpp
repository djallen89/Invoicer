#include "pdfbuilder.h"
#include <QStringBuilder>
#include <iostream>

PDFBuilder::PDFBuilder()
{
    build_preamble();
    biller_info = new QString();
    client_info = new QString();
    invoice_number = new QString();
    line_items = QVector<QString*>();
}

PDFBuilder::~PDFBuilder()
{
    delete preamble;
    delete biller_info;
    delete client_info;
    delete invoice_number;
    foreach(auto line_item, line_items) {
        delete line_item;
    }
}

QString PDFBuilder::simple_output()
{
    using std::cout;
    using std::endl;

    auto output = QString("");
    output = *preamble % *biller_info % *client_info % *invoice_number;
    foreach(auto line_item, line_items) {
        output = output % *line_item;
    }
    return output;
}

void PDFBuilder::build_preamble()
{
    preamble = new QString("");
}
