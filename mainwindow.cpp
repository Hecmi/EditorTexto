#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "qpushbutton.h"
#include "qshortcut.h"
#include "qfiledialog.h"
#include "qmessagebox.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , ultima_ruta_archivo("")
    , tamanio_texto(12)
    , incremento_texto(2)
    , archivo_modificado(false)
    , nombre_archivo("Sín título")
{
    //Establecer la configuración de la interfaz de usuario y el título de la ventana
    ui->setupUi(this);
    setWindowTitle(tr("%1: Editor de texto").arg(nombre_archivo));

    //Estilos de los widgets
    ui->pteEditor->setStyleSheet("QPlainTextEdit { border: none; }");

    //Definir los eventos para los widgets
    connect(ui->mbSeleccionarArchivo, &QAction::triggered, this, &MainWindow::on_seleccionarArchivo_click);
    connect(ui->mbGuardarArchivo, &QAction::triggered, this, &MainWindow::on_guardarArchivo_click);
    connect(ui->mbGuardarComo, &QAction::triggered, this, &MainWindow::on_guardarComo_click);
    connect(ui->mbNuevoArchivo, &QAction::triggered, this, &MainWindow::on_nuevoArchivo_click);

    connect(ui->mbAumentarZoom, &QAction::triggered, this, &MainWindow::zoomIn);
    connect(ui->mbDisminuirZoom, &QAction::triggered, this, &MainWindow::zoomOut);

    connect(ui->pteEditor, &QPlainTextEdit::textChanged, this, &MainWindow::on_editorArchivo_textChanged);

    //Eventos para combinación de teclas (atajos)
    QShortcut *shortcut_guardar = new QShortcut(QKeySequence("Ctrl+S"), this);
    QShortcut *shortcut_guardarComo = new QShortcut(QKeySequence("Ctrl+Shift+S"), this);
    QShortcut *shortcut_nuevoArchivo = new QShortcut(QKeySequence("Ctrl+N"), this);
    QShortcut *shortcut_abrirArchivo = new QShortcut(QKeySequence("Ctrl+O"), this);

    QShortcut *shortcut_aumentarZoom = new QShortcut(QKeySequence("Ctrl++"), this);
    QShortcut *shortcut_disminuirZoom = new QShortcut(QKeySequence("Ctrl+-"), this);

    connect(shortcut_guardar, &QShortcut::activated, this, &MainWindow::on_guardarArchivo_click);
    connect(shortcut_guardarComo, &QShortcut::activated, this, &MainWindow::on_guardarComo_click);
    connect(shortcut_nuevoArchivo, &QShortcut::activated, this, &MainWindow::on_nuevoArchivo_click);
    connect(shortcut_abrirArchivo, &QShortcut::activated, this, &MainWindow::on_seleccionarArchivo_click);

    connect(shortcut_aumentarZoom, &QShortcut::activated, this, &MainWindow::zoomIn);
    connect(shortcut_disminuirZoom, &QShortcut::activated, this, &MainWindow::zoomOut);

    ui->pteEditor->installEventFilter(this);

    //Colocar el texto en la barra inferior
    statusBar()->showMessage("Línea: 1, Columna: 1");
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    //En caso de que el objeto sea el editor de texto y el evento sea una entrada
    //(un click o ingreso de texto desde el teclado) redefinir el índice de fila y columna
    if (obj == ui->pteEditor && event->type() == QEvent::InputMethodQuery)
    {
        on_pteEditor_input();
        return true;
    }

    return QMainWindow::eventFilter(obj, event);
}

void MainWindow::wheelEvent(QWheelEvent *event)
{
    //Sí el evento asociado al mover la rueda del mouse se presenta al mismo tiempo
    //que se presiona la tecla control, entonces calcular el ángulo para reconocer
    //sí el movimiento es hacia arriba (aumentar zoom) o abajo (disminuir zoom)
    if (event->modifiers() == Qt::ControlModifier) {
        int delta = event->angleDelta().y();
        if (delta > 0)
            zoomIn();
        else if (delta < 0)
            zoomOut();
    } else {
        QMainWindow::wheelEvent(event);
    }
}

