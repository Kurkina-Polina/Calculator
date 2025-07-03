#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QRegularExpression>
#include <QTabBar>
#include <QTabWidget>
#include <QFile>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tabWidget->tabBar()->hide();
    ui->tabWidget->setCurrentIndex(0);

    for (int i = 0; i < 10; ++i) {
        QString btnName = "btn_" + QString::number(i);
        connect(findChild<QPushButton*>(btnName), &QPushButton::clicked,
                this, &MainWindow::handleDigit);
    }

    QStringList ops = {"plus", "minus", "mult", "devide"};
    foreach (QString op, ops) {
        connect(findChild<QPushButton*>("btn_" + op), &QPushButton::clicked,
                this, &MainWindow::handleOperation);
    }

    connect(ui->btn_percent, &QPushButton::clicked, this, &MainWindow::handlePercent);
    connect(ui->btn_parentheses, &QPushButton::clicked, this, &MainWindow::handleParentheses);
    connect(ui->btn_invert, &QPushButton::clicked, this, &MainWindow::handleInvertSign);
    connect(ui->btn_C, &QPushButton::clicked, this, &MainWindow::handleClear);
    connect(ui->btn_point, &QPushButton::clicked, this, &MainWindow::handlePoint);
    connect(ui->btn_calculate, &QPushButton::clicked, this, &MainWindow::calculate);
    connect(ui->btn_calculate, &QPushButton::pressed, this, &MainWindow::onCalcPressed);
    connect(ui->btn_calculate, &QPushButton::released, this, &MainWindow::onCalcReleased);
    connect(ui->btn_back, &QPushButton::released, this, &MainWindow::backMainMenu);


    m_precedence["+"] = 1;
    m_precedence["-"] = 1;
    m_precedence["*"] = 2;
    m_precedence["/"] = 2;

    inputSequence = "";
    isLongPress = false;
    longPressTimer = new QTimer(this);
    longPressTimer->setInterval(4000);
    longPressTimer->setSingleShot(true);

    connect(longPressTimer, &QTimer::timeout, this, &MainWindow::handleLongPress);

    int fontId = QFontDatabase::addApplicationFont(":/fonts/OpenSans-SemiBold.ttf");
    if (fontId == -1) {
        qWarning() << "Font loading error!";
    }
    QFile styleFile(":/styles.qss");
    styleFile.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(styleFile.readAll());
    this->setStyleSheet(styleSheet);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::backMainMenu(){
    ui->tabWidget->setCurrentIndex(0);
}

void MainWindow::handleDigit() {
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    QString currentText = ui->lineEdit->text();
    if (currentText == "0") {
        currentText = "";
    }
    if (isLongPress) {
        inputSequence += button->text();
        if (inputSequence == "123"){
            ui->tabWidget->setCurrentIndex(1);
        }
    }
    else{
        isLongPress = false;
        inputSequence = "";
    }
    ui->lineEdit->setText(currentText + button->text());
}

void MainWindow::handleOperation() {
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    QString op = button->text();
    if (op == "×") {
        op = "*";
    } else if ( op == "÷") {
        op = "/";
    }
    QString lastChar = ui->lineEdit->text().back();
    if(isOperator(lastChar) && (op != "-")){
        return;
    }

    ui->lineEdit->setText(ui->lineEdit->text() + op);
}


void MainWindow::handlePercent() {
    QString currentText = ui->lineEdit->text();
    QRegularExpression rx("(-?\\d+\\.?\\d*)(?!.*\\d)");

    QRegularExpressionMatch match = rx.match(currentText);
    if (match.hasMatch()) {
        QString lastNumberStr = match.captured(1);
        double lastNumber = lastNumberStr.toDouble();
        double percentValue = lastNumber / 100.0;

        currentText.replace(match.capturedStart(1), lastNumberStr.length(),
                            QString::number(percentValue, 'g', 10));

        ui->lineEdit->setText(currentText);
    }
}

void MainWindow::handleParentheses(){
    QString text = ui->lineEdit->text();
    if (text == "0") {
        text = "";
    }

    int openCount = text.count('(');
    int closeCount = text.count(')');

    if (openCount <= closeCount) {
        if(isNumber(ui->lineEdit->text().back())){
            return;
        }
        text += "(";
    } else {
        QChar lastChar = text.back();
        if (isOperator(lastChar)) {
            return;
        }
        text += ")";
    }
    ui->lineEdit->setText(text);
}

void MainWindow::handleInvertSign() {
    QString currentText = ui->lineEdit->text();
    QRegularExpression rx("(-?\\d+\\.?\\d*)(?!.*\\d)");

    QRegularExpressionMatch match = rx.match(currentText);
    if (match.hasMatch()) {
        QString lastNumberStr = match.captured(1);
        double lastNumber = lastNumberStr.toDouble();
        double percentValue = -lastNumber;

        currentText.replace(match.capturedStart(1), lastNumberStr.length(),
                            QString::number(percentValue, 'g', 10));

        ui->lineEdit->setText(currentText);
    }
}

void MainWindow::handleClear() {
    if (!ui->lineEdit->text().isEmpty()) {
        ui->display_hitory->setText(ui->lineEdit->text());
    }

    ui->lineEdit->clear();
    QString text = "0";
    ui->lineEdit->setText(text);
}


