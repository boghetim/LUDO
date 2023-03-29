#ifndef LUDO_H
#define LUDO_H
#include <QCoreApplication>
#include <QObject>
#include <nzmqt/nzmqt.hpp>
#include <nzmqt/impl.hpp>

#include <QList>

class Ludo: public QObject
{
public:
    Ludo(QCoreApplication *a);
    void game(const QList<QByteArray>& messages);
    void help();
    void pickcolor();
    void rolDice();
    void bye();

private:
    nzmqt::ZMQSocket *pusher;
    nzmqt::ZMQSocket *subscriber;
    int count;
    QList<int> players;
};

#endif // LUDO_H
