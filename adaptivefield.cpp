#include "adaptivefield.h"

#include <QDebug>
#include <QResizeEvent>

AdaptiveLineEdit::AdaptiveLineEdit(const QString &label, QWidget *parent)
    : QLineEdit(parent)
    , m_label(label)
{
    qDebug() << "[CREATE]" << m_label << "(AdaptiveLineEdit)";
}

void AdaptiveLineEdit::setHorizontalPolicy(QSizePolicy::Policy policy)
{
    QSizePolicy sp = sizePolicy();
    sp.setHorizontalPolicy(policy);
    setSizePolicy(sp);
    qDebug() << "[POLICY]" << m_label << "horiz ->" << int(policy);
    updateGeometry();
}

void AdaptiveLineEdit::setVerticalPolicy(QSizePolicy::Policy policy)
{
    QSizePolicy sp = sizePolicy();
    sp.setVerticalPolicy(policy);
    setSizePolicy(sp);
    qDebug() << "[POLICY]" << m_label << "vert ->" << int(policy);
    updateGeometry();
}

void AdaptiveLineEdit::resizeEvent(QResizeEvent *event)
{
    QLineEdit::resizeEvent(event);
    qDebug() << "[RESIZE]" << m_label << "->" << event->size().width()
             << "x" << event->size().height();
}

AdaptiveTextEdit::AdaptiveTextEdit(const QString &label, QWidget *parent)
    : QPlainTextEdit(parent)
    , m_label(label)
{
    qDebug() << "[CREATE]" << m_label << "(AdaptiveTextEdit)";
}

void AdaptiveTextEdit::setHorizontalPolicy(QSizePolicy::Policy policy)
{
    QSizePolicy sp = sizePolicy();
    sp.setHorizontalPolicy(policy);
    setSizePolicy(sp);
    qDebug() << "[POLICY]" << m_label << "horiz ->" << int(policy);
    updateGeometry();
}

void AdaptiveTextEdit::setVerticalPolicy(QSizePolicy::Policy policy)
{
    QSizePolicy sp = sizePolicy();
    sp.setVerticalPolicy(policy);
    setSizePolicy(sp);
    qDebug() << "[POLICY]" << m_label << "vert ->" << int(policy);
    updateGeometry();
}

void AdaptiveTextEdit::resizeEvent(QResizeEvent *event)
{
    QPlainTextEdit::resizeEvent(event);
    qDebug() << "[RESIZE]" << m_label << "->" << event->size().width()
             << "x" << event->size().height();
}
