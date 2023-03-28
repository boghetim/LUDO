#ifndef LUDO_H
#define LUDO_H
#include <QCoreApplication>
#include <QObject>
#include <nzmqt/nzmqt.hpp>
#include <nzmqt/impl.hpp>

class Ludo: public QObject
{
public:
    Ludo(QCoreApplication *a);
    void game(const QList<QByteArray>& messages);
    void help();
    void rolDice();
    void bye();

private:
    nzmqt::ZMQSocket *pusher;
    nzmqt::ZMQSocket *subscriber;
};

#endif // LUDO_H
