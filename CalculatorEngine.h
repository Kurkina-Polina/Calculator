#include <QObject>
#include <QString>
#include <QQueue>
#include <QStack>
#include <QMap>
#include <QVector>

enum TokenType { Number, Operator, LeftParen, RightParen };

class CalculatorEngine : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString displayText READ displayText NOTIFY displayTextChanged)
    Q_PROPERTY(QString historyText READ historyText NOTIFY historyTextChanged)

public:
    explicit CalculatorEngine(QObject *parent = nullptr);

    QString displayText() const;
    QString historyText() const { return m_historyText; }

public slots:
    void handleDigit(const QString& digit);
    void handleOperation(const QString& operation);
    void handlePercent();
    void handleInvertSign();
    void handleClear();
    void handlePoint();
    void handleParentheses();
    void calculate();

signals:
    void displayTextChanged(const QString &text);
    void historyTextChanged(const QString &text);
    void secretCodeVerified();


private:
    QString m_historyText;
    QString m_displayText;
    QQueue<QString> m_outputQueue;
    QStack<QString> m_operatorStack;
    QMap<QString, int> m_precedence;
    double applyOperation(double a, double b, QString op);
    bool isOperator(const QString& token);
    bool isNumber(const QString& token);
    QVector<QPair<QString, TokenType>> tokenize(QString expr);
    void parseExpression();
    double evaluateRPN();
};
