#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QQueue>
#include <QString>
#include <QStack>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

enum TokenType { Number, Operator, LeftParen, RightParen };

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void handleDigit();
    void handleOperation();
    void handlePercent();
    void handleInvertSign();
    void handleClear();
    void handlePoint();
    void handleParentheses();
    void calculate();



private:
    Ui::MainWindow *ui;
    QString inputSequence;
    QTimer* longPressTimer;
    bool isLongPress;

    QQueue<QString> m_outputQueue;
    QStack<QString> m_operatorStack;
    QMap<QString, int> m_precedence;
    void backMainMenu();
    void onCalcPressed();
    void onCalcReleased();
    void handleLongPress();
    double applyOperation(double a, double b, QString op);
    bool isOperator(const QString& token);
    bool isNumber(const QString& token);
    QVector<QPair<QString, TokenType>> tokenize(QString expr);
    void parseExpression();
    double evaluateRPN();


};
#endif // MAINWINDOW_H
