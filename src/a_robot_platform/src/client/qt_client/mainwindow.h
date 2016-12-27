#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../tcp_socket.h"
#include "../key_control.h"
#include<QMouseEvent>
#include <QMessageBox>
#include <QPainter>
#include <QPixmap>
#include <QDateTime>
#include <QTimer>
#include <QPoint>
#include <QTableWidget>
#include<QProgressBar>

#define Myhigh 600
#define Mywidth 600
#define  MIDD 720

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QPixmap *pixmap;
    QProgressBar  *ProgressBar;
    float MyPoint[3600];
    float ShowPoint[MIDD][2];

protected:
    void keyPressEvent(QKeyEvent *e);


private slots:
    void ShowLaser();
    void on_pBtn_start2connect_clicked(bool checked);
    void on_pBtn_key_control_open_clicked(bool checked);
    void on_lEdit_ip_returnPressed();
    void on_lEdit_port_returnPressed();
    void on_xTimerUpdate();
    void on_cmdTimerUpdate();

private :
    void KeyControlMsgRefalsh(const zw::KeyControlMsg & kMsg);
    void MsgControlRefalsh(void);

private:
    Ui::MainWindow *ui;
    zw::KeyControl *m_keyControl;
    zw::TcpSocket *m_tcpSocketClient;

};

#endif // MAINWINDOW_H