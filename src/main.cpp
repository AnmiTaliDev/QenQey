#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QCheckBox>
#include <QGroupBox>
#include <QTextEdit>
#include <QClipboard>
#include <QMessageBox>
#include <QProgressBar>
#include <QSlider>
#include <QStatusBar>
#include <QMenuBar>
#include <QAction>
#include <QIcon>
#include <QFont>
#include <QRandomGenerator>
#include <QDateTime>
#include <QTextCursor>
#include <QKeySequence>
#include <QShortcut>

class QenQeyMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QenQeyMainWindow(QWidget *parent = nullptr);
    ~QenQeyMainWindow();

private slots:
    void generatePassword();
    void copyToClipboard();
    void lengthSliderChanged(int value);
    void lengthSpinChanged(int value);
    void updatePasswordStrength();
    void showAbout();
    void clearPassword();
    void onCharacterTypeChanged();

private:
    void setupUI();
    void setupMenuBar();
    void setupShortcuts();
    QString generateRandomPassword();
    int calculatePasswordStrength(const QString &password);
    void updateStrengthIndicator(int strength);

    
    // UI Elements
    QWidget *m_centralWidget;
    QLineEdit *m_passwordLineEdit;
    QPushButton *m_generateButton;
    QPushButton *m_copyButton;
    QPushButton *m_clearButton;
    QSpinBox *m_lengthSpinBox;
    QSlider *m_lengthSlider;
    QCheckBox *m_uppercaseCheckBox;
    QCheckBox *m_lowercaseCheckBox;
    QCheckBox *m_numbersCheckBox;
    QCheckBox *m_symbolsCheckBox;
    QCheckBox *m_excludeSimilarCheckBox;
    QProgressBar *m_strengthBar;
    QLabel *m_strengthLabel;
    QTextEdit *m_historyTextEdit;
    QLabel *m_lengthLabel;
    
    // Menu and actions
    QAction *m_aboutAction;
    QAction *m_exitAction;
    QAction *m_generateAction;
    QAction *m_copyAction;
    QAction *m_clearAction;
    
    // Password generation settings
    static const QString UPPERCASE_CHARS;
    static const QString LOWERCASE_CHARS;
    static const QString NUMBER_CHARS;
    static const QString SYMBOL_CHARS;
    static const QString SIMILAR_CHARS;
};

const QString QenQeyMainWindow::UPPERCASE_CHARS = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const QString QenQeyMainWindow::LOWERCASE_CHARS = "abcdefghijklmnopqrstuvwxyz";
const QString QenQeyMainWindow::NUMBER_CHARS = "0123456789";
const QString QenQeyMainWindow::SYMBOL_CHARS = "!@#$%^&*()_+-=[]{}|;:,.<>?";
const QString QenQeyMainWindow::SIMILAR_CHARS = "il1Lo0O";

QenQeyMainWindow::QenQeyMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_centralWidget(nullptr)
{
    setWindowTitle("QenQey - Генератор паролей");
    setWindowIcon(QIcon::fromTheme("preferences-system-privacy"));
    setMinimumSize(500, 650);
    resize(600, 750);
    
    setupMenuBar();
    setupUI();
    setupShortcuts();
    
    // Connect signals
    connect(m_generateButton, &QPushButton::clicked, this, &QenQeyMainWindow::generatePassword);
    connect(m_copyButton, &QPushButton::clicked, this, &QenQeyMainWindow::copyToClipboard);
    connect(m_clearButton, &QPushButton::clicked, this, &QenQeyMainWindow::clearPassword);
    connect(m_lengthSlider, &QSlider::valueChanged, this, &QenQeyMainWindow::lengthSliderChanged);
    connect(m_lengthSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &QenQeyMainWindow::lengthSpinChanged);
    connect(m_passwordLineEdit, &QLineEdit::textChanged, this, &QenQeyMainWindow::updatePasswordStrength);
    
    // Connect character type checkboxes
    connect(m_uppercaseCheckBox, &QCheckBox::toggled, this, &QenQeyMainWindow::onCharacterTypeChanged);
    connect(m_lowercaseCheckBox, &QCheckBox::toggled, this, &QenQeyMainWindow::onCharacterTypeChanged);
    connect(m_numbersCheckBox, &QCheckBox::toggled, this, &QenQeyMainWindow::onCharacterTypeChanged);
    connect(m_symbolsCheckBox, &QCheckBox::toggled, this, &QenQeyMainWindow::onCharacterTypeChanged);
    connect(m_excludeSimilarCheckBox, &QCheckBox::toggled, this, &QenQeyMainWindow::onCharacterTypeChanged);
    
    // Set initial values
    m_lengthSlider->setValue(16);
    m_lengthSpinBox->setValue(16);
    m_lengthLabel->setText("16");
    m_uppercaseCheckBox->setChecked(true);
    m_lowercaseCheckBox->setChecked(true);
    m_numbersCheckBox->setChecked(true);
    m_symbolsCheckBox->setChecked(false);
    m_excludeSimilarCheckBox->setChecked(false);
    
    // Generate initial password
    generatePassword();
}

