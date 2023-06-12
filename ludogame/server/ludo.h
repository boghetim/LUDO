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
    void rolDice(int gamenumber);
    void bye();
    void setplayer(int gamenumber);
    void overview(int gamenumber);


private:
    nzmqt::ZMQSocket *pusher;
    nzmqt::ZMQSocket *subscriber;
    QList<int> count;
    int gamenumber=0;
    bool goBig=false;
    QList<QList<int>> allTokensPos;
    QList<int> playerAmount;
    QList<QList<QString>> names;

};

#endif // LUDO_H
