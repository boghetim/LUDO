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

    for (int i = 0; i < 16; ++i)
        allTokensPos.append(0);

          allTokensPos[1]=10;
          allTokensPos[2]=20;
          allTokensPos[3]=30;


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
    std::cout << "Server online" << std::endl;
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
            if (msg.contains("ludo?>setplayer"))
            {
                setplayer();
                QList<QString> messageSplit = msg.split('>');
                if(messageSplit.size()>2)
                {
                playerAmount= messageSplit[2].toInt();
                std::cout << "amount is " << playerAmount << std::endl;
                }
                else
                {
                    QString error = "not enough to '>' to find amount ";
                    nzmqt::ZMQMessage message = nzmqt::ZMQMessage( error.toUtf8() );
                    pusher->sendMessage(message);
                    std::cout << "not enough to '>' to find amount " << std::endl;
                }
            }
            if (msg.contains("ludo?>player>roll"))
            {
                rolDice();
            }
            if(msg.contains("ludo?>exit"))
            {
                bye();
            }
            if (msg.contains("ludo?>overview"))
            {
                overview();
            }
        }
    }
}

void Ludo::help()
{
    QString info = "ludo!>help\n   Welcome in the Game: LUDO.\n"
                   "   How to play:  \n"
                   "   First use the command 's' followed with a number for the amount of players like 's3' for example \n"
                   "   1st player is Green, 2nd is Blue, 3th is Red and the 4th player is Yellow those colors are fixed \n"
                   "   To roll use the command 'r' \n"
                   "   To change the token use the command 't' followed with a number of the token 't3' for example\n"
                   "   if you roll a 6 you can roll again with 'r' or free a extra token by 'f' \n"
                   "   if you wane see the board overview give 'o' command. \n"
                   "   To rage quit to the game enter 'b' \n";
    nzmqt::ZMQMessage message = nzmqt::ZMQMessage( info.toUtf8() );
    pusher->sendMessage(message);

    std::cout << "  test help menu "  <<std::endl;
   // std::cout << "  Welcome in the Game: LUDO "  <<std::endl;
    //std::cout << "  How to play:  "  <<std::endl;
    //std::cout << "  to roll the dice give 'ludo?>player>roll' command "  <<std::endl;
    //std::cout << "  To quit to the game enter 'ludo>exit' " <<std::endl;
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

void Ludo::setplayer()
{
    if (playerAmount ==2 || playerAmount ==3 || playerAmount ==4 )
    {
        QString countplayers = "ludo!>setplayer>are set on:"+ QString::number (playerAmount) +" players";
        nzmqt::ZMQMessage message = nzmqt::ZMQMessage( countplayers.toUtf8() );
        pusher->sendMessage(message);

        std::cout << "  test setplayer2 menu "  <<std::endl;
    }
    else
    {
        QString countplayers = "ludo!>setplayer>something wrong please try again (must be 2-4)";
        nzmqt::ZMQMessage message = nzmqt::ZMQMessage( countplayers.toUtf8() );
        pusher->sendMessage(message);

        std::cout << "  test setplayer menu "  <<std::endl;
    }
}



void Ludo::overview()
{
    QString tokensPos;
    for(int i=0; i<16; i++)
    {
      tokensPos.append(QString::number(allTokensPos[i]) + ',');
    }
    QString overview = QString("ludo!>overview>location of al tokens>" + tokensPos);
    nzmqt::ZMQMessage message = nzmqt::ZMQMessage( overview.toUtf8() );
    pusher->sendMessage(message);

    std::cout << "  test overview menu "  <<std::endl;
}

void Ludo::rolDice()
{
    if (playerAmount ==0)
    {
        QString countplayers = "ludo!>player>to go the set players menu first ";
        nzmqt::ZMQMessage message = nzmqt::ZMQMessage( countplayers.toUtf8() );
        pusher->sendMessage(message);
    }
    else
    {
        int temp = (rand() % 6 + 1);
        QString activeplayer= "";
        allTokensPos[count]=allTokensPos[count]+temp;
        if (count==0)
            activeplayer = "Green";
        if (count==1)
            activeplayer = "Blue";
        if (count==2)
            activeplayer = "Red";
        if (count==3)
            activeplayer = "Yellow";

        /* is around the field and pion is safe in base */
        if (allTokensPos[count]> count*10+40)
        {
            QString diceRoller = "ludo!>player>roll "+ activeplayer +" is finished";
            nzmqt::ZMQMessage message = nzmqt::ZMQMessage( diceRoller.toUtf8() );
            pusher->sendMessage(message);
            players[count]= count*10;

            std::cout << "  test roll done menu "  <<std::endl;
        }
        else
        {
            QString diceRoller = "ludo!>player>" + activeplayer +" rolled " + QString::number (temp)+", total place " + QString::number (allTokensPos[count]);
            std::cout << "Rolled: " <<  diceRoller.toStdString() <<std::endl;
            nzmqt::ZMQMessage message = nzmqt::ZMQMessage( diceRoller.toUtf8() );
            pusher->sendMessage(message);

            std::cout << "  test roll menu "  <<std::endl;
        }

    /* selecting next player and reseting to the first one after last one is done */
        if (count<playerAmount-1)
        {
            if (temp!=6)
            count++;
        }
        else
            count=0;
    }
}
