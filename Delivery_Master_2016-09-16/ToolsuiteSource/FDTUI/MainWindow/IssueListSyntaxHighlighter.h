#ifndef ISSUELISTSYNTAXHIGHLIGHTER_H
#define ISSUELISTSYNTAXHIGHLIGHTER_H

class QRegularExpression;
class QString;
class QSyntaxHighlighter;
class QTextDocument;
class QTextCharFormat;

class IssueListSyntaxHighlighter : public QSyntaxHighlighter
{
public:
    explicit IssueListSyntaxHighlighter(QTextDocument* parent);

private:
    QRegularExpression m_warningRegexp;
    QRegularExpression m_errorRegexp;

    QTextCharFormat m_warningTextFormat;
    QTextCharFormat m_errorTextFormat;

    // QSyntaxHighlighter interface
protected:
    void highlightBlock(const QString& text);
};

#endif // ISSUELISTSYNTAXHIGHLIGHTER_H
