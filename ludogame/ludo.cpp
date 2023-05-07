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
        playerAmount=0;
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
        for (QByteArray msgByteArray : messages){
            QString msg = QString::fromUtf8(msgByteArray);
            std::cout << std::endl;
            std::cout << "Received: " << msg.toStdString() << std::endl;
            if (msg.contains("ludo?>help"))
            {
                help();
            }
            if (msg.contains("ludo?>start"))
            {
                start();
                playerAmount=2;
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
                   "   to start to give give'ludo?>start' command. \n"
                   "   then u need to give with how moany players you wane play 2-4 with 'ludo?>start>3' command. (example 3 players) \n"
                   "   1st player is Green, 2nd is Blue, 3th is Red and the 4th player is Yellow those colors are fixed \n"
                   "   to roll the dice give 'ludo?>player>roll>1st token' command. \n"
                   "   if you wane roll the other tokens just replace '1st' with '2nd' , '3rd' or '4th'\n"
                   "   if you roll a 6 and want to free an extra token give 'ludo?>player>free token' command. \n"
                   "   if you wane see the board overview give 'ludo?>overview Board' command. \n"
                   "   To rage quit to the game enter 'ludo?>exit' \n";
    nzmqt::ZMQMessage message = nzmqt::ZMQMessage( info.toUtf8() );
    pusher->sendMessage(message);

    std::cout << "  test help menu "  <<std::endl;
   // std::cout << "  Welcome in the Game: LUDO "  <<std::endl;
    //std::cout << "  How to play:  "  <<std::endl;
    //std::cout << "  to roll the dice give 'ludo?>player>roll' command "  <<std::endl;
    //std::cout << "  To quit to the game enter 'ludo>exit' " <<std::endl;
}

void Ludo::start()
{

    if (playerAmount ==0)
    {
        QString countplayers = "ludo!>start>how many players are playing?";
        nzmqt::ZMQMessage message = nzmqt::ZMQMessage( countplayers.toUtf8() );
        pusher->sendMessage(message);

        std::cout << "  test start menu "  <<std::endl;
    }
    else
    {
        QString color = "ludo!>start\n  Welcome in the Game: LUDO.\n"
                       "  You are playing with "+ QString::number (playerAmount) +  "\n"
                       "  Enjoy the game ! \n";
        nzmqt::ZMQMessage message = nzmqt::ZMQMessage( color.toUtf8() );
        pusher->sendMessage(message);

        std::cout << "  test start2 menu "  <<std::endl;
    }
}

void Ludo::bye()
{
    QString goodbye = "ludo!>exiting";
    //std::cout << "  Exiting " <<std::endl;
    nzmqt::ZMQMessage message = nzmqt::ZMQMessage( goodbye.toUtf8() );
    pusher->sendMessage(message);

    std::cout << "  test bye menu "  <<std::endl;

    Sleep(2000);
    exit(0);
}

void Ludo::rolDice()
{
    int temp = (rand() % 6 + 1);
    QString activeplayer= "";
    players[count]=players[count]+temp;

    if (count==0)
        activeplayer = "Green";
    if (count==1)
        activeplayer = "Blue";
    if (count==2)
        activeplayer = "Red";
    if (count==3)
        activeplayer = "Yellow";

 /* is around the field and pion is safe in base */
    if (players[count]> count*10+40)
    {
        QString diceRoller = "ludo!>player>roll "+ activeplayer +" is finished";
        nzmqt::ZMQMessage message = nzmqt::ZMQMessage( diceRoller.toUtf8() );
        pusher->sendMessage(message);
        players[count]= count*10;

        std::cout << "  test roll done menu "  <<std::endl;
    }
    else
    {
        QString diceRoller = "ludo!>player>" + activeplayer +" rolled " + QString::number (temp)+", total place " + QString::number (players[count]);
        std::cout << "Rolled: " <<  diceRoller.toStdString() <<std::endl;
        nzmqt::ZMQMessage message = nzmqt::ZMQMessage( diceRoller.toUtf8() );
        pusher->sendMessage(message);

        std::cout << "  test roll menu "  <<std::endl;
    }

/* selecting next player and reseting to the first one after last one is done */
    if (count<=playerAmount)
    {
        if (temp!=6)
        count++;
    }
    else
    {
        count=0;
    }

}
