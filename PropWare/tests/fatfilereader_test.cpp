/**
 * @file    fatfilereader_test.cpp
 *
 * @author  David Zemon
 *
 * Prerequisites:
 *      - SD card connected with the following pins:
 *          - MOSI = P0
 *          - MISO = P1
 *          - SCLK = P2
 *          - CS   = P4
 *      - FAT16 or FAT32 Filesystem on the first partition of the SD card
 *      - File named "fat_test.txt" in this directory should be loaded into the root directory
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

#include "PropWareTests.h"
#include <PropWare/filesystem/sd.h>
#include <PropWare/filesystem/fat/fatfs.h>
#include <PropWare/filesystem/fat/fatfilereader.h>
#include <PropWare/staticstringbuilder.h>

using namespace PropWare;

const Pin::Mask MOSI = Pin::P0;
const Pin::Mask MISO = Pin::P1;
const Pin::Mask SCLK = Pin::P2;
const Pin::Mask CS   = Pin::P4;

const char FILE_NAME[] = "fat_test.txt";
const char FILE_NAME_UPPER[] = "FAT_TEST.TXT";
FatFS *g_fs;
FatFileReader *testable;

void error_checker (const ErrorCode err) {
    if (SPI::BEG_ERROR <= err && err <= SPI::END_ERROR)
        SPI::get_instance()->print_error_str(&pwOut, (const SPI::ErrorCode) err);
    else if (SD::BEG_ERROR <= err && err <= SD::END_ERROR)
        ((SD *) g_fs->m_driver)->print_error_str(pwOut, (const SD::ErrorCode) err);
    else if (Filesystem::BEG_ERROR <= err && err <= Filesystem::END_ERROR)
        FatFS::print_error_str(pwOut, (const Filesystem::ErrorCode) err);
    else if (FatFS::BEG_ERROR <= err && err <= FatFS::END_ERROR)
        pwOut << "No print string yet for FatFS's error #" << err - FatFS::BEG_ERROR << " (raw = " << err << ")\n";
    else if (err)
        pwOut << "Unknown error: " << err << '\n';
}

SETUP {
    PropWare::ErrorCode err;
    testable = new FatFileReader(*g_fs, FILE_NAME);
    err = testable->open();
    if (err) {
        MESSAGE("Setup failed!");
        error_checker(err);
    }
}

TEARDOWN {
    delete testable;
}

TEST(ConstructorDestructor) {
    testable = new FatFileReader(*g_fs, FILE_NAME);

    // Ensure the requested filename was not all upper case (that wouldn't be a very good test if it were)
    ASSERT_NEQ_MSG(0, strcmp(FILE_NAME, FILE_NAME_UPPER));

    ASSERT_EQ_MSG(0, strcmp(FILE_NAME_UPPER, testable->get_name()));
    ASSERT_EQ_MSG((unsigned int) &pwOut, (unsigned int) testable->m_logger);
    ASSERT_EQ_MSG((unsigned int) g_fs->get_driver(), (unsigned int) testable->m_driver);
    ASSERT_EQ_MSG((unsigned int) &g_fs->m_buf, (unsigned int) testable->m_buf);
    ASSERT_EQ_MSG((unsigned int) testable->m_fs, (unsigned int) g_fs);
    ASSERT_EQ_MSG(false, testable->m_mod);

    tearDown();
}

TEST(OpenClose) {
    ErrorCode err;
    testable = new FatFileReader(*g_fs, FILE_NAME);

    err = testable->open();
    error_checker(err);
    ASSERT_EQ_MSG(0, err);

    ASSERT_NEQ_MSG(0, testable->get_length());

    err = testable->close();
    error_checker(err);
    ASSERT_EQ_MSG(0, err);

    tearDown();
}

TEST(SafeGetChar) {
    PropWare::ErrorCode err;
    setUp();

    char c;
    err = testable->safe_get_char(c);
    error_checker(err);
    ASSERT_EQ_MSG(0, err);
    ASSERT_EQ_MSG('/', c);

    tearDown();
}

TEST(Tell) {
    PropWare::ErrorCode err;
    setUp();

    for (int i = 0; i < 1024; ++i) {
        char c;
        err = testable->safe_get_char(c);
        error_checker(err);
        ASSERT_EQ_MSG(0, err);

        ASSERT_EQ_MSG(i + 1, testable->tell());
    }

    tearDown();
}

TEST(Seek) {
    const int SEEK_ITERATIONS = 2048;
    char stringBuffer[SEEK_ITERATIONS];
    StaticStringBuilder stringBuilder(stringBuffer);
    PropWare::ErrorCode err;
    setUp();

    for (int i = 0; i < SEEK_ITERATIONS - 1; ++i) {
        char c;
        err = testable->safe_get_char(c);
        error_checker(err);
        ASSERT_EQ_MSG(0, err);
        stringBuilder.put_char(c);
    }

    srand(CNT);
    for (int i = 0; i < 128; ++i) {
        const int charIndex = rand() % (SEEK_ITERATIONS - 1);
        err = testable->seek(charIndex, File::BEG);
        error_checker(err);
        ASSERT_EQ_MSG(0, err);

        char actual;
        err = testable->safe_get_char(actual);
        error_checker(err);
        ASSERT_EQ_MSG(0, err);
        ASSERT_EQ_MSG(stringBuilder.to_string()[charIndex], actual);
    }

    tearDown();
}

int main () {
    START(FatFileReaderTest);

    PropWare::ErrorCode err;

    FatFS fs(new SD(SPI::get_instance(), MOSI, MISO, SCLK, CS));
    if ((err = fs.mount())) {
        error_checker(err);
        passed = false;
        COMPLETE();
    }
    g_fs = &fs;

    RUN_TEST(ConstructorDestructor);
    RUN_TEST(OpenClose);
    RUN_TEST(SafeGetChar);
    RUN_TEST(Tell);
    RUN_TEST(Seek);

    COMPLETE();
}