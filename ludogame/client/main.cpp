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
		//QObject::connect( subscriber, &nzmqt::ZMQSocket::messageReceived, printMessages );
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
            while( 1 )
            {
                QString input = s.readLine();
                if (input.size()==0)
                     std::cout << "No command found" << std::endl;
                if (input.at(0)=='s')
                {
                    input = "ludo?>startGame";
                    nzmqt::ZMQMessage message = nzmqt::ZMQMessage( input.toUtf8() );
                    pusher->sendMessage( message );
                    std::cout << "Message send !" << std::endl;
                }
                if (input.at(1)=='r')
                {
                    input = "ludo?>player>roll";
                    nzmqt::ZMQMessage message = nzmqt::ZMQMessage( input.toUtf8() );
                    pusher->sendMessage( message );
                    std::cout << "Message send !" << std::endl;
                }
                else if (input.at(1)=='a')
                {
                    if (input.size()>=3  && (input.at(2)=='2' || input.at(2)=='3' || input.at(2)=='4'))
                    {
                    input = "ludo?>setplayer>"+ QString(input.at(2));
                    nzmqt::ZMQMessage message = nzmqt::ZMQMessage( input.toUtf8() );
                    pusher->sendMessage( message );
                    std::cout << "Message send !" << std::endl;
                    }
                    else
                       std::cout << "Amount of players need to be 2-4!" << std::endl;
                }
                else if (input.at(1)=='h')
                {
                    QString input = "ludo?>help";
                    nzmqt::ZMQMessage message = nzmqt::ZMQMessage( input.toUtf8() );
                    pusher->sendMessage( message );
                    std::cout << "Message send !" << std::endl;
                }
                else if (input.at(1)=='o')
                {
                    QString input = "ludo?>overview";
                    nzmqt::ZMQMessage message = nzmqt::ZMQMessage( input.toUtf8() );
                    pusher->sendMessage( message );
                    std::cout << "Message send !" << std::endl;
                }
                else if (input.at(1)=='b')
                {
                    QString input = "ludo?>exit";
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
