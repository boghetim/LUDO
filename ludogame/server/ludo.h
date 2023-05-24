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
    void rolDice();
    void bye();
    void setplayer();
    void overview();
    QChar findCharacter(const QString& str);


private:
    nzmqt::ZMQSocket *pusher;
    nzmqt::ZMQSocket *subscriber;
    int count;
    QList<int> allTokensPos;
    int playerAmount;
    QList<int> players;

};

#endif // LUDO_H
