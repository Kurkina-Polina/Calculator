#include "CalculatorEngine.h"
#include <QRegularExpression>

CalculatorEngine::CalculatorEngine(QObject *parent)
    : QObject(parent), m_displayText("0") {}

QString CalculatorEngine::displayText() const {
    return m_displayText;
}

void CalculatorEngine::handleDigit(const QString &digit) {
    if (m_displayText == "0" || m_displayText == "Error") {
        m_displayText = digit;
    } else {
        m_displayText += digit;
    }
    emit displayTextChanged(m_displayText);

}

void CalculatorEngine::handleOperation(const QString &op) {
    QString actualOp = op;

    if (op == "mult") actualOp = "*";
    else if (op == "devide") actualOp = "/";
    else if (op == "minus") actualOp = "-";
    else if (op == "plus") actualOp = "+";

    if (!m_displayText.isEmpty()) {
        QChar lastChar = m_displayText.back();
        if (isOperator(lastChar) && (actualOp != "-")) {
            return;
        }
    }

    m_displayText += actualOp;
    emit displayTextChanged(m_displayText);
}


void CalculatorEngine::handlePercent() {

    QRegularExpression rx("(-?\\d+\\.?\\d*)(?!.*\\d)");

    QRegularExpressionMatch match = rx.match(m_displayText);
    if (match.hasMatch()) {
        QString lastNumberStr = match.captured(1);
        double lastNumber = lastNumberStr.toDouble();
        double percentValue = lastNumber / 100.0;

        m_displayText.replace(match.capturedStart(1), lastNumberStr.length(),
                            QString::number(percentValue, 'g', 10));

        emit displayTextChanged(m_displayText);
    }
}

void CalculatorEngine::handleParentheses(){
    if (m_displayText == "0") {
        m_displayText = "(";
        emit displayTextChanged(m_displayText);
        return;
    }

    int openCount = m_displayText.count('(');
    int closeCount = m_displayText.count(')');

    if (openCount <= closeCount) {
        if(isNumber(m_displayText.back())){
            return;
        }
        m_displayText += "(";
    } else {
        QChar lastChar = m_displayText.back();
        if (isOperator(lastChar)) {
            return;
        }
        m_displayText += ")";
    }
    emit displayTextChanged(m_displayText);
}

void CalculatorEngine::handleInvertSign() {
    QRegularExpression rx("(-?\\d+\\.?\\d*)(?!.*\\d)");

    QRegularExpressionMatch match = rx.match(m_displayText);
    if (match.hasMatch()) {
        QString lastNumberStr = match.captured(1);
        double lastNumber = lastNumberStr.toDouble();
        double percentValue = -lastNumber;

        m_displayText.replace(match.capturedStart(1), lastNumberStr.length(),
                            QString::number(percentValue, 'g', 10));

        emit displayTextChanged(m_displayText);
    }
}

void CalculatorEngine::handleClear() {
    if (!m_displayText.isEmpty()) {
        emit historyTextChanged(m_displayText);
    }

    m_displayText = "0";
    emit displayTextChanged(m_displayText);
}


void CalculatorEngine::handlePoint() {
    QRegularExpression rx("(-?\\d+\\.?\\d*)(?!.*\\d)");

    QRegularExpressionMatch match = rx.match(m_displayText);
    if (match.hasMatch()) {
        QString lastNumberStr = match.captured(1);
        if (!lastNumberStr.contains('.')) {
            m_displayText += ".";
        }
    }
    emit displayTextChanged(m_displayText);
}

double CalculatorEngine::applyOperation(double a, double b, QString op) {
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
bool CalculatorEngine::isOperator(const QString& token) {
    return token == "+" || token == "-" || token == "*" || token == "/" || token == "×" || token == "÷";
}

bool  CalculatorEngine::isNumber(const QString& token) {
    bool ok;
    token.toDouble(&ok);
    return ok;
}

QVector<QPair<QString, TokenType>> CalculatorEngine::tokenize( QString expr) {
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

void CalculatorEngine::parseExpression() {
    auto tokens = tokenize(m_displayText);

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

double CalculatorEngine::evaluateRPN() {
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


void CalculatorEngine::calculate() {

    parseExpression();
    double result = evaluateRPN();
    handleClear();
    if (!qIsNaN(result)) {
        m_displayText = QString::number(result);
    } else {
        m_displayText = "Error";
    }

    emit displayTextChanged(m_displayText);

}
