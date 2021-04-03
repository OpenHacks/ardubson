#include "ardubsonStreamParser.h"


BSONStreamParser::BSONStreamParser(void) {
    buffer_index = 0;
}

void BSONStreamParser::update() {
    unsigned long diff = millis() - rx_time_stamp;
    if ((buffer_index > 0) && (diff > COMMAND_TIMEOUT)) {
        if (parseErrorPointer) {
            parseErrorPointer(PARSE_ERROR_TIMEOUT);
        }
        buffer_index = 0;
    }
}

void BSONStreamParser::setParseErrorHandler(errorCallbackPtr func) {
    parseErrorPointer = func;
}

void BSONStreamParser::setMessageHandler(messageCallbackPtr func) {
    messagePointer = func;
}

void BSONStreamParser::analizeIncomingChars(char c) {
    if (buffer_index < BSON_DOC_SIZE) {
        frame[buffer_index] = c;

        if (buffer_index == 0)
            rx_time_stamp = millis();

        buffer_index++;

        if (buffer_index >= 5) {
            uint32_t size = *(uint32_t *) frame;

            if (size == buffer_index) {
                if (frame[buffer_index - 1] != 0) {
                    parseErrorPointer(PARSE_ERROR_END_OF_DOC);
                    buffer_index = 0;
                    return;
                }

                buffer_index = 0;

                if (messagePointer) {
                    BSONObject bo = BSONObject(frame);
                    messagePointer(&bo);
                    return;
                }
            }
        }
    } else {
        if (parseErrorPointer) {
            parseErrorPointer(PARSE_ERROR_BUFFER_OVERFLOW);
            buffer_index = 0;
            return;
        }
    }
}

uint16_t BSONStreamParser::getBufferIndex() {
    return buffer_index;
}

uint8_t BSONStreamParser::getBufferByte(uint16_t index) {
    return frame[index];
}
