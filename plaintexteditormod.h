#include <QTextEdit>
#include <QMouseEvent>

class MyTextEdit : public QTextEdit {
    Q_OBJECT
public:
    using QTextEdit::QTextEdit; // Usar constructor base

protected:
    void mousePressEvent(QMouseEvent *event) override {
        // Emitir señal personalizada si es necesario
        emit textEditClicked();
        QTextEdit::mousePressEvent(event); // Llamar al comportamiento base
    }

signals:
    void textEditClicked(); // Señal personalizada
};
