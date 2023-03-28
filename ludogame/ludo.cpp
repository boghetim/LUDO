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
        subscriber->subscribeTo("ludo>");

        if( !pusher->isConnected() || !subscriber->isConnected() )
        {
            std::cerr << "NOT CONNECTED !!!" << std::endl;
        }



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
            if (msg.contains("ludo>help"))
            {
                help();
            }
            if (msg.contains("ludo>player>roll?")) //VASTE 4 KLEUREN MEEGEVEN (DUS 4 IFS) waar ik makkelijk de varriable kan optellen van dice
            {
               rolDice();
            }
            if(msg.contains("ludo>exit"))
            {
                bye();
            }
        }
    }
}

void Ludo::help()
{
    QString info = "Welcome in the Game: LUDO.\n"
                   "  How to play:  "
                   "  to roll the dice give 'ludo>player>green' command for the color your picked "
                   "  To quit to the game enter 'ludo>exit' "   ;
    nzmqt::ZMQMessage message = nzmqt::ZMQMessage( info.toUtf8() );
    pusher->sendMessage(message);

    std::cout << "  Welcome in the Game: LUDO "  <<std::endl;
    std::cout << "  How to play:  "  <<std::endl;
    std::cout << "  to roll the dice give 'ludo>player>green' command for the color your picked "  <<std::endl;
    std::cout << "  To quit to the game enter 'ludo>exit' " <<std::endl;
}

void Ludo::rolDice()
{
    QString diceRoller = "ludo>player>roll!>" + QString::number(rand() % 6 + 1);
    std::cout << "Rolled: " <<  diceRoller.toStdString() <<std::endl;
    nzmqt::ZMQMessage message = nzmqt::ZMQMessage( diceRoller.toUtf8() );
    pusher->sendMessage(message);

}

void Ludo::bye()
{
    std::cout << "  Exiting " <<std::endl;
    Sleep(2000);
    exit(0);
}
