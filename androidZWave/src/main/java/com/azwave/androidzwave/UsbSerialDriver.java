package com.azwave.androidzwave;

import java.io.IOError;
import java.io.IOException;

/**
 * Created by archermind on 11/4/15.
 */
public class UsbSerialDriver {
    /**
     * Opens and initializes the device as a USB serial device. Upon success,
     * caller must ensure that {@link #close()} is eventually called.
     *
     * @throws IOException on error opening or initializing the device.
     */

    public static final String DevicePath = "/dev/ttyACM0";

    public native int SerialOpen(String path);
    public native void SerialClose();
    public native int SerialWrite(final byte[] src);
    public native int SerialRead(final byte[] dest);

    protected final Object mReadLock = new Object();
    protected final Object mWriteLock = new Object();

    public void open() throws IOException {
        int ret = SerialOpen(DevicePath);
        if (ret != 0)
            throw new IOException("Open the device error!");
    }

    /**
     * Closes the serial device.
     *
     * @throws IOException on error closing the device.
     */
    public void close() throws IOException {
        SerialClose();
    }

    /**
     * Reads as many bytes as possible into the destination buffer.
     *
     * @param dest the destination byte buffer
     * @param timeoutMillis the timeout for reading
     * @return the actual number of bytes read
     * @throws IOException if an error occurred during reading
     */
    public int read(final byte[] dest, final int timeoutMillis) throws IOException {
        int nread;
        synchronized (mReadLock) {
            nread = SerialRead(dest);
        }
        return nread;
    }

    /**
     * Writes as many bytes as possible from the source buffer.
     *
     * @param src the source byte buffer
     * @param timeoutMillis the timeout for writing
     * @return the actual number of bytes written
     * @throws IOException if an error occurred during writing
     */
    public int write(final byte[] src, final int timeoutMillis) throws IOException {
        int nwrite;
        synchronized (mWriteLock) {
            nwrite = SerialWrite(src);
        }
        return nwrite;
    }

    static {
        System.loadLibrary("SerialDriver");
    }

}