void MainWindow::on_editorArchivo_textChanged() {
    //Establecer el título de la ventana con el nombre del archivo
    setWindowTitle(tr("*%1: Editor de texto").arg(nombre_archivo));
    archivo_modificado = true;
}

void MainWindow::zoomIn() {
    if (tamanio_texto + incremento_texto < 100) {
        tamanio_texto += incremento_texto;
        updateFontSize();
    }
}

void MainWindow::zoomOut() {
    if (tamanio_texto - incremento_texto > 8) {
        tamanio_texto -= incremento_texto;
        updateFontSize();
    }
}

void MainWindow::updateFontSize() {
    //Actualizar el tamaño del texto basado en el zoom actual
    QFont font = ui->pteEditor->font();
    font.setPointSize(tamanio_texto);
    ui->pteEditor->setFont(font);
}

void MainWindow::on_nuevoArchivo_click(){
    //Bandera para reconocer cuando se puede abrir un nuevo archivo y reiniciar
    //las especificaciones actuales
    bool puede_reiniciar = true;

    //Si el archivo ha sido modificado, entonces cuestionar al usuario si desea guardar
    //el archivo actual
    if (archivo_modificado) {

        //Crear el cuadro del diálogo
        QMessageBox::StandardButton respuesta;
        respuesta = QMessageBox::question(this, "Editor de archivos",
                                      "¿Desea guardar el archivo antes de continuar?",
                                      QMessageBox::Yes | QMessageBox::No);

        //Sí la respuesta del diálogo es positiva, entonces verificar si es posible
        //reiniciar el contenido y la ruta del archivo que se está modificando
        if (respuesta == QMessageBox::Yes) {
            puede_reiniciar = guardar_archivo(ultima_ruta_archivo);
        }
    }

    //Sí puede reiniciar, reestablecer el contenido del editor y la ruta
    if (puede_reiniciar){
        //Para evitar que se active el evento textchange puesto que se reiniciará el contenido, desconectar
        //el evento temporalmente
        disconnect(ui->pteEditor, &QPlainTextEdit::textChanged, this, &MainWindow::on_editorArchivo_textChanged);

        archivo_modificado = false;
        ultima_ruta_archivo = "";
        nombre_archivo = "Sin título";
        ui->pteEditor->setPlainText("");
        setWindowTitle(tr("%1: Editor de texto").arg(nombre_archivo));

        //Volver a activar el componente textchange para el editor
        connect(ui->pteEditor, &QPlainTextEdit::textChanged, this, &MainWindow::on_editorArchivo_textChanged);
    }
}

void MainWindow::on_seleccionarArchivo_click() {
    //Verificar si el archivo ha sido modificado para así preguntar al usuario si desea
    //guardar antes de abrir otro archivo
    if (archivo_modificado) {
        QMessageBox::StandardButton respuesta = QMessageBox::question(
            this,
            tr("Confirmar"),
            tr("Hay cambios sin guardar. ¿Deseas guardar los cambios antes de abrir otro archivo?"),
            QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel
        );

        //Sí la respuesta del diálogo es positiva, entonces guardar el archivo,
        if (respuesta == QMessageBox::Yes) {
            guardar_archivo(ultima_ruta_archivo);
        }
    }

    //Abrir un cuadro de diálogo para seleccionar un archivo
    QString ruta_archivo = QFileDialog::getOpenFileName(this, tr("Abrir archivo"), "", tr("Files (*.*)"));

    //Comprobar si el usuario seleccionó un archivo
    if (!ruta_archivo.isEmpty()) {

        //Crear el archivo y definir la ruta a la seleccionada
        QFile file(ruta_archivo);
        ultima_ruta_archivo = ruta_archivo;

        //Abrir el archivo en forma de solo lectura
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {

            //Desactivar el evento text change hasta que cargue el texto del contenido del archivo seleccionado
            disconnect(ui->pteEditor, &QPlainTextEdit::textChanged, this, &MainWindow::on_editorArchivo_textChanged);

            QTextStream in(&file);

            //Guardar le contenido del texto y colocarlo en el widget
            QString contenido_archivo = in.readAll();
            ui->pteEditor->setPlainText(contenido_archivo);

            //Cerrar el archivo
            file.close();

            archivo_modificado = false;
            cargar_titulo_nombreArchivo(ultima_ruta_archivo);

            //Volver a activar el evento
            connect(ui->pteEditor, &QPlainTextEdit::textChanged, this, &MainWindow::on_editorArchivo_textChanged);
        }
    } else {
        qDebug() << "No se seleccionó ningún archivo.";
    }
}

