#include "formwindow.h"

#include <QCheckBox>
#include <QComboBox>
#include <QDebug>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QIntValidator>
#include <QLabel>
#include <QPushButton>
#include <QResizeEvent>
#include <QSizePolicy>
#include <QVBoxLayout>

FormWindow::FormWindow(QWidget *parent)
    : QWidget(parent)
    , m_name(QStringLiteral("Имя"))
    , m_email(QStringLiteral("Email"))
    , m_age(QStringLiteral("Возраст"))
    , m_country(nullptr)
    , m_description(QStringLiteral("Описание"))
    , m_agree(nullptr)
    , m_rootLayout(nullptr)
    , m_formLayout(nullptr)
    , m_descriptionExpanding(true)
    , m_ageExpanding(false)
{
    setupUi();
    setComfortableMode();
    qDebug() << "[INIT] окно создано; растяните его и смотрите [RESIZE] в логе";
}

FormWindow::~FormWindow()
{
    qDebug() << "[DESTROY] FormWindow";
}

void FormWindow::setupUi()
{
    setWindowTitle(QStringLiteral("Адаптивная форма — QSizePolicy / addStretch / margins"));
    resize(560, 620);

    m_rootLayout = new QVBoxLayout(this);
    auto *title = new QLabel(QStringLiteral("<b>Анкета</b>"), this);
    m_rootLayout->addWidget(title);

    m_formLayout = new QFormLayout();
    m_formLayout->setContentsMargins(0, 0, 0, 0);
    m_formLayout->setSpacing(8);
    m_rootLayout->addLayout(m_formLayout);

    // Имя и email расширяются по горизонтали, но не по вертикали.
    m_name.setHorizontalPolicy(QSizePolicy::Expanding);
    m_name.setVerticalPolicy(QSizePolicy::Fixed);
    m_name.setPlaceholderText(QStringLiteral("Иван Иванов"));
    m_formLayout->addRow(QStringLiteral("&Имя:"), &m_name);

    m_email.setHorizontalPolicy(QSizePolicy::Expanding);
    m_email.setVerticalPolicy(QSizePolicy::Fixed);
    m_email.setPlaceholderText(QStringLiteral("ivan@example.com"));
    m_formLayout->addRow(QStringLiteral("&Email:"), &m_email);

    // Поле возраста по умолчанию короткое. «Пружина» забирает свободное место.
    m_age.setHorizontalPolicy(QSizePolicy::Fixed);
    m_age.setVerticalPolicy(QSizePolicy::Fixed);
    m_age.setPlaceholderText(QStringLiteral("лет"));
    m_age.setValidator(new QIntValidator(0, 130, &m_age));

    auto *ageRow = new QHBoxLayout();
    ageRow->setContentsMargins(0, 0, 0, 0);
    ageRow->addWidget(&m_age);
    ageRow->addStretch();
    m_formLayout->addRow(QStringLiteral("&Возраст:"), ageRow);

    m_country = new QComboBox(this);
    m_country->addItems({QStringLiteral("Россия"), QStringLiteral("Беларусь"),
                         QStringLiteral("Казахстан"), QStringLiteral("Другая")});
    m_country->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_formLayout->addRow(QStringLiteral("&Страна:"), m_country);

    // Поле описания занимает свободную высоту окна.
    m_description.setHorizontalPolicy(QSizePolicy::Expanding);
    m_description.setVerticalPolicy(QSizePolicy::Expanding);
    m_description.setPlaceholderText(QStringLiteral("Расскажите о себе…"));
    m_formLayout->addRow(QStringLiteral("&Описание:"), &m_description);

    m_agree = new QCheckBox(QStringLiteral("Я согласен с условиями"), this);
    m_agree->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_formLayout->addRow(QString(), m_agree);

    m_trackedFields = {&m_name, &m_email, &m_age,
                       m_country, &m_description, m_agree};

    // Шесть кнопок управления. Пружина справа прижимает их к левому краю.
    auto *modeRow = new QHBoxLayout();
    auto *btnCompact = new QPushButton(QStringLiteral("Компактный"), this);
    auto *btnComfort = new QPushButton(QStringLiteral("Развёрнутый"), this);
    auto *btnDesc = new QPushButton(QStringLiteral("Описание: тянуть/фикс."), this);
    auto *btnAge = new QPushButton(QStringLiteral("Возраст: фикс./тянуть"), this);
    auto *btnPol = new QPushButton(QStringLiteral("Дамп политик"), this);
    auto *btnSize = new QPushButton(QStringLiteral("Дамп размеров"), this);

    modeRow->addWidget(btnCompact);
    modeRow->addWidget(btnComfort);
    modeRow->addWidget(btnDesc);
    modeRow->addWidget(btnAge);
    modeRow->addWidget(btnPol);
    modeRow->addWidget(btnSize);
    modeRow->addStretch();
    m_rootLayout->addLayout(modeRow);

    // Пружина слева прижимает ОК и Отмена к правому краю.
    auto *actionRow = new QHBoxLayout();
    auto *btnOk = new QPushButton(QStringLiteral("ОК"), this);
    auto *btnCancel = new QPushButton(QStringLiteral("Отмена"), this);
    actionRow->addStretch();
    actionRow->addWidget(btnOk);
    actionRow->addWidget(btnCancel);
    m_rootLayout->addLayout(actionRow);

    connect(btnCompact, &QPushButton::clicked, this, &FormWindow::setCompactMode);
    connect(btnComfort, &QPushButton::clicked, this, &FormWindow::setComfortableMode);
    connect(btnDesc, &QPushButton::clicked, this, &FormWindow::toggleDescriptionPolicy);
    connect(btnAge, &QPushButton::clicked, this, &FormWindow::toggleAgePolicy);
    connect(btnPol, &QPushButton::clicked, this, &FormWindow::dumpSizePolicies);
    connect(btnSize, &QPushButton::clicked, this, &FormWindow::dumpSizes);
    connect(btnOk, &QPushButton::clicked, this, &FormWindow::onAccept);
    connect(btnCancel, &QPushButton::clicked, this, &FormWindow::onReject);
}

