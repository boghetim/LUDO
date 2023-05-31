#include <iostream>
#include <nzmqt/nzmqt.hpp>
#include <nzmqt/impl.hpp>
#include <QCoreApplication>
#include <QString>
#include <QTimer>
#include <QThread>
#include <QDateTime>

int main( int argc, char *argv[] )
{
    QCoreApplication a(argc, argv);
	try
	{
		nzmqt::ZMQContext *context = nzmqt::createDefaultContext( &a );
		nzmqt::ZMQSocket *pusher = context->createSocket( nzmqt::ZMQSocket::TYP_PUSH, context );
		nzmqt::ZMQSocket *subscriber = context->createSocket( nzmqt::ZMQSocket::TYP_SUB, context );
		QObject::connect( subscriber, &nzmqt::ZMQSocket::messageReceived, []( const QList<QByteArray>& messages )
		{
			if( messages.size() < 1 )
			{
				std::cout << "Received empty message !" << std::endl;
			}
			else if( messages.size() == 1 )
			{
				std::cout << "Received (" << messages.constFirst().size() << ") : " << messages.constFirst().toStdString() << std::endl;
			}
			else
			{
				std::cout << "Received " << messages.size() << " parts" << std::endl;
				int part = 1;
				for( const QByteArray & message : messages )
				{
					std::cout << "Part " << part << " (" << message.size() << ") : " << message.toStdString() << std::endl;
					part++;
				}
			}
		} );

            QThread *thread = QThread::create([pusher]{
            QTextStream s(stdin);

            /*
             *  Reading the user input and checking if its a command or not
             g -> starGame menu where the client get the info what gametag there are
             r and gametag number as second user input -> roll dice menu where the dice will roll and get your place of your token
             s, gametag number as second user input and number of players as third user input -> will set the amount of players playing
             h -> help menu where al the commands are listed
             o and gametag number as second user input -> overview of all tokens
             b -> server shutdown ,please dont do it if ure not the admin ;)

             */
            while( 1 )
            {
                QString input = s.readLine();
                if (input.size()==0)
                     std::cout << "No command found" << std::endl;

                else if (input.at(0)=='g')
                {
                    input = "ludo?>startGame>";
                    nzmqt::ZMQMessage message = nzmqt::ZMQMessage( input.toUtf8() );
                    pusher->sendMessage( message );
                    std::cout << "Message send !" << std::endl;
                }



                else if (input.at(0)=='r')
                {
                    if(input.size()>=2)
                    {
                        input = "ludo?>player>roll>"+ QString(input.at(1));
                        nzmqt::ZMQMessage message = nzmqt::ZMQMessage( input.toUtf8() );
                        pusher->sendMessage( message );
                        std::cout << "Message send !" << std::endl;
                    }
                    else
                        std::cout << "Please do give your gametag number with the command" << std::endl;
                }

                else if (input.at(0)=='s')
                {
                    if (input.size()>=3  && (input.at(2)=='2' || input.at(2)=='3' || input.at(2)=='4'))
                    {
                    input = "ludo?>setplayer>"+ QString(input.at(1))+">"+ QString(input.at(2));
                    nzmqt::ZMQMessage message = nzmqt::ZMQMessage( input.toUtf8() );
                    pusher->sendMessage( message );
                    std::cout << "Message send !" << std::endl;
                    }

                    else
                       std::cout << "check if you give your gametag number as second input and player amount as third (only 2 till 4 is possible)" << std::endl;
                }
                else if (input.at(0)=='h')
                {
                    QString input = "ludo?>help";
                    nzmqt::ZMQMessage message = nzmqt::ZMQMessage( input.toUtf8() );
                    pusher->sendMessage( message );
                    std::cout << "Message send !" << std::endl;
                }
                else if (input.at(0)=='o')
                {
                    input = "ludo?>overview>"+QString(input.at(1));
                    nzmqt::ZMQMessage message = nzmqt::ZMQMessage( input.toUtf8() );
                    pusher->sendMessage( message );
                    std::cout << "Message send !" << std::endl;
                }
                else if (input.at(0)=='b')
                {
                    input = "ludo?>exit";
                    nzmqt::ZMQMessage message = nzmqt::ZMQMessage( input.toUtf8() );
                    pusher->sendMessage( message );
                    std::cout << "Message send !" << std::endl;
                }
                else
                    std::cout << "command not found" << std::endl;
            }
        });

		pusher->connectTo( "tcp://benternet.pxl-ea-ict.be:24041" );
		subscriber->connectTo( "tcp://benternet.pxl-ea-ict.be:24042" );

		if( argc > 1 )
		{
			for( int i = 1 ; i < argc ; i++ )
			{
				subscriber->subscribeTo( argv[i] );
			}
		}
		else
		{
            subscriber->subscribeTo( "ludo!>" );
		}

		if( !pusher->isConnected() || !subscriber->isConnected() )
		{
			std::cerr << "NOT CONNECTED !!!" << std::endl;
		}

        context->start();
        thread->start();
	}
	catch( nzmqt::ZMQException & ex )
	{
		std::cerr << "Caught an exception : " << ex.what();
	}
    std::cout << "Client online!" << std::endl;
	return a.exec();
}
