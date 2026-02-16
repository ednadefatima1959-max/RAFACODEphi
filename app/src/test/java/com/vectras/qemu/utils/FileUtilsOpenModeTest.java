package com.vectras.qemu.utils;

import static org.junit.Assert.assertEquals;

import android.os.ParcelFileDescriptor;

import org.junit.Test;

public class FileUtilsOpenModeTest {

    @Test
    public void resolveContentOpenMode_shouldHonorBackendReadOnly() {
        assertEquals("r", FileUtils.resolveContentOpenMode("content://disk.qcow2", "r"));
    }

    @Test
    public void resolveContentOpenMode_shouldForceIsoToReadOnly() {
        assertEquals("r", FileUtils.resolveContentOpenMode("content://disk.iso", "rw"));
    }

    @Test
    public void resolveParcelOpenMode_shouldForceIsoToReadOnly() {
        assertEquals(ParcelFileDescriptor.MODE_READ_ONLY,
                FileUtils.resolveParcelOpenMode("/storage/emulated/0/vm/disk.ISO", "rw"));
    }

    @Test
    public void resolveParcelOpenMode_shouldDefaultToReadWriteForWritableDisk() {
        assertEquals(ParcelFileDescriptor.MODE_READ_WRITE,
                FileUtils.resolveParcelOpenMode("/storage/emulated/0/vm/disk.qcow2", ""));
    }
}
