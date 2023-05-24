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



            if (msg.contains("ludo?>startGame>"))
            {
                /*QList<QString> messageSplit = msg.split('>');
                if(messageSplit.size()>2)
                {
                gamenumber= messageSplit[2].toInt();
                }*/

                int newGameNumber = allTokensPos.size();
                allTokensPos.append(QList<int>());
                for (int i = 0; i < 16; ++i)
                    allTokensPos[newGameNumber].append(0);
                allTokensPos[newGameNumber][1]=10;
                allTokensPos[newGameNumber][2]=20;
                allTokensPos[newGameNumber][3]=30;

                for (int i = 0; i < 4; ++i)
                    players.append(0);

                players[1]=10;
                players[2]=20;
                players[3]=30;

                QString start = "ludo!>start>game number is:"+ QString::number(newGameNumber)+ "please do not forget this";
                nzmqt::ZMQMessage message = nzmqt::ZMQMessage( start.toUtf8() );
                pusher->sendMessage(message);
            }

            if (msg.contains("ludo?>help"))
            {
                help();
            }
            if (msg.contains("ludo?>setplayer"))
            {
                setplayer(gamenumber);
                QList<QString> messageSplit = msg.split('>');
                if(messageSplit.size()>3)
                {
                playerAmount[gamenumber]= messageSplit[2].toInt();
                std::cout << "amount is " << playerAmount[gamenumber] << std::endl;
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
                rolDice(gamenumber);
            }
            if(msg.contains("ludo?>exit"))
            {
                bye();
            }
            if (msg.contains("ludo?>overview"))
            {
                overview(gamenumber);
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
}


void Ludo::bye()
{
    QString goodbye = "ludo!>exiting";
    nzmqt::ZMQMessage message = nzmqt::ZMQMessage( goodbye.toUtf8() );
    pusher->sendMessage(message);

    std::cout << "  test bye menu "  <<std::endl;

    Sleep(2000);
    exit(0);
}

void Ludo::setplayer(int gamenumber)
{
    if (playerAmount[gamenumber] ==2 || playerAmount[gamenumber] ==3 || playerAmount[gamenumber] ==4 )
    {
        QString countplayers = "ludo!>setplayer>are set on:"+ QString::number (playerAmount[gamenumber]) +" players";
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



void Ludo::overview(int gamenumber)
{
    QString tokensPos;
    for(int i=0; i<16; i++)
    {
      tokensPos.append(QString::number(allTokensPos[i][gamenumber]) + ',');
    }
    QString overview = QString("ludo!>overview>location of al tokens>" + tokensPos);
    nzmqt::ZMQMessage message = nzmqt::ZMQMessage( overview.toUtf8() );
    pusher->sendMessage(message);

    std::cout << "  test overview menu "  <<std::endl;
}

void Ludo::rolDice(int gamenumber)
{
    if (playerAmount[gamenumber] ==0)
    {
        QString countplayers = "ludo!>player>to go the set players menu first ";
        nzmqt::ZMQMessage message = nzmqt::ZMQMessage( countplayers.toUtf8() );
        pusher->sendMessage(message);
    }
    else
    {
        int temp = (rand() % 6 + 1);
        QString activeplayer= "";
        allTokensPos[count[gamenumber]][gamenumber]=allTokensPos[count[gamenumber]][gamenumber]+temp;
        if (count[gamenumber]==0)
            activeplayer = "Green";
        if (count[gamenumber]==1)
            activeplayer = "Blue";
        if (count[gamenumber]==2)
            activeplayer = "Red";
        if (count[gamenumber]==3)
            activeplayer = "Yellow";

        /* is around the field and pion is safe in base */
        if (allTokensPos[count[gamenumber]][gamenumber]> count[gamenumber]*10+40)
        {
            QString diceRoller = "ludo!>player>roll "+ activeplayer +" is finished";
            nzmqt::ZMQMessage message = nzmqt::ZMQMessage( diceRoller.toUtf8() );
            pusher->sendMessage(message);
            players[count[gamenumber]]= count[gamenumber]*10;

            std::cout << "  test roll done menu "  <<std::endl;
        }
        else
        {
            QString diceRoller = "ludo!>player>" + activeplayer +" rolled " + QString::number (temp)+", total place " + QString::number (allTokensPos[count[gamenumber]][gamenumber]);
            std::cout << "Rolled: " <<  diceRoller.toStdString() <<std::endl;
            nzmqt::ZMQMessage message = nzmqt::ZMQMessage( diceRoller.toUtf8() );
            pusher->sendMessage(message);

            std::cout << "  test roll menu "  <<std::endl;
        }

    /* selecting next player and reseting to the first one after last one is done */
        if (count[gamenumber]<playerAmount[gamenumber]-1)
        {
            if (temp!=6)
            count[gamenumber]++;
        }
        else
            count[gamenumber]=0;
    }
}