QenQeyMainWindow::~QenQeyMainWindow()
{
}

void QenQeyMainWindow::setupMenuBar()
{
    QMenuBar *menuBar = this->menuBar();
    
    // Файл menu
    QMenu *fileMenu = menuBar->addMenu("&Файл");
    
    m_generateAction = new QAction("&Сгенерировать пароль", this);
    m_generateAction->setShortcut(QKeySequence("Ctrl+G"));
    m_generateAction->setIcon(QIcon::fromTheme("view-refresh"));
    connect(m_generateAction, &QAction::triggered, this, &QenQeyMainWindow::generatePassword);
    fileMenu->addAction(m_generateAction);
    
    m_copyAction = new QAction("&Копировать пароль", this);
    m_copyAction->setShortcut(QKeySequence::Copy);
    m_copyAction->setIcon(QIcon::fromTheme("edit-copy"));
    connect(m_copyAction, &QAction::triggered, this, &QenQeyMainWindow::copyToClipboard);
    fileMenu->addAction(m_copyAction);
    
    fileMenu->addSeparator();
    
    m_clearAction = new QAction("&Очистить историю", this);
    m_clearAction->setShortcut(QKeySequence("Ctrl+L"));
    m_clearAction->setIcon(QIcon::fromTheme("edit-clear"));
    connect(m_clearAction, &QAction::triggered, this, &QenQeyMainWindow::clearPassword);
    fileMenu->addAction(m_clearAction);
    
    fileMenu->addSeparator();
    
    m_exitAction = new QAction("&Выход", this);
    m_exitAction->setShortcut(QKeySequence::Quit);
    m_exitAction->setIcon(QIcon::fromTheme("application-exit"));
    connect(m_exitAction, &QAction::triggered, this, &QWidget::close);
    fileMenu->addAction(m_exitAction);
    
    // Помощь menu
    QMenu *helpMenu = menuBar->addMenu("&Помощь");
    
    m_aboutAction = new QAction("&О программе", this);
    m_aboutAction->setIcon(QIcon::fromTheme("help-about"));
    connect(m_aboutAction, &QAction::triggered, this, &QenQeyMainWindow::showAbout);
    helpMenu->addAction(m_aboutAction);
}

void QenQeyMainWindow::setupShortcuts()
{
    // Additional shortcuts
    QShortcut *f5Shortcut = new QShortcut(QKeySequence::Refresh, this);
    connect(f5Shortcut, &QShortcut::activated, this, &QenQeyMainWindow::generatePassword);
    
    QShortcut *enterShortcut = new QShortcut(QKeySequence(Qt::Key_Return), this);
    connect(enterShortcut, &QShortcut::activated, this, &QenQeyMainWindow::generatePassword);
}

