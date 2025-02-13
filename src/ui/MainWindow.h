#pragma once
#include <QMainWindow>
#include "GameWidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
GameWidget *getGameWidget(){ return gameWidget; };
private slots:
    void startNewGame();
    void showAbout();
    void showHelp();
    void gameOver();
    void gameStop();
private:
    GameWidget *gameWidget;

    void setupUI();
    void createMenus();
signals:
    void restartGame();
};