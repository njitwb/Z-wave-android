//
// Created by archermind on 11/11/15.
//

#include "UsbSerialDriver.h"

#include <errno.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/file.h>

int fd;

void set_blocking(int fd, int should_block)
{
    struct termios tty;
    memset(&tty, 0, sizeof tty);
    if (tcgetattr(fd, &tty) != 0)
    {
        LOGE("error %d from tggetattr", errno);
        return;
    }

    tty.c_cc[VMIN] = should_block ? 1 : 0;
    tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

    if (tcsetattr(fd, TCSANOW, &tty) != 0)
        LOGE("error %d setting term attributes", errno);
}

int xfer(unsigned char* buf,unsigned char len,unsigned char rlen) {
    unsigned char null =0 ;
    unsigned char r;
    int i;
    if(buf) {
        i=write(fd, buf, len);
        //tcdrain(fd);
        //printf("TX %2.2x %2.2x %2.2x %2.2x\n",buf[0],buf[1],buf[2],buf[3]);

        r=0;
        i=0;
        while((r < rlen) && (i < 3)) {
            r+= read(fd, &buf[r], rlen-r );
            i++;
        }

        if(rlen ==  r )  {
            //printf("RX %2.2x %2.2x %2.2x %2.2x\n",buf[0],buf[1],buf[2],buf[3]);
            return r;
        } else {
            printf("Read too short len = %i\n",r);
            return r;
        }
    } else {
        i=write(fd,&null,1);
//        tcdrain(fd);
        return 1;
    }
}

/*
 * Class:     com_azwave_androidzwave_UsbSerialDriver
 * Method:    SerialOpen
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT jint JNICALL Java_com_azwave_androidzwave_UsbSerialDriver_SerialOpen
(JNIEnv *env, jobject obj, jstring path)
{
    int fileDescriptor = -1;
    struct termios options;
    const char *bsdPath = NULL;

    if (path != NULL) {
        bsdPath = env->GetStringUTFChars(path, NULL);
        LOGD("tty device path is: %s", bsdPath);
    } else {
        LOGD("tty device path is empty");
        return -1;
    }

    fileDescriptor = open(bsdPath, O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (fileDescriptor == -1 || flock(fileDescriptor, LOCK_EX) == -1 )
    {
        LOGE("Error opening serial port %s - %s(%d).\n",
             bsdPath, strerror(errno), errno);
        goto error;
    }

    if (fcntl(fileDescriptor, F_SETFL, 0) == -1)
    {
        LOGE("Error clearing O_NONBLOCK %s - %s(%d).\n",
             bsdPath, strerror(errno), errno);
        goto error;
    }

    if (ioctl(fileDescriptor, TIOCEXCL, (char *) 0) < 0) {
        LOGE("Error setting TIOCEXCL %s - %s(%d).\n",
             bsdPath, strerror(errno), errno);
        goto error;
    }

    memset(&options,0,sizeof(options));
    // The baud rate, word length, and handshake options can be set as follows:
    options.c_iflag=0;
    options.c_oflag=0;


    options.c_cflag=CS8|CREAD|CLOCAL|CSTOPB;    // 8n2, see termios.h for more information
    options.c_lflag=0;
    options.c_cc[VMIN]=1;
    options.c_cc[VTIME]=100;
    cfsetospeed(&options, B115200); // Set 115200 baud
    cfsetispeed(&options, B115200);

    tcflush(fileDescriptor, TCIFLUSH);
    //cfmakeraw(&options);

    // Cause the new options to take effect immediately.
    if (tcsetattr(fileDescriptor, TCSANOW, &options) == -1)
    {
        LOGE("Error setting tty attributes %s - %s(%d).\n",
               bsdPath, strerror(errno), errno);
        goto error;
    }

    // Success
    fd = fileDescriptor;
    set_blocking(fd, 0);
    return 0;

    // Failure path
    error:
    if (fileDescriptor != -1)
    {
        close(fileDescriptor);
    }

    return -1;
}

/*
 * Class:     com_azwave_androidzwave_UsbSerialDriver
 * Method:    SerialClose
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_azwave_androidzwave_UsbSerialDriver_SerialClose
(JNIEnv *env, jobject obj)
{
    flock(fd, LOCK_UN);
    close(fd);
}

void dumphex(jbyte *data, int len)
{
    int i;

    for(i = 0; i < len; i++) {
        LOGD("0x%x ",data[i]);
    }
}

/*
 * Class:     com_azwave_androidzwave_UsbSerialDriver
 * Method:    SerialWrite
 * Signature: ([B)I
 */
JNIEXPORT jint JNICALL Java_com_azwave_androidzwave_UsbSerialDriver_SerialWrite
        (JNIEnv *env, jobject obj, jbyteArray src)
{
    jbyte * buffer;
    jint length;
    jint i;

    buffer = env->GetByteArrayElements(src, false);
    length = env->GetArrayLength(src);

    if(buffer) {
        i = write(fd, buffer, length);
        LOGD("write data, i = %d length = %d", i, length);
//        dumphex(buffer, i);
        env->ReleaseByteArrayElements(src, buffer, 0);
        return i;
    } else {
        env->ReleaseByteArrayElements(src, buffer, 0);
        return 0;
    }
}

/*
 * Class:     com_azwave_androidzwave_UsbSerialDriver
 * Method:    SerialRead
 * Signature: ([B)I
 */
JNIEXPORT jint JNICALL Java_com_azwave_androidzwave_UsbSerialDriver_SerialRead
        (JNIEnv *env, jobject obj, jbyteArray dest)
{
    jbyte * buffer;
    jint i, length;

    buffer = env->GetByteArrayElements(dest, false);
    length = env->GetArrayLength(dest);
    i = read(fd, buffer, length);
    LOGD("read data, i = %d length = %d", i, length);
  //  dumphex(buffer, i);
    env->ReleaseByteArrayElements(dest, buffer, 0);

    return i;
}