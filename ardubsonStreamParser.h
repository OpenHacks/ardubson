#ifndef ardubson_stream_parser_h
#define ardubson_stream_parser_h

#include "Arduino.h"
#include <ardubson.h>

#define COMMAND_TIMEOUT 500

#define PARSE_ERROR_DATA 0
#define PARSE_ERROR_TIMEOUT 1
#define PARSE_ERROR_BUFFER_OVERFLOW 2
#define PARSE_ERROR_END_OF_DOC 3

typedef void (*errorCallbackPtr)(int error_code);
typedef void (*messageCallbackPtr)(BSONObject * bson_object);

static errorCallbackPtr parseErrorPointer;
static messageCallbackPtr messagePointer;

class BSONStreamParser
{
    protected:
        char frame[BSON_DOC_SIZE];
        uint16_t buffer_index;
        unsigned long rx_time_stamp;

    public:
        BSONStreamParser(void);
        void analizeIncomingChars(char c);
        void update();

        void setParseErrorHandler(errorCallbackPtr func);
        void setMessageHandler(messageCallbackPtr func);
        void parseError(void);
        void timeoutError(void);
        uint16_t getBufferIndex();
        uint8_t getBufferByte(uint16_t index);
};

#endif
