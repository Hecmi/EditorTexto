#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

protected:
    void wheelEvent(QWheelEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_nuevoArchivo_click();
    void on_seleccionarArchivo_click();
    void on_guardarArchivo_click();
    void on_guardarComo_click();
    void on_pteEditor_input();

    void on_editorArchivo_textChanged();

private:
    bool guardar_archivo(QString ruta_archivo);
    void cargar_titulo_nombreArchivo(QString ruta);
    void zoomIn();
    void zoomOut();
    void updateFontSize();

    Ui::MainWindow *ui;
    QString ultima_ruta_archivo;
    QString nombre_archivo;
    int tamanio_texto;
    int incremento_texto;
    bool archivo_modificado;
};
#endif // MAINWINDOW_H
