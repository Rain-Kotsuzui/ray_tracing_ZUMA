#include "MainWindow.h"
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QMessageBox>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      gameWidget(new GameWidget(this))
{
    setupUI();
    createMenus();

    setWindowTitle(tr("3D Zuma Game!"));
    resize(SCREEN_MIN_X, SCREEN_MIN_Y);
}
void MainWindow::setupUI()
{
    setCentralWidget(gameWidget);
}
void MainWindow::createMenus()
{
    QMenu *gameMenu = menuBar()->addMenu(tr("Game"));

    QAction *newGameAction = new QAction(tr("New Game"), this);
    newGameAction->setShortcut(QKeySequence::New);
    connect(newGameAction, &QAction::triggered, this, &MainWindow::startNewGame);
    gameMenu->addAction(newGameAction);

    gameMenu->addSeparator();

    QAction *exitAction = new QAction(tr("Exit"), this);
    exitAction->setShortcut(QKeySequence::Quit);
    connect(exitAction, &QAction::triggered, this, &QWidget::close);
    gameMenu->addAction(exitAction);

    QMenu *helpMenu = menuBar()->addMenu(tr("Help"));

    QAction *aboutAction = new QAction(tr("About"), this);
    connect(aboutAction, &QAction::triggered, this, &MainWindow::showAbout);
    helpMenu->addAction(aboutAction);

    helpMenu->addSeparator();

    QAction *helpAction = new QAction(tr("Help"), this);
    connect(helpAction, &QAction::triggered, this, &MainWindow::showHelp);
    helpMenu->addAction(helpAction);

    connect(gameWidget->getGame(), &Game::gameOver, this, &MainWindow::gameOver);
    connect(this, &MainWindow::restartGame, this, &MainWindow::startNewGame);

    connect(gameWidget, &GameWidget::gameStop, this, &MainWindow::gameStop);
}
void MainWindow::gameStop()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("暂停");
    msgBox.setText(tr("Esc键继续"));
    QTimer::singleShot(1000, &msgBox, &QMessageBox::close);
    msgBox.exec();
}
void MainWindow::gameOver()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("game over!");
    msgBox.setText(tr("再来一局？"));
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel); // 添加Ok和Cancel按钮
    int ret = msgBox.exec();
    if (ret == QMessageBox::Ok)
        emit restartGame();
    else if (ret == QMessageBox::Cancel)
        qApp->quit();
}
void MainWindow::showAbout()
{
    QMessageBox::about(this, tr("3D Zuma!"),
                       tr("Zuma 游戏\n"
                          "一个基于Qt的3DZuma游戏实现"));
}
void MainWindow::showHelp()
{
    QMessageBox::about(this, tr("玩法"),
                       tr("P 显示路径\n"
                          "L 打开泛光\n"
                          "J 减小泛光\n"
                          "K 增大泛光\n"
                          "滚轮 缩放"
                          ));
}
void MainWindow::startNewGame()
{
    gameWidget->getTimer()->start();
    gameWidget->getGame()->initialize();
}