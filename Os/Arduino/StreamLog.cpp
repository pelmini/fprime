/**
 * LogArduinoStream:
 *
 * Implementation of Os::Log that defers to Arduino's Stream type. This allows for a hardware based
 * stream capture. Note: in order to set the stream, the companion header Os/Arduino/StreamLog.hpp
 * should be used.
 *
 *  Created on: Apr 13, 2019
 *      Author: lestarch
 */

#include <Os/Log.hpp>
#include <Os/Arduino/StreamLog.hpp>

extern "C" {
    #include <stdio.h>
    #include <string.h>
}

namespace Os {
    Stream* A_STREAM = NULL; // I love it when a plan comes together.
    void setArduinoStreamLogHandler(Stream* stream) {
        A_STREAM = stream;
    }
    /**
     * An Arduino Stream implementation of the Os::Log function. Essentially, this log message
     * handler formats a message an sends it out to the serial port/Wire port or Arduino stream
     * backing the stream object. This allows for a side-channel serial console.
     */
    void Log::logMsg(
        const char* format,
        POINTER_CAST a1,
        POINTER_CAST a2,
        POINTER_CAST a3,
        POINTER_CAST a4,
        POINTER_CAST a5,
        POINTER_CAST a6
    ) {
        char message[128];
        // If stream is defined, then write to it.
        if (A_STREAM != NULL) {
            snprintf(message, sizeof(message), format, a1, a2, a3, a4, a5, a6);
            A_STREAM->write(message, sizeof(message));
        }
    }
}