void MainWindow::on_guardarComo_click() {
    //Establecer la ruta del archivo como vacía para forzar la apertura
    //del cuadro de diálogo
    guardar_archivo("");
}

void MainWindow::on_guardarArchivo_click()
{
    //Envíar como parámetro la última ruta de archivo abierta
    guardar_archivo(ultima_ruta_archivo);
}

bool MainWindow::guardar_archivo(QString ruta_archivo){
    //Comprobar si ya anteriormente se ha seleccionado un archivo
    if (ruta_archivo.isEmpty()) {

        //Si no se ha seleccionado un archivo, abrir un cuadro de diálogo para guardar el archivo
        ruta_archivo = QFileDialog::getSaveFileName(this, tr("Guardar archivo"),
                                                            "", tr("Text Files (*.txt);;All Files (*)"));

        if (!ruta_archivo.isEmpty()) {
            ultima_ruta_archivo = ruta_archivo;
            cargar_titulo_nombreArchivo(ultima_ruta_archivo);
        } else {
            qDebug() << "No se seleccionó ningún archivo.";
            return false;
        }
    }

    //Escribir el contenido del QPlainTextEdit en el archivo
    QFile file(ultima_ruta_archivo);

    //Abrir el archivo en modo de escritura
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {

        //Colocar el texto del text plain editor en el archivo seleccionado
        QTextStream out(&file);
        out << ui->pteEditor->toPlainText();

        //Cerrar el archivo
        file.close();

        setWindowTitle(tr("%1: Editor de texto").arg(nombre_archivo));
        archivo_modificado = false;        
        return true;
    } else {
        qDebug() << "Error al guardar el archivo.";
    }

    return false;
}

void MainWindow::on_pteEditor_input()
{
    //Obtener la posición del clic en el editor de texto plano
    //QPoint pos = ui->pteEditor->viewport()->mapFromGlobal(QCursor::pos());
    //QTextCursor cursor = ui->pteEditor->cursorForPosition();
    QTextCursor cursor = ui->pteEditor->textCursor();

    //Obtener la fila y columna donde se hizo clic
    int fila = cursor.blockNumber() + 1;
    int columna = cursor.columnNumber() + 1;

    //Actualizar la barra de estado con la fila y columna
    statusBar()->showMessage(QString("Línea: %1, Columna: %2").arg(fila).arg(columna));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    //Si hay contenido y no hay una ruta específicada entonces preguntar
    //si desea guardar o no
    if (archivo_modificado) {

        QMessageBox::StandardButton respuesta = QMessageBox::question(
            this,
            tr("Editor de archivos"),
            tr("Hay cambios sin guardar. ¿Deseas guardar los cambios antes de salir?"),
            QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel
        );

        //Sí la respuesta del diálogo es positiva, entonces verificar si es posible
        //reiniciar el contenido y la ruta del archivo que se está modificando
        if (respuesta == QMessageBox::Yes) {

            //Sí se guardó correctamente, cerrar la ventana
            if (guardar_archivo(ultima_ruta_archivo)) {
                event->accept();
            } else {
                event->ignore();
            }
        }
        else if (respuesta == QMessageBox::No) {
            event->accept();
        }
        else if (respuesta == QMessageBox::Cancel) {
            event->ignore();
        }
    } else {
        event->accept();
    }
}

void MainWindow::cargar_titulo_nombreArchivo(QString ruta_archivo) {
    //Obtener el nombre del archivo y su extensión
    QFileInfo fileInfo(ruta_archivo);
    nombre_archivo = fileInfo.fileName();

    //Establecer el título de la ventana con el nombre del archivo
    setWindowTitle(tr("%1: Editor de texto").arg(nombre_archivo));
}

MainWindow::~MainWindow()
{
    delete ui;
}