void QenQeyMainWindow::setupUI()
{
    m_centralWidget = new QWidget;
    setCentralWidget(m_centralWidget);
    
    QVBoxLayout *mainLayout = new QVBoxLayout(m_centralWidget);
    mainLayout->setSpacing(16);
    mainLayout->setContentsMargins(16, 16, 16, 16);
    
    // Title section
    QWidget *titleWidget = new QWidget;
    QVBoxLayout *titleLayout = new QVBoxLayout(titleWidget);
    titleLayout->setSpacing(4);
    titleLayout->setContentsMargins(0, 0, 0, 0);
    
    QLabel *titleLabel = new QLabel("QenQey");
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(28);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLayout->addWidget(titleLabel);
    
    QLabel *subtitleLabel = new QLabel("Генератор безопасных паролей");
    subtitleLabel->setAlignment(Qt::AlignCenter);
    subtitleLabel->setStyleSheet("color: #666; font-size: 14px; margin-bottom: 8px;");
    titleLayout->addWidget(subtitleLabel);
    
    mainLayout->addWidget(titleWidget);
    
    // Password output section
    QGroupBox *passwordGroup = new QGroupBox("Сгенерированный пароль");
    QVBoxLayout *passwordLayout = new QVBoxLayout(passwordGroup);
    passwordLayout->setSpacing(12);
    
    m_passwordLineEdit = new QLineEdit;
    m_passwordLineEdit->setReadOnly(true);
    m_passwordLineEdit->setFont(QFont("monospace", 14));
    m_passwordLineEdit->setMinimumHeight(45);
    m_passwordLineEdit->setStyleSheet("QLineEdit { padding: 8px; border: 2px solid #ddd; border-radius: 4px; }");
    passwordLayout->addWidget(m_passwordLineEdit);
    
    // Password strength indicator
    QHBoxLayout *strengthLayout = new QHBoxLayout;
    strengthLayout->setSpacing(8);
    
    QLabel *strengthTextLabel = new QLabel("Сила пароля:");
    strengthTextLabel->setMinimumWidth(80);
    strengthLayout->addWidget(strengthTextLabel);
    
    m_strengthBar = new QProgressBar;
    m_strengthBar->setRange(0, 100);
    m_strengthBar->setTextVisible(false);
    m_strengthBar->setMinimumHeight(20);
    strengthLayout->addWidget(m_strengthBar);
    
    m_strengthLabel = new QLabel("Отлично");
    m_strengthLabel->setMinimumWidth(80);
    m_strengthLabel->setAlignment(Qt::AlignCenter);
    strengthLayout->addWidget(m_strengthLabel);
    
    passwordLayout->addLayout(strengthLayout);
    
    // Action buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->setSpacing(8);
    
    m_generateButton = new QPushButton("Сгенерировать");
    m_generateButton->setDefault(true);
    m_generateButton->setMinimumHeight(40);
    m_generateButton->setIcon(QIcon::fromTheme("view-refresh"));
    
    m_copyButton = new QPushButton("Копировать");
    m_copyButton->setMinimumHeight(40);
    m_copyButton->setIcon(QIcon::fromTheme("edit-copy"));
    
    m_clearButton = new QPushButton("Очистить");
    m_clearButton->setMinimumHeight(40);
    m_clearButton->setIcon(QIcon::fromTheme("edit-clear"));
    
    buttonLayout->addWidget(m_generateButton);
    buttonLayout->addWidget(m_copyButton);
    buttonLayout->addWidget(m_clearButton);
    
    passwordLayout->addLayout(buttonLayout);
    mainLayout->addWidget(passwordGroup);
    
    // Settings section
    QGroupBox *settingsGroup = new QGroupBox("Настройки генерации");
    QVBoxLayout *settingsLayout = new QVBoxLayout(settingsGroup);
    settingsLayout->setSpacing(12);
    
    // Length settings
    QHBoxLayout *lengthLayout = new QHBoxLayout;
    lengthLayout->setSpacing(8);
    
    QLabel *lengthTextLabel = new QLabel("Длина пароля:");
    lengthTextLabel->setMinimumWidth(100);
    lengthLayout->addWidget(lengthTextLabel);
    
    m_lengthSpinBox = new QSpinBox;
    m_lengthSpinBox->setRange(4, 128);
    m_lengthSpinBox->setValue(16);
    m_lengthSpinBox->setMinimumWidth(60);
    lengthLayout->addWidget(m_lengthSpinBox);
    
    m_lengthSlider = new QSlider(Qt::Horizontal);
    m_lengthSlider->setRange(4, 128);
    m_lengthSlider->setValue(16);
    lengthLayout->addWidget(m_lengthSlider);
    
    m_lengthLabel = new QLabel("16");
    m_lengthLabel->setMinimumWidth(30);
    m_lengthLabel->setAlignment(Qt::AlignCenter);
    lengthLayout->addWidget(m_lengthLabel);
    
    settingsLayout->addLayout(lengthLayout);
    
    // Character type checkboxes
    QGridLayout *checkboxLayout = new QGridLayout;
    checkboxLayout->setSpacing(8);
    
    m_uppercaseCheckBox = new QCheckBox("Заглавные буквы (A-Z)");
    m_lowercaseCheckBox = new QCheckBox("Строчные буквы (a-z)");
    m_numbersCheckBox = new QCheckBox("Цифры (0-9)");
    m_symbolsCheckBox = new QCheckBox("Символы (!@#$%^&*)");
    m_excludeSimilarCheckBox = new QCheckBox("Исключить похожие символы (il1Lo0O)");
    
    checkboxLayout->addWidget(m_uppercaseCheckBox, 0, 0);
    checkboxLayout->addWidget(m_lowercaseCheckBox, 1, 0);
    checkboxLayout->addWidget(m_numbersCheckBox, 0, 1);
    checkboxLayout->addWidget(m_symbolsCheckBox, 1, 1);
    checkboxLayout->addWidget(m_excludeSimilarCheckBox, 2, 0, 1, 2);
    
    settingsLayout->addLayout(checkboxLayout);
    mainLayout->addWidget(settingsGroup);
    
    // History section
    QGroupBox *historyGroup = new QGroupBox("История паролей");
    QVBoxLayout *historyLayout = new QVBoxLayout(historyGroup);
    
    m_historyTextEdit = new QTextEdit;
    m_historyTextEdit->setMaximumHeight(120);
    m_historyTextEdit->setReadOnly(true);
    m_historyTextEdit->setFont(QFont("monospace", 9));
    m_historyTextEdit->setStyleSheet("QTextEdit { border: 1px solid #ddd; border-radius: 4px; }");
    historyLayout->addWidget(m_historyTextEdit);
    
    mainLayout->addWidget(historyGroup);
    
    // Status bar
    statusBar()->showMessage("Готов к генерации паролей");
    statusBar()->setStyleSheet("QStatusBar { border-top: 1px solid #ddd; }");
}

