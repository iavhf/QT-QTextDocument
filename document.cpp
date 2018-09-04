#include <QApplication>
#include <QTextDocument>
#include <QTextFrame>
#include <QTextBlock>
#include <QTextCursor>
#include <QTextTableCell>
#include <QPrinter>
#include <QDebug>

void SaveToPDF(QTextDocument* document)
{
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName("output.pdf");
    document->print(&printer);
}

void ProcessImage(QTextCursor& cursor, QString image_file_name)
{
    QTextImageFormat imageFormat;
    imageFormat.setName(image_file_name);
    cursor.insertImage(imageFormat);
}

void ProcessTable(QTextDocument* document, QTextCursor& cursor)
{
    const int rows = 2;
    const int cols = 3;

    QTextTableFormat tableFormat;
    tableFormat.setAlignment(Qt::AlignHCenter);
    tableFormat.setBorder(1);
    QTextTable* table = cursor.insertTable(rows, cols, tableFormat);

    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            QTextTableCell cell = table->cellAt(i,j);
            QTextCursor cellCursor = cell.firstCursorPosition();
            QString content = QString::number(i) + "," + QString::number(j);
            cellCursor.insertText(content);
        }
    }
    /*cell = table->cellAt(0,0);
    cellCursor = cell.firstCursorPosition();
    cellCursor.insertText("0,0");

    cell = table->cellAt(0,1);
    cellCursor = cell.firstCursorPosition();
    cellCursor.insertText("0,1");

    cell = table->cellAt(0,2);
    cellCursor = cell.firstCursorPosition();
    cellCursor.insertText("0,2");

    cell = table->cellAt(1,0);
    cellCursor = cell.firstCursorPosition();
    cellCursor.insertText("1,0");

    cell = table->cellAt(1,1);
    cellCursor = cell.firstCursorPosition();
    cellCursor.insertText("1,1");

    cell = table->cellAt(1,2);
    cellCursor = cell.firstCursorPosition();
    cellCursor.insertText("1,2");*/
}
void ProcessList(QTextDocument* document, QTextCursor& cursor)
{
    QTextListFormat listFormat;
    listFormat.setIndent(3);
    listFormat.setStyle(QTextListFormat::ListCircle);

    QTextList *list = cursor.insertList(listFormat);
    cursor.insertText("Hello world in list");
}

void PrintBlock(QTextBlock currentBlock)
{
    //QTextBlock currentBlock = document->begin();
    int num_block = 0;

    while(currentBlock.isValid()) {
        qDebug() << num_block << "  Current block index = " << currentBlock.position() << "The content of block is " << currentBlock.text();
        currentBlock = currentBlock.next();
        num_block++;
    }

}
void PrintFrame(QTextDocument* document)
{
    QTextFrame::iterator it;
    for(it = document->rootFrame()->begin(); !(it.atEnd()); ++it)
    {
        QTextFrame* childFrame = it.currentFrame();
        QTextBlock childBlock = it.currentBlock();

        if(childFrame)
        {
            qDebug() << "child frame";
        }
        else if(childBlock.isValid())
        {
            qDebug() << "child block";
            PrintBlock(childBlock);
        }
    }
}
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QTextDocument* document = new QTextDocument();

    QFont textFont("Helvetica");
    document->setDefaultFont(textFont);

    QTextFrame* root_frame = document->rootFrame();

    QTextCursor cursor(document);
    cursor.insertText("Hello world");

    ProcessTable(document, cursor);

    cursor.setPosition(root_frame->lastPosition());
    cursor.insertText("Hello world0");
    cursor.insertText("Hello world1");
    cursor.insertText("Hello world2\n");
    cursor.insertText("Hello world3");
    cursor.insertText("Hello world4");
    cursor.insertText("Hello world5");

    ProcessImage(cursor, "IP.png");
    SaveToPDF(document);
    //return app.exec();
    //
    qDebug() << "Print All TextBlocks";
    PrintBlock(document->begin());
    qDebug() << "Print All frames";
    PrintFrame(document);
    return 0;
}
