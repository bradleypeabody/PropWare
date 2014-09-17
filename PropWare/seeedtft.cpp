/**
* @file    seeedtft.cpp
*
* @author  David Zemon
*
* @copyright
* The MIT License (MIT)<br>
* <br>Copyright (c) 2013 David Zemon<br>
* <br>Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"), to
* deal in the Software without restriction, including without limitation the
* rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
* sell copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:<br>
* <br>The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.<br>
* <br>THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

#include <PropWare/seeedtft.h>

const uint8_t PropWare::SeeedTFT::init_seq_cmd1[] = {
        0x01,
        0x02,
        0x03,
        0x04,
        0x08,
        0x0A,
        0x0C,
        0x0D,
        0x0F,
        0xFF
};

const uint16_t PropWare::SeeedTFT::init_seq_dat1[] = {
        0x0100,
        0x0700,
        0x1030,
        0x0000,
        0x0302,
        0x0000,
        0x0000,
        0x0000,
        0x0000
};

const uint8_t PropWare::SeeedTFT::init_seq_cmd2[] = {
        0x30,
        0x31,
        0x32,
        0x35,
        0x36,
        0x37,
        0x38,
        0x39,
        0x3c,
        0x3d,
        0x20,
        0x21,
        0x50,
        0x51,
        0x52,
        0x53,
        0xFF
};

const uint16_t PropWare::SeeedTFT::init_seq_dat2[] = {
        0x0000,
        0x0405,
        0x0203,
        0x0004,
        0x0B07,
        0x0000,
        0x0405,
        0x0203,
        0x0004,
        0x0B07,
        0x0000,
        0x0000,
        0x0000,
        0x00ef,
        0x0000,
        0x013f
};

const uint8_t PropWare::SeeedTFT::init_seq_cmd3[] = {
        0x60,
        0x61,
        0x90,
        0x95,
        0x80,
        0x81,
        0x82,
        0x83,
        0x84,
        0x85,
        0xFF,
        0xB0,
        0xFF,
        0xFF
};

const uint16_t PropWare::SeeedTFT::init_seq_dat3[] = {
        0xa700,
        0x0001,
        0x003A,
        0x021E,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x0001,
        0x140D,
        0x0000
};

const uint8_t PropWare::SeeedTFT::SIMPLE_FONT[][8] = {
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0x5F, 0x00, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0x07, 0x00, 0x07, 0x00, 0x00, 0x00},
        {0x00, 0x14, 0x7F, 0x14, 0x7F, 0x14, 0x00, 0x00},
        {0x00, 0x24, 0x2A, 0x7F, 0x2A, 0x12, 0x00, 0x00},
        {0x00, 0x23, 0x13, 0x08, 0x64, 0x62, 0x00, 0x00},
        {0x00, 0x36, 0x49, 0x55, 0x22, 0x50, 0x00, 0x00},
        {0x00, 0x00, 0x05, 0x03, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x1C, 0x22, 0x41, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x41, 0x22, 0x1C, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x08, 0x2A, 0x1C, 0x2A, 0x08, 0x00, 0x00},
        {0x00, 0x08, 0x08, 0x3E, 0x08, 0x08, 0x00, 0x00},
        {0x00, 0xA0, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00},
        {0x00, 0x60, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x20, 0x10, 0x08, 0x04, 0x02, 0x00, 0x00},
        {0x00, 0x3E, 0x51, 0x49, 0x45, 0x3E, 0x00, 0x00},
        {0x00, 0x00, 0x42, 0x7F, 0x40, 0x00, 0x00, 0x00},
        {0x00, 0x62, 0x51, 0x49, 0x49, 0x46, 0x00, 0x00},
        {0x00, 0x22, 0x41, 0x49, 0x49, 0x36, 0x00, 0x00},
        {0x00, 0x18, 0x14, 0x12, 0x7F, 0x10, 0x00, 0x00},
        {0x00, 0x27, 0x45, 0x45, 0x45, 0x39, 0x00, 0x00},
        {0x00, 0x3C, 0x4A, 0x49, 0x49, 0x30, 0x00, 0x00},
        {0x00, 0x01, 0x71, 0x09, 0x05, 0x03, 0x00, 0x00},
        {0x00, 0x36, 0x49, 0x49, 0x49, 0x36, 0x00, 0x00},
        {0x00, 0x06, 0x49, 0x49, 0x29, 0x1E, 0x00, 0x00},
        {0x00, 0x00, 0x36, 0x36, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0xAC, 0x6C, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x08, 0x14, 0x22, 0x41, 0x00, 0x00, 0x00},
        {0x00, 0x14, 0x14, 0x14, 0x14, 0x14, 0x00, 0x00},
        {0x00, 0x41, 0x22, 0x14, 0x08, 0x00, 0x00, 0x00},
        {0x00, 0x02, 0x01, 0x51, 0x09, 0x06, 0x00, 0x00},
        {0x00, 0x32, 0x49, 0x79, 0x41, 0x3E, 0x00, 0x00},
        {0x00, 0x7E, 0x09, 0x09, 0x09, 0x7E, 0x00, 0x00},
        {0x00, 0x7F, 0x49, 0x49, 0x49, 0x36, 0x00, 0x00},
        {0x00, 0x3E, 0x41, 0x41, 0x41, 0x22, 0x00, 0x00},
        {0x00, 0x7F, 0x41, 0x41, 0x22, 0x1C, 0x00, 0x00},
        {0x00, 0x7F, 0x49, 0x49, 0x49, 0x41, 0x00, 0x00},
        {0x00, 0x7F, 0x09, 0x09, 0x09, 0x01, 0x00, 0x00},
        {0x00, 0x3E, 0x41, 0x41, 0x51, 0x72, 0x00, 0x00},
        {0x00, 0x7F, 0x08, 0x08, 0x08, 0x7F, 0x00, 0x00},
        {0x00, 0x41, 0x7F, 0x41, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x20, 0x40, 0x41, 0x3F, 0x01, 0x00, 0x00},
        {0x00, 0x7F, 0x08, 0x14, 0x22, 0x41, 0x00, 0x00},
        {0x00, 0x7F, 0x40, 0x40, 0x40, 0x40, 0x00, 0x00},
        {0x00, 0x7F, 0x02, 0x0C, 0x02, 0x7F, 0x00, 0x00},
        {0x00, 0x7F, 0x04, 0x08, 0x10, 0x7F, 0x00, 0x00},
        {0x00, 0x3E, 0x41, 0x41, 0x41, 0x3E, 0x00, 0x00},
        {0x00, 0x7F, 0x09, 0x09, 0x09, 0x06, 0x00, 0x00},
        {0x00, 0x3E, 0x41, 0x51, 0x21, 0x5E, 0x00, 0x00},
        {0x00, 0x7F, 0x09, 0x19, 0x29, 0x46, 0x00, 0x00},
        {0x00, 0x26, 0x49, 0x49, 0x49, 0x32, 0x00, 0x00},
        {0x00, 0x01, 0x01, 0x7F, 0x01, 0x01, 0x00, 0x00},
        {0x00, 0x3F, 0x40, 0x40, 0x40, 0x3F, 0x00, 0x00},
        {0x00, 0x1F, 0x20, 0x40, 0x20, 0x1F, 0x00, 0x00},
        {0x00, 0x3F, 0x40, 0x38, 0x40, 0x3F, 0x00, 0x00},
        {0x00, 0x63, 0x14, 0x08, 0x14, 0x63, 0x00, 0x00},
        {0x00, 0x03, 0x04, 0x78, 0x04, 0x03, 0x00, 0x00},
        {0x00, 0x61, 0x51, 0x49, 0x45, 0x43, 0x00, 0x00},
        {0x00, 0x7F, 0x41, 0x41, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x02, 0x04, 0x08, 0x10, 0x20, 0x00, 0x00},
        {0x00, 0x41, 0x41, 0x7F, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x04, 0x02, 0x01, 0x02, 0x04, 0x00, 0x00},
        {0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00},
        {0x00, 0x01, 0x02, 0x04, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x20, 0x54, 0x54, 0x54, 0x78, 0x00, 0x00},
        {0x00, 0x7F, 0x48, 0x44, 0x44, 0x38, 0x00, 0x00},
        {0x00, 0x38, 0x44, 0x44, 0x28, 0x00, 0x00, 0x00},
        {0x00, 0x38, 0x44, 0x44, 0x48, 0x7F, 0x00, 0x00},
        {0x00, 0x38, 0x54, 0x54, 0x54, 0x18, 0x00, 0x00},
        {0x00, 0x08, 0x7E, 0x09, 0x02, 0x00, 0x00, 0x00},
        {0x00, 0x18, 0xA4, 0xA4, 0xA4, 0x7C, 0x00, 0x00},
        {0x00, 0x7F, 0x08, 0x04, 0x04, 0x78, 0x00, 0x00},
        {0x00, 0x00, 0x7D, 0x00, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x80, 0x84, 0x7D, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x7F, 0x10, 0x28, 0x44, 0x00, 0x00, 0x00},
        {0x00, 0x41, 0x7F, 0x40, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x7C, 0x04, 0x18, 0x04, 0x78, 0x00, 0x00},
        {0x00, 0x7C, 0x08, 0x04, 0x7C, 0x00, 0x00, 0x00},
        {0x00, 0x38, 0x44, 0x44, 0x38, 0x00, 0x00, 0x00},
        {0x00, 0xFC, 0x24, 0x24, 0x18, 0x00, 0x00, 0x00},
        {0x00, 0x18, 0x24, 0x24, 0xFC, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0x7C, 0x08, 0x04, 0x00, 0x00, 0x00},
        {0x00, 0x48, 0x54, 0x54, 0x24, 0x00, 0x00, 0x00},
        {0x00, 0x04, 0x7F, 0x44, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x3C, 0x40, 0x40, 0x7C, 0x00, 0x00, 0x00},
        {0x00, 0x1C, 0x20, 0x40, 0x20, 0x1C, 0x00, 0x00},
        {0x00, 0x3C, 0x40, 0x30, 0x40, 0x3C, 0x00, 0x00},
        {0x00, 0x44, 0x28, 0x10, 0x28, 0x44, 0x00, 0x00},
        {0x00, 0x1C, 0xA0, 0xA0, 0x7C, 0x00, 0x00, 0x00},
        {0x00, 0x44, 0x64, 0x54, 0x4C, 0x44, 0x00, 0x00},
        {0x00, 0x08, 0x36, 0x41, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x41, 0x36, 0x08, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x02, 0x01, 0x01, 0x02, 0x01, 0x00, 0x00},
        {0x00, 0x02, 0x05, 0x05, 0x02, 0x00, 0x00, 0x00}
};