void QenQeyMainWindow::generatePassword()
{
    QString password = generateRandomPassword();
    
    if (password.isEmpty()) {
        QMessageBox::warning(this, "Предупреждение", 
                           "Необходимо выбрать хотя бы один тип символов!");
        return;
    }
    
    m_passwordLineEdit->setText(password);
    
    // Add to history
    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
    QString historyEntry = QString("[%1] %2 (длина: %3)")
                          .arg(timestamp, password, QString::number(password.length()));
    m_historyTextEdit->append(historyEntry);
    
    // Scroll to bottom
    QTextCursor cursor = m_historyTextEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    m_historyTextEdit->setTextCursor(cursor);
    
    statusBar()->showMessage("Пароль сгенерирован", 3000);
}

QString QenQeyMainWindow::generateRandomPassword()
{
    QString charset;
    
    if (m_uppercaseCheckBox->isChecked()) {
        charset += UPPERCASE_CHARS;
    }
    if (m_lowercaseCheckBox->isChecked()) {
        charset += LOWERCASE_CHARS;
    }
    if (m_numbersCheckBox->isChecked()) {
        charset += NUMBER_CHARS;
    }
    if (m_symbolsCheckBox->isChecked()) {
        charset += SYMBOL_CHARS;
    }
    
    if (m_excludeSimilarCheckBox->isChecked()) {
        for (const QChar &c : SIMILAR_CHARS) {
            charset.remove(c);
        }
    }
    
    if (charset.isEmpty()) {
        return QString();
    }
    
    QString password;
    int length = m_lengthSpinBox->value();
    
    // Ensure we have at least one character from each selected type
    QStringList requiredSets;
    if (m_uppercaseCheckBox->isChecked()) {
        QString set = UPPERCASE_CHARS;
        if (m_excludeSimilarCheckBox->isChecked()) {
            for (const QChar &c : SIMILAR_CHARS) {
                set.remove(c);
            }
        }
        requiredSets.append(set);
    }
    if (m_lowercaseCheckBox->isChecked()) {
        QString set = LOWERCASE_CHARS;
        if (m_excludeSimilarCheckBox->isChecked()) {
            for (const QChar &c : SIMILAR_CHARS) {
                set.remove(c);
            }
        }
        requiredSets.append(set);
    }
    if (m_numbersCheckBox->isChecked()) {
        QString set = NUMBER_CHARS;
        if (m_excludeSimilarCheckBox->isChecked()) {
            for (const QChar &c : SIMILAR_CHARS) {
                set.remove(c);
            }
        }
        requiredSets.append(set);
    }
    if (m_symbolsCheckBox->isChecked()) {
        requiredSets.append(SYMBOL_CHARS);
    }
    
    // Add required characters first
    for (const QString &set : requiredSets) {
        if (!set.isEmpty() && password.length() < length) {
            int index = QRandomGenerator::global()->bounded(set.length());
            password.append(set.at(index));
        }
    }
    
    // Fill remaining positions randomly
    for (int i = password.length(); i < length; ++i) {
        int index = QRandomGenerator::global()->bounded(charset.length());
        password.append(charset.at(index));
    }
    
    // Shuffle the password to avoid predictable patterns
    for (int i = password.length() - 1; i > 0; --i) {
        int j = QRandomGenerator::global()->bounded(i + 1);
        QChar temp = password[i];
        password[i] = password[j];
        password[j] = temp;
    }
    
    return password;
}

void QenQeyMainWindow::copyToClipboard()
{
    QString password = m_passwordLineEdit->text();
    if (password.isEmpty()) {
        QMessageBox::information(this, "Информация", "Нет пароля для копирования!");
        return;
    }
    
    QApplication::clipboard()->setText(password);
    statusBar()->showMessage("Пароль скопирован в буфер обмена", 3000);
}

