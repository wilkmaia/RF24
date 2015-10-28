#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include "./RF24.h"

using namespace std;

RF24 radio( RPI_V2_GPIO_P1_15, RPI_V2_GPIO_P1_24, BCM2835_SPI_SPEED_8MHZ );

const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

const int min_payload_size = 4;
const int max_payload_size = 32;
const int payload_size_increments_by = 1;
int next_payload_size = min_payload_size;

char receive_payload[max_payload_size+1];

int main( int argc, char **argv )
{
	ofstream FILE( "_res_test", ofstream::out );
	printf( "Iniciando servidor RasPi\n" );
	
	// Initial settings
	radio.begin();
	radio.enableDynamicPayloads();
	radio.setRetries( 5, 15 );
	radio.printDetails();
	
	radio.openWritingPipe( pipes[1] );
	radio.openReadingPipe( 1, pipes[0] );
	radio.startListening();
	
	
	// Inifinite loop
	for(;;)
	{
		// Check for data
		if( radio.available() )
		{
			uint8_t len;
			
			while( radio.available() )
			{
				len = radio.getDynamicPayloadSize();
				radio.read( receive_payload, len );
				
				receive_payload[len] = 0;
				
				printf( "Dados recebidos:\nTamanho: %i\nValor: %s\n", len, receive_payload );
			}
			
			FILE.open();
			FILE << receive_payload << endl;
			FILE.close();
		}
	}
	
	return 0;
}