void MainWindow::handlePoint() {
    QString currentText = ui->lineEdit->text();
    QRegularExpression rx("(-?\\d+\\.?\\d*)(?!.*\\d)");

    QRegularExpressionMatch match = rx.match(currentText);
    if (match.hasMatch()) {
        QString lastNumberStr = match.captured(1);
        if (!lastNumberStr.contains('.')) {
            ui->lineEdit->setText(ui->lineEdit->text() + ".");
        }
    }

}

double MainWindow::applyOperation(double a, double b, QString op) {
    if (op.isEmpty()) {
        qWarning() << "Empty operator";
        return std::numeric_limits<double>::quiet_NaN();
    }

    switch (op[0].toLatin1()) {
    case '+':
        return a + b;

    case '-':
        return a - b;

    case '*':
        return a * b;

    case '/':
        if (b == 0.0) {
            qWarning() << "Division by zero:" << a << "/" << b;
            return std::numeric_limits<double>::quiet_NaN();
        }
        return a / b;

    default:
        qWarning() << "Invalid operator:" << op;
        return std::numeric_limits<double>::quiet_NaN();
    }
}
bool MainWindow::isOperator(const QString& token) {
    return token == "+" || token == "-" || token == "*" || token == "/" || token == "×" || token == "÷";
}

bool  MainWindow::isNumber(const QString& token) {
    bool ok;
    token.toDouble(&ok);
    return ok;
}

QVector<QPair<QString, TokenType>> MainWindow::tokenize( QString expr) {
    QVector<QPair<QString, TokenType>> tokens;
    QString buffer;
    bool prevWasOperator = true;

    for (QChar ch : expr) {
        if (ch.isSpace()) continue;

        if (ch.isDigit() || ch == '.') {
            buffer += ch;
            continue;
        }

        if (!buffer.isEmpty()) {
            tokens.append({buffer, Number});
            buffer.clear();
            prevWasOperator = false;
        }

        if (ch == '-' && prevWasOperator) {
            if (!buffer.isEmpty()) {
                tokens.append({buffer, Number});
                buffer.clear();
            }
            buffer += ch;
            prevWasOperator = false;
            continue;
        }

        if (ch == '('){
            tokens.append({QString(ch), LeftParen});
            prevWasOperator = true;
        }

        else if (ch == ')'){
            tokens.append({QString(ch), RightParen});
            prevWasOperator = false;
        }

        else if (isOperator(ch)){
            prevWasOperator = true;
            tokens.append({QString(ch), Operator});
        }
    }

    if (!buffer.isEmpty())
        tokens.append({buffer, Number});

    return tokens;
}

void MainWindow::parseExpression() {
    auto tokens = tokenize(ui->lineEdit->text());

    for (auto& [token, type] : tokens) {
        switch (type) {
        case Number:
            m_outputQueue.enqueue(token);
            break;

        case Operator: {
            while (!m_operatorStack.isEmpty() &&
                   m_operatorStack.top() != "(" &&
                   m_precedence[m_operatorStack.top()] >= m_precedence[token]) {
                m_outputQueue.enqueue(m_operatorStack.pop());
            }
            m_operatorStack.push(token);
            break;
        }

        case LeftParen:
            m_operatorStack.push(token);
            break;

        case RightParen:
            while (!m_operatorStack.isEmpty() && m_operatorStack.top() != "(") {
                m_outputQueue.enqueue(m_operatorStack.pop());
            }
            if (m_operatorStack.isEmpty()) {
                qDebug() << "Mismatched parentheses";
                return;
            }
            m_operatorStack.pop();
            break;
        }
    }

    while (!m_operatorStack.isEmpty()) {
        if (m_operatorStack.top() == "(") {
            qDebug() << "Mismatched parentheses";
            return;
        }
        m_outputQueue.enqueue(m_operatorStack.pop());
    }
}

double MainWindow::evaluateRPN() {
    QStack<double> evalStack;

    while (!m_outputQueue.isEmpty()) {
        QString token = m_outputQueue.dequeue();

        if (isNumber(token)) {
            double num = token.toDouble();
            evalStack.push(num);
        }
        else if (isOperator(token)) {
            if(evalStack.size() < 2){
                qWarning() << "Invalid numbers: less than 2" << token;
                return NAN;
            }
            double b = evalStack.pop();
            double a = evalStack.pop();
            evalStack.push(applyOperation(a, b, token));
        }
        else {
            qWarning() << "Invalid token:" << token;
            return NAN;
        }

    }

    return evalStack.pop();
}

void MainWindow::onCalcPressed() {
    longPressTimer->start();
}

void MainWindow::onCalcReleased() {
    if (longPressTimer->isActive()) {
        longPressTimer->stop();
    }
}

void MainWindow::handleLongPress() {
    isLongPress = true;
}


void MainWindow::calculate() {

    parseExpression();
    double result = evaluateRPN();
    handleClear();
    QString text;
    if (!qIsNaN(result)) {
        text = QString::number(result);
    } else {
        text = "Error";
    }

    ui->lineEdit->setText(text);

}
