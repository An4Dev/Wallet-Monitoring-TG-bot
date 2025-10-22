#pragma once

class WebShit
{
public:
	std::string ReadInsult()
	{
		HINTERNET connect = InternetOpen( "MyBrowser", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0 );
		if( !connect )
			return 0;

		HINTERNET OpenAddress = InternetOpenUrl( connect, "http://www.insultgenerator.org", NULL, 0, INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_KEEP_CONNECTION, 0 );
		if( !OpenAddress )
		{
			InternetCloseHandle( connect );
			return 0;
		}

		char  dataReceived[ 1024 ];
		DWORD numberOfBytesRead = 0;

		if( InternetReadFile( OpenAddress, dataReceived, 1024, &numberOfBytesRead ) && numberOfBytesRead )
		{
			std::string str( dataReceived );

			std::size_t posStart = str.find( "<br><br>" );
			std::size_t posEnd = str.find( "</div>\n<center>" );

			int size = posEnd - posStart;

			std::string output = str.substr( posStart + 8, size - 8 );

			InternetCloseHandle( OpenAddress );
			InternetCloseHandle( connect );

			return output;
		}

		InternetCloseHandle( OpenAddress );
		InternetCloseHandle( connect );

		return 0;
	}

	std::string ReadIrc()
	{
		HINTERNET connect = InternetOpen("MyBrowser", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
		if (!connect)
			return 0;

		HINTERNET OpenAddress = InternetOpenUrl(connect, "http://fodderfap.me/cheatsay.php", NULL, 0, INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_KEEP_CONNECTION, 0);
		if (!OpenAddress)
		{
			InternetCloseHandle(connect);
			return 0;
		}

		char  dataReceived[1024];
		DWORD numberOfBytesRead = 0;

		if (InternetReadFile(OpenAddress, dataReceived, 1024, &numberOfBytesRead) && numberOfBytesRead)
		{
			std::string str(dataReceived);

			std::size_t posStart = str.find("l");
			std::size_t posEnd = str.find(";");

			int size = posEnd - posStart;

			std::string output = str.substr(posStart + 1, size - 1);

			InternetCloseHandle(OpenAddress);
			InternetCloseHandle(connect);
			return output;
		}

		InternetCloseHandle(OpenAddress);
		InternetCloseHandle(connect);

		return 0;
	}
};

extern WebShit* g_pWebShit;