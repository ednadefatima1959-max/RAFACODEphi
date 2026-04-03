package com.termux.app;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertArrayEquals;
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.fail;

import java.io.File;

import org.junit.After;
import org.junit.Rule;
import org.junit.Test;
import org.junit.rules.TemporaryFolder;

public class TermuxInstallerTest {

    @Rule
    public TemporaryFolder temporaryFolder = new TemporaryFolder();

    @After
    public void tearDown() {
        TermuxInstaller.resetNativeBootstrapProviderForTests();
    }

    @Test
    public void resolveWithinStaging_allowsNormalRelativeEntry() throws Exception {
        File stagingRoot = temporaryFolder.newFolder("staging");

        File resolved = TermuxInstaller.resolveWithinStaging(stagingRoot, "bin/bash");

        assertTrue(resolved.getCanonicalPath().startsWith(stagingRoot.getCanonicalPath() + File.separator));
        assertEquals(new File(stagingRoot, "bin/bash").getCanonicalPath(), resolved.getCanonicalPath());
    }

    @Test
    public void resolveWithinStaging_rejectsParentTraversalEntry() throws Exception {
        File stagingRoot = temporaryFolder.newFolder("staging");

        try {
            TermuxInstaller.resolveWithinStaging(stagingRoot, "../outside");
            fail("Expected RuntimeException for parent traversal");
        } catch (RuntimeException e) {
            assertTrue(e.getMessage().contains("Path escapes bootstrap staging root"));
            assertTrue(e.getMessage().contains("../outside"));
        }
    }

    @Test
    public void resolveWithinStaging_rejectsAbsoluteEntry() throws Exception {
        File stagingRoot = temporaryFolder.newFolder("staging");

        String absolutePath = new File(File.separator + "tmp", "escape").getAbsolutePath();
        try {
            TermuxInstaller.resolveWithinStaging(stagingRoot, absolutePath);
            fail("Expected RuntimeException for absolute path");
        } catch (RuntimeException e) {
            assertTrue(e.getMessage().contains("Absolute path is not allowed"));
            assertTrue(e.getMessage().contains(absolutePath));
        }
    }

    @Test
    public void resolveWithinStaging_rejectsSymlinkDestinationEscapingStaging() throws Exception {
        File stagingRoot = temporaryFolder.newFolder("staging");

        try {
            TermuxInstaller.resolveWithinStaging(stagingRoot, "lib/../../etc/passwd");
            fail("Expected RuntimeException for escaping symlink destination");
        } catch (RuntimeException e) {
            assertTrue(e.getMessage().contains("Path escapes bootstrap staging root"));
            assertTrue(e.getMessage().contains("lib/../../etc/passwd"));
        }
    }

    @Test
    public void loadZipBytes_jniUnavailable_returnsFallbackAndDetailedDiagnostic() {
        TermuxInstaller.JniNativeBootstrapProvider provider =
            new TermuxInstaller.JniNativeBootstrapProvider(
                "termux-bootstrap",
                ignored -> {
                    throw new UnsatisfiedLinkError("no termux-bootstrap in java.library.path");
                },
                () -> new byte[]{1, 2, 3},
                () -> "arm64-v8a,x86_64"
            );
        TermuxInstaller.setNativeBootstrapProviderForTests(provider);

        byte[] zipBytes = TermuxInstaller.loadZipBytes();
        String diagnostic = TermuxInstaller.getBootstrapNativeLoadError();

        assertEquals(0, zipBytes.length);
        assertTrue(diagnostic.contains("abi=arm64-v8a,x86_64"));
        assertTrue(diagnostic.contains("lib=termux-bootstrap"));
        assertTrue(diagnostic.contains("error=UnsatisfiedLinkError: no termux-bootstrap in java.library.path"));
    }

    @Test
    public void loadZipBytes_jniAvailable_usesInjectedJniReturn() {
        byte[] expectedBytes = new byte[]{9, 8, 7, 6};
        TermuxInstaller.JniNativeBootstrapProvider provider =
            new TermuxInstaller.JniNativeBootstrapProvider(
                "termux-bootstrap",
                ignored -> {
                },
                () -> expectedBytes,
                () -> "arm64-v8a"
            );
        TermuxInstaller.setNativeBootstrapProviderForTests(provider);

        byte[] actual = TermuxInstaller.loadZipBytes();
        String diagnostic = TermuxInstaller.getBootstrapNativeLoadError();

        assertArrayEquals(expectedBytes, actual);
        assertTrue(diagnostic.contains("abi=arm64-v8a"));
        assertTrue(diagnostic.contains("lib=termux-bootstrap"));
        assertTrue(diagnostic.contains("error=none"));
    }

    @Test
    public void loadZipBytes_nativeReadFailure_reportsDetailedDiagnosticWithoutGenericException() {
        TermuxInstaller.JniNativeBootstrapProvider provider =
            new TermuxInstaller.JniNativeBootstrapProvider(
                "termux-bootstrap",
                ignored -> {
                },
                () -> {
                    throw new IllegalStateException("zip read failed");
                },
                () -> "x86_64"
            );
        TermuxInstaller.setNativeBootstrapProviderForTests(provider);

        byte[] actual = TermuxInstaller.loadZipBytes();
        String diagnostic = TermuxInstaller.getBootstrapNativeLoadError();

        assertEquals(0, actual.length);
        assertTrue(diagnostic.contains("abi=x86_64"));
        assertTrue(diagnostic.contains("lib=termux-bootstrap"));
        assertTrue(diagnostic.contains("error=IllegalStateException: zip read failed"));
    }
}
