#include "docmodel.h"

namespace Model {
    DocModel::DocModel()
    {
        invoiceNumber = new QString();
        currency = new QString();
        currencySymbol = new QString();
        yourInfo = new ContactInfo();
        clientInfo = new ContactInfo();
        catalog = new ItemCatalog();
        currency = new QString();
    }

    DocModel::~DocModel()
    {
        delete yourInfo;
        delete clientInfo;
        delete invoiceNumber;
        delete catalog;
        delete currency;
        delete currencySymbol;
    }

    /*
    void DocModel::updateInvoiceNumber(QString *newNumber)
    {
        if (invoiceNumber) {
            delete invoiceNumber;
            invoiceNumber = newNumber;
        }
    }

    void DocModel::updateCurrency(QString *newCurrency)
    {
        if (currency) {
            delete currency;
            currency = newCurrency;
        }
    }
    */

    void DocModel::createPDF()
    {
        const char *page_title = "Invoice";
        HPDF_Doc pdf;
        HPDF_Page page;
        HPDF_Font def_font;
        HPDF_REAL tw;
        HPDF_REAL height;
        HPDF_REAL width;
        HPDF_UINT i;

        pdf = HPDF_New(error_handler, NULL);

        page = HPDF_AddPage(pdf);

        height = HPDF_Page_GetHeight(page);
        width = HPDF_Page_GetWidth(page);

        def_font = HPDF_GetFont(pdf, "Times-Roman", NULL);
        HPDF_Page_SetFontAndSize(page, def_font, 14);

        tw = HPDF_Page_TextWidth(page, page_title);
        HPDF_Page_BeginText(page);
        HPDF_Page_TextOut(page, (width - tw) / 2, height - 50, page_title);
        HPDF_Page_EndText(page);

        /* output subtitle. */
        HPDF_Page_BeginText(page);
        HPDF_Page_SetFontAndSize(page, def_font, 18);
        HPDF_Page_TextOut(page, 60, height - 80, "Sample");
        HPDF_Page_EndText(page);

        HPDF_Page_BeginText(page);
        HPDF_Page_MoveTextPos(page, 60, height - 105);

        const char* samp_text = "test test test";
        //HPDF_Font font = HPDF_GetFont (pdf, font_list[i], NULL);

        /* print a label of text */
        HPDF_Page_SetFontAndSize(page, def_font, 9);
        HPDF_Page_ShowText(page, "Times-Roman");
        HPDF_Page_MoveTextPos(page, 0, -18);

        /* print a sample text. */
        HPDF_Page_SetFontAndSize(page, def_font, 20);
        HPDF_Page_ShowText(page, samp_text);
        HPDF_Page_MoveTextPos(page, 0, -20);

        HPDF_Page_EndText(page);

        HPDF_SaveToFile(pdf, "foo.pdf");

        /* clean up */
        HPDF_Free (pdf);
    }
}
