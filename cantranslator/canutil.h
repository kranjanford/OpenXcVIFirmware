#ifndef _CANUTIL_H_
#define _CANUTIL_H_

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "bitfield.h"

/* Public: A CAN transceiver message filter mask.
 *
 * number - the ID of this mask, e.g. 0, 1, 2, 3. This is neccessary to link
 *     filters with the masks they match.
 * value - the value of the mask, e.g. 0x7ff.
 */
struct CanFilterMask {
    int number;
    int value;
};

/* Public: A CAN transceiver message filter.
 *
 * number - the ID of this filter, e.g. 0, 1, 2.
 * value - the filter's value.
 * channel - the CAN channel this filter should be applied to.
 * maskNumber - the ID of the mask this filter should be paired with.
 */
struct CanFilter {
    int number;
    int value;
    int channel;
    int maskNumber;
};

/* Public: A state-based (SED) signal's mapping from numerical values to OpenXC
 * state names.
 *
 * value - the integer value of the state on the CAN bus.
 * name  - the corresponding string name for the state in OpenXC>
 */
struct CanSignalState {
    int value;
    char* name;
};

/* Public: A CAN signal to decode from the bus and output over USB.
 *
 * id          - the ID of the signal on the bus.
 * genericName - the name of the signal to be output over USB.
 * bitPosition - the starting bit of the signal in its CAN message.
 * bitSize     - the width of the bit field in the CAN message.
 * factor      - the final value will be multiplied by this factor.
 * offset      - the final value will be added to this offset.
 * minValue    - the minimum value for the processed signal.
 * maxValue    - the maximum value for the processed signal.
 * sendFrequency - how often to pass along this message when received.
 * sendSame    - if true, will re-send even if the value hasn't changed.
 * received    - mark true if this signal has ever been received.
 */
struct CanSignal {
    int id;
    char* genericName;
    int bitPosition;
    int bitSize;
    float factor;
    float offset;
    float minValue;
    float maxValue;
    int sendFrequency;
    int sendClock;
    bool sendSame;
    bool received;
    CanSignalState* states;
    int stateCount;
    float lastValue;
};

/* Public: Parses a CAN signal from a message and applies required
 *           transformation.
 *
 * signal - the details of the signal to decode and forward.
 * data   - the raw bytes of the CAN message that contains the signal.
 *
 * Returns the final, transformed value of the signal.
 */
float decodeCanSignal(CanSignal* signal, uint8_t* data);

/* Public: Finds and returns the corresponding string state for an integer
 *         value.
 *
 * signal  - the details of the signal that contains the state mapping.
 * signals - the list of all signals
 * signalCount - the length of the signals array
 * value   - the numerical value that maps to a state
 */
char* stateHandler(CanSignal* signal, CanSignal* signals, int signalCount,
        float value, bool* send);

/* Public: Coerces a numerical value to a boolean.
 *
 * signal  - the details of the signal that contains the state mapping.
 * signals - the list of all signals
 * signalCount - the length of the signals array
 * value   - the numerical value that will be converted to a boolean.
 */
bool booleanHandler(CanSignal* signal, CanSignal* signals, int signalCount,
        float value, bool* send);

/* Public: Store the value of a signal, but flip the send flag to false.
 *
 * signal  - the details of the signal that contains the state mapping.
 * signals - the list of all signals
 * signalCount - the length of the signals array
 * value   - the numerical value that will be converted to a boolean.
 */
float ignoreHandler(CanSignal* signal, CanSignal* signals, int signalCount,
        float value, bool* send);

float passthroughHandler(CanSignal* signal, CanSignal* signals, int signalCount,
        float value, bool* send);

/* Public: Look up the CanSignal representation of a signal based on its generic
 *         name.
 *
 * name - the generic, OpenXC name of the signal
 * signals - the list of all signals
 * signalCount - the length of the signals array
 *
 * Returns a pointer to the CanSignal if found, otherwise null;
 */
CanSignal* lookupSignal(char* name, CanSignal* signals, int signalCount);

#endif // _CANUTIL_H_
