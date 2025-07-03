#include "walletwindow.h"
#include "ui_walletwindow.h"

WalletWindow::WalletWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::WalletWindow)
{
    ui->setupUi(this);
}

WalletWindow::~WalletWindow()
{
    delete ui;
}
