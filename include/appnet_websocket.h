
#ifndef WEBSOCKET_H
#define WEBSOCKET_H

#include <assert.h>
#include <ctype.h>      /* isdigit */
#include <netinet/in.h> /*htons*/
#include <stdint.h>     /* uint8_t */
#include <stdio.h>      /* sscanf */
#include <stdlib.h>     /* strtoul */
#include <string.h>
//#include <stddef.h> /* size_t */
#include "base64.h"
#include "sha1.h"

#define PROGMEM
#define PSTR
#define strstr_P strstr
#define sscanf_P sscanf
#define sprintf_P sprintf
#define strlen_P strlen
#define memcmp_P memcmp
#define memcpy_P memcpy

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

static const char connectionField[] = "Connection: ";
static const char upgrade[] = "upgrade";
static const char upgrade2[] = "Upgrade";
static const char upgradeField[] = "Upgrade: ";
static const char websocket[] = "websocket";
static const char hostField[] = "Host: ";
static const char originField[] = "Origin: ";
static const char keyField[] = "Sec-WebSocket-Key: ";
static const char protocolField[] = "Sec-WebSocket-Protocol: ";
static const char versionField[] = "Sec-WebSocket-Version: ";
static const char version[] = "13";
static const char secret[] = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

enum wsFrameType
{ // errors starting from 0xF0
	WS_EMPTY_FRAME = 0xF0,
	WS_ERROR_FRAME = 0xF1,
	WS_INCOMPLETE_FRAME = 0xF2,
	WS_TEXT_FRAME = 0x01,
	WS_BINARY_FRAME = 0x02,
	WS_PING_FRAME = 0x09,
	WS_PONG_FRAME = 0x0A,
	WS_OPENING_FRAME = 0xF3,
	WS_CLOSING_FRAME = 0x08
};

enum wsState
{
	WS_STATE_OPENING,
	WS_STATE_NORMAL,
	WS_STATE_CLOSING
};

typedef struct _handshake
{
	char host[32];
	char origin[64];
	char key[32];
	char key_ext[64];
	int ver[3];
	char *resource;
	enum wsFrameType frame_type;
	enum wsState state;
} handshake;

/**
 * @param inputFrame Pointer to input frame
 * @param inputLength Length of input frame
 * @param hs Cleared with nullHandshake() handshake structure
 * @return Type of parsed frame
 */
enum wsFrameType wsParseHandshake( const uint8_t *inputFrame , size_t inputLength ,
		handshake *hs );

/**
 * @param hs Filled handshake structure
 * @param outFrame Pointer to frame buffer
 * @param outLength Length of frame buffer. Return length of out frame
 */
void wsGetHandshakeAnswer( const handshake *hs , uint8_t *outFrame ,
		size_t *outLength , char *ver );

// void initHandshake( int connfd , httpHeader* header ,  handshake* hs );
/**
 * @param data Pointer to input data array
 * @param dataLength Length of data array
 * @param outFrame Pointer to frame buffer
 * @param outLength Length of out frame buffer. Return length of out frame
 * @param frameType [WS_TEXT_FRAME] frame type to build
 */
void wsMakeFrame( const uint8_t *data , size_t dataLength , uint8_t *outFrame ,
		size_t *outLength , enum wsFrameType frameType );

/**
 *
 * @param inputFrame Pointer to input frame. Frame will be modified.
 * @param inputLen Length of input frame
 * @param outDataPtr Return pointer to extracted data in input frame
 * @param outLen Return length of extracted data
 * @return Type of parsed frame
 */
enum wsFrameType wsParseInputFrame( uint8_t *inputFrame , size_t inputLength ,
		uint8_t **dataPtr , size_t *dataLength );

/**
 * @param hs NULL handshake structure
 */
void nullHandshake( handshake *hs );

/**
 * @param hs free and NULL handshake structure
 */
void freeHandshake( handshake *hs );

#endif /* WEBSOCKET_H */
