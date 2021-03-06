/**
 * @file    I2CSlave_Demo.cpp
 *
 * @author  Markus Ebner
 *
 * @copyright
 * The MIT License (MIT)<br>
 * <br>Copyright (c) 2013 David Zemon<br>
 * <br>Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and
 * to permit persons to whom the Software is furnished to do so, subject to the following conditions:<br>
 * <br>The above copyright notice and this permission notice shall be included in all copies or substantial portions
 * of the Software.<br>
 * <br>THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
 * TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <PropWare/hmi/output/printer.h>
#include <PropWare/serial/i2c/i2cslave.h>
#include <PropWare/serial/i2c/i2cmaster.h>
#include <PropWare/utility/collection/queue.h>
#include <PropWare/gpio/simpleport.h>

using PropWare::Queue;
using PropWare::I2CMaster;
using PropWare::I2CSlave;
using PropWare::SimplePort;
using PropWare::Port;
using PropWare::Pin;
using PropWare::Runnable;

static const int SLAVE_ADDRESS         = 42;
static const int SHIFTED_SLAVE_ADDRESS = SLAVE_ADDRESS << 1;

void error (const PropWare::ErrorCode err) {
    // Set the Quickstart LEDs for output (used to display the error code)
    const SimplePort debugLEDs(Port::P16, 8, Pin::Dir::OUT);

    while (1) {
        debugLEDs.write((uint32_t) err);
        waitcnt(CLKFREQ / 5 + CNT);
        debugLEDs.write(0);
        waitcnt(CLKFREQ / 5 + CNT);
    }
}

/**
 * @brief	Callback fired, when the master requests data from us (the slave).
 *
 * In this method, you should answer the master through the slave's write() method
 */
void onRequest (I2CSlave<Queue<uint8_t>> &slave, Queue<uint8_t> &queue) {
    static uint8_t sum = 0;
    while (!queue.is_empty())
        sum += queue.dequeue();
    slave.write(sum);
}

/**
 * @brief	Callback fired, when the master sent us some nice packet of fresh data.
 *
 * In this method, you should take the data received from the master out of the receive buffer.
 */
void onReceive (I2CSlave<Queue<uint8_t>> &slave, Queue<uint8_t> &queue) {
    while (slave.available()) {
        const int result = slave.read();
        if (-1 == result)
            error(-1);
        else
            queue.insert(static_cast<uint8_t>(result));
    }
}

/**
 * @example     I2CSlave_Demo.cpp
 *
 * Use an easy I2CSlave class to imitate an I2C slave with address 42
 *
 * @include PropWare_I2CSlave/CMakeLists.txt
 */
int main () {
    uint8_t        queueBuffer[32];
    Queue<uint8_t> queue(queueBuffer);

    uint8_t                  buffer[32];
    uint32_t                 stack[128];
    I2CSlave<Queue<uint8_t>> slave(SLAVE_ADDRESS, buffer, stack);

    // register the bus callbacks that will be fired on incoming requests or data
    slave.set_on_request(onRequest);
    slave.set_on_receive(onReceive);
    slave.set_user_data(queue);

    // Start a new cog to monitor the I2C bus and respond to events
    Runnable::invoke(slave);
    waitcnt(400 * MILLISECOND + CNT);

    // Run the master
    I2CMaster master;

    // Set the master to a very low frequency. The onRequest() and onReceive() methods in this sample are not optimized
    // for high speed communications, and extreme optimizations may be necessary in order to run a synchronous serial
    // bus like I2C in slave mode. Those types of optimizations are beyond the scope of this demo, so we will simply
    // set the frequency low enough that they are unnecessary.
    master.set_frequency(10000);

    if (master.ping(SHIFTED_SLAVE_ADDRESS)) {
        pwOut << "ACK received!\n";

        pwOut << "Expecting  0: " << master.get(SHIFTED_SLAVE_ADDRESS, static_cast<uint8_t>(0)) << '\n';
        pwOut << "Expecting  1: " << master.get(SHIFTED_SLAVE_ADDRESS, static_cast<uint8_t>(1)) << '\n';
        pwOut << "Expecting  3: " << master.get(SHIFTED_SLAVE_ADDRESS, static_cast<uint8_t>(2)) << '\n';
        pwOut << "Expecting  6: " << master.get(SHIFTED_SLAVE_ADDRESS, static_cast<uint8_t>(3)) << '\n';
        pwOut << "Expecting 10: " << master.get(SHIFTED_SLAVE_ADDRESS, static_cast<uint8_t>(4)) << '\n';
    } else
        pwOut << "No ack! :(\n I guess we're done.\n";
}