void QenQeyMainWindow::clearPassword()
{
    m_passwordLineEdit->clear();
    m_historyTextEdit->clear();
    statusBar()->showMessage("История очищена", 2000);
}

void QenQeyMainWindow::lengthSliderChanged(int value)
{
    m_lengthSpinBox->setValue(value);
    m_lengthLabel->setText(QString::number(value));
}

void QenQeyMainWindow::lengthSpinChanged(int value)
{
    m_lengthSlider->setValue(value);
    m_lengthLabel->setText(QString::number(value));
}

void QenQeyMainWindow::onCharacterTypeChanged()
{
    // Auto-generate new password when character types change
    if (!m_passwordLineEdit->text().isEmpty()) {
        generatePassword();
    }
}

void QenQeyMainWindow::updatePasswordStrength()
{
    QString password = m_passwordLineEdit->text();
    int strength = calculatePasswordStrength(password);
    updateStrengthIndicator(strength);
}

int QenQeyMainWindow::calculatePasswordStrength(const QString &password)
{
    if (password.isEmpty()) {
        return 0;
    }
    
    int score = 0;
    int length = password.length();
    
    // Length scoring
    if (length >= 8) score += 20;
    if (length >= 12) score += 15;
    if (length >= 16) score += 10;
    if (length >= 20) score += 5;
    
    // Character variety bonus
    bool hasUpper = false, hasLower = false, hasDigit = false, hasSymbol = false;
    
    for (const QChar &c : password) {
        if (c.isUpper()) hasUpper = true;
        else if (c.isLower()) hasLower = true;
        else if (c.isDigit()) hasDigit = true;
        else hasSymbol = true;
    }
    
    int varietyCount = 0;
    if (hasUpper) { score += 10; varietyCount++; }
    if (hasLower) { score += 10; varietyCount++; }
    if (hasDigit) { score += 10; varietyCount++; }
    if (hasSymbol) { score += 15; varietyCount++; }
    
    // Bonus for using multiple character types
    if (varietyCount >= 3) score += 10;
    if (varietyCount == 4) score += 10;
    
    return qMin(score, 100);
}

void QenQeyMainWindow::updateStrengthIndicator(int strength)
{
    m_strengthBar->setValue(strength);
    
    QString strengthText;
    QString styleSheet;
    
    if (strength < 25) {
        strengthText = "Очень слабый";
        styleSheet = "QProgressBar::chunk { background-color: #d32f2f; }";
    } else if (strength < 50) {
        strengthText = "Слабый";
        styleSheet = "QProgressBar::chunk { background-color: #f57c00; }";
    } else if (strength < 75) {
        strengthText = "Средний";
        styleSheet = "QProgressBar::chunk { background-color: #fbc02d; }";
    } else if (strength < 90) {
        strengthText = "Хороший";
        styleSheet = "QProgressBar::chunk { background-color: #689f38; }";
    } else {
        strengthText = "Отличный";
        styleSheet = "QProgressBar::chunk { background-color: #388e3c; }";
    }
    
    m_strengthLabel->setText(strengthText);
    m_strengthBar->setStyleSheet(styleSheet);
}

void QenQeyMainWindow::showAbout()
{
    QMessageBox::about(this, "О программе QenQey",
                      "<h3>QenQey - Генератор паролей</h3>"
                      "<p><b>Версия:</b> 1.0.0</p>"
                      "<p><b>Описание:</b> Безопасный генератор паролей с настраиваемыми параметрами.</p>"
                      "<p>Создано с использованием Qt C++ и следованием принципам KDE HIG.</p>"
                      "<p><b>Возможности:</b></p>"
                      "<ul>"
                      "<li>Настраиваемая длина пароля (4-128 символов)</li>"
                      "<li>Выбор типов символов</li>"
                      "<li>Оценка силы пароля</li>"
                      "<li>История генерации</li>"
                      "<li>Горячие клавиши</li>"
                      "</ul>"
                      "<p><b>Горячие клавиши:</b></p>"
                      "<ul>"
                      "<li>Ctrl+G - Сгенерировать пароль</li>"
                      "<li>Ctrl+C - Копировать пароль</li>"
                      "<li>Ctrl+L - Очистить историю</li>"
                      "<li>F5 - Обновить пароль</li>"
                      "</ul>");
}

#include "main.moc"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    app.setApplicationName("QenQey");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("QenQey Project");
    app.setOrganizationDomain("qenqey.org");
    
    QenQeyMainWindow window;
    window.show();
    
    return app.exec();
}