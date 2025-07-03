#ifndef WALLETWINDOW_H
#define WALLETWINDOW_H

#include <QMainWindow>

namespace Ui {
class WalletWindow;
}

class WalletWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit WalletWindow(QWidget *parent = nullptr);
    ~WalletWindow();

private:
    Ui::WalletWindow *ui;
};

#endif // WALLETWINDOW_H
