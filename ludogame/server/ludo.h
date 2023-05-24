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
    QChar findCharacter(const QString& str);


private:
    nzmqt::ZMQSocket *pusher;
    nzmqt::ZMQSocket *subscriber;
    QList<int> count;
    int gamenumber=0;
    QList<QList<int>> allTokensPos;
    QList<int> playerAmount;
    QList<int> players;
};

#endif // LUDO_H
