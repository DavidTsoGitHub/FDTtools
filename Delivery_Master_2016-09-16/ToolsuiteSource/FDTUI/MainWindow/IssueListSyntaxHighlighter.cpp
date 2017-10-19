#include "IssueListSyntaxHighlighter.h"

#include <QSyntaxHighlighter>
#include <QRegularExpression>

IssueListSyntaxHighlighter::IssueListSyntaxHighlighter(QTextDocument* parent) : QSyntaxHighlighter(parent),
    m_warningRegexp(QRegularExpression("^.+\\[WARNING\\].+$")),
    m_errorRegexp(QRegularExpression("^.+\\[ERROR\\].+$"))
{
    m_warningTextFormat.setForeground(QBrush(QColor(255, 127, 39)));
    m_errorTextFormat.setForeground(Qt::darkRed);
}

void IssueListSyntaxHighlighter::highlightBlock(const QString& text)
{
    QRegularExpressionMatchIterator errors = m_errorRegexp.globalMatch(text);
    while (errors.hasNext())
    {
        QRegularExpressionMatch match = errors.next();
        setFormat(match.capturedStart(), match.capturedLength(), m_errorTextFormat);
    }

    QRegularExpressionMatchIterator warnings = m_warningRegexp.globalMatch(text);
    while (warnings.hasNext())
    {
        QRegularExpressionMatch match = warnings.next();
        setFormat(match.capturedStart(), match.capturedLength(), m_warningTextFormat);
    }
}
