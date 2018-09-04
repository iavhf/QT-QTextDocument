#include <QApplication>
#include <QTextDocument>
#include <QTextFrame>
#include <QTextBlock>
#include <QTextCursor>
#include <QTextList>
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

void ProcessTable(QTextCursor& cursor)
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
}
void ProcessList(QTextCursor& cursor)
{
    QTextListFormat listFormat;
    listFormat.setIndent(1);
    listFormat.setStyle(QTextListFormat::ListCircle);

    QTextList *list1 = cursor.insertList(listFormat);
    cursor.insertText("Hello world in list1");
    QTextList *list2 = cursor.insertList(listFormat);
    cursor.insertText("Hello world in list2");
    QTextList *list3 = cursor.insertList(listFormat);
    cursor.insertText("Hello world in list3");
}

void PrintAllBlock(QTextDocument* document)
{
    QTextBlock currentBlock = document->begin();
    int num_block = 0;

    while(currentBlock.isValid()) {
        qDebug() << num_block << "  Current block index = " << currentBlock.position() << "The content of block is " << currentBlock.text();
        currentBlock = currentBlock.next();
        num_block++;
    }

}
void PrintBlock(QTextBlock currentBlock)
{
    qDebug() << "Current block index = " << currentBlock.position() << "The content of block is " << currentBlock.text();
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

            QTextList* list = childBlock.textList();
            if(list)
            {
                int index = list->itemNumber(childBlock);
                qDebug() << "list index: " << index;
            }
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

    ProcessTable(cursor);

    cursor.setPosition(root_frame->lastPosition());
    cursor.insertText("Hello world0");
    cursor.insertText("Hello world1");
    cursor.insertText("Hello world2\n");
    cursor.insertText("Hello world3");
    cursor.insertText("Hello world4");
    cursor.insertText("Hello world5");

    ProcessImage(cursor, "IP.png");
    ProcessList(cursor);
    SaveToPDF(document);
    //return app.exec();
    //
    qDebug() << "Print All TextBlocks";
    PrintAllBlock(document);
    qDebug() << "Print All frames";
    PrintFrame(document);
    return 0;
}
