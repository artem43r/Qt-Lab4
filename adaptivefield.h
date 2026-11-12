#ifndef ADAPTIVEFIELD_H
#define ADAPTIVEFIELD_H

#include <QLineEdit>
#include <QPlainTextEdit>
#include <QSizePolicy>
#include <QString>

class QResizeEvent;

// Поле одной строки: меняет размерную политику и логирует resizeEvent.
// Q_OBJECT не нужен: здесь нет собственных сигналов или слотов.
class AdaptiveLineEdit : public QLineEdit
{
public:
    explicit AdaptiveLineEdit(const QString &label, QWidget *parent = nullptr);

    void setHorizontalPolicy(QSizePolicy::Policy policy);
    void setVerticalPolicy(QSizePolicy::Policy policy);
    QString label() const { return m_label; }

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    QString m_label;
};

// Многострочное поле: растягивается по двум осям и пишет новый размер в лог.
class AdaptiveTextEdit : public QPlainTextEdit
{
public:
    explicit AdaptiveTextEdit(const QString &label, QWidget *parent = nullptr);

    void setHorizontalPolicy(QSizePolicy::Policy policy);
    void setVerticalPolicy(QSizePolicy::Policy policy);
    QString label() const { return m_label; }

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    QString m_label;
};

#endif // ADAPTIVEFIELD_H