void FormWindow::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    qDebug() << "[RESIZE] окно ->" << event->size().width() << "x"
             << event->size().height()
             << "| описание:" << m_description.width() << "x"
             << m_description.height();
}

void FormWindow::setCompactMode()
{
    m_rootLayout->setContentsMargins(8, 8, 8, 8);
    m_rootLayout->setSpacing(4);
    qDebug() << "[MARGIN] компактный: margins=8, spacing=4";
}

void FormWindow::setComfortableMode()
{
    m_rootLayout->setContentsMargins(24, 24, 24, 24);
    m_rootLayout->setSpacing(12);
    qDebug() << "[MARGIN] развёрнутый: margins=24, spacing=12";
}

void FormWindow::toggleDescriptionPolicy()
{
    m_descriptionExpanding = !m_descriptionExpanding;
    m_description.setVerticalPolicy(
        m_descriptionExpanding ? QSizePolicy::Expanding : QSizePolicy::Fixed);
    qDebug() << "[TOGGLE] описание вертикально ->"
             << (m_descriptionExpanding ? "Expanding" : "Fixed");
}

void FormWindow::toggleAgePolicy()
{
    m_ageExpanding = !m_ageExpanding;
    m_age.setHorizontalPolicy(
        m_ageExpanding ? QSizePolicy::Expanding : QSizePolicy::Fixed);
    qDebug() << "[TOGGLE] возраст горизонтально ->"
             << (m_ageExpanding ? "Expanding" : "Fixed");
}

QString FormWindow::policyName(QSizePolicy::Policy policy)
{
    switch (policy) {
    case QSizePolicy::Fixed: return QStringLiteral("Fixed");
    case QSizePolicy::Minimum: return QStringLiteral("Minimum");
    case QSizePolicy::Maximum: return QStringLiteral("Maximum");
    case QSizePolicy::Preferred: return QStringLiteral("Preferred");
    case QSizePolicy::Expanding: return QStringLiteral("Expanding");
    case QSizePolicy::MinimumExpanding: return QStringLiteral("MinimumExpanding");
    case QSizePolicy::Ignored: return QStringLiteral("Ignored");
    default: return QStringLiteral("Other");
    }
}

void FormWindow::dumpSizePolicies()
{
    qDebug() << "===== ДАМП ПОЛИТИК =====";
    for (QWidget *widget : m_trackedFields) {
        const QSizePolicy sp = widget->sizePolicy();
        qDebug().noquote() << " " << widget->metaObject()->className()
                           << "horiz=" << policyName(sp.horizontalPolicy())
                           << "vert=" << policyName(sp.verticalPolicy());
    }
    qDebug() << "========================";
}

void FormWindow::dumpSizes()
{
    qDebug() << "===== ДАМП РАЗМЕРОВ (текущий | sizeHint | minHint) =====";
    for (QWidget *widget : m_trackedFields) {
        const QSize hint = widget->sizeHint();
        const QSize minimum = widget->minimumSizeHint();
        qDebug().noquote() << " " << widget->metaObject()->className()
                           << "cur=" << widget->width() << "x" << widget->height()
                           << "hint=" << hint.width() << "x" << hint.height()
                           << "min=" << minimum.width() << "x" << minimum.height();
    }
    qDebug() << "=========================================================";
}

void FormWindow::onAccept()
{
    qDebug() << "[ACTION] ОК: имя=" << m_name.text()
             << "email=" << m_email.text()
             << "возраст=" << m_age.text()
             << "страна=" << m_country->currentText()
             << "согласие=" << m_agree->isChecked();
    close();
}

void FormWindow::onReject()
{
    qDebug() << "[ACTION] Отмена";
    close();
}
