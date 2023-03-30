#include "ludo.h"
#include <iostream>
#include <QCoreApplication>
#include <QString>
#include <QTimer>
#include <QThread>
#include <QDateTime>
#include <cstdlib>
#include <ctime>
#include<windows.h>

Ludo::Ludo(QCoreApplication *a)
{
    srand(time(0));

    try
    {
        nzmqt::ZMQContext *context = nzmqt::createDefaultContext( a );
        pusher = context->createSocket( nzmqt::ZMQSocket::TYP_PUSH, context );
        subscriber = context->createSocket( nzmqt::ZMQSocket::TYP_SUB, context );
        QObject::connect( subscriber, &nzmqt::ZMQSocket::messageReceived, this, &Ludo::game);
        //als sub message received, dat hier (this) functie game opgeroepen wordt.

        pusher->connectTo( "tcp://benternet.pxl-ea-ict.be:24041" );
        subscriber->connectTo( "tcp://benternet.pxl-ea-ict.be:24042" );
        subscriber->subscribeTo("ludo?>");

        if( !pusher->isConnected() || !subscriber->isConnected() )
        {
            std::cerr << "NOT CONNECTED !!!" << std::endl;
        }

        count = 0;
        for (int i = 0; i < 4; ++i)
            players.append(0);

        players[1]=10;
        players[2]=20;
        players[3]=30;

        context->start();

    }
    catch( nzmqt::ZMQException & ex )
    {
        std::cerr << "Caught an exception : " << ex.what();
    }
    std::cout << "Start!" << std::endl;
}

void Ludo::game(const QList<QByteArray>& messages)
{
    if( messages.size() < 1 )
    {
        std::cout << "Received empty message !";
    }
    else
    {
        for (QByteArray msgByteArray : messages) {
            QString msg = QString::fromUtf8(msgByteArray);
            std::cout << std::endl;
            std::cout << "Received: " << msg.toStdString() << std::endl;
            if (msg.contains("ludo?>help"))
            {
                help();
            }
            if (msg.contains("ludo?>player>roll"))
            {
                rolDice();
            }
            if(msg.contains("ludo?>exit"))
            {
                bye();
            }
        }
    }
}

void Ludo::help()
{
    QString info = "ludo!>help\n   Welcome in the Game: LUDO.\n"
                   "   How to play:  \n"
                   "   to pick your color give 'ludo?>player>pick' command. \n"
                   "   to roll the dice give 'ludo?>player>roll>' command. \n"
                   "   To quit to the game enter 'ludo?>exit' \n";
    nzmqt::ZMQMessage message = nzmqt::ZMQMessage( info.toUtf8() );
    pusher->sendMessage(message);

    std::cout << "  Welcome in the Game: LUDO "  <<std::endl;
    std::cout << "  How to play:  "  <<std::endl;
    std::cout << "  to roll the dice give 'ludo?>player>roll' command "  <<std::endl;
    std::cout << "  To quit to the game enter 'ludo>exit' " <<std::endl;
}

void Ludo::rolDice()
{
    players[count]=players[count]+(rand() % 6 + 1);

 /* is around the field and pion is safe in base */
    if (players[count]> count*10+40)
    {
        QString diceRoller = "ludo!>player>roll "+ QString::number (count) +" is finished";
        nzmqt::ZMQMessage message = nzmqt::ZMQMessage( diceRoller.toUtf8() );
        pusher->sendMessage(message);
        players[count]= count*10;
    }
    else
    {
        QString diceRoller = "ludo!>player>" + QString::number (count) +" total place " + QString::number (players[count]);
        std::cout << "Rolled: " <<  diceRoller.toStdString() <<std::endl;
        nzmqt::ZMQMessage message = nzmqt::ZMQMessage( diceRoller.toUtf8() );
        pusher->sendMessage(message);
    }

/* selecting next player and reseting to the first one after last one is done */
    if (count<3)
    {
        count++;
    }
    else
    {
        count=0;
    }

}

void Ludo::bye()
{
    QString goodbye = "ludo!>exiting";
    std::cout << "  Exiting " <<std::endl;
    nzmqt::ZMQMessage message = nzmqt::ZMQMessage( goodbye.toUtf8() );
    pusher->sendMessage(message);
    Sleep(2000);
    exit(0);
}
