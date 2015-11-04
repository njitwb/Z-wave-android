package com.azwave.androidzwave;

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
    public void open() {

    }

    /**
     * Closes the serial device.
     *
     * @throws IOException on error closing the device.
     */
    public void close() {

    }

    /**
     * Reads as many bytes as possible into the destination buffer.
     *
     * @param dest the destination byte buffer
     * @param timeoutMillis the timeout for reading
     * @return the actual number of bytes read
     * @throws IOException if an error occurred during reading
     */
    public int read(final byte[] dest, final int timeoutMillis) {
        return 0;
    }

    /**
     * Writes as many bytes as possible from the source buffer.
     *
     * @param src the source byte buffer
     * @param timeoutMillis the timeout for writing
     * @return the actual number of bytes written
     * @throws IOException if an error occurred during writing
     */
    public int write(final byte[] src, final int timeoutMillis) {
        return 0;
    }

